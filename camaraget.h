#ifndef CAMARAGET_H
#define CAMARAGET_H

#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;



class camaraget : public QLabel
{
    Q_OBJECT

public:
    explicit camaraget(QWidget *parent = 0);
    ~camaraget();

//signals:
   // void getimage(QPixmap );
public slots:
    void openCamara();      // 打开摄像头
    int readFarme();       // 读取当前帧信息
    void closeCamara();     // 关闭摄像头。
    QImage takingPictures();  // 拍照

public:
    QPixmap getimage;
    CvCapture *cam;
    IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    QImage    *imag; // 视频获取结构， 用来作为视频获取函数的一个参数
    IplImage  *outframe;
    QTimer    *timer;

private:




};

#endif // CAMARAGET_H
