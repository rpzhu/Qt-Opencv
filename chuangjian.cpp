#include "chuangjian.h"
#include "ui_chuangjian.h"
#include <QCursor>
#include <QDebug>

volatile int function = 0;

chuangjian::chuangjian(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chuangjian)
{

    function = 0;
    ui->setupUi(this);
     this->setFont(QFont("DejaVuSansMono.ttf"));
   // this->setWindowFlags(Qt::WindowSystemMenuHint);

     QList<QPushButton *> btns = this->findChildren<QPushButton *>();//通过类模板。创建新的对象
    foreach (QPushButton * btn, btns)
    {//foreach为btns集合中的每一个类型执行都执行一遍这个函数。
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }

     connect(ui->pb_tuodong,SIGNAL(clicked(bool)),this,SLOT(tuodongbuttonSlot(bool)));
     connect(ui->pb_kuangtu1,SIGNAL(clicked(bool)),this,SLOT(kuangtu1Slot(bool)));
     connect(ui->pb_kuangtu2,SIGNAL(clicked(bool)),this,SLOT(kuangtu2Slot(bool)));
    connect(ui->pb_ok,SIGNAL(clicked(bool)),ui->label,SLOT(okSlot()));
    connect(ui->label,SIGNAL(dragSignal(float,float)),this,SLOT(tuodongSlot(float,float)));


}

chuangjian::~chuangjian()
{
    delete ui;
}


void chuangjian::buttonClick()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objName = btn->objectName();
    if (objName == "pb_tuodong")
    {

      //ui->label->tuodongSlot();



    }
    else if (objName == "pb_kuangtu1")
    {

    }
    else if (objName == "pb_kuangtu2")
    {


    }
    else if (objName == "pb_fangda")
    {

        ui->label->ZoomInSlot();
        //scaleImage(2);
        //ui->scrollArea->bei = ui->label->scale;
    }
    else if (objName == "pb_suoxiao")
    {

        ui->label->ZoomOutSlot();
       // scaleImage(0.5);
    }
    else if (objName == "pb_ok")
    {

        ui->label->okSlot();
       // emit okSignal();

       // this->close();

    }
}

void chuangjian::tuodongbuttonSlot(bool istuodong)
{   if(istuodong)
    {
        if(ui->pb_kuangtu1->isChecked())
            ui->pb_kuangtu1->setChecked(false);
        if(ui->pb_kuangtu2->isChecked())
            ui->pb_kuangtu2->setChecked(false);

        //      emit buttonSlect(1);//拖动放送信号1；
        function = 1;
        ui->label->setCursor(Qt::OpenHandCursor);
     //  ui->pb_tuodong->setStyleSheet("background-color:#ffffff");
    }
    else
    {
        function = 0 ;
        ui->label->setCursor(Qt::ArrowCursor);
 //       ui->pb_tuodong->setStyleSheet( "background-color:0");
//                                      padding: 0px;
//                                      min-height: 10px;
//                                      border-radius:3px;
//                                      background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #1B89CA, stop:1 #1077B5)");
    }
}

void chuangjian::kuangtu1Slot(bool iskuangtu)
{
    if(iskuangtu)
    {
    if(ui->pb_tuodong->isChecked())
        ui->pb_tuodong->setChecked(false);
    if(ui->pb_kuangtu2->isChecked())
        ui->pb_kuangtu2->setChecked(false);

    ui->label->setCursor(Qt::CrossCursor);
    ui->label->PaintRecSlot();

    }
    else
    {
        function = 0;
        ui->label->setCursor(Qt::ArrowCursor);
    }
}

void chuangjian::kuangtu2Slot(bool iskuangtu2)
{
    if(iskuangtu2)
    {
        if(ui->pb_tuodong->isChecked())
            ui->pb_tuodong->setChecked(false);
        if(ui->pb_kuangtu1->isChecked())
            ui->pb_kuangtu1->setChecked(false);
        ui->label->setCursor(Qt::CrossCursor);
        ui->label->PaintPolygonSlot();
    }
    else
    {
        function = 0;
        ui->label->setCursor(Qt::ArrowCursor);
    }
}

void chuangjian::tuodongSlot(float x, float y)
{
    current_x=ui->scrollArea->horizontalScrollBar()->value();
    current_y=ui->scrollArea->verticalScrollBar()->value();
    //qDebug()<<current_x<<current_y;
    ui->scrollArea->horizontalScrollBar()->setValue(current_x+x);
    ui->scrollArea->verticalScrollBar()->setValue(current_y+y);
}

void chuangjian::sendimage()
{
    ui->label->myframe_image = m_image;

}



