#include "ClientManager.h"
#include <QDebug>

ClientManager::ClientManager(QObject *parent) : QObject(parent) {

}

ClientManager::~ClientManager() {
    for (auto it = userClients.begin(); it != userClients.end(); ++it) {
        delete it.value();
    }
    userClients.clear();
}

void ClientManager::addClient(int userId, Client *client) {
    if (!userClients.contains(userId)) {
        userClients[userId] = client;
        connect(client, &Client::historyReceived, this, &ClientManager::onClientHistoryReceived);
    } else {
        qDebug() << "Клиент для пользователя " << userId << " уже существует.";
        delete client;
    }
}

void ClientManager::removeClient(int userId) {
    if (userClients.contains(userId)) {
        Client *client = userClients[userId];
        userClients.remove(userId);
        delete client;
    }
}

QByteArray ClientManager::authenticateUser (const QString &login, const QString &password) {
    Client tempClient(this);
    QByteArray response = tempClient.auth(login, password);

    if (response.startsWith("auth&success&")) {
        QStringList parts = QString::fromUtf8(response).split("&");
        if (parts.size() == 3) {
            int userId = parts[2].toInt();
            Client *newClient = new Client(this);
            addClient(userId, newClient);
            return response; // Возвращаем ответ об успешной аутентификации
        }
    }
    return response; // Возвращаем ответ об ошибке
}

QByteArray ClientManager::registerUser (const QString &login, const QString &password, const QString &email) {
    Client tempClient(this);
    QByteArray response = tempClient.reg(login, password, email);

    if (response.startsWith("reg&success&")) {
        QStringList parts = QString::fromUtf8(response).split("&");
        if (parts.size() == 3) {
            int userId = parts[2].toInt();
            Client *newClient = new Client(this);
            addClient(userId, newClient);
            return response; // Возвращаем ответ об успешной регистрации
        }
    }
    return response; // Возвращаем ответ об ошибке
}


QByteArray ClientManager::saveCalculation(int userId, const QString &operation) {
    if (userClients.contains(userId)) {
        QByteArray response = userClients[userId]->saveCalculation(userId, operation);
        return response;
    }
    return QByteArray();
}

void ClientManager::requestHistory(int userId, int limit) {
    if (userClients.contains(userId)) {
        userClients[userId]->getHistory(userId, limit);
    }
}

void ClientManager::onClientHistoryReceived(int userId, const QStringList &history) {
    emit historyReceived(userId, history);
}

