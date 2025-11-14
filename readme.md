本项目实现了局域网内一对一的网络聊天室

技术栈： C++, Linux, TCP/IP, Epoll, Reactor 模式, 线程池, RAII, Socket

运行方法
1.首先编译服务器代码
g++ s.cpp -o s

2。使用QT编译客户端(或者用telnet测试）

3.启动服务器
./s 127.0.0.1 8888

4.双击打开TcpChat/tcpchat

打开两个聊天窗口

输入服务器ip端口后点击确认

输入自己和对方的ip和端口，发送消息

因为服务器以回车分隔消息，所以消息必须以回车结尾

如果用telnet测试，需要netstat -anp | grep telnet查看telnet占用的端口并以

目标ip:目标端口:消息

的形式发送消息，telnet会自动加上\n，这也是设计\n为分隔符的初衷

