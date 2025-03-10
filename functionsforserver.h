#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H
#include <QString>
#include <QByteArray>
#include <QStringList>
QByteArray parsing(QString data_from_client);
QByteArray auth(QString log, QString pass, QString mail = "");
QByteArray reg(QString log, QString pass, QString mail);
#endif // FUNCTIONSFORSERVER_H
