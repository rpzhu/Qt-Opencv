#include "myframe.h"
#include <QPainter>
#include <QTimerEvent>
#include <QDebug>
#include <QRect>
#include <QImage>
#include <QColor>
#include <QDebug>
#include <QSize>
#include <QRegion>
#include "shoudongceshi.h"
#include <QImageReader>
#include <QCursor>



extern  QImage * get_image;
//volatile float bei = 1;
extern volatile int function;

myframe::myframe(QWidget *parent)
    : QLabel(parent),rec_scare_times(0)
{
    scale = 1;
    rec_scale = 1;
    memset(rec_points,0,sizeof(rec_points));
    memset(a,0,sizeof(a));
    b.clear();
    //m_points.clear();
    Polygon_points.clear();
    opencv_points.clear();
    c.clear();
    rec_index = 0;
    movex = 0;
    movey = 0;
    m_pressflag = false;
    isPolygonScreenShot = false;
    isRecScreenShot = false;
    isOk = false;
//    m_picture.fromImage(*get_image);
    this->setPixmap(m_picture);
    m_picture.load("./temp.png");//注意从文件浏览器复制粘贴的路径为反斜杠，改成正斜扛
    //img = imread("C:/Users/zhu/Desktop/20161213100118.png",1);
    width = m_picture.width();//获得画布宽度
    height = m_picture.height();//获得画布高度。
    qDebug()<<width;
    this->setMinimumWidth(width);
    this->setMinimumHeight(height);


}

myframe::~myframe()
{
    //m_points.clear();
    Polygon_points.clear();
    // opencv_points.clear();
     memset(rec_points,0,sizeof(rec_points));
     b.clear();
}


void myframe::paintEvent ( QPaintEvent * event )
{
    this->setMinimumWidth(width*scale);
    this->setMinimumHeight(height*scale);


    QPainter painter(this);
    Mat temp1(width*scale, height*scale, CV_8U, Scalar(0));
    imgmask = temp1;
    m_picture = m_picture.scaled(QSize(width*scale,height*scale));
    //将图片的宽和高都扩大两倍，并且在给定的矩形内保持宽高的比值

   // m_picture.save("C:/Users/zhu/Desktop/b.png");



    QBrush brush(Qt::white);
    painter.setBackground(brush);
    painter.drawPixmap(0,0,m_picture);

    QPen pen(Qt::red, 1);
    painter.setPen(pen);

    if(isRecScreenShot)
    {
        painter.drawPoint(rec_points[0]);
        painter.drawPoint(rec_points[1]);
        QRect selecter(rec_points[0],rec_points[1]);
        painter.drawRect(selecter);
        area = selecter;

    }

    if(isPolygonScreenShot)
    {

        QPen pen(Qt::red, 1);
        painter.setPen(pen);

        m_polygon = QPolygon(Polygon_points);
        painter.drawPoints(Polygon_points);
        painter.drawPolygon(m_polygon);

        if(isOk)
        {
            QPixmap ok_picture = m_picture;
             QPainter p(&ok_picture);

            QRect full(0,0,ok_picture.width(),ok_picture.height());

           // p.setBrush(QColor(255,255,255,255));
            p.setBrush(QColor(0,0,0,130));
            p.setClipRegion(QRegion(full)-QRegion(m_polygon));
            p.drawRect(full);
            p.end();

            int xmax,  xmin, ymax, ymin;
           bool x =  MaxMinx(m_polygon, &xmax, &xmin);
           bool y =  MaxMiny(m_polygon, &ymax, &ymin) ;

           qDebug()<<xmin<<","<< xmax<<","<< ymin<<","<<ymax;
           QRect ok_polygon(QPoint(xmin,ymin),QPoint(xmax,ymax));

           QPixmap pp;
           pp = ok_picture.copy(ok_polygon);
           //////////////////////////////////////////////////
             pp = pp.scaled(QSize(pp.width()/scale,pp.height()/scale));
           ///////////////////////////////////////
            pp.save("./a.png");
          //p.setBrush(QColor(255,255,255,255));

            isOk = false;
            //qDebug()<<"多边形";
            update();
        }
    }

}



void myframe::mouseMoveEvent ( QMouseEvent * event )
{

    if(m_pressflag)
    {
        QPoint pos = event->pos();
        endpos = pos;

        m_curpos = pos;
        //m_points.append(pos);


        // update();
    }
    if(function == 1)
    {
        x_offset = startpos.x()-endpos.x();
        y_offset = startpos.y()-endpos.y();
        emit dragSignal(x_offset,y_offset);
    }

    if(function == 3)
    {
        if(isPolygonScreenShot)
        {

        }
    }
    QWidget::mouseMoveEvent(event);

}

void myframe::mousePressEvent ( QMouseEvent * event )
{
    m_pressflag = true ;


    if(function == 1)
    {
        this->setCursor(Qt::ClosedHandCursor);
    }

    startpos = event->pos();
    endpos = event->pos();
    int x = startpos.x();
    int y = startpos.y();
    if(function == 3 && event->button()==Qt::LeftButton)
    {
         current_scale = scale;
        isRecScreenShot = false;

        Polygon_points.append(startpos);
        b = Polygon_points;

        isPolygonScreenShot = true;

        //opencv_points.append(Point(x,y));
        opencv_points.resize(Polygon_points.size());
        opencv_points[0].push_back(Point(x,y));
        c.clear();
        c = opencv_points;
        update();
    }
    if(function == 2 && event->button()==Qt::LeftButton)
    {
        isPolygonScreenShot = false;
        current_scale = scale;
        if(rec_index == 1)
        {
        isRecScreenShot = true;

        }
        if(rec_index == 2)
        {
            memset(rec_points,0,sizeof(rec_points));
            rec_index = 0;
        }
       rec_points[rec_index] = startpos;
        if(rec_index == 1)
        {
            a[0]= rec_points[0];
            a[1]= rec_points[1];

             update();
       }
             rec_index++;


    }

    // m_points.clear();//清空数组




}

void myframe::mouseReleaseEvent ( QMouseEvent * event )
{

    endpos = event->pos();
    if(function == 2)
    {
        QRect selecter (startpos,endpos);//不用等号。此为构建函数。
       // area = selecter;
    }
    m_pressflag = false ;

    if(function == 1)
    {
        this->setCursor(Qt::OpenHandCursor);

    }

    if(function == 3)
    {
        m_region = QRegion(m_polygon);
    }

    if(event->button()==Qt::RightButton)
    {
        // function = 0;
        isRecScreenShot = false;
        isPolygonScreenShot = false;
        isOk = false;
         rec_scare_times = 0;
        rec_scale = 1;
        qDebug()<<"hello ````"<<endl;
        if(Polygon_points.size() != 0)
        {
            Polygon_points.clear();
            opencv_points.clear();
            c.clear();
            b.clear();
        }

         memset(rec_points,0,sizeof(rec_points));
         memset(a,0,sizeof(a));


        update();
    }
}

bool myframe::MaxMinx(QVector <QPoint> array, int* xmax, int* xmin)
{

    if (array.size() < 1) {
       return false;
     }
     *xmax = array[0].x();
    *xmin = array[0].x();
     size_t array_size = array.size();
     for (int i = 1; i < array_size; ++i) {
      if (array[i].x() > *xmax) {
         *xmax = array[i].x();
       } else if (array[i].x() < *xmin) {
         *xmin = array[i].x();
       }
     }
     return true;

}

bool myframe::MaxMiny(QVector<QPoint> array, int *ymax, int *ymin)
{
    if (array.size() < 1) {
       return false;
     }
     *ymax = array[0].y();
    *ymin = array[0].y();
     size_t array_size = array.size();
     for (int i = 1; i < array_size; ++i) {
      if (array[i].y() > *ymax) {
         *ymax = array[i].y();
       } else if (array[i].y() < *ymin) {
         *ymin = array[i].y();
       }
     }
     return true;
}



void myframe::PaintRecSlot()
{

    function = 2;
//    if(!(m_points.isEmpty()))
//    {
//        m_points.clear();
//    }

    //update();
}

void myframe::PaintPolygonSlot()
{

    function = 3;
//    if(!(Polygon_points.isEmpty()))
//    {
//        Polygon_points.clear();
//    }
}
void myframe::ZoomInSlot()
{

    scale = scale +0.25;

    qDebug()<<scale;
    if(isRecScreenShot)
    {
        rec_scale = rec_scale + 0.25;
        qDebug()<<rec_scale;

        rec_points[0] = (a[0]/current_scale)*((1)*scale);
        rec_points[1] = (a[1]/current_scale)*((1)*scale);

    }

    if(isPolygonScreenShot)
    {
        for(int polygon_index = 0 ;polygon_index<Polygon_points.size();polygon_index++)
        {
            Polygon_points[polygon_index] = (b[polygon_index]/current_scale)*scale;
          //  opencv_points[0][polygon_index].x = (c[0][polygon_index].x/current_scale)*scale;
           // opencv_points[0][polygon_index].y = (c[0][polygon_index].y/current_scale)*scale;

            pt[0][polygon_index] = Point((c[0][polygon_index].x/current_scale)*scale,(c[0][polygon_index].y/current_scale)*scale);

            ppt[0] = {pt[0]};
            npt[0] = {Polygon_points.size()};
        }
        Scalar color = Scalar(255,255,255);
       fillPoly(imgmask, ppt, npt, 1, color);

//       imshow("te",imgmask);
//       Mat r;
//       QImage a("C:/Users/zhu/Desktop/b.png");

//       img = QImage2Mat(a);
////     // bitwise_and(img, imgmask, r);
////      imshow("Bit_and", r);

//       img.copyTo(r , imgmask);

      //  p1 [0]= &opencv_points[0];
//        int a = Polygon_points.size();
//        Point b[1][a];

//        const Point* ppt[1] = {opencv_points[0]};
//        //int a = Polygon_points.size();
//        npt[0] = {a};


    }

    //bei = scale;
    m_picture.load("./temp.png");//必须重载，不然图片会失真。
    update();


}

void myframe::ZoomOutSlot()
{

    qDebug()<<"suoxiao";
    scale = scale - 0.25;
    //bei = scale;
    m_picture.load("./temp.png");;

    if(isRecScreenShot)
    {

        rec_scale = rec_scale - 0.25;
        rec_points[0] = (a[0]/current_scale)*((1)*scale);
        rec_points[1] = (a[1]/current_scale)*((1)*scale);

    }
    if(isPolygonScreenShot)
    {
        for(int polygon_index = 0 ;polygon_index<Polygon_points.size();polygon_index++)
        {
            Polygon_points[polygon_index] = (b[polygon_index]/current_scale)*scale;
        }

    }


    update();
}

void myframe::okSlot()
{
    qDebug()<<"ok";
    if(isRecScreenShot)
    {
         p=m_picture.copy(area);

         /////////////////////////////////////////////////////
         p = p.scaled(QSize(p.width()/scale,p.height()/scale));
         ////////////////////////////////////////////

         p.save("./a.png");
    }
    if(isPolygonScreenShot)
    {
        isOk = true;
        // QPixmap p = m_picture;
        update();
    }
}





//QImage myframe::Mat2QImage(const Mat &src)
//{
//    cv::Mat temp; // make the same cv::Mat
//        cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
//        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
//        dest.bits(); // enforce deep copy, see documentation
//        // of QImage::QImage ( const uchar * data, int width, int height, Format format )
//            return dest;
//}

//Mat myframe::QImage2Mat(const QImage &src)
//{
//    cv::Mat tmp(src.height(),src.width(),CV_8U,(uchar*)src.bits(),src.bytesPerLine());
//        cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
//        cvtColor(tmp, result,CV_RGB2BGR);
//        return result;
//}







