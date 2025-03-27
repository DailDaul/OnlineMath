#include "dataBase.h"
#include <QDateTime>

bool DataBase::createTables() {
    QSqlQuery query(db);

    // Создание таблицы Users
    if (!query.exec("CREATE TABLE IF NOT EXISTS Users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "login TEXT UNIQUE, "
                    "password TEXT, "
                    "email TEXT)")) {
        qDebug() << "Ошибка создания таблицы Users:" << query.lastError().text();
        return false;
    }

    // Создание таблицы CalculatorHistory
    if (!query.exec("CREATE TABLE IF NOT EXISTS CalculatorHistory ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "user_id INTEGER, "
                    "operation TEXT, "
                    "result TEXT, "
                    "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Ошибка создания таблицы CalculatorHistory:" << query.lastError().text();
        return false;
    }

    return true;
}

// Конструктор класса DataBase (не создает базу данных, только устанавливает параметры)
DataBase::DataBase(const QString &host, const QString &dbName, const QString &user, const QString &password) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
}

// Деструктор класса DataBase (не закрывает соединение, этим занимается синглтон)
DataBase::~DataBase() {
    if (db.isOpen())
        qDebug() << "База данных все еще открыта!";
}

// Метод для открытия соединения с базой данных (самостоятельно)
bool DataBase::open() {
    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::isOpen() const{
    return db.isOpen();
}

// Метод для закрытия соединения с базой данных (самоятоятельно)
void DataBase::close() {
    db.close();
}

// Метод для создания пользователя
bool DataBase::createUser(const QString &login, const QString &password, const QString &email, QString& errorMessage, int& userId) {
    QSqlQuery query(db);

    // 1. Проверяем, существует ли пользователь с таким же логином
    query.prepare("SELECT COUNT(*) FROM Users WHERE login = :login");
    query.bindValue(":login", login);
    if (!query.exec()) {
        errorMessage = "Ошибка при проверке логина: " + query.lastError().text();
        qDebug() << "createUser: " << errorMessage;
        return false; // Ошибка
    }
    query.next();
    int loginCount = query.value(0).toInt();
    if (loginCount > 0) {
        errorMessage = "Пользователь с логином " + login + " уже существует";
        qDebug() << "createUser: " << errorMessage;
        return false; // Логин уже существует
    }

    // 2. Проверяем, существует ли пользователь с таким же email
    query.prepare("SELECT COUNT(*) FROM Users WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec()) {
        errorMessage = "Ошибка при проверке email: " + query.lastError().text();
        qDebug() << "createUser: " << errorMessage;
        return false; // Ошибка
    }
    query.next();
    int emailCount = query.value(0).toInt();
    if (emailCount > 0) {
        errorMessage = "Пользователь с email " + email + " уже существует";
        qDebug() << "createUser: " << errorMessage;
        return false; // Email уже существует
    }

    // 3. Создаем пользователя (если логин и email уникальны)
    query.prepare("INSERT INTO Users (login, password, email) VALUES (:login, :password, :email)");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":email", email);

    if (!query.exec()) {
        errorMessage = "Ошибка при добавлении пользователя: " + query.lastError().text();
        qDebug() << "createUser: " << errorMessage;
        return false; // Ошибка
    }

    // 4. Получаем userId
    query.prepare("SELECT id FROM Users WHERE login = :login");
    query.bindValue(":login", login);
    if (!query.exec()) {
        errorMessage = "Ошибка при получении id пользователя: " + query.lastError().text();
        qDebug() << "createUser: " << errorMessage;
        return false;
    }
    if (query.next()) {
        userId = query.value("id").toInt(); // Получаем userId
        errorMessage = ""; // Очищаем сообщение об ошибке в случае успеха
        return true;
    } else {
        errorMessage = "Не удалось получить ID пользователя после создания.";
        qDebug() << "createUser: " << errorMessage;
        return false;
    }
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
    query.prepare("SELECT * FROM Users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return true; // Пользователь найден
    }
    return false; // Пользователь не найден
}

bool DataBase::saveCalculation(int userId, const QString &operation) {
    QSqlQuery query(db);

    // Подготавливаем SQL-запрос с использованием плейсхолдеров
    query.prepare("INSERT INTO CalculatorHistory (user_id, operation) "
                  "VALUES (:user_id, :operation)");

    // Привязываем значения к плейсхолдерам
    query.bindValue(":user_id", userId);
    query.bindValue(":operation", operation);

    // Выполняем SQL-запрос
    if (!query.exec()) {
        qDebug() << "Ошибка при сохранении вычисления:" << query.lastError().text();
        return false;
    }

    // Ограничение количества записей
    int maxHistorySize = 30; // храним 30 последних записей
    QSqlQuery countQuery(db);
    countQuery.prepare("SELECT COUNT(*) FROM CalculatorHistory WHERE user_id = :user_id");
    countQuery.bindValue(":user_id", userId);
    if (countQuery.exec() && countQuery.next()) {
        int count = countQuery.value(0).toInt();
        if (count > maxHistorySize) {
            // Удаляем самую старую запись
            QSqlQuery deleteQuery(db);
            deleteQuery.prepare("DELETE FROM CalculatorHistory WHERE user_id = :user_id ORDER BY timestamp ASC LIMIT 1");
            deleteQuery.bindValue(":user_id", userId);
            deleteQuery.exec();
        }
    }

    return true;
}

QList<QMap<QString, QVariant>> DataBase::getHistory(int userId, int limit) {
    QList<QMap<QString, QVariant>> historyList;
    QSqlQuery query(db);

    // Подготавливаем SQL-запрос
    query.prepare("SELECT operation, timestamp FROM CalculatorHistory "
                  "WHERE user_id = :user_id "
                  "ORDER BY timestamp DESC "
                  "LIMIT :limit");

    // Привязываем значения к плейсхолдерам
    query.bindValue(":user_id", userId);
    query.bindValue(":limit", limit);

    // Выполняем SQL-запрос
    if (!query.exec()) {
        qDebug() << "Ошибка при получении истории вычислений:" << query.lastError().text();
        return historyList; // Возвращаем пустой список в случае ошибки
    }

    // Обрабатываем результаты запроса
    while (query.next()) {
        QMap<QString, QVariant> historyEntry;
        historyEntry["operation"] = query.value("operation").toString();
        historyEntry["timestamp"] = query.value("timestamp").toDateTime();

        historyList.append(historyEntry);
    }

    return historyList;
}

// Метод для удаления таблицы
void DataBase::dropUserTable() {
    QSqlQuery query(db);
    query.exec("DROP TABLE IF EXISTS User");
}

int DataBase::getUserId(const QString &login) {
    QSqlQuery query(db);
    query.prepare("SELECT id FROM Users WHERE login = :login");
    query.bindValue(":login", login);

    if (query.exec() && query.next()) {
        return query.value("id").toInt();
    } else {
        qDebug() << "getUserId: Ошибка при получении userId для логина: " << login << ", " << query.lastError().text();
        return 0;
    }
}
