#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>
//#include "dataBase.h"
QByteArray parsing(QString data_from_client) {
    // Проверка на наличие символа '&'
    if (!data_from_client.contains('&')) {
        qDebug() << "Ошибка: неверный формат данных. Введенная строка:" << data_from_client;
        return "Используйте reg/auth для регистрации или авторизации\n";
    }

    // Если символ '&' найден, продолжаем парсинг
    QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));
    QString nameOfFunc = data_from_client_list.front();
    data_from_client_list.pop_front();

    if (nameOfFunc == "auth") {
        if (data_from_client_list.size() < 2) {
            return "Error: not enough parameters for auth\n";
        }
        return auth(data_from_client_list.at(0), data_from_client_list.at(1));
    } else if (nameOfFunc == "reg") {
        if (data_from_client_list.size() < 3) {
            return "Error: not enough parameters for reg\n";
        }
        return reg(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2));
    } else {
        return "Error: unknown function\n";
    }
}
QByteArray auth(QString log, QString pass){
    //DataBase::sendQuerry("xcbvm");
    QString result = QString("auth&%1&%2").arg(log).arg(pass);
    return result.toUtf8() + "\n";
}

QByteArray reg(QString log, QString pass, QString mail){
    QString result = QString("reg&%1&%2&%3").arg(log).arg(pass).arg(mail);
    return result.toUtf8() + "\n";
}

