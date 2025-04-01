#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <QtTest/QtTest>
#include "client.h"
#include <QSignalSpy>
#include <QStringList>

class TestClient : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(); // Инициализация перед тестами
    void cleanupTestCase(); // Очистка после тестов

    void testAuth_success();
    void testAuth_failure();
    void testReg_success();
    void testReg_failure();
    void testSaveCalculation_success();
    void testGetHistory(); // Объявляем testGetHistory как слот

    void onHistoryReceived(int userId, const QStringList& history) {
        receivedHistory = history; // Сохраняем полученную историю
    }

private:
    Client *client; // Указатель на тестируемый объект
    QStringList receivedHistory; // Переменная для хранения полученной истории
};

#endif // TEST_CLIENT_H


