#ifndef TEST_AUTH_H
#define TEST_AUTH_H

#include <QtTest/QtTest>
#include "auth.h"
#include "ClientManager.h"

class TestAuth : public QObject {
    Q_OBJECT

private slots:
    void init(); // Метод для инициализации тестов
    void cleanup(); // Метод для очистки после тестов

    void testAuthenticationSuccess();
    void testAuthenticationFailure();
    void testInvalidUserId();

private:
    ClientManager *clientManager;
    auth *authDialog;
};

#endif // TEST_AUTH_H


