#include "mytcpserver.h"
#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);//выделяем память под сервер

    connect(mTcpServer, &QTcpServer::newConnection,//связываем сигнал который издает сервер с тем действием которое он же должен совершить
            this, &MyTcpServer::slotNewConnection);//источник сигнала, имя сигнала, получатель сигнала, что должен исполнить получатель

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){//с любого айпи на порт 33333
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    mTcpSocket = mTcpServer->nextPendingConnection();
    if (!mTcpSocket->isOpen()) {
        qDebug() << "Сокет не открыт";
        return;
    }
    if (!mTcpSocket->write("Hello, World!!! I am echo server!\r\n")) {
        qDebug() << "Ошибка записи в сокет";
        return;
    }
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);//если готов к чтению
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);//если клиент отключился
}

void MyTcpServer::slotServerRead() {
    QString res = "";
    while (mTcpSocket->bytesAvailable() > 0) {
        QByteArray array = mTcpSocket->readAll();
        if (array.isEmpty()) {
            qDebug() << "Пустой массив";
            return;
        }
        res.append(array);
    }

    qDebug() << "Получено от клиента: " << res.toUtf8(); // вывод в консоль сервера


    QByteArray response = parsing(res); // Получаем ответ от функции parsing
    if (!mTcpSocket->write(response)) { // Записываем response в сокет
        qDebug() << "Ошибка записи в сокет";
        return;
    }
}

void MyTcpServer::slotClientDisconnected(){
    if (mTcpSocket->isOpen()) {
        mTcpSocket->close();
    }
    delete mTcpSocket;
}

