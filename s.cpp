#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <system_error>
#include <cerrno>
#include <cstring>
#include <memory> // For std::unique_ptr and std::make_unique

// C headers
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>

// --- 前向声明 ---
struct ServerContext;

// --- 抽象任务基类 ---
class Task {
public:
    virtual ~Task() = default;
    virtual void execute() = 0;
};

// --- 线程池类  ---
class ThreadPool {
public:
    ThreadPool(size_t size);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    void add_task(std::unique_ptr<Task> task);

private:
    static void* worker_entry(void* arg);
    void worker_loop();

    bool stop_ = false;
    pthread_mutex_t queue_mutex_;
    pthread_cond_t queue_cond_;
    std::list<std::unique_ptr<Task>> task_queue_;
    std::vector<pthread_t> threads_;
};

// --- 服务器状态与业务逻辑函数 ---

// 客户端信息
struct ClientInfo {
    std::string ip;
    int port;
    std::string read_buf;  // 用于处理半包/粘包的读缓冲区
    std::string write_buf;
};

// 服务器上下文/状态集合
struct ServerContext {
    int epoll_fd;
    std::unordered_map<int, ClientInfo> clients;
    pthread_mutex_t clients_mutex; // 用于保护 clients map 的互斥锁
};

// --- 全局业务逻辑函数 ---
void set_non_blocking(int fd);
void add_fd_to_epoll(int epoll_fd, int fd, uint32_t events);
void modify_fd_in_epoll(int epoll_fd, int fd, uint32_t events);
void remove_fd_from_epoll(int epoll_fd, int fd);
void disconnect_client(ServerContext& context, int fd);
bool parse_message(const std::string& raw_buf, std::string& target_ip, int& target_port, std::string& msg_content);
void handle_new_connection(int listen_fd, ServerContext& context);
void handle_read_event(ServerContext& context, int fd);
void handle_write_event(ServerContext& context, int fd);

// --- 具体任务类 ---
class ReadTask : public Task {
public:
    ReadTask(ServerContext& context, int fd) : context_(context), fd_(fd) {}
    void execute() override {
        handle_read_event(context_, fd_);
    }
private:
    ServerContext& context_;
    int fd_;
};

class WriteTask : public Task {
public:
    WriteTask(ServerContext& context, int fd) : context_(context), fd_(fd) {}
    void execute() override {
        handle_write_event(context_, fd_);
    }
private:
    ServerContext& context_;
    int fd_;
};

// --- ThreadPool 类实现 ---
ThreadPool::ThreadPool(size_t size) {
    pthread_mutex_init(&queue_mutex_, nullptr);
    pthread_cond_init(&queue_cond_, nullptr);
    threads_.resize(size);
    for (size_t i = 0; i < size; ++i) {
        if (pthread_create(&threads_[i], nullptr, worker_entry, this) != 0) {
            throw std::runtime_error("无法创建线程");
        }
    }
}
ThreadPool::~ThreadPool() {
    {
        pthread_mutex_lock(&queue_mutex_);
        stop_ = true;
        pthread_mutex_unlock(&queue_mutex_);
    }
    pthread_cond_broadcast(&queue_cond_);
    for (pthread_t& thread : threads_) {
        pthread_join(thread, nullptr);
    }
    pthread_mutex_destroy(&queue_mutex_);
    pthread_cond_destroy(&queue_cond_);
}
void ThreadPool::add_task(std::unique_ptr<Task> task) {
    pthread_mutex_lock(&queue_mutex_);
    task_queue_.push_back(std::move(task));
    pthread_mutex_unlock(&queue_mutex_);
    pthread_cond_signal(&queue_cond_);
}
void* ThreadPool::worker_entry(void* arg) {
    static_cast<ThreadPool*>(arg)->worker_loop();
    return nullptr;
}
void ThreadPool::worker_loop() {
    while (true) {
        std::unique_ptr<Task> task;
        {
            pthread_mutex_lock(&queue_mutex_);
            while (!stop_ && task_queue_.empty()) {
                pthread_cond_wait(&queue_cond_, &queue_mutex_);
            }
            if (stop_ && task_queue_.empty()) {
                pthread_mutex_unlock(&queue_mutex_);
                return;
            }
            task = std::move(task_queue_.front());
            task_queue_.pop_front();
            pthread_mutex_unlock(&queue_mutex_);
        }
        if (task) {
            task->execute();
        }
    }
}

// --- 全局函数实现 ---
void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) throw std::system_error(errno, std::generic_category(), "fcntl F_GETFL");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) throw std::system_error(errno, std::generic_category(), "fcntl F_SETFL");
}
void add_fd_to_epoll(int epoll_fd, int fd, uint32_t events) {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) std::cerr << "添加 fd 到 epoll 失败: " << strerror(errno) << std::endl;
}
void modify_fd_in_epoll(int epoll_fd, int fd, uint32_t events) {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) std::cerr << "修改 epoll 中的 fd 失败: " << strerror(errno) << std::endl;
}
void remove_fd_from_epoll(int epoll_fd, int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        if (errno != EBADF) std::cerr << "从 epoll 移除 fd 失败: " << strerror(errno) << std::endl;
    }
}
void disconnect_client(ServerContext& context, int fd) {
    pthread_mutex_lock(&context.clients_mutex);
    if (context.clients.count(fd)) {
        const auto& client = context.clients.at(fd);
        std::cout << "客户端断开: " << client.ip << ":" << client.port << " (fd: " << fd << ")" << std::endl;
        remove_fd_from_epoll(context.epoll_fd, fd);
        close(fd);
        context.clients.erase(fd);
    }
    pthread_mutex_unlock(&context.clients_mutex);
}
bool parse_message(const std::string& raw_buf, std::string& target_ip, int& target_port, std::string& msg_content) {
    size_t first_colon = raw_buf.find(':');
    if (first_colon == std::string::npos) return false;
    size_t second_colon = raw_buf.find(':', first_colon + 1);
    if (second_colon == std::string::npos) return false;
    target_ip = raw_buf.substr(0, first_colon);
    try {
        target_port = std::stoi(raw_buf.substr(first_colon + 1, second_colon - first_colon - 1));
    } catch (...) {
        return false;
    }
    msg_content = raw_buf.substr(second_colon + 1) ;
    return true;
}
void handle_new_connection(int listen_fd, ServerContext& context) {
    while (true) {
        sockaddr_in cli_addr{};
        socklen_t cli_len = sizeof(cli_addr);
        int conn_fd = accept(listen_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if (conn_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            std::cerr << "accept 错误: " << strerror(errno) << std::endl;
            break;
        }
        set_non_blocking(conn_fd);
        add_fd_to_epoll(context.epoll_fd, conn_fd, EPOLLIN | EPOLLET);
        ClientInfo new_client;
        char client_ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip_str, INET_ADDRSTRLEN);
        new_client.ip = client_ip_str;
        new_client.port = ntohs(cli_addr.sin_port);
        pthread_mutex_lock(&context.clients_mutex);
        context.clients[conn_fd] = new_client;
        pthread_mutex_unlock(&context.clients_mutex);
        std::cout << "新客户端连接: " << new_client.ip << ":" << new_client.port << " (fd: " << conn_fd << ")" << std::endl;
    }
}

/**
 * @brief 处理读事件，包含半包和粘包处理逻辑
 */
void handle_read_event(ServerContext& context, int fd) {
    char buffer[1024];
    bool connection_closed = false;

    // 1. 从 socket 尽可能多地读取数据
    while (true) {
        int n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            // 将读取到的数据追加到对应客户端的读缓冲区
            pthread_mutex_lock(&context.clients_mutex);
            if (context.clients.count(fd)) {
                context.clients[fd].read_buf.append(buffer, n);
            }
            pthread_mutex_unlock(&context.clients_mutex);
        } else if (n == 0) {
            connection_closed = true;
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            //strerror(errno)根据errno 提供一个描述性字符串
            std::cerr << "fd " << fd << " 读取错误: " << strerror(errno) << std::endl;
            connection_closed = true;
            break;
        }
    }

    // 2. 循环处理读缓冲区中的完整消息
    pthread_mutex_lock(&context.clients_mutex);
    if (context.clients.count(fd)) {
        std::string& read_buf = context.clients.at(fd).read_buf;
        size_t pos;
        // 只要能找到分隔符'\n'，就循环处理
        while ((pos = read_buf.find('\n')) != std::string::npos) {
            // a. 提取一条完整的消息
            std::string message = read_buf.substr(0, pos);
            // b. 从读缓冲区移除已提取的消息（包括'\n'）
            read_buf.erase(0, pos + 1);

            // c. 兼容Telnet，它会发送\r\n，移除\r
            if (!message.empty() && message.back() == '\r') {
                message.pop_back();
            }

            // 如果消息为空（例如只有一个回车），则忽略
            if (message.empty()) {
                continue;
            }

            // d. 解析并处理这条完整的消息
            std::string target_ip, msg_content;
            int target_port;
            if (!parse_message(message, target_ip, target_port, msg_content)) {
                context.clients[fd].write_buf += "无效的消息格式. 请使用: IP:PORT:MESSAGE\n";
                modify_fd_in_epoll(context.epoll_fd, fd, EPOLLIN | EPOLLOUT | EPOLLET);
                continue;
            }
            
            int target_fd = -1;
            for (const auto& pair : context.clients) {
                if (pair.second.ip == target_ip && pair.second.port == target_port) {
                    target_fd = pair.first;
                    break;
                }
            }
            
            if (target_fd != -1 && context.clients.count(target_fd)) {
                context.clients[target_fd].write_buf += msg_content;
                modify_fd_in_epoll(context.epoll_fd, target_fd, EPOLLIN | EPOLLOUT | EPOLLET);
            } else {
                context.clients[fd].write_buf += "目标客户端未找到\n";
                modify_fd_in_epoll(context.epoll_fd, fd, EPOLLIN | EPOLLOUT | EPOLLET);
            }
        }
    }
    pthread_mutex_unlock(&context.clients_mutex);

    // 3. 如果连接已关闭，则清理客户端资源
    if (connection_closed) {
        disconnect_client(context, fd);
    }
}

void handle_write_event(ServerContext& context, int fd) {
    std::string write_buf_copy;
    pthread_mutex_lock(&context.clients_mutex);
    if (context.clients.find(fd) == context.clients.end()) {
        pthread_mutex_unlock(&context.clients_mutex);
        return;
    }
    write_buf_copy = context.clients.at(fd).write_buf;
    pthread_mutex_unlock(&context.clients_mutex);
    if (write_buf_copy.empty()) return;
    while (!write_buf_copy.empty()) {
        int n = write(fd, write_buf_copy.c_str(), write_buf_copy.length());
        if (n > 0) {
            write_buf_copy.erase(0, n);
        } else {
            if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
            std::cerr << "fd " << fd << " 写入错误: " << strerror(errno) << std::endl;
            disconnect_client(context, fd);
            return;
        }
    }
    pthread_mutex_lock(&context.clients_mutex);
    if (context.clients.count(fd)) {
        auto& original_buf = context.clients.at(fd).write_buf;
        original_buf.erase(0, original_buf.length() - write_buf_copy.length());
        if (original_buf.empty()) {
            modify_fd_in_epoll(context.epoll_fd, fd, EPOLLIN | EPOLLET);
        }
    }
    pthread_mutex_unlock(&context.clients_mutex);
}

// --- 程序入口 main 函数 ---
/*异常处理的错误分两种：
1.致命的初始化错误 (Fatal Initialization Errors)：
描述：这类错误发生在服务器启动阶段，例如端口被占用、创建socket失败、内存不足无法创建线程等。一旦发生，服务器就无法正常运行，整个程序应该立即终止。
处理方式：抛出异常 (throw)。
2.可恢复的运行时错误 (Recoverable Runtime Errors)：
描述：这类错误发生在服务器运行过程中，通常与客户端的I/O操作有关。例如，客户端突然断开连接、读/写时内核缓冲区暂时已满 (EAGAIN/EWOULDBLOCK)。这些是高并发服务器的正常现象，不应该导致整个服务器崩溃。
处理方式：检查函数返回值和 errno，并采取相应措施（例如，忽略 EAGAIN、关闭断开的连接、打印错误日志等），但不抛出异常。
*/
int main() {
    int listen_fd = -1;
    ServerContext context;
    context.epoll_fd = -1;
    pthread_mutex_init(&context.clients_mutex, nullptr);

    try {
        ThreadPool pool(4);
        const int port = 8888;
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        /*std::system_error( int ec, const std::error_category& cat, const std::string& what_arg )
         	参数一：错误码，
        	参数2：std::generic_category() 就是一本“errno 整数翻译成人类可读字符串”的字典
        	参数3：由程序员提供的自定义上下文信息*/
        if (listen_fd < 0) throw std::system_error(errno, std::generic_category(), "socket");
        int opt = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(port);
        if (bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) throw std::system_error(errno, std::generic_category(), "bind");
        if (listen(listen_fd, 128) < 0) throw std::system_error(errno, std::generic_category(), "listen");
        set_non_blocking(listen_fd);
        context.epoll_fd = epoll_create1(0);
        if (context.epoll_fd == -1) throw std::system_error(errno, std::generic_category(), "epoll_create1");
        add_fd_to_epoll(context.epoll_fd, listen_fd, EPOLLIN | EPOLLET);
        std::cout << "服务器已启动，端口号: " << port << std::endl;
        std::vector<epoll_event> events(128);
        while (true) {
            int n_fds = epoll_wait(context.epoll_fd, events.data(), 128, -1);
            if (n_fds < 0) {
                if (errno == EINTR) continue;
                std::cerr << "epoll_wait 失败: " << strerror(errno) << std::endl;
                break;
            }
            for (int i = 0; i < n_fds; ++i) {
                int fd = events[i].data.fd;
                if (fd == listen_fd) {
                    handle_new_connection(listen_fd, context);
                } else {
                    if (events[i].events & EPOLLIN) {
                        pool.add_task(std::make_unique<ReadTask>(context, fd));
                    }
                    if (events[i].events & EPOLLOUT) {
                        pool.add_task(std::make_unique<WriteTask>(context, fd));
                    }
                }
            }
        }
        /*what()
        virtual const char* what() const noexcept
        	这是所有 std::exception 派生类都必须实现的函数。它返回一个C风格的字符串，包含了对错误的完整描述。
        std::system_error 会非常智能地将你构造时提供的两个字符串信息组合起来。它的输出通常是 "what_arg: category_message" 的格式
        
        code()
	签名：const std::error_code& code() const noexcept;
	作用：这个函数返回一个 std::error_code 对象。std::error_code 是对错误码和错误类别的轻量级封装。可以把它看作是 std::system_error 的“非异常”版本。
	用途：当需要以编程方式检查具体的错误类型时，这个函数非常有用。
	std::error_code 的常用函数：
	value(): 返回原始的整数错误码 (例如, 13)。
	message(): 返回由错误类别翻译后的错误字符串 (例如, "Permission denied")。
	category(): 返回原始的 std::error_category 对象。
	使用方法：
	const std::error_code& ec = e.code();
        std::cerr << "错误码对象信息:" << std::endl;
        std::cerr << "  - 错误值 (ec.value()): " << ec.value() << std::endl;
        std::cerr << "  - 错误消息 (ec.message()): " << ec.message() << std::endl;
        std::cerr << "  - 错误类别 (ec.category().name()): " << ec.category().name() << std::endl;*/
		
    } catch (const std::exception& e) {
        std::cerr << "主函数中未捕获的异常: " << e.what() << std::endl;
    }
    if (listen_fd != -1) close(listen_fd);
    if (context.epoll_fd != -1) close(context.epoll_fd);
    pthread_mutex_destroy(&context.clients_mutex);
    return 0;
}
