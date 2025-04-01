#include "test_reg.h"
#include <QMessageBox>
#include "ui_reg.h"

void TestReg::init() {
    clientManager = new ClientManager();
    registrationDialog = new reg(clientManager);
}

void TestReg::cleanup() {
    delete registrationDialog;
    delete clientManager;
}

void TestReg::testRegisterSuccess() {
    // Установка тестовых данных
    registrationDialog->ui->login->setText("testUser");
    registrationDialog->ui->password->setText("testPassword");
    registrationDialog->ui->password_2->setText("testPassword");
    registrationDialog->ui->email->setText("test@example.com");

    // Создаем QSignalSpy для отслеживания сигнала
    QSignalSpy spy(clientManager, &ClientManager::historyReceived); // Измените на соответствующий сигнал

    // Нажимаем кнопку регистрации
    registrationDialog->on_registerButton_clicked();

}

void TestReg::testRegisterEmptyFields() {
    // Очищаем все поля
    registrationDialog->ui->login->clear();
    registrationDialog->ui->password->clear();
    registrationDialog->ui->password_2->clear();
    registrationDialog->ui->email->clear();

    // Нажимаем кнопку регистрации
    registrationDialog->on_registerButton_clicked();

    // Проверяем, что появилось предупреждение
    QVERIFY(QMessageBox::warning(registrationDialog, "Ошибка", "Пожалуйста, заполните все поля.") == QMessageBox::Ok);
}

void TestReg::testRegisterPasswordMismatch() {
    // Установка тестовых данных
    registrationDialog->ui->login->setText("testUser");
    registrationDialog->ui->password->setText("testPassword");
    registrationDialog->ui->password_2->setText("differentPassword");
    registrationDialog->ui->email->setText("test@example.com");

    // Нажимаем кнопку регистрации
    registrationDialog->on_registerButton_clicked();

    // Проверяем, что появилось предупреждение
    QVERIFY(QMessageBox::warning(registrationDialog, "Ошибка", "Пароли не совпадают.") == QMessageBox::Ok);
}
