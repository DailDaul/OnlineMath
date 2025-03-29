#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <QDebug>

class Client : public QObject {
    Q_OBJECT

public:
    Client(QObject *parent = nullptr);
    ~Client();

    QByteArray auth(const QString &login, const QString &password);
    QByteArray reg(const QString &login, const QString &password, const QString &email);
    QByteArray sendRequest(const QByteArray &request);
    QByteArray saveCalculation(int userId, const QString &operation);
    void getHistory(int userId, int limit);

signals:
    void historyReceived(int userId, const QStringList &history);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
