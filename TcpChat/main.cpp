#include "tcpchat.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//这是 Qt GUI 应用程序的启动核心。


    TcpChat w;

    w.show();//把窗口显示在屏幕上

    return a.exec();//它启动了 Qt 的 事件循环机制；

}



