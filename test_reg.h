#ifndef TEST_REG_H
#define TEST_REG_H

#include <QObject>
#include <QTest>
#include "reg.h"
#include "clientmanager.h"
#include <QSignalSpy>

class TestReg : public QObject
{
    Q_OBJECT
public:
    // Убираем override
    QByteArray mockRegisterUser (const QString &login, const QString &password, const QString &email) {
        return "reg&success&1"; // Возвращаем мок-ответ
    }

private slots:
    void init();
    void cleanup();
    void testRegisterSuccess();
    void testRegisterEmptyFields();
    void testRegisterPasswordMismatch();

private:
    reg *registrationDialog;
    ClientManager *clientManager;
};

#endif // TEST_REG_H
