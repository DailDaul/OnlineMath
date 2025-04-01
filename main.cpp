#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTest>
#include "firstwindow.h"
#include "test_firstwindow.h"  // Подключите ваш тестовый класс для первого окна
#include "test_client.h"       // Подключите тесты для Client
#include "testclientmanager.h" // Подключите тесты для ClientManager
#include "test_reg.h"          // Подключите тесты для регистрации

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Перевод интерфейса
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Frontend_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Запуск тестов
    TestFirstWindow testFirstWindow;  // Создайте экземпляр вашего тестового класса для первого окна
    TestClient testClient;             // Создайте экземпляр тестового класса для клиента
    TestClientManager testClientManager; // Создайте экземпляр тестового класса для ClientManager
    TestReg testReg;                   // Создайте экземпляр тестового класса для регистрации

    // Запустите тесты
    int result = QTest::qExec(&testFirstWindow, argc, argv);  // Запустите тесты для первого окна
    result |= QTest::qExec(&testClient, argc, argv);           // Запустите тесты для клиента
    result |= QTest::qExec(&testClientManager, argc, argv);    // Запустите тесты для ClientManager
    result |= QTest::qExec(&testReg, argc, argv);              // Запустите тесты для регистрации

    return result; // Верните результат выполнения тестов
}
