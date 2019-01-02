#include "logindialog.h"
#include "ui_logindialog.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
     this->setFont(QFont("DejaVuSansMono.ttf"));
    ui->le_password->setEchoMode(QLineEdit::Password);
    //this->setWindowFlags(Qt::WindowSystemMenuHint);


        bool a = readInit(QString ("./user.ini"), "password", password);
        bool b = readInit(QString ("./user.ini"), "username", username);



    connect(ui->pb_ok,SIGNAL(clicked(bool)),this,SLOT(loginSlot()));
    isLogin = false;
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::loginSlot()
{
    QString un = ui->le_username->text();
    QString pw = ui->le_password->text().toLatin1().toBase64();
    if(pw == password & un == username)
    {
        qDebug("successful");
        isLogin = true;
        this->close();
    }
    else{

        ui->le_username->setFocus();
        QMessageBox::information(this,"error","\用户名或密码错误，请重新输入！");
        isLogin = false;
    }
}

void loginDialog::cancelSlot()
{

}

bool loginDialog::writeInit(QString path, QString user_key, QString user_value)
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

bool loginDialog::readInit(QString path, QString user_key, QString &user_value)
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

//void loginDialog::closeEvent( QCloseEvent * event )//
//{
//    if(isLogin)
//    {
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//        QMessageBox::information(this,"提示","只有您正确登陆后此窗口才会退出");
//    }
//}






//switch( QMessageBox::information( this, tr("退出"),tr("您将要退出?"),tr("Yes"), tr("No"),0, 1 ) )
//{
//case 0:
//      event->accept();
//    break;
//case 1:
//default:
//    event->ignore();
//    break;
//}
