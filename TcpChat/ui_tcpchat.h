/********************************************************************************
** Form generated from reading UI file 'tcpchat.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCHAT_H
#define UI_TCPCHAT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpChat
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonSend;
    QLabel *label_5;
    QToolButton *pushButtonBell;
    QTextEdit *textEdit_msgContent;
    QFrame *frame;
    QLineEdit *lineEdit_serverIP;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_localPort;
    QLineEdit *lineEdit_serverPort;
    QPushButton *pushButtonConnect;
    QListWidget *lineEdit_localIP;
    QLabel *label;
    QLabel *label_7;
    QLineEdit *lineEdit_targetIP;
    QLineEdit *lineEdit_targetPort;
    QLabel *label_6;
    QPushButton *pushButtonExi;
    QTextEdit *textEdit_recv;
    QPushButton *pushButtonClearRecv;
    QPushButton *pushButtonClearSend;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TcpChat)
    {
        if (TcpChat->objectName().isEmpty())
            TcpChat->setObjectName(QString::fromUtf8("TcpChat"));
        TcpChat->resize(531, 568);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        TcpChat->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("111.png"), QSize(), QIcon::Normal, QIcon::Off);
        TcpChat->setWindowIcon(icon);
        TcpChat->setIconSize(QSize(288, 236));
        TcpChat->setAnimated(true);
        centralWidget = new QWidget(TcpChat);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButtonSend = new QPushButton(centralWidget);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));
        pushButtonSend->setGeometry(QRect(30, 440, 91, 31));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 150, 71, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\200\235\346\272\220\351\273\221\344\275\223 CN"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label_5->setFont(font1);
        pushButtonBell = new QToolButton(centralWidget);
        pushButtonBell->setObjectName(QString::fromUtf8("pushButtonBell"));
        pushButtonBell->setGeometry(QRect(210, 270, 57, 59));
        pushButtonBell->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("bell.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonBell->setIcon(icon1);
        pushButtonBell->setIconSize(QSize(51, 51));
        pushButtonBell->setCheckable(true);
        pushButtonBell->setChecked(true);
        pushButtonBell->setAutoRepeat(true);
        pushButtonBell->setAutoExclusive(true);
        pushButtonBell->setPopupMode(QToolButton::DelayedPopup);
        pushButtonBell->setToolButtonStyle(Qt::ToolButtonIconOnly);
        pushButtonBell->setAutoRaise(true);
        textEdit_msgContent = new QTextEdit(centralWidget);
        textEdit_msgContent->setObjectName(QString::fromUtf8("textEdit_msgContent"));
        textEdit_msgContent->setGeometry(QRect(30, 340, 471, 76));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(20, 10, 471, 131));
        frame->setFont(font1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        lineEdit_serverIP = new QLineEdit(frame);
        lineEdit_serverIP->setObjectName(QString::fromUtf8("lineEdit_serverIP"));
        lineEdit_serverIP->setGeometry(QRect(70, 50, 171, 31));
        label_1 = new QLabel(frame);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(0, 20, 61, 21));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\346\200\235\346\272\220\351\273\221\344\275\223 CN"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        label_1->setFont(font2);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 20, 81, 21));
        label_2->setFont(font2);
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 50, 61, 31));
        label_3->setFont(font2);
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(250, 60, 81, 21));
        label_4->setFont(font2);
        lineEdit_localPort = new QLineEdit(frame);
        lineEdit_localPort->setObjectName(QString::fromUtf8("lineEdit_localPort"));
        lineEdit_localPort->setGeometry(QRect(340, 10, 61, 31));
        lineEdit_serverPort = new QLineEdit(frame);
        lineEdit_serverPort->setObjectName(QString::fromUtf8("lineEdit_serverPort"));
        lineEdit_serverPort->setGeometry(QRect(340, 50, 61, 31));
        pushButtonConnect = new QPushButton(frame);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));
        pushButtonConnect->setGeometry(QRect(420, 20, 41, 41));
        pushButtonConnect->setFont(font2);
        lineEdit_localIP = new QListWidget(frame);
        lineEdit_localIP->setObjectName(QString::fromUtf8("lineEdit_localIP"));
        lineEdit_localIP->setEnabled(true);
        lineEdit_localIP->setGeometry(QRect(70, 10, 171, 31));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_localIP->sizePolicy().hasHeightForWidth());
        lineEdit_localIP->setSizePolicy(sizePolicy);
        QFont font3;
        font3.setFamily(QString::fromUtf8("\346\200\235\346\272\220\351\273\221\344\275\223 CN"));
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        lineEdit_localIP->setFont(font3);
        lineEdit_localIP->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 90, 67, 17));
        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(250, 90, 67, 17));
        lineEdit_targetIP = new QLineEdit(frame);
        lineEdit_targetIP->setObjectName(QString::fromUtf8("lineEdit_targetIP"));
        lineEdit_targetIP->setGeometry(QRect(70, 90, 171, 31));
        lineEdit_targetPort = new QLineEdit(frame);
        lineEdit_targetPort->setObjectName(QString::fromUtf8("lineEdit_targetPort"));
        lineEdit_targetPort->setGeometry(QRect(340, 90, 61, 31));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 300, 61, 21));
        label_6->setFont(font1);
        pushButtonExi = new QPushButton(centralWidget);
        pushButtonExi->setObjectName(QString::fromUtf8("pushButtonExi"));
        pushButtonExi->setGeometry(QRect(220, 440, 91, 31));
        textEdit_recv = new QTextEdit(centralWidget);
        textEdit_recv->setObjectName(QString::fromUtf8("textEdit_recv"));
        textEdit_recv->setEnabled(true);
        textEdit_recv->setGeometry(QRect(20, 190, 471, 76));
        pushButtonClearRecv = new QPushButton(centralWidget);
        pushButtonClearRecv->setObjectName(QString::fromUtf8("pushButtonClearRecv"));
        pushButtonClearRecv->setGeometry(QRect(390, 290, 91, 31));
        pushButtonClearSend = new QPushButton(centralWidget);
        pushButtonClearSend->setObjectName(QString::fromUtf8("pushButtonClearSend"));
        pushButtonClearSend->setGeometry(QRect(390, 440, 91, 31));
        TcpChat->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TcpChat);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 531, 25));
        TcpChat->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TcpChat);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TcpChat->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TcpChat);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TcpChat->setStatusBar(statusBar);

        retranslateUi(TcpChat);
        QObject::connect(pushButtonExi, SIGNAL(clicked()), TcpChat, SLOT(close()));

        QMetaObject::connectSlotsByName(TcpChat);
    } // setupUi

    void retranslateUi(QMainWindow *TcpChat)
    {
        TcpChat->setWindowTitle(QCoreApplication::translate("TcpChat", "TcpChat", nullptr));
        pushButtonSend->setText(QCoreApplication::translate("TcpChat", "\345\217\221\351\200\201", nullptr));
        label_5->setText(QCoreApplication::translate("TcpChat", "\346\224\266\345\210\260\357\274\232", nullptr));
        pushButtonBell->setText(QCoreApplication::translate("TcpChat", "BELL", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonBell->setShortcut(QString());
#endif // QT_CONFIG(shortcut)
        label_1->setText(QCoreApplication::translate("TcpChat", "\346\234\254\345\234\260IP\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("TcpChat", "\346\234\254\345\234\260\347\253\257\345\217\243\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("TcpChat", "\346\234\215\345\212\241\345\231\250IP\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("TcpChat", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("TcpChat", "\347\241\256\350\256\244", nullptr));
        label->setText(QCoreApplication::translate("TcpChat", "\345\257\271\346\226\271ip:", nullptr));
        label_7->setText(QCoreApplication::translate("TcpChat", "\345\257\271\346\226\271\347\253\257\345\217\243\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("TcpChat", "\345\217\221\345\207\272\357\274\232", nullptr));
        pushButtonExi->setText(QCoreApplication::translate("TcpChat", "\351\200\200\345\207\272", nullptr));
        pushButtonClearRecv->setText(QCoreApplication::translate("TcpChat", "\346\270\205\351\231\244\346\216\245\346\224\266", nullptr));
        pushButtonClearSend->setText(QCoreApplication::translate("TcpChat", "\346\270\205\351\231\244\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpChat: public Ui_TcpChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCHAT_H
