/*#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DataBase {
public:
    DataBase(const QString &host, const QString &dbName, const QString &user, const QString &password);
    ~DataBase();

    bool open();
    void close();
    bool createUser (const QString &login, const QString &password, const QString &email);
    void listUsers();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H*/
