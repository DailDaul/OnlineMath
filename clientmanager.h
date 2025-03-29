#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include "Client.h"

class ClientManager : public QObject {
    Q_OBJECT

public:
    explicit ClientManager(QObject *parent = nullptr);
    ~ClientManager() override;

    void requestHistory(int userId, int limit);
    QByteArray authenticateUser(const QString &login, const QString &password);
    QByteArray registerUser(const QString &login, const QString &password, const QString &email);
    QByteArray saveCalculation(int userId, const QString &operation);

    void addClient(int userId, Client *client);
    void removeClient(int userId);

signals:
    void historyReceived(int userId, const QStringList &history);

private slots:
    void onClientHistoryReceived(int userId, const QStringList &history);

private:
    QMap<int, Client*> userClients;

};

#endif // CLIENTMANAGER_H
