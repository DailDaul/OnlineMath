#include "test_auth.h"

void TestAuth::init() {
    clientManager = new ClientManager();
    authDialog = new auth(clientManager, nullptr);
}

void TestAuth::cleanup() {
    delete authDialog;
    delete clientManager;
}

void TestAuth::testAuthenticationSuccess() {
    // Подготовка данных
    QString login = "0";
    QString password = "0";

    // Настройка mock-ответа от ClientManager
    QByteArray response = "auth&success&19"; // Предполагаем, что 19 - это ID пользователя
    QSignalSpy spy(authDialog, &auth::accepted); // Сигнал, который будет вызван при успешной аутентификации

    // Вызываем метод аутентификации
    authDialog->ui->loginreg->setText(login);
    authDialog->ui->passwordforreg->setText(password);

    // Подменяем метод authenticateUser  для теста
    QSignalSpy authSpy(clientManager, &ClientManager::authenticateUser );
    // Здесь вы должны использовать механизм, который позволяет вам возвращать mock-ответ
    // Например, если у вас есть метод, который можно подменить, сделайте это здесь.

    authDialog->on_authButton_clicked();



}

void TestAuth::testAuthenticationFailure() {
    // Подготовка данных
    QString login = "invalidUser ";
    QString password = "invalidPassword";

    // Настройка mock-ответа от ClientManager
    QByteArray response = "auth&failure"; // Симулируем ошибку аутентификации
    QSignalSpy spy(authDialog, &auth::rejected); // Сигнал, который будет вызван при ошибке аутентификации

    // Вызываем метод аутентификации
    authDialog->ui->loginreg->setText(login);
    authDialog->ui->passwordforreg->setText(password);

    // Подменяем метод authenticateUser  для теста
    QSignalSpy authSpy(clientManager, &ClientManager::authenticateUser );
    // Здесь вы должны использовать механизм, который позволяет вам возвращать mock-ответ

    authDialog->on_authButton_clicked();


}

void TestAuth::testInvalidUserId() {
    // Подготовка данных
    QString login = "validUser ";
    QString password = "validPassword";

    // Настройка mock-ответа от ClientManager
    QByteArray response = "auth&success&invalid"; // Неверный ID пользователя
    QSignalSpy spy(authDialog, &auth::rejected); // Сигнал, который будет вызван при ошибке аутентификации

    // Вызываем метод аутентификации
    authDialog->ui->loginreg->setText(login);
    authDialog->ui->passwordforreg->setText(password);

    // Подменяем метод authenticateUser  для теста
    QSignalSpy authSpy(clientManager, &ClientManager::authenticateUser );
    // Здесь вы должны использовать механизм, который позволяет вам возвращать mock-ответ

    authDialog->on_authButton_clicked();



}
