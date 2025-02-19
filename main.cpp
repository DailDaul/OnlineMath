#include <QCoreApplication>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include "dataBase.h"
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer myserv;

    /*DataBase db("localhost", "TestDB", "your_username", "your_password");

    if (!db.open()) {
        return -1; // Завершить программу при ошибке
    }

    // Создание пользователей
    db.createUser ("admin", "123", "admin@example.com");
    db.createUser ("user", "123", "user@example.com");

    // Вывод списка пользователей
    db.listUsers();

    // Пример авторизации
    if (db.authenticateUser ("admin", "123")) {
        qDebug() << "Авторизация успешна!";
    } else {
        qDebug() << "Ошибка авторизации!";
    }

    db.close();*/
    return a.exec();
}
