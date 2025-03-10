#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>//базовый класс для всех объектов Qt
#include <QTcpServer>// класс для создания TCP-сервера
#include <QTcpSocket>// класс для работы с TCP-соединениями

#include <QtNetwork>//модуль, предоставляющий сетевую функциональность
#include <QByteArray>//класс для работы с массивами байтов
#include <QDebug>//класс для отладки и вывода сообщений

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();//будет вызываться при новом подключении клиента
    void slotClientDisconnected();//будет вызываться, когда клиент отключается
    void slotServerRead();//будет вызываться для чтения данных от клиента
private:
    QTcpServer * mTcpServer;//указатель на объект TCP-сервера
    QTcpSocket * mTcpSocket;//указатель на объект TCP-сокета для взаимодействия с клиентами
    //int server_status;
};
#endif // MYTCPSERVER_H
