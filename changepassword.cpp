#include "changepassword.h"
#include "ui_changepassword.h"

changepassword::changepassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changepassword)
{
    ui->setupUi(this);
     this->setFont(QFont("DejaVuSansMono.ttf"));
    ui->le_currentPassword->setEchoMode(QLineEdit::Password);
    ui->le_newpassword->setEchoMode(QLineEdit::Password);
    ui->le_again->setEchoMode(QLineEdit::Password);
    bool a = readInit(QString ("./user.ini"), "password", password);
    connect(ui->pb_ok,SIGNAL(clicked(bool)),this,SLOT(okSlot()));
    connect(ui->pb_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
}

changepassword::~changepassword()
{
    delete ui;
}

void changepassword::okSlot()
{
    QString pw = ui->le_currentPassword->text().toLatin1().toBase64();

    if(pw != password)
    {
        QMessageBox::information(this,"error","原密码输入不正确，请重新输入");
        ui->le_currentPassword->setFocus();
    }


    else
    {
        if(ui->le_again->text() != ui->le_newpassword->text())
        {
            QMessageBox::information(this,"error","两次密码输入不一致，请重新设置");
            ui->le_newpassword->setFocus();
        }
        else if(ui->le_newpassword->text().length() < 6)
        {
            QMessageBox::information(this,"error","输入密码长度请大于等于6");
            ui->le_newpassword->setFocus();
        }
        else
        {
            QString password = ui->le_again->text().toLatin1().toBase64();
           // bool is_username = writeInit(QString("./user.ini"), "username", "address");
            bool is_password = writeInit(QString("./user.ini"), "password", password);

            if(is_password)
            {
                QMessageBox::information(this,"设置成功","更改成功，设置已生效");
                this->close();
            }
        }
    }
}

bool changepassword::writeInit(QString path, QString user_key, QString user_value)
{
    if(path.isEmpty() || user_key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings *config = new QSettings(path, QSettings::IniFormat);

        //将信息写入配置文件
        config->beginGroup("config");
        config->setValue(user_key, user_value);
        config->endGroup();

        return true;
    }
}

bool changepassword::readInit(QString path, QString user_key, QString &user_value)
{
    user_value = QString("");
    if(path.isEmpty() || user_key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings *config = new QSettings(path, QSettings::IniFormat);

        //读取用户配置信息
        user_value = config->value(QString("config/") + user_key).toString();

        return true;
    }
}
