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
    //if (!mTcpSocket->write("Hello!!! I am calculator!\r\n")) {
    //   qDebug() << "Ошибка записи в сокет";
    //    return;
    //}
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
    // 1. Отключаем все сигналы и слоты
    disconnect(mTcpSocket, nullptr, this, nullptr);

    // 2. Убеждаемся, что все данные отправлены (пример)
    if (mTcpSocket->bytesToWrite() > 0) {
        mTcpSocket->flush(); // Пытаемся отправить оставшиеся данные
        mTcpSocket->waitForBytesWritten(1000); // Ждем 1 секунду
    }

    // 3. Закрываем сокет
    mTcpSocket->disconnectFromHost();

    // 4. Проверяем состояние сокета перед вызовом waitForDisconnected()
    if (mTcpSocket->state() == QAbstractSocket::ConnectedState) {
        mTcpSocket->waitForDisconnected(1000); // Ждем отключения
    }

    // 5. Удаляем сокет безопасно
    mTcpSocket->deleteLater();
    mTcpSocket = nullptr;
}
