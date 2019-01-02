#ifndef MYFRAME_H
#define MYFRAME_H

#include <QVector>  // STL: vector
#include <QMouseEvent>
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QPolygon>
#include <QPicture>
#include <QPointF>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;



class myframe : public QLabel
{
    Q_OBJECT
public:
    myframe(QWidget *parent);
    ~myframe();
signals:
    void selected(QRect area);
    void dragSignal(float ,float);
private:
    void paintEvent ( QPaintEvent * event );

    // 鼠标支持
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    bool MaxMinx(QVector <QPoint> array, int* xmax, int* xmin) ;
    bool MaxMiny(QVector <QPoint> array, int* ymax, int* ymin) ;

    //void tuodong(QPainter);
public slots:
   // void tuodongSlot();
    void PaintRecSlot();
    void PaintPolygonSlot();
    void ZoomInSlot();
    void ZoomOutSlot();
    void okSlot();


public:
    float scale;

    float rec_scale;

    float current_scale;

    bool isRecScreenShot;

    bool isPolygonScreenShot;

    bool isOk;

    QPixmap p;

    Mat img,showImg;
    Mat imgmask;
public:
    QImage myframe_image;//接收来自创建模块窗口的图片。

private:
  // QImage  Mat2QImage(cv::Mat const& src);
   //cv::Mat QImage2Mat(QImage const& src)  ;

    int timerId;//every timer have a id
    double rec_scare_times;//次数。
    bool m_pressflag;

    int rec_index;//矩形数组的索引
    int polygon_index;//多边形数组的索引


    QPixmap m_picture;
    QPoint m_curpos;//鼠标当前位置。
    QPoint startpos ,endpos;

    int width;//获得画布宽度
    int height ;//获得画布高度。


    QRect area;
    QPolygon m_polygon;
    QRegion m_region;

    int movex;
    int movey;


    float x_offset;
    float y_offset;



   QImage image;

    QVector <QPoint> m_points;
    QVector <QPoint> Polygon_points;
    QVector <QPoint> b; //用于存临时多边形坐标
    vector <vector <Point> > opencv_points;
    vector <vector <Point> >  c; //用于存临时opencv坐标
  //const  Point *p1[1];

   Point pt[1][20];
  const Point* ppt[1];
    int npt[1];
    QPoint rec_points[2];
    QPoint a[2]; //用于存临时矩形坐标




};

#endif // MYFRAME_H
