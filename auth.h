#ifndef AUTH_H
#define AUTH_H
#include "calculator.h"
#include <QDialog>
#include <QLabel>
#include <QMovie>
#include "clientmanager.h"
namespace Ui {
class auth;
}

class auth : public QDialog
{
    Q_OBJECT

public:
    explicit auth(ClientManager *clientManager, QDialog *parent );
    ~auth();
private slots:
    void on_authButton_clicked();
    void on_backtoButton_clicked();
private:
    Ui::auth *ui;
    QString userId;
    QLabel *backgroundLabel;
    QMovie *movie;
    ClientManager *clientManager;

};

#endif // AUTH_H
