#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDebug>
class SingletonClient : public QObject {
    Q_OBJECT

public:
    SingletonClient();

    // Получение единственного экземпляра класса
    static SingletonClient& getInstance();

    // Метод для отправки запроса на сервер
    QByteArray sendRequest(const QByteArray& request);

    // Запрет копирования и присваивания
    SingletonClient(const SingletonClient&) = delete;
    SingletonClient& operator=(const SingletonClient&) = delete;

    QByteArray auth(const QString& login, const QString& password);
    QByteArray reg(const QString& login, const QString& password, const QString& email);
    void getHistory(int userId, int limit);
    QByteArray saveCalculation(int userId, const QString& operation);

signals:
    void historyReceived(int userId, const QStringList& history);

private:
    // Указатель на сокет для связи с сервером
    QTcpSocket* socket;

    ~SingletonClient();
};

#endif // SINGLETONCLIENT_H

