#ifndef TCPTCHAT_H
#define TCPTCHAT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class TcpChat;
}

class TcpChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpChat(QWidget *parent = nullptr);
    ~TcpChat();

signals:
    // 自定义信号，用于触发窗口振铃（晃动）效果
    void Bell();

private slots:
    // 连接服务器按钮的槽函数
    void on_pushButtonConnect_clicked();
    // 发送消息按钮的槽函数
    void on_pushButtonSend_clicked();
    // 振铃按钮的槽函数
    void on_pushButtonBell_clicked();

    // 当与服务器的连接成功时触发
    void on_connected();
    // 当服务器有数据发送过来时触发
    void on_readyRead();
    // 当服务器断开连接时触发
    void on_disconnected();

    // 定时器超时槽，用于实现窗口晃动效果
    void on_timer1_timeout();
    // 响应 Bell 信号，进行窗口晃动
    void on_Bell();

    // 退出程序按钮
    void on_pushButtonExit_clicked();
    // 清除发送框内容按钮
    void on_pushButtonClearSend_clicked();
    // 清除接收框内容按钮
    void on_pushButtonClearRecv_clicked();

private:
    Ui::TcpChat *ui;

    // 与服务器通信的 socket 对象
    QTcpSocket *m_socket;

    // 定时器对象，用于实现窗口振铃效果
    QTimer *timer1;

    // 计时变量，用于控制窗口振铃（晃动）次数
    int m_time;
    // 保存窗口晃动时的起始位置
    int m_X, m_Y;
    // 标记是否发送“振铃”消息
    bool m_bBell;
};

#endif // TCPTCHAT_H

