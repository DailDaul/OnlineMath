#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QMessageBox>
#include "clientmanager.h"
#include "reg.h"
#include "auth.h"
namespace Ui {
class firstwindow;
}

class firstwindow : public QDialog
{
    Q_OBJECT

public:
    explicit firstwindow(QWidget *parent = nullptr);
    ~firstwindow();
private slots:
    void on_regButton_clicked();
    void on_authButton_clicked();

private:
    reg *regWindow = nullptr;
    auth *authWindow = nullptr;
    Ui::firstwindow *ui;
    QLabel *backgroundLabel;
    QMovie *movie;
 ClientManager *clientManager;
};

#endif // FIRSTWINDOW_H
