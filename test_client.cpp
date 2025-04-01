#include "test_client.h"

void TestClient::initTestCase()
{
    client = new Client();
}

void TestClient::cleanupTestCase()
{
    delete client;
}

void TestClient::testAuth_success()
{
    QByteArray response = client->auth("0", "0");
    QVERIFY(response.startsWith("auth&success&"));
}

void TestClient::testAuth_failure()
{
    QByteArray response = client->auth("invalidUser ", "invalidPassword");
    QVERIFY(response.startsWith("auth&failed\n"));
}

void TestClient::testReg_success()
{
    QByteArray response = client->reg("70", "newPassword", "70");
    QVERIFY(response.startsWith("reg&success&"));
}

void TestClient::testReg_failure()
{
    QByteArray response = client->reg("0", "0", "existingUser @example.com");
    QVERIFY(response.startsWith("reg&failed"));
}

void TestClient::testSaveCalculation_success()
{
    QByteArray response = client->saveCalculation(1, "5 + 5");
    QVERIFY(response.startsWith("saveCalculation&success"));
}


void TestClient::testGetHistory() {
    QSignalSpy spy(client, &Client::historyReceived);

    // Подписываемся на сигнал, чтобы сохранить историю
    connect(client, &Client::historyReceived, this, &TestClient::onHistoryReceived);

    client->getHistory(19, 30); // Запрашиваем историю


    // Проверяем, что история не пуста
    QVERIFY(!receivedHistory.isEmpty());
}

