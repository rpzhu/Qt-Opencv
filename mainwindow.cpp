#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFont(QFont("DejaVuSansMono.ttf"));
    operation_interface = NULL;
    manually_debug = NULL;
    set = NULL;
   // setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);//去除窗体边框

  //this->showFullScreen();

    ///////////////////////////////////////////
    QFile file("./user.ini");
    if(file.exists())
    {

    }
    else
    {
         QString pw = "123456";
                pw = pw.toLatin1().toBase64();
                QString username = "root";
        bool a = writeInit(QString ("./user.ini"), "password", pw);
        bool b = writeInit(QString ("./user.ini"), "username", username);
//        file.open( QIODevice::ReadWrite  );
//        file.close();
    }
    ///////////////////////////////////////////////////


    connect(ui->tb_yunxing,SIGNAL(clicked(bool)),
            this,SLOT(operationSlot()) );//运行界面窗口

    connect(ui->tb_manually_debug,SIGNAL(clicked(bool)),
            this,SLOT(manually_debugSlot()));//手动调试窗口

    connect(ui->tb_set,SIGNAL(clicked(bool)),
            this,SLOT(setSlot()));//系统设置窗口

    connect(ui->actionYu,SIGNAL(triggered(bool)),
            this,SLOT(close()));//菜单栏退出

    connect(ui->actionDs,SIGNAL(triggered(bool)),
            this,SLOT(changepasswordSlot()));//菜单栏重设置密码；

    connect(ui->actionAa,SIGNAL(triggered(bool)),
            this,SLOT(logoutSlot())); //注销

    connect(ui->actionLog,SIGNAL(triggered(bool)),
            this,SLOT(loginSlot()));

    connect(ui->actionSd,SIGNAL(triggered(bool)),
            this,SLOT(informationSlot()));

    //logoutSlot();
    //loginSlot();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    int a = this->width();
    int b = this->height();
    int c = this->menuBar()->height();
    int d = this->ui->label->height();
    QRect rect(-1,0,a+1,c+d);
    QPainter p(this);
    p.setPen(QColor(255,255,255));
    //QColor b(4c93ef);
    QLinearGradient linear(QPointF(-1, c), QPointF(-1, c+d+100));
        linear.setColorAt(0, QColor(76, 147, 239));
        linear.setColorAt(1, Qt::white);

        // 设置显示模式
        linear.setSpread(QGradient::PadSpread);

    p.setBrush(linear);
    //p.setBrush(QColor(76, 147, 239));
    p.drawRect(rect);

    p.setBrush(QColor(255,255,255));
    QRect rect2(-1,c+d,a+1,b);//292
    p.drawRect(rect2);

}

void MainWindow:: operationSlot()
{
    operation_interface = new autowork();
    operation_interface->setWindowModality(Qt::ApplicationModal);
    operation_interface->show();

}

void MainWindow::manually_debugSlot()
{
    qDebug()<<"open camera777777";

    manually_debug = new shoudongceshi();
    manually_debug->setWindowModality(Qt::ApplicationModal);
    manually_debug->show();
}

void MainWindow::setSlot()
{
    if(set != NULL)
        delete set;
    set = new setting();
    set->setWindowModality(Qt::ApplicationModal);
    set->show();
}

void MainWindow::changepasswordSlot()
{
     cp = new changepassword();
    cp->exec();
}

void MainWindow::logoutSlot()
{
    ui->tb_manually_debug->setEnabled(false);
    ui->tb_set->setEnabled(false);
    ui->tb_yunxing->setEnabled(false);
    ui->actionDs->setEnabled(false);
    ui->actionAa->setEnabled(false);



}

void MainWindow::loginSlot()
{
    loginDialog l;
    l.exec();
    if(l.isLogin)
    {
        ui->tb_manually_debug->setEnabled(true);
        ui->tb_set->setEnabled(true);
        ui->tb_yunxing->setEnabled(true);
        ui->actionDs->setEnabled(true);
        ui->actionAa->setEnabled(true);
    }
}

void MainWindow::informationSlot()
{
    QMessageBox::information(this,"信息","制造厂商：深圳市*****有限公司\n服务热线：0755-86548719");
}

bool MainWindow::writeInit(QString path, QString user_key, QString user_value)
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

bool MainWindow::readInit(QString path, QString user_key, QString &user_value)
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

