#include "auth.h"
#include "ui_auth.h"
#include "firstwindow.h"
#include "calculator.h"
#include <QMessageBox>
#include <QVBoxLayout>
auth::auth(ClientManager* clientManager, QDialog *parent) :
    QDialog(parent),
 clientManager(clientManager),
    ui(new Ui::auth),

    backgroundLabel(new QLabel(this)),
    movie(new QMovie("C:/Users/novos/Documents/Frontend/Frontend/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/Blossoms.gif"))
{
    ui->setupUi(this);

    backgroundLabel->setMovie(movie);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect());
    movie->start();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(backgroundLabel);
    setLayout(layout);

    QString lineStyle = "QLineEdit {"
                        "background-color: rgba(255, 255, 255, 200);"
                        "border: 1px solid rgba(255, 184, 224, 1);"
                        "border-radius: 10px;"
                        "padding: 10px;"
                        "font-size: 14px;"
                        "}"
                        "QLineEdit:hover {"
                        "background-color: rgba(255, 255, 255, 255);"
                        "border: 1px solid rgba(252, 169, 216, 1);"
                        "}";
    ui->loginreg->setStyleSheet(lineStyle);
    ui->passwordforreg->setStyleSheet(lineStyle);

    QString labelStyle = "QLabel {"
                         "background-color: rgba(255, 255, 255, 200);"
                         "border: 1px solid rgba(255, 184, 224, 1);"
                         "border-radius: 10px;"
                         "padding: 10px;"
                         "font-size: 14px;"
                         "}";
    ui->loginn->setStyleSheet(labelStyle);
    ui->passwordfor->setStyleSheet(labelStyle);

    QString buttonStyle = "QPushButton {"
                          "background-color: rgba(255, 255, 255, 200);"
                          "border: 1px solid rgba(255, 184, 224, 1);"
                          "border-radius: 10px;"
                          "padding: 10px;"
                          "font-size: 16px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: rgba(255, 255, 255, 255);"
                          "border: 1px solid rgba(252, 169, 216, 1);"
                          "}";
    ui->enter->setStyleSheet(buttonStyle);
    ui->backto->setStyleSheet(buttonStyle);

    connect(ui->enter, &QPushButton::clicked, this, &auth::on_authButton_clicked);
    connect(ui->backto, &QPushButton::clicked, this, &auth::on_backtoButton_clicked);
}

auth::~auth() {
    delete ui;
    delete movie;

}


void auth::on_authButton_clicked() {
    QString login = ui->loginreg->text();
    QString password = ui->passwordforreg->text();

    QByteArray response = clientManager->authenticateUser (login, password);

    if (response.startsWith("auth&success&")) {
        QStringList responseParts = QString(response).split('&');
        if (responseParts.size() > 2) {
            bool ok;
            int userId = responseParts.at(2).toInt(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Ошибка", "Не удалось преобразовать ID пользователя.");
                return;
            }
            qDebug() << "Аутентификация успешна. userId:" << userId;

            // Создаем окно калькулятора, передавая userId
            calculator *calculatorWindow = new calculator(userId, clientManager, this);
            calculatorWindow->show();

            // Удаляем текущее окно авторизации при закрытии калькулятора
            connect(calculatorWindow, &QDialog::finished, this, &QWidget::deleteLater);
            this->hide(); // Скрываем текущее окно авторизации
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный формат ответа от сервера.");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль.");
    }
}



void auth::on_backtoButton_clicked()
{
    firstwindow *firstWindow = new firstwindow(this);
    firstWindow->setAttribute(Qt::WA_DeleteOnClose); // Удаляем окно при закрытии
    connect(firstWindow, &QDialog::finished, this, &auth::show); // Показать текущее окно при закрытии нового
    this->hide(); // Скрываем текущее окно
    firstWindow->show(); // Открываем первое окно
}

