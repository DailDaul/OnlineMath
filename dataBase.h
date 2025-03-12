#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>//класс для работы с базами данных
#include <QSqlQuery>//класс для выполнения SQL-запросов
#include <QSqlError>//класс для обработки ошибок, связанных с базой данных
#include <QDebug>//класс для отладки, позволяющий выводить сообщения в консоль

class DataBase {
public:
    DataBase(const QString &host, const QString &dbName, const QString &user, const QString &password);//Конструктор, который принимает параметры для подключения к базе данных: хост, имя базы данных, имя пользователя и пароль
    ~DataBase();
    bool createTables();//Метод для создания таблиц
    bool open();//Метод для открытия соединения с базой данных
    bool isOpen() const;
    void close();//Метод для закрытия соединения с базой данных
    bool createUser(const QString &login, const QString &password, const QString &email, QString& errorMessage, int& userId);//Метод для создания нового пользователя в базе данных
    bool authenticateUser(const QString &login, const QString &password);//Метод для аутентификации пользователя
    bool saveCalculation(int userId, const QString &operation);//Метод для сохранения результатов вычислений в базе данных
   QList<QMap<QString, QVariant>> getHistory(int userId, int limit);//Метод для получения истории операций пользователя. Возвращает список, содержащий карты, где ключи — это строки, а значения — это объекты QVariant
    void listUsers();//Метод для вывода списка пользователей
    void dropUserTable();//Метод для удаления таблицы пользователей из базы данных
    int getUserId(const QString &login);

private:
    QSqlDatabase db;//Объект базы данных
};

#endif // DATABASE_H
