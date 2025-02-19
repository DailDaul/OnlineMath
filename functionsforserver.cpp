#include "functionsforserver.h"
//#include "dataBase.h"
QByteArray parsing (QString data_from_client){
    /* data_from_client = NameOfFunc&Param1&Param2&Param3
     * Цель парсинга:
     * return NameOfFunc(Param1, Param2, Param3)
     */
    QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));//использует & в качестве разделителя и сохраняет данные в data_from_client_list
    // data_from_client_list = "NameOfFunc" "Param1" "Param2" "Param3"
    QString nameOfFunc = data_from_client_list.front();// получает первое значение из списка и сохраняет его
    data_from_client_list.pop_front();
    if (nameOfFunc == "auth"){
        if (data_from_client_list.size() < 2){
            return "Error: not enough parameters for auth\n";
        }
        return auth(data_from_client_list.at(0),data_from_client_list.at(1));
    }
    else if (nameOfFunc == "reg"){
        if (data_from_client_list.size() < 3){
            return "Error: not enough parameters for reg\n";
        }
        return reg(data_from_client_list.at(0),data_from_client_list.at(1), data_from_client_list.at(2));
    }
    else
        return "Error: unknown function\n";
}

QByteArray auth(QString log, QString pass){
    //DataBase::sendQuerry("xcbvm");
    return "auth\n";
}

QByteArray reg(QString log, QString pass, QString mail){
    return "reg\n";
}
