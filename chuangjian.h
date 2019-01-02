#ifndef CHUANGJIAN_H
#define CHUANGJIAN_H

#include <QWidget>
#include <QScrollBar>
#include "myframe.h"

namespace Ui {
class chuangjian;
}

class chuangjian : public QWidget
{
    Q_OBJECT

public:
    explicit chuangjian(QWidget *parent = 0);
    ~chuangjian();


   // QImage image;
//signals:
//    void buttonSlect(int );
public slots:
   void buttonClick();

   void tuodongbuttonSlot(bool);
   void kuangtu1Slot(bool);
   void kuangtu2Slot(bool);
   void tuodongSlot(float,float);

private:
   double scaleFactor;
   float current_x;
   float current_y;

signals:
   void okSignal();

public:
   QImage m_image;////接收从手动测试窗口过来的图片。
   void sendimage();//发送m_image到myframe;

public:
    Ui::chuangjian *ui;
};

#endif // CHUANGJIAN_H
