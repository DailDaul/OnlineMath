#include "functionsforserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QCryptographicHash>
#include "singleton.h"
#include <QDateTime>


QByteArray auth(QString log, QString pass, QString mail);  // Email по умолчанию пустой
QByteArray reg(QString log, QString pass, QString mail);


QByteArray parsing(QString data_from_client) {
    // 1. Проверка на наличие символа '&'
    if (!data_from_client.contains('&')) {
        qDebug() << "Ошибка: неверный формат данных. Введенная строка:" << data_from_client;
        return "Используйте reg/auth для регистрации или авторизации\n";
    }

    // 2. Разделение строки на части
    QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));

    // 3. Проверка, что что-то есть после разделения
    if (data_from_client_list.isEmpty()) {
        qDebug() << "Ошибка: пустой запрос";
        return "Error: пустой запрос\n";
    }

    // 4. Получение имени функции
    QString nameOfFunc = data_from_client_list.first();
    data_from_client_list.removeFirst(); // Удаляем имя функции из списка

    qDebug() << "parsing: nameOfFunc = " << nameOfFunc; // Отладочный вывод

    // 5. Обработка запросов
    if (nameOfFunc == "auth") {
        if (data_from_client_list.size() < 2) {
            qDebug() << "parsing: Ошибка: недостаточно параметров для auth";
            return "Error: not enough parameters for auth\n";
        }
        QString log = data_from_client_list.at(0);
        QString pass = data_from_client_list.at(1);
        QString email = (data_from_client_list.size() > 2) ? data_from_client_list.at(2) : ""; // Если есть email - берем, иначе - пустая строка
        qDebug() << "parsing: auth: log = " << log << ", pass = " << pass; // Отладочный вывод
        return auth(log, pass, email); // вызываем auth с тремя аргументами
    } else if (nameOfFunc == "reg") {
        if (data_from_client_list.size() < 3) {
            qDebug() << "parsing: Ошибка: недостаточно параметров для reg";
            return "Error: not enough parameters for reg\n";
        }
        return reg(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2));
    } else if (nameOfFunc == "saveCalculation") {
        if (data_from_client_list.size() < 2) {
            qDebug() << "parsing: Ошибка: недостаточно параметров для saveCalculation";
            return "Error: not enough parameters for saveCalculation\n";
        }
        bool ok;
        int userId = data_from_client_list.at(0).toInt(&ok);
        if (!ok) {
            qDebug() << "parsing: Ошибка: неверный userId";
            return "Error: invalid userId\n";
        }
        QString operation = data_from_client_list.at(1).simplified();
        // Вызываем saveCalculationCommand
        return saveCalculation(userId, operation);
    } else if (nameOfFunc == "getHistory") {
        if (data_from_client_list.size() < 2) {
            qDebug() << "parsing: Ошибка: недостаточно параметров для getHistory";
            return "Error: not enough parameters for getHistory\n";
        }

        bool ok1, ok2;
        int userId = data_from_client_list.at(0).toInt(&ok1);
        int limit = data_from_client_list.at(1).toInt(&ok2);

        if (!ok1 || !ok2) {
            qDebug() << "parsing: Ошибка: неверные userId или limit";
            return "Error: invalid userId or limit\n";
        }

        // Вызываем getHistoryCommand
        return getHistory(userId, limit);
    }else {
        qDebug() << "parsing: Ошибка: неизвестная функция";
        return "Error: unknown function\n";
    }
}

QByteArray auth(QString log, QString pass, QString email) {
    // 1. Получаем экземпляр Singleton
    Singleton* singleton = Singleton::get_instance();

    // 2. Хешируем пароль
    QByteArray passwordBytes = pass.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();

    // 3. Аутентифицируем пользователя
    if (singleton->authenticateUser(log, hashedPassword)) {
        // Аутентификация успешна

        // 4. Получаем userId
        int userId = singleton->getUserId(log);
        if (userId > 0) {
            // Формируем строку результата
            QString result = QString("auth&success&%1").arg(userId);

            // Преобразуем QString в QByteArray и добавляем символ новой строки
            return result.toUtf8() + "\n";
        } else {
            qDebug() << "auth: Ошибка при получении userId для логина: " << log;
            return "auth&failed\n"; // Или другое сообщение об ошибке
        }
    } else {
        // Аутентификация не удалась
        return "auth&failed\n";
    }
}

QByteArray reg(QString log, QString pass, QString mail) {
    // 1. Получаем экземпляр Singleton
    Singleton *singleton = Singleton::get_instance();

    // 2. Хешируем пароль
    QByteArray passwordBytes = pass.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();

    // 3. Пытаемся создать пользователя
    QString errorMessage; // Для получения сообщения об ошибке
    int userId = 0; // Переменная для хранения userId
    bool creationResult = singleton->createUser(log, hashedPassword, mail, errorMessage, userId);

    // 4. Обрабатываем результат создания пользователя
    if (creationResult) {
        // Пользователь успешно создан
        qDebug() << "reg: Пользователь " << log << " успешно зарегистрирован";
        QString result = QString("reg&success&%1").arg(userId); // Формируем строку с userId
        return result.toUtf8() + "\n";
    } else {
        // Произошла ошибка при создании пользователя
        qDebug() << "reg: Ошибка при регистрации пользователя " << log << ": " << errorMessage;
        return ("reg&failed&" + errorMessage + "\n").toUtf8(); // Возвращаем сообщение об ошибке
    }
}

QByteArray saveCalculation(int userId, const QString& operation) {
    //  Получаем экземпляр Singleton
    Singleton *singleton = Singleton::get_instance();

    // Вызываем saveCalculation через singleton
    bool saveResult = singleton->saveCalculation(userId, operation);

    if (saveResult) {
        return "saveCalculation&success\n";
    } else {
        return "saveCalculation&failed\n";
    }
}

QByteArray getHistory(int userId, int limit) {
    // Получаем экземпляр Singleton
    Singleton* singleton = Singleton::get_instance();

    // Получаем историю вычислений
    QList<QMap<QString, QVariant>> historyList = singleton->getHistory(userId, limit);

    // Формируем текстовый ответ
    QString response;
    for (const auto& entry : historyList) { // Используем range-based for loop
        QString operation = entry["operation"].toString();
        QString timestamp = entry["timestamp"].toDateTime().toString(Qt::ISODate);

        // Формат: operation=...&timestamp=...\n
       response += QString("operation=%1&timestamp=%2\n").arg(operation).arg(timestamp);
    }

    return response.toUtf8(); // Преобразуем QString в QByteArray
}
