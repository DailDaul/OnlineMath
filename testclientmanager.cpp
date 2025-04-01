#include "testclientmanager.h"

void TestClientManager::init() {
    manager = new ClientManager();
}

void TestClientManager::cleanup() {
    delete manager;
}

void TestClientManager::testAddClient() {
    Client *client = new Client();
    manager->addClient(1, client);
    QVERIFY(manager->userClients.contains(1)); // Проверяем, что клиент добавлен
}

void TestClientManager::testRemoveClient() {
    Client *client = new Client();
    manager->addClient(1, client);
    manager->removeClient(1);
    QVERIFY(!manager->userClients.contains(1)); // Проверяем, что клиент удален
}

void TestClientManager::testAuthenticateUser () {
    QByteArray response = manager->authenticateUser ("0", "0");
    QVERIFY(response.startsWith("auth&success&")); // Проверяем успешную аутентификацию
    QStringList parts = QString::fromUtf8(response).split("&");
    int userId = parts[2].toInt();
    QVERIFY(manager->userClients.contains(userId)); // Проверяем, что клиент добавлен
}

void TestClientManager::testRegisterUser () {
    QByteArray response = manager->registerUser ("99", "99", "email@example.com");
    QVERIFY(response.startsWith("reg&success&")); // Проверяем успешную регистрацию
    QStringList parts = QString::fromUtf8(response).split("&");
    int userId = parts[2].toInt();
    QVERIFY(manager->userClients.contains(userId)); // Проверяем, что клиент добавлен
}

void TestClientManager::testSaveCalculation() {
    Client *client = new Client();
    manager->addClient(1, client);
    QByteArray response = manager->saveCalculation(1, "5 + 5");
    QVERIFY(!response.isEmpty()); // Проверяем, что ответ не пустой
}

void TestClientManager::testRequestHistory() {
    Client *client = new Client();
    manager->addClient(1, client);
    manager->requestHistory(1, 10);
    // Здесь можно добавить проверку на сигнал historyReceived, если он реализован в Client
}



