#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTest>
#include "firstwindow.h"
#include "test_firstwindow.h"
#include "test_client.h"
#include "testclientmanager.h"

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
    TestFirstWindow testFirstWindow;
    TestClient testClient;
    TestClientManager testClientManager;

    // Запустите тесты
    int result = QTest::qExec(&testFirstWindow, argc, argv);
    result |= QTest::qExec(&testClient, argc, argv);
    result |= QTest::qExec(&testClientManager, argc, argv);

    return result;
}
