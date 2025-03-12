#ifndef SINGLETON_H
#define SINGLETON_H
#include "dataBase.h"
#include <QTcpSocket>

class Singleton;
class SingletonDestroyer
{
private:
    Singleton *instance;
public:
    ~SingletonDestroyer() {
        if (instance) {
            delete instance;
        }
    }
    void initialize (Singleton *link) {instance = link;}
};

class Singleton:public QObject
{
    Q_OBJECT
private:
    static Singleton *instance;
    static SingletonDestroyer destroyer;
    DataBase *db;
    //QTcpSocket *socket;
protected:
    Singleton ();// Защищенный конструктор
    Singleton (const Singleton &) = delete;// Запрет на копирование
    Singleton &operator = (Singleton &) = delete;// Запрет на присваивание
    ~Singleton ();// Деструктор, закрывающий соединение с базой данных
    friend class SingletonDestroyer;

public:
    static Singleton * get_instance();
    DataBase* getDataBase() { return db; } // Добавлен метод
    //void send_msg(QString){};
    bool authenticateUser(const QString& login, const QString& password);
    bool createUser(const QString& login, const QString& password, const QString& email, QString& errorMessage, int& userId);
    int getUserId(const QString &login);
    bool saveCalculation(int userId, const QString &operation);
    QList<QMap<QString, QVariant>> getHistory(int userId, int limit);

public slots:
    //void slot_on_read() {}
    //void slot_on_disconnect() {}
};

#endif // SINGLETON_H
