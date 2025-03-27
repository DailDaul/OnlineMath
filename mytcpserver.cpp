#include "mytcpserver.h"
#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

MyTcpServer::~MyTcpServer()
{
    mTсpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTсpServer = new QTcpServer(this);
    mSockets = QList<QTcpSocket*>();

    connect(mTсpServer, &QTcpServer::newConnection,//связываем сигнал который издает сервер с тем действием которое он же должен совершить
            this, &MyTcpServer::slotNewConnection);//источник сигнала, имя сигнала, получатель сигнала, что должен исполнить получатель

    if(!mTсpServer->listen(QHostAddress::Any, 33333)){//с любого айпи на порт 33333
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket *socket =  mTсpServer->nextPendingConnection(); //Получаем сокет нового подключения
    if (!socket->isOpen()) {
        qDebug() << "Сокет не открыт";
        socket->deleteLater(); //Удаляем сокет, если он не открылся
        return;
    }

    //if (!mTcpSocket->write("Hello!!! I am calculator!\r\n")) {
    //   qDebug() << "Ошибка записи в сокет";
    //    return;
    //}
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);//если готов к чтению, используем сокет
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);//если клиент отключился, используем сокет

    mSockets.append(socket); // Добавляем сокет в список активных сокетов
    qDebug() << "Новое подключение. Всего клиентов:" << mSockets.size();
}

void MyTcpServer::slotServerRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender()); // Определяем сокет, который вызвал сигнал

    if (!socket) {
        qDebug() << "Ошибка: не удалось определить сокет, вызвавший сигнал";
        return;
    }
    QString res = "";
    while (socket->bytesAvailable() > 0) {
        QByteArray array = socket->readAll();
        if (array.isEmpty()) {
            qDebug() << "Пустой массив";
            return;
        }
        res.append(array);
    }

    qDebug() << "Получено от клиента: " << res.toUtf8(); // вывод в консоль сервера


    QByteArray response = parsing(res); // Получаем ответ от функции parsing
    if (!socket->write(response)) { // Записываем response в сокет
        qDebug() << "Ошибка записи в сокет";
        return;
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket) {
        qDebug() << "Ошибка: не удалось определить сокет, вызвавший сигнал об отключении";
        return;
    }

    // 1. Отключаем все сигналы и слоты для этого сокета
    disconnect(socket, nullptr, this, nullptr);

    // 2. Убеждаемся, что все данные отправлены
    if (socket->bytesToWrite() > 0) {
        socket->flush(); // Пытаемся отправить оставшиеся данные
        socket->waitForBytesWritten(1000); // Ждем 1 секунду
    }

    // 3. Закрываем сокет
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->waitForDisconnected(1000); // Ждем отключения
    }

    // 4. Удаляем сокет из списка и безопасно удаляем его
    mSockets.removeOne(socket);
    socket->deleteLater();
    qDebug() << "Клиент отключился. Осталось клиентов:" << mSockets.size();
}
