#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QDialog>
#include <QStringList>
#include <QLabel>
#include "memory.h"
#include <QPointer>
namespace Ui {
class calculator;
}

class calculator : public QDialog {
    Q_OBJECT

public:
    explicit calculator(int userId, QDialog *parent = nullptr);

    QStringList getRecentCalculations() const { return recentCalculations; }
    ~calculator();

private slots:
    void on_oneButton_clicked();
    void on_twoButton_clicked();
    void on_threeButton_clicked();
    void on_fourButton_clicked();
    void on_fiveButton_clicked();
    void on_sixButton_clicked();
    void on_sevenButton_clicked();
    void on_eightButton_clicked();
    void on_nineButton_clicked();
    void on_zeroButton_clicked();
    void on_plusButton_clicked();
    void on_minsButton_clicked();
    void on_multButton_clicked();
    void on_dashButton_clicked();
    void on_sqrtButton_clicked();
    void on_editButton_clicked();
    void on_cleanButton_clicked();
    void on_scobButton_clicked();
    void on_procentButton_clicked();
    void on_cubeButton_clicked();
    void on_drButton_clicked();
    void on_pointButton_clicked();
    void on_minButton_clicked();
public slots:
    void on_memoryButton_clicked();
    void onHistoryReceived(int userId, const QStringList& history);
    void onMemoryClosed(int userId);



private:
    Ui::calculator *ui;
    QLabel *backgroundLabel;
    QStringList recentCalculations;
    QPointer<memory> memoryInstance;
    bool isOpen = false;
    bool isConnected = false;
    double evaluateExpression(const QString &expression);
    void saveCalculation(const QString &calculation, int userId);
    int userId;

};

#endif // CALCULATOR_H


