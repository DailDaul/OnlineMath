#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H
#include <QString>
#include <QByteArray>
#include <QStringList>
QByteArray parsing(QString data_from_client);
QByteArray auth(QString log, QString pass, QString mail = "");
QByteArray reg(QString log, QString pass, QString mail);
QByteArray saveCalculation(int userId, const QString& operation);
QByteArray getHistory(int userId, int limit);
#endif // FUNCTIONSFORSERVER_H
