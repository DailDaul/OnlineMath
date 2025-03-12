#include "singleton.h"
#include <QDebug> // Добавлено для вывода отладочной информации

Singleton *Singleton::instance = nullptr;
SingletonDestroyer Singleton::destroyer;

Singleton::Singleton() : QObject(nullptr) {
    // Инициализация DataBase
    db = new DataBase("localhost", "my_database.sqlite", "", "");
    if (!db->open()) {
        qDebug() << "Ошибка при открытии базы данных в Singleton!";
    }
    if (!db->createTables()) {
        qDebug() << "Ошибка при создании базы данных!";
    }
}

Singleton::~Singleton() {
    // Закрытие и удаление DataBase
    if (db) {
        if (db->isOpen()) {
            db->close(); // Закрываем соединение
        }
        delete db; // Освобождаем память
        db = nullptr; // Обнуляем указатель
    }
}

Singleton *Singleton::get_instance() {
    if (!instance) {
        instance = new Singleton();
        destroyer.initialize(instance);
    }
    return instance;
}

bool Singleton::authenticateUser(const QString& login, const QString& password) {
    return db->authenticateUser(login, password); // Перенаправляем вызов к DataBase
}

bool Singleton::createUser(const QString& login, const QString& password, const QString& email, QString& errorMessage, int& userId) {
    return db->createUser(login, password, email, errorMessage, userId);
}

int Singleton::getUserId(const QString &login) {
    return db->getUserId(login);
}

bool Singleton::saveCalculation(int userId, const QString &operation) {
    return db->saveCalculation(userId, operation);
}

QList<QMap<QString, QVariant>> Singleton::getHistory(int userId, int limit) {
    return db->getHistory(userId, limit);
}
