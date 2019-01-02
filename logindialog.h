#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>
#include <QSettings>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();


    QString password;
    QString username;
public slots:
    void loginSlot();
    void cancelSlot();

public:
    bool writeInit(QString path, QString user_key, QString user_value);
    bool readInit(QString path, QString user_key, QString &user_value);
    bool isLogin;
private:
    Ui::loginDialog *ui;

  // void closeEvent( QCloseEvent * event );
};

#endif // LOGINDIALOG_H
