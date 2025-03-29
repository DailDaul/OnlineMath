#include "client.h"
#include <QDateTime>
Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 33333);
    if (!socket->waitForConnected(3000)) {
        qDebug() << "Ошибка подключения к серверу:" << socket->errorString();
    }
}

Client::~Client() {
    if (socket) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
        delete socket;
    }
}

QByteArray Client::sendRequest(const QByteArray &request) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Сервер не доступен.";
        return "Error: server not available\n";
    }

    socket->write(request);
    socket->flush();

    QByteArray response;
    while (socket->waitForReadyRead(3000)) {
        response.append(socket->readAll());
    }

    if (response.isEmpty()) {
        qDebug() << "Ответ от сервера пуст.";
        return "Error: empty response from server\n";
    }

    return response;
}

QByteArray Client::auth(const QString &login, const QString &password) {
    QByteArray request = QString("auth&%1&%2").arg(login, password).toUtf8();
    QByteArray response = sendRequest(request);

    QString responseString = QString::fromUtf8(response);
    if (responseString.startsWith("auth&success&")) {
        QStringList parts = responseString.split("&");
        if (parts.size() == 3) {
            int userId = parts[2].toInt();
            qDebug() << "Аутентификация успешна. UserId:" << userId;
            return response; // Возвращаем полный ответ
        }
    }
    qDebug() << "Ошибка аутентификации:" << responseString;
    return response; // Возвращаем ответ об ошибке
}

QByteArray Client::reg(const QString &login, const QString &password, const QString &email) {
    QByteArray request = QString("reg&%1&%2&%3").arg(login, password, email).toUtf8();
    QByteArray response = sendRequest(request);

    QString responseString = QString::fromUtf8(response);
    if (responseString.startsWith("reg&success&")) {
        QStringList parts = responseString.split("&");
        if (parts.size() == 3) {
            int userId = parts[2].toInt();
            qDebug() << "Регистрация успешна. UserId:" << userId;
            return response; // Возвращаем полный ответ
        }
    }

    qDebug() << "Ошибка регистрации:" << responseString;
    return response; // Возвращаем ответ об ошибке
}


QByteArray Client::saveCalculation(int userId, const QString &operation) {
    QByteArray request = QString("saveCalculation&%1&%2").arg(userId).arg(operation).toUtf8();
    QByteArray response = sendRequest(request);

    if (response.startsWith("saveCalculation&success")) {
        qDebug() << "saveCalculation: Вычисление для пользователя " << userId << " успешно сохранено";
        return response;
    } else {
        qDebug() << "saveCalculation: Ошибка при сохранении вычисления для пользователя " << userId;
        return response;
    }
}

void Client::getHistory(int userId, int limit) {
    QByteArray request = QString("getHistory&%1&%2").arg(userId).arg(limit).toUtf8();
    QByteArray responseData = sendRequest(request);
    QString response = QString::fromUtf8(responseData);

    if (response.startsWith("Error:")) {
        qDebug() << "getHistory: Ошибка от сервера: " << response;
        emit historyReceived(userId, QStringList());
        return;
    }

    QMap<QString, QList<QString>> groupedHistory; // Для группировки операций по дате
    QStringList entries = response.split("\n", Qt::SkipEmptyParts);

    for (const QString &entry : entries) {
        QString operation;
        QString timestamp;

        QStringList parts = entry.split("&", Qt::SkipEmptyParts);
        for (const QString &part : parts) {
            if (part.startsWith("operation=")) {
                operation = part.mid(QString("operation=").length());
            } else if (part.startsWith("timestamp=")) {
                timestamp = part.mid(QString("timestamp=").length());
            }
        }

        if (!operation.isEmpty() && !timestamp.isEmpty()) {
            QString dateKey = QDateTime::fromString(timestamp, Qt::ISODate).toString("yyyy-MM-dd");
            groupedHistory[dateKey].append(operation);
        }
    }

    // Формируем текстовый ответ
    QStringList history;

    // Получаем список ключей и сортируем их по убыванию
    QList<QString> sortedKeys = groupedHistory.keys();
    std::sort(sortedKeys.begin(), sortedKeys.end(), std::greater<QString>());

    for (const QString& dateKey : sortedKeys) {
        QDate date = QDate::fromString(dateKey, "yyyy-MM-dd");
        if (date.isValid()) {
            QString formattedDate = date.toString("d MMMM yyyy");
            history.append(formattedDate + ":");
            for (const QString& op : groupedHistory[dateKey]) {
                history.append(" " + op);
            }
            history.append("");
        } else {
            qDebug() << "getHistory: Неверная дата - " << dateKey;
        }
    }

    if (!history.isEmpty()) {
        emit historyReceived(userId, history);
    } else {
        qDebug() << "getHistory: История пуста для userId:" << userId;
        emit historyReceived(userId, QStringList());
    }
}
