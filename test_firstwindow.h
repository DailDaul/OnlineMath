#ifndef TEST_FIRSTWINDOW_H
#define TEST_FIRSTWINDOW_H

#include <QtTest/QtTest>
#include "firstwindow.h"
#include "clientmanager.h"

class TestFirstWindow : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testOpenRegWindow();
    void testOpenAuthWindow();
private:
    firstwindow *firstWindow;
    ClientManager *clientManager;
};

#endif // TEST_FIRSTWINDOW_H

