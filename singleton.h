/*#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton;
class SingletonDestroyer
{
private:
    Singleton *instance;// Указатель на экземпляр Singleton
public:
    ~SingletonDestroyer() {delete instance;}// Деструктор, который удаляет экземпляр Singleton
    void initialize (Singleton *link) {instance = link;}//Метод, который устанавливает указатель instance на переданный экземпляр Singleton
};

class Singleton
{
private:
    static Singleton *instance;// Статический указатель на единственный экземпляр
    static SingletonDestroyer destroyer;// Объект для управления временем жизни экземпляра
protected:
    Singleton () {}// Защищенный конструктор
    Singleton (const Singleton &) = delete;// Запрет на копирование
    Singleton &operator = (Singleton &) = delete;// Запрет на присваивание
    ~Singleton () {DB.close(); }// Деструктор, закрывающий соединение с базой данных
    friend class SingletonDestroyer; // Дает доступ к приватным членам класса SingletonDestroyer
public:
    static Singleton * get_instance() {
        if (!instance)
        {
            instance = new Singleton();// Создание нового экземпляра, если он еще не существует
            destroyer.initialize(instance);// Инициализация destroyer с новым экземпляром
        }
        return instance;// Возвращение единственного экземпляра
    }
};

Singleton *Singleton ::instance;// Определение статического указателя на экземпляр
SingletonDestroyer Singleton ::destroyer;// Определение статического объекта destroyer
#endif // SINGLETON_H*/
