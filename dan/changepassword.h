#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class changepassword;
}

class changepassword : public QDialog
{
    Q_OBJECT

public:
    explicit changepassword(QWidget *parent = 0);
    ~changepassword();
private slots:
    void okSlot();

public:
    QString password;
    bool writeInit(QString path, QString user_key, QString user_value);
    bool readInit(QString path, QString user_key, QString &user_value);

private:
    Ui::changepassword *ui;
};

#endif // CHANGEPASSWORD_H
