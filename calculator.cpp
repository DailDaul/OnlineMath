#include "calculator.h"
#include "ui_calculator.h"
#include "memory.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QJSEngine>
#include "singletonclient.h"
calculator::calculator(int userId, QDialog *parent)
    : QDialog(parent), userId(userId), ui(new Ui::calculator), backgroundLabel(new QLabel(this))
{
    ui->setupUi(this);

    QPixmap backgroundPixmap("C:/Users/novos/Documents/Frontend/Frontend/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fon.jpg");
    if (backgroundPixmap.isNull()) {
        qDebug() << "Не удалось загрузить изображение!";
    } else {
        backgroundLabel->setPixmap(backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        backgroundLabel->setScaledContents(true);
        backgroundLabel->setFixedSize(this->size());
    }

    backgroundLabel->lower();

    QString lineStyle = "QLineEdit { "
                        "background-color: rgba(255, 255, 255, 230); "
                        "border: 1px solid rgba(255, 184, 224, 1); "
                        "border-radius: 10px; "
                        "padding: 10px; "
                        "font-size: 20px; }";
    ui->wind->setStyleSheet(lineStyle);

    QString buttonStyle = "QPushButton { background-color: rgba(255, 255, 255, 200);"
                          " border: 1px solid rgba(255, 184, 224, 1); "
                          "border-radius: 10px; padding: 10px; font-size: 16px; }"
                          "QPushButton:hover { background-color: rgba(255, 255, 255, 255); "
                          "border: 1px solid rgba(252, 169, 216, 1); }";

    for (auto button : findChildren<QPushButton*>()) {
        button->setStyleSheet(buttonStyle);
    }

    // Подключение сигналов к слотам
    connect(ui->one, &QPushButton::clicked, this, &calculator::on_oneButton_clicked);
    connect(ui->two, &QPushButton::clicked, this, &calculator::on_twoButton_clicked);
    connect(ui->three, &QPushButton::clicked, this, &calculator::on_threeButton_clicked);
    connect(ui->four, &QPushButton::clicked, this, &calculator::on_fourButton_clicked);
    connect(ui->five, &QPushButton::clicked, this, &calculator::on_fiveButton_clicked);
    connect(ui->six, &QPushButton::clicked, this, &calculator::on_sixButton_clicked);
    connect(ui->seven, &QPushButton::clicked, this, &calculator::on_sevenButton_clicked);
    connect(ui->eight, &QPushButton::clicked, this, &calculator::on_eightButton_clicked);
    connect(ui->nine, &QPushButton::clicked, this, &calculator::on_nineButton_clicked);
    connect(ui->zero, &QPushButton::clicked, this, &calculator::on_zeroButton_clicked);
    connect(ui->plus, &QPushButton::clicked, this, &calculator::on_plusButton_clicked);
    connect(ui->mins, &QPushButton::clicked, this, &calculator::on_minsButton_clicked);
    connect(ui->mult, &QPushButton::clicked, this, &calculator::on_multButton_clicked);
    connect(ui->dash, &QPushButton::clicked, this, &calculator::on_dashButton_clicked);
    connect(ui->sqrt, &QPushButton::clicked, this, &calculator::on_sqrtButton_clicked);
    connect(ui->edit, &QPushButton::clicked, this, &calculator::on_editButton_clicked);
    connect(ui->clean, &QPushButton::clicked, this, &calculator::on_cleanButton_clicked);
    connect(ui->memory, &QPushButton::clicked, this, &calculator::on_memoryButton_clicked);
    connect(ui->scob, &QPushButton::clicked, this, &calculator::on_scobButton_clicked);
    connect(ui->procent, &QPushButton::clicked, this, &calculator::on_procentButton_clicked);
    connect(ui->cube, &QPushButton::clicked, this, &calculator::on_cubeButton_clicked);
    connect(ui->min, &QPushButton::clicked, this, &calculator::on_minButton_clicked);
    connect(ui->dr, &QPushButton::clicked, this, &calculator::on_drButton_clicked);
    connect(ui->point, &QPushButton::clicked, this, &calculator::on_pointButton_clicked);

}

calculator::~calculator() {
    delete ui;
}

void calculator::on_oneButton_clicked() { ui->wind->insert("1"); }
void calculator::on_twoButton_clicked() { ui->wind->insert("2"); }
void calculator::on_threeButton_clicked() { ui->wind->insert("3"); }
void calculator::on_fourButton_clicked() { ui->wind->insert("4"); }
void calculator::on_fiveButton_clicked() { ui->wind->insert("5"); }
void calculator::on_sixButton_clicked() { ui->wind->insert("6"); }
void calculator::on_sevenButton_clicked() { ui->wind->insert("7"); }
void calculator::on_eightButton_clicked() { ui->wind->insert("8"); }
void calculator::on_nineButton_clicked() { ui->wind->insert("9"); }
void calculator::on_zeroButton_clicked() { ui->wind->insert("0"); }
void calculator::on_plusButton_clicked() { ui->wind->insert("+"); }
void calculator::on_minsButton_clicked() { ui->wind->insert("-"); }
void calculator::on_multButton_clicked() { ui->wind->insert("*"); }
void calculator::on_dashButton_clicked() { ui->wind->insert("/"); }
void calculator::on_procentButton_clicked() { ui->wind->insert("%"); }
void calculator::on_drButton_clicked() { ui->wind->insert("1/"); }
void calculator::on_pointButton_clicked() { ui->wind->insert("."); }
void calculator::on_minButton_clicked() { ui->wind->insert("-"); }

void calculator::on_scobButton_clicked() {
    if (isOpen) {
        ui->wind->insert(")");
    } else {
        ui->wind->insert("(");
    }
    isOpen = !isOpen; // Переключение состояния
}

void calculator::on_cleanButton_clicked() {
    ui->wind->clear();
    isOpen = false;
}

void calculator::on_memoryButton_clicked() {

    SingletonClient& client = SingletonClient::getInstance();

    // Проверяем, существует ли memoryInstance
    if (!memoryInstance) {
        memoryInstance = new memory(userId, this);
        connect(memoryInstance, &memory::closed, this, &calculator::onMemoryClosed);
    }

    // Проверяем, подключен ли сигнал
    if (!isConnected) {
        connect(&client, &SingletonClient::historyReceived, this, &calculator::onHistoryReceived);
        isConnected = true;
    }

    client.getHistory(userId, 30);
    memoryInstance->show();
}




void calculator::onHistoryReceived(int userId, const QStringList& history) {
    if (memoryInstance) {
        for (const QString &calc : history) {
        }
        memoryInstance->updateMemoryDisplay(history);
    } else {
        qDebug() << "onHistoryReceived: memoryInstance is null!";
    }
}


void calculator::onMemoryClosed(int userId) {
    if (userId != this->userId) {
        return;
    }
    memoryInstance = nullptr;
}



void calculator::on_editButton_clicked() {
    QString expression = ui->wind->text();
    double result = evaluateExpression(expression);

    if (!qIsNaN(result)) {
        QString resultString = QString::number(result); // Преобразуем результат в строку

        ui->wind->setText(resultString);

        QString calculation = expression + " = " + resultString;

        saveCalculation(calculation, userId);
    } else {
        ui->wind->setText("Ошибка");
    }
}


void calculator::on_sqrtButton_clicked() {
    QString currentText = ui->wind->text();
    bool ok;
    double value = currentText.toDouble(&ok);

    if (ok && value >= 0) { // Проверка на неотрицательное значение
        double result = sqrt(value);
        ui->wind->setText(QString::number(result));
        saveCalculation("√" + currentText + " = " + QString::number(result), userId);
    } else {
        ui->wind->setText("Ошибка");
    }
}

void calculator::on_cubeButton_clicked() {
    QString currentText = ui->wind->text();
    bool ok;
    double value = currentText.toDouble(&ok); // Преобразуем текст в число

    if (ok) {
        double result = value * value;
        ui->wind->setText(QString::number(result));
        saveCalculation(currentText + " ^2 = " + QString::number(result), userId);
    } else {
        ui->wind->setText("Ошибка");
    }

}

double calculator::evaluateExpression(const QString &expression) {
    QJSEngine engine;
    QJSValue result = engine.evaluate(expression);

    if (result.isError()) {
        return qSNaN();
    }
    return result.toNumber();
}

void calculator::saveCalculation(const QString &calculation, int userId) {
    // Получаем экземпляр клиента
    SingletonClient& client = SingletonClient::getInstance();

    // Вызываем метод для сохранения вычисления на сервере
    QByteArray response = client.saveCalculation(userId, calculation);

    // Обрабатываем ответ
    if (response.startsWith("saveCalculation&success")) {
        qDebug() << "Вычисление '" << calculation << "' успешно сохранено для пользователя " << userId;
    } else {
        qDebug() << "Ошибка при сохранении вычисления '" << calculation << "' для пользователя " << userId << ": " << response;
    }
}

