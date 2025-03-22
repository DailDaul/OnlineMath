#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <QListWidget>
#include <QStringList>
#include <QDebug>

class memory : public QDialog {
    Q_OBJECT

public:
    explicit memory(int userId, QWidget *parent = nullptr);
    ~memory();

public slots:
    void updateMemoryDisplay(const QStringList& calculations);

signals:
    void closed(int userId);

private:
    int userId;
    QListWidget *memoryListWidget;
};

#endif // MEMORY_H
