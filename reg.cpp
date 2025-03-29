#include "reg.h"
#include "ui_reg.h"
#include "firstwindow.h"
#include "calculator.h"
#include <QMessageBox>
#include <QVBoxLayout>
reg::reg(ClientManager *clientManager, QWidget *parent) :
    QDialog(parent), m_clientManager(clientManager),
    ui(new Ui::reg),
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

    ui->login->setStyleSheet(lineStyle);
    ui->password->setStyleSheet(lineStyle);
    ui->password_2->setStyleSheet(lineStyle);
    ui->email->setStyleSheet(lineStyle);
    QString labelStyle = "QLabel {"
                         "background-color: rgba(255, 255, 255, 200);"
                         "border: 1px solid rgba(255, 184, 224, 1);"
                         "border-radius: 10px;"
                         "padding: 10px;"
                         "font-size: 14px;"
                         "}"
        ;
    ui->loginL->setStyleSheet(labelStyle);
    ui->passwordL->setStyleSheet(labelStyle);
    ui->passwordL_2->setStyleSheet(labelStyle);
    ui->emailL->setStyleSheet(labelStyle);

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

    ui->reg_2->setStyleSheet(buttonStyle);
    ui->back->setStyleSheet(buttonStyle);
    connect(ui->reg_2, &QPushButton::clicked, this, &reg::on_registerButton_clicked);
    connect(ui->back, &QPushButton::clicked, this, &reg::on_backButton_clicked);
}
reg::~reg()
{
    delete ui;
    delete movie;
}

void reg::on_registerButton_clicked()
{
    QString email = ui->email->text();
    QString username = ui->login->text();
    QString password = ui->password->text();
    QString confirmPassword = ui->password_2->text();

    // Проверка на пустые поля
    if (email.isEmpty() || username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    // Проверка на совпадение паролей
    if (password != confirmPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают.");
        return;
    }

    QByteArray response = m_clientManager->registerUser (username, password, email);

    // Обрабатываем ответ от сервера
    if (response.startsWith("reg&success&")) {
        QStringList responseParts = QString::fromUtf8(response).split('&');

        if (responseParts.size() > 2) {
            bool ok;
            int userId = responseParts.at(2).toInt(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Ошибка", "Не удалось преобразовать ID пользователя.");
                return;
            }
            qDebug() << "Регистрация успешна. userId:" << userId;

            Client *newClient = new Client(this);
            m_clientManager->addClient(userId, newClient);

            // Открываем окно калькулятора и передаем userId
            calculator *calculatorWindow = new calculator(userId, m_clientManager, this);
            calculatorWindow->show();


             this->hide();
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный формат ответа от сервера.");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось зарегистрироваться. Проверьте введенные данные.");
    }
}

void reg::on_backButton_clicked()
{
    firstwindow *firstWindow = new firstwindow(this);
    firstWindow->setAttribute(Qt::WA_DeleteOnClose); // Удаляем окно при закрытии
    connect(firstWindow, &QDialog::finished, this, &reg::show); // Показать текущее окно при закрытии нового
    this->hide(); // Скрываем текущее окно
    firstWindow->show(); // Открываем первое окно
}

