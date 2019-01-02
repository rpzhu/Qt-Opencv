#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QRect>
#include "autowork.h"
#include "shoudongceshi.h"
#include "setting.h"
#include "changepassword.h"
#include "logindialog.h"
#include <QMenu>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    autowork *operation_interface ;
    shoudongceshi *manually_debug ;
    setting *set ;
    changepassword *cp;

private:
    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent* event);
    bool readInit(QString path, QString user_key, QString &user_value);
    bool writeInit(QString path, QString user_key, QString user_value);
private slots:
    void operationSlot();
    void manually_debugSlot();
    void setSlot();
    void changepasswordSlot();
    void logoutSlot();
    void loginSlot();
    void informationSlot();

   // void usersys_menuSlot();
};

#endif // MAINWINDOW_H
