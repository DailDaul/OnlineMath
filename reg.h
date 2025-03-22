#ifndef REG_H
#define REG_H

#include <QDialog>
#include <QLabel>
#include <QMovie>

namespace Ui {
class reg;
}

class reg : public QDialog
{
    Q_OBJECT

public:
    explicit reg(QWidget *parent = nullptr);
    QString getUserId() const;
    ~reg();

private slots:
    void on_registerButton_clicked();
    void on_backButton_clicked();
private:
    Ui::reg *ui;
    QLabel *backgroundLabel;
    QMovie *movie;
    QString userId;
};

#endif // REG_H
