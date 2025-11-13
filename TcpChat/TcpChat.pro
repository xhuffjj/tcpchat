
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpChat

TEMPLATE = app


SOURCES += main.cpp\
        tcpchat.cpp



HEADERS  += tcpchat.h

FORMS    += tcpchat.ui
