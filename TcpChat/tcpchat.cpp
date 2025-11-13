#include "tcpchat.h"
#include "ui_tcpchat.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QString>
#include <QDebug>

TcpChat::TcpChat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpChat)
{
    ui->setupUi(this);

    // 初始化用于窗口闪动的定时器
    m_time = 0;
    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(on_timer1_timeout()));
    timer1->start(50);
    m_X = 0; m_Y = 0;
    m_bBell = false;

    // 创建与中央服务器保持连接的 QTcpSocket 对象
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    // 将本机所有 IP 显示到 listWidget 中供参考（或选择）
    QString str;
    int i, j;
    j = QNetworkInterface::allAddresses().count();
    for(i = 0; i < j; i++) {
        str = QNetworkInterface::allAddresses().at(i).toString();
        ui->lineEdit_localIP->addItem(str);
    }
    connect(this, SIGNAL(Bell()), this, SLOT(on_Bell()));


}

TcpChat::~TcpChat()
{
    // 关闭与服务器的连接
    if(m_socket->isOpen())
        m_socket->close();
    delete ui;
}

// 点击“连接服务器”按钮
void TcpChat::on_pushButtonConnect_clicked()
{
    // 读取本地 IP 和端口，用于绑定本地端口（例如：lineEdit_localIP、lineEdit_localPort）
    // 如果你的 local IP 控件是一个 QListWidget，从当前选中的项获取 IP 地址
        QString localIP;
        QListWidgetItem *item = ui->lineEdit_localIP->currentItem();
        if (item) {
            localIP = item->text();
        } else {
            // 如果没有选中项，则可以使用一个默认值（例如 127.0.0.1），也可以弹出错误提示
            localIP = "127.0.0.1";
            qDebug() << "No local IP selected, defaulting to" << localIP;
        }
        int localPort = ui->lineEdit_localPort->text().toInt();

        // 绑定本地地址和端口
        if (!m_socket->bind(QHostAddress(localIP), localPort)) {
            qDebug() << "Bind error:" << m_socket->errorString();
            // 可以在这里提示用户绑定失败，返回，或继续尝试
        } else {
            qDebug() << "Successfully bound to local address:" << localIP << "port:" << localPort;
        }
    // 从界面上读取服务器 IP 和端口（例如：lineEdit_serverIP、lineEdit_serverPort）
    QString serverIP = ui->lineEdit_serverIP->text();
    int serverPort = ui->lineEdit_serverPort->text().toInt();
    m_socket->connectToHost(serverIP, serverPort, QIODevice::ReadWrite);
}

// 连接成功后更新界面状态（可选）
void TcpChat::on_connected()
{
    ui->statusBar->showMessage("已连接到服务器");
    qDebug() << "Connected to server.";
}

// 接收服务器转发的消息
void TcpChat::on_readyRead()
{
    QByteArray data = m_socket->readAll();
    QString str(data);

    // 如果收到特殊字符串 "bell"，则触发窗口振铃效果
    if(str.trimmed() == "bell") {
        emit Bell();
    } else {
        // 否则将收到的消息追加显示到接收消息区域中
        ui->textEdit_recv->append(str);
    }
}

// 连接断开后（可选的处理）
void TcpChat::on_disconnected()
{
    ui->statusBar->showMessage("服务器连接断开");
    qDebug() << "Disconnected from server.";
}

// 点击“发送”按钮后执行
void TcpChat::on_pushButtonSend_clicked()
{
    // 从界面读取“目标 IP”、“目标端口”和“消息内容”
    QString targetIP = ui->lineEdit_targetIP->text();
    QString targetPort = ui->lineEdit_targetPort->text();
    QString msgContent = ui->textEdit_msgContent->toPlainText();

    QString sendMsg;
    if(m_bBell)
        sendMsg = targetIP + ":" + targetPort + ":" +"bell";
    else
        // 构造发送格式： 目标IP:目标端口:消息内容
        sendMsg = targetIP + ":" + targetPort + ":" + msgContent;

    m_bBell = false;

    // 发送时附加字符串结束符（如果服务器端要求）
    QByteArray outData = sendMsg.toUtf8();
    // 注意：是否要发送 \0 可以与服务器协商，本例直接发送文本即可
    m_socket->write(outData);
    bool b = m_socket->waitForBytesWritten(200);
    if(!b)
        qDebug() << "Write timeout or error.";
}

// 点击“振铃”按钮时的槽函数
void TcpChat::on_pushButtonBell_clicked()
{
    m_bBell = true;
    // 此处可以自动调用发送（比如采用默认目标为某个特定客户端，或发出 bell 消息）
    // 如果你希望立即发送振铃消息，可以调用 on_pushButtonSend_clicked()
    on_pushButtonSend_clicked();
}

// 振铃信号（Bell）处理槽，与之前保持一致，用于窗口晃动效果
void TcpChat::on_Bell()
{
    m_time = 26;
    m_X = this->geometry().x() + 7;
    m_Y = this->geometry().y() - 23;
}

// 定时器槽，每 50 毫秒触发一次，用来实现窗口晃动效果
void TcpChat::on_timer1_timeout()
{
    static bool b;
    if(m_time > 0) {
        b = !b;
        if(b)
            this->move(m_X + 10, m_Y);
        else
            this->move(m_X - 10, m_Y);
        m_time--;
    } else {
        m_time = 0;
    }
}

// 退出程序按钮
void TcpChat::on_pushButtonExit_clicked()
{
    this->close();
}

// 清除发送框内容
void TcpChat::on_pushButtonClearSend_clicked()
{
    ui->textEdit_msgContent->clear();
}

// 清除接收框内容
void TcpChat::on_pushButtonClearRecv_clicked()
{
    ui->textEdit_recv->clear();
}

