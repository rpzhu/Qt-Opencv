#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
     this->setFont(QFont("DejaVuSansMono.ttf"));
    setAttribute(Qt::WA_DeleteOnClose);//解决子窗口关闭不执行析构函数问题。
  //  this->setWindowFlags(Qt::WindowSystemMenuHint);
}

setting::~setting()
{
    delete ui;
}

void setting::paintEvent()
{
    QPainter p(this);
    QRect rec(-1,0,this->width(),ui->label->height());
    p.setBrush(QColor(0,0,255));
    p.drawRect(rec);
}
