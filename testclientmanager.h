#ifndef TESTCLIENTMANAGER_H
#define TESTCLIENTMANAGER_H

#include <QObject>
#include <QTest>
#include "ClientManager.h"
#include "Client.h" // Убедитесь, что у вас есть этот заголовочный файл

class TestClientManager : public QObject {
    Q_OBJECT

private slots:
    void init();      // Инициализация перед каждым тестом
    void cleanup();   // Очистка после каждого теста

    void testAddClient();           // Тест добавления клиента
    void testRemoveClient();        // Тест удаления клиента
    void testAuthenticateUser ();    // Тест аутентификации пользователя
    void testRegisterUser ();        // Тест регистрации пользователя
    void testSaveCalculation();      // Тест сохранения вычисления
    void testRequestHistory();       // Тест запроса истории

private:
    ClientManager *manager; // Указатель на объект ClientManager
};

#endif // TESTCLIENTMANAGER_H
