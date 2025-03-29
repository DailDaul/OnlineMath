#ifndef REG_H
#define REG_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include "clientmanager.h"
namespace Ui {
class reg;
}

class reg : public QDialog
{
    Q_OBJECT

public:
    explicit reg(ClientManager *clientManager, QWidget *parent = nullptr);
    QString getUserId() const;
    ~reg();

private slots:
    void on_registerButton_clicked(); // Слот для кнопки регистрации
    void on_backButton_clicked();
private:
    ClientManager *m_clientManager;
    Ui::reg *ui;
    QLabel *backgroundLabel; // QLabel для фона
    QMovie *movie; // QMovie для GIF
    QString userId;
};

#endif // REG_H
