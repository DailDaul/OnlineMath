#include "firstwindow.h"
#include "ui_firstwindow.h"
#include "auth.h"
#include "reg.h"
#include "clientmanager.h"

firstwindow::firstwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstwindow),
    backgroundLabel(new QLabel(this)),
    movie(new QMovie("C:/Users/novos/Documents/Frontend/Frontend/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/Blossoms.gif")),
    clientManager(new ClientManager(this))
{
    ui->setupUi(this);

    backgroundLabel->setMovie(movie);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect());
    movie->start();
    backgroundLabel->lower();

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

    ui->reg->setStyleSheet(buttonStyle);
    ui->auth->setStyleSheet(buttonStyle);

    connect(ui->reg, &QPushButton::clicked, this, &firstwindow::on_regButton_clicked);
    connect(ui->auth, &QPushButton::clicked, this, &firstwindow::on_authButton_clicked);
}

firstwindow::~firstwindow()
{
    delete ui;
    delete movie;
}

void firstwindow::on_regButton_clicked()
{
    if (!regWindow) { // Проверяем, открыто ли окно регистрации
        regWindow = new reg(clientManager, this);
        regWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(regWindow, &QDialog::finished, [this]() {
            regWindow = nullptr; // Обнуляем указатель при закрытии
        });
        regWindow->show();
    }
}

void firstwindow::on_authButton_clicked()
{
    if (!authWindow) { // Проверяем, открыто ли окно аутентификации
        authWindow = new auth(clientManager, this);
        authWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(authWindow, &QDialog::finished, [this]() {
            authWindow = nullptr; // Обнуляем указатель при закрытии
        });
        authWindow->show();
    }
}
