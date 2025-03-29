#include "memory.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QStringList>

memory::memory(int userId, QWidget *parent) : QDialog(parent), userId(userId) {
    memoryListWidget = new QListWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(memoryListWidget);
    setLayout(layout);


    this->setStyleSheet("QDialog { background-image: url(C:/Users/novos/Documents/Frontend/Frontend/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fon.jpg); }");


    QString listStyle = "QListWidget {"
                        "background-color: rgba(255, 255, 255, 230);"
                        "border: 1px solid rgba(255, 184, 224, 1);"
                        "border-radius: 10px;"
                        "padding: 10px;"
                        "font-size: 14px;"
                        "}"
                        "QListWidget:hover {"
                        "background-color: rgba(255, 255, 255, 235);"
                        "border: 1px solid rgba(252, 169, 216, 1);"
                        "}";


    memoryListWidget->setStyleSheet(listStyle);
    memoryListWidget->setVisible(true);
}


memory::~memory() {

}

void memory::updateMemoryDisplay(const QStringList& calculations) {
    // Очищаем предыдущие записи
    memoryListWidget->clear();

    // Добавляем новые вычисления в список
    for (const QString &calculation : calculations) {
        memoryListWidget->addItem(calculation);
    }
}

