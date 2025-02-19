/*#include "dataBase.h"

// Конструктор класса DataBase
DataBase::DataBase(const QString &host, const QString &dbName, const QString &user, const QString &password) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
}

// Деструктор класса DataBase
DataBase::~DataBase() {
    close();
}

// Метод для открытия соединения с базой данных
bool DataBase::open() {
    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return false;
    }
    return true;
}

// Метод для закрытия соединения с базой данных
void DataBase::close() {
    db.close();
}

// Метод для создания пользователя
bool DataBase::createUser (const QString &login, const QString &password, const QString &email) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO User(login, password, email) VALUES (:login, :password, :email)");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Ошибка при добавлении пользователя:" << query.lastError().text();
        return false;
    }
    return true;
}

// Метод для вывода списка пользователей
void DataBase::listUsers() {
    QSqlQuery query(db);
    query.exec("SELECT * FROM User");

    while (query.next()) {
        qDebug() << "Login:" << query.value("login").toString()
        << "Password:" << query.value("password").toString()
        << "Email:" << query.value("email").toString();
    }
}

// Метод для авторизации пользователя
bool DataBase::authenticateUser (const QString &login, const QString &password) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM User WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return true; // Пользователь найден
    }
    return false; // Пользователь не найден
}

// Метод для удаления таблицы (если нужно)
void DataBase::dropUser Table() {
    QSqlQuery query(db);
    query.exec("DROP TABLE IF EXISTS User");
}*/
