#include "camaraget.h"
//#include "ui_camaraget.h"

camaraget::camaraget(QWidget *parent) :
    QLabel(parent)

{

    //setAttribute(Qt::WA_DeleteOnClose);//解决qt代码关闭子窗口不走析构函数问题。
    qDebug()<<"open camera 22222";
    cam     = NULL;
    qDebug()<<"open camera 33333";
    timer   = new QTimer(this);
    qDebug()<<"open camera4444444";
    //imag    = new QImage(this);         // 初始化

    /*信号和槽*/
    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
qDebug()<<"open camera  111";

    openCamara();


   int a= readFarme();
}


camaraget::~camaraget()
{

   //timer->stop();         // 停止读取数据。
    //cvReleaseCapture(&cam);//释放内存；
    closeCamara();
    cam = NULL;
    //disconnect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));

    qDebug()<<"close camera";
}

/******************************
********* 打开摄像头 ***********
*******************************/
void camaraget::openCamara()
{
    cam = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频
    if(cam != NULL)
    timer->start(33);              // 开始计时，超时则发出timeout()信号
    qDebug()<<"open camera";
}

/*********************************
********* 读取摄像头信息 ***********
**********************************/
int camaraget::readFarme()
{
    if(cam != NULL)
    {
        //qDebug()<<"空";
        frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧
        outframe = (IplImage*)malloc(sizeof(*frame));
        if(!outframe)
        {
           // qDebug()<<"空";
         return 0 ;
        }
            *outframe = *frame;

        QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
        this->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上

    }
    else{

        return 0 ;
    }
}
/*************************
********* 拍照 ***********
**************************/
QImage camaraget::takingPictures()
{
    frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧

    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
        QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();


        return image;
        //getimage=(QPixmap::fromImage(image));

   // this->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上
}

/*******************************
***关闭摄像头，释放资源，必须释放***
********************************/
void camaraget::closeCamara()
{
    timer->stop();         // 停止读取数据。

    cvReleaseCapture(&cam);//释放内存；
}

// 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
//   int width = frame->width;//获得画布宽度
//   int height = frame->height;//获得画布高度。

//    this->setMinimumWidth(width);
//    this->setMinimumHeight(height);
// QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);
