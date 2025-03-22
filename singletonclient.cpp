#include "singletonclient.h"
#include <QDebug>

SingletonClient::SingletonClient() {
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 33333);
    if (!socket->waitForConnected(3000)) {
        qDebug() << "Ошибка подключения к серверу:" << socket->errorString();
    }
}

SingletonClient::~SingletonClient() {
    if (socket) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
        delete socket;
    }
}
SingletonClient& SingletonClient::getInstance() {
    static SingletonClient instance; // Создание единственного экземпляра
    return instance;
}

QByteArray SingletonClient::sendRequest(const QByteArray& request) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Сервер не доступен.";
        return "Error: server not available\n";
    }

    // Отправление запроса на сервер
    socket->write(request);
    socket->flush();

    QByteArray response;

    // Читение ответа от сервера
    while (socket->waitForReadyRead(3000)) {
        response.append(socket->readAll());
    }

    // Проверяем, что ответ не пуст
    if (response.isEmpty()) {
        qDebug() << "Ответ от сервера пуст.";
        return "Error: empty response from server\n";
    }

    return response;
}

QByteArray SingletonClient::auth(const QString& login, const QString& password) {
    // 1. Формируем запрос для авторизации
    QByteArray request = QString("auth&%1&%2").arg(login, password).toUtf8();
    // 2. Отправляем запрос на сервер и получаем ответ
    QByteArray response = sendRequest(request);

    QString responseString = QString::fromUtf8(response);
    // 3. Обрабатываем ответ от сервера
    if (responseString.startsWith("auth&success&")) {
        QStringList parts = responseString.split("&");
        if (parts.size() == 3) { // Проверяем, что ответ содержит 3 части
            int userId = parts[2].toInt();
            qDebug() << "Аутентификация успешна. UserId:" << userId;
            return response;
        }
    } else {
        qDebug() << "Ошибка аутентификации:" << responseString;
    }

    return response; // Возвращаем ответ от сервера
}

QByteArray SingletonClient::reg(const QString& login, const QString& password, const QString& email) {
    // 1. Формируем запрос для регистрации
    QByteArray request = QString("reg&%1&%2&%3").arg(login, password, email).toUtf8();
        // 2. Отправляем запрос на сервер и получаем ответ
    QByteArray response = sendRequest(request);
    qDebug() << "Ответ от сервера:" << QString::fromUtf8(response);
    // 3. Обрабатываем ответ от сервера
    if (response.startsWith("reg&success&")) {
        QStringList responseParts = QString::fromUtf8(response).split('&');
        if (responseParts.size() == 3) {
            int userId = responseParts[2].toInt();
            qDebug() << "Регистрация успешна. userId:" << userId;
            return response;
        } else {
            qDebug() << "Ошибка: неверный формат ответа от сервера:" << QString::fromUtf8(response);
            return "Error: неверный формат ответа от сервера\n";
        }
    } else {
        qDebug() << "Ошибка регистрации:" << QString::fromUtf8(response);
        return "Error: не удалось зарегистрироваться\n";
    }
}
QByteArray SingletonClient::saveCalculation(int userId, const QString& operation) {
    // 1. Формируем запрос для сохранения вычисления
    QString request = QString("saveCalculation&%1&%2").arg(userId).arg(operation);
    QByteArray requestData = request.toUtf8();

    // 2. Отправляем запрос на сервер и получаем ответ
    QByteArray response = sendRequest(requestData);

    // 3. Обрабатываем ответ от сервера
    if (response.startsWith("saveCalculation&success")) {
        qDebug() << "saveCalculation: Вычисление для пользователя " << userId << " успешно сохранено";
        return response;
    } else {
        qDebug() << "saveCalculation: Ошибка при сохранении вычисления для пользователя " << userId;
        return response;
    }
}

void SingletonClient::getHistory(int userId, int limit) {

    // 1. Формируем строку запроса
    QString request = QString("getHistory&%1&%2").arg(userId).arg(limit);
    QByteArray requestData = request.toUtf8();

    // 2. Отправляем запрос на сервер и получаем ответ
    QByteArray responseData = sendRequest(requestData);

    // 3. Преобразуем ответ в QString
    QString response = QString::fromUtf8(responseData);

    // 4.  Обрабатываем ответ
    if (response.startsWith("Error:")) {
        qDebug() << "getHistory: Ошибка от сервера: " << response;
        emit historyReceived(userId, QStringList());
    } else {
        QStringList history;
        QStringList entries = response.split("\n", Qt::SkipEmptyParts); // Разделяем на отдельные записи
        for (const QString& entry : entries) {
            QString operation;

            if (entry.startsWith("operation=")) {
                operation = entry.mid(QString("operation=").length()); //Извлекаем значение operation
            }

            if (!operation.isEmpty()) {
                history.append(operation); // Добавляем operation в список
            }
        }

        // Отправляем сигнал с историей
        emit historyReceived(userId, history);
    }

}
