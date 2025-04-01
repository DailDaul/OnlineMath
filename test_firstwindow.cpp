#include "test_firstwindow.h"
void TestFirstWindow::init() {
    clientManager = new ClientManager();
    firstWindow = new firstwindow();
    firstWindow->show();
}

void TestFirstWindow::cleanup() {
    delete firstWindow;
    delete clientManager;
}


void TestFirstWindow::testOpenRegWindow() {
    firstWindow->on_regButton_clicked(); // Эмулируем нажатие на кнопку регистрации
    QVERIFY(firstWindow->findChild<reg *>() != nullptr); // Проверяем, что окно регистрации открыто
}

void TestFirstWindow::testOpenAuthWindow() {
    firstWindow->on_authButton_clicked(); // Эмулируем нажатие на кнопку аутентификации
    QVERIFY(firstWindow->findChild<auth *>() != nullptr); // Проверяем, что окно аутентификации открыто
}




