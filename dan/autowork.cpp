#include "autowork.h"
#include "ui_autowork.h"
#include <QImage>
#include <QPixmap>
autowork::autowork(QWidget *parent) :
    QWidget(parent),sumnum(0),
    ui(new Ui::autowork)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);//去除窗体边框
    //this->showFullScreen();
     this->setFont(QFont("DejaVuSansMono.ttf"));
     setAttribute(Qt::WA_DeleteOnClose);//解决子窗口关闭不执行析构函数问题。
    connect(ui->pb_tuichu,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pb_auto,SIGNAL(clicked(bool)),this,SLOT(manualPrintingSlot(bool)));
     connect(this,SIGNAL(processValue(int)),ui->progressBar,SLOT(setValue(int)));//进度条
    imageList = GetAllImages();
    for(int i = 0;i<imageList.size();i++)
    {

        ui->comboBox->addItem(imageList.at(i));
    }
    connect(ui->pb_printer,SIGNAL(clicked(bool)),
            this,SLOT(printSlot()));
    connect(ui->pb_start,SIGNAL(clicked(bool)),
            this,SLOT(startSlot()));

    connect(ui->pb_clearline,SIGNAL(clicked(bool)),
            this,SLOT(clearLintSlot()));
    //qDebug()<<this->width();
}

autowork::~autowork()
{
    delete ui;
}

void autowork::manualPrintingSlot(bool ispush)
{
    if(ispush)
    {
        ui->pb_auto->setText("手动打印");
    }
    else
    {
        ui->pb_auto->setText("自动打印");
    }
}

void autowork::printSlot()
{
    // 创建打印机对象
        QPrinter printer;

        QPrintDialog dlg(&printer, this);

        if (dlg.exec() == QDialog::Accepted)
        {
            QTextDocument textdocument;
            QString totalnum = QString("总累计数 = %1").arg(ui->le_sumnum->text());
            QString tempname = QString("模板名字 = %1").arg(ui->comboBox->currentText());

            QString str = "<p1>" + totalnum + "</p1>" "<p>" + tempname +"</p>";

            textdocument.setHtml(str);
            textdocument.print(&printer);

       //("C:/Users/zhu/Desktop/unnamed.txt");
        }

}
QStringList autowork::GetAllImages()
{
    QStringList imageList;
    QDirIterator it("template",QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while (it.hasNext())
    {
         QString imageFile=it.next();
         QStringList list=imageFile.split('.');
         QString qian=list.takeFirst();
         QString hou=list.takeFirst();//文件后缀名
         if(hou=="jpg"||hou=="JPG"||hou=="png"||hou=="PNG"||hou=="bmp"||hou=="BMP")
             imageList<<imageFile;
    }
    for(int i=0;i<imageList.size();i++)
        qDebug()<<imageList.at(i)<<"\n";
    return imageList;
}

void autowork::startSlot()
{
    if(ui->lb_image->cam == NULL)  //判断摄像头有没有打开
    {
        return ;
    }

    //////////////////////
    ////////要把原图和模板显示为灰度图，而输出的图片则为3通道图。
    /////////////////////////////////

    sum = 0 ;
    CvPoint errpoint;
    errpoint.x = -1;
    errpoint.y = -1;
    IplImage *inframe ;
    inframe = (IplImage*)malloc(sizeof(*(ui->lb_image->outframe)));
    if( !inframe)
    {
        return ;
    }
    if( !ui->lb_image->outframe)
        return;
    *inframe = *(ui->lb_image->outframe);//输入图片



    ui->lb_image->timer->stop(); //停止摄像头采集。


    IplImage *src ;
    IplImage *srcResult;
     IplImage *result;//用来存放结果
    srcResult = cvCreateImage( cvGetSize( inframe ), inframe->depth, 3 );//用来显示
    srcResult = inframe;
    src = cvCreateImage( cvGetSize( inframe ), inframe->depth, 1 );
    cvCvtColor( inframe, src, CV_BGR2GRAY );//src，灰度  在cvCvtColor的第三个参数中设置CV_BGR2GRAY 就可以将彩色空间转化为灰度空间
    // cvShowImage("dd",srcResult);
   // cvShowImage("ff",src);
   // cvShowImage("aa",inframe);

    IplImage *templat = cvLoadImage((QString("./%1").arg(ui->comboBox->currentText())).toLatin1().data(), 0);
     //cvShowImage("bb",templat);

    int srcW, srcH, templatW, templatH, resultH, resultW;
    srcW = src->width;
    srcH = src->height;
    templatW = templat->width;
    templatH = templat->height;
    if(srcW < templatW || srcH < templatH)
    {

        return ;
    }
    resultW = srcW - templatW + 1;
    resultH = srcH - templatH + 1;
    result = cvCreateImage(cvSize(resultW, resultH), 32, 1);    //  匹配方法计算的结果最小值为float
    cvMatchTemplate(src, templat, result, CV_TM_SQDIFF);     //方差最小，匹配最好
    double minValue, maxValue;
    CvPoint minLoc, maxLoc;
    cvMinMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);

    cvRectangle(srcResult, minLoc, cvPoint(minLoc.x + templatW, minLoc.y+ templatH), cvScalar(0,0,255));

    //cvShowImage("dd",srcResult);

    CvPoint new_minLoc;
    qDebug()<<"minValue = "<<minValue;
   cvNormalize(result, result, 0, 1, CV_MINMAX,NULL);
    sum = 1;
     // 计算下一个最小值
     if(minValue < 0.12)
     {


     new_minLoc = getNextMinLoc(srcResult,result, minLoc, maxValue, templatW, templatH);
    // if (new_minLoc.x != errpoint.x)

     cvRectangle(srcResult, new_minLoc, cvPoint(new_minLoc.x + templatW, new_minLoc.y+ templatH), cvScalar(0,0,255));
     // 再下一个

        }
      emit processValue(100);

    // cvShowImage("dd",srcResult);

     cv::Mat resul=cv::Mat(srcResult);
     QImage Qimg_pic_resul;
     Qimg_pic_resul = QImage((const unsigned char*)(resul.data),
                             resul.cols,resul.rows,QImage::Format_RGB888).rgbSwapped();
     ui->lb_image->setPixmap(QPixmap::fromImage(Qimg_pic_resul));
     ui->lb_image->resize(ui->lb_image->pixmap()->size());


     ui->le_currentnum->setText(QString::number(sum,10));
     if(ui->pb_gain ->isChecked())
     sumnum = sumnum + sum;
     else
     {
         sumnum = sum;
     }
     ui->le_sumnum->setText(QString::number(sumnum,10));
    // delete srcResult;
     // delete src;
}

void autowork::clearSlot()
{
    sum = 0;
    sumnum = 0;
}

void autowork::clearLintSlot()
{
    ui->lb_image->timer->start(33);

}



CvPoint autowork::getNextMinLoc(IplImage *srcResult, IplImage *result, CvPoint minLoc, int maxVaule, int templatW, int templatH)
{
    // 先将第一个最小值点附近两倍模板宽度和高度的都设置为最大值防止产生干扰
    CvPoint errpoint;
    errpoint.x = -1;
    errpoint.y = -1;
       int startX = minLoc.x - templatW;
       int startY = minLoc.y - templatH;
       int endX = minLoc.x + templatW;
       int endY = minLoc.y + templatH;
       if(startX < 0 || startY < 0)
       {
           startX = 0;
           startY = 0;
       }
       if(endX > result->width - 1 || endY > result->height - 1)
       {
           endX = result->width - 1;
           endY = result->height - 1;
       }
       int y, x;
       for(y = startY; y < endY; y++)
       {
           for(x = startX; x < endX; x++)
           {
               cvSetReal2D(result, y, x, maxVaule);
           }
       }
       // 然后得到下一个最小值并且返回
       double new_minVaule, new_maxValue;
       CvPoint new_minLoc, new_maxLoc;
       cvMinMaxLoc(result, &new_minVaule, &new_maxValue, &new_minLoc, &new_maxLoc);
       qDebug()<<"minValue = "<<new_minVaule;

       emit processValue((int)(new_minVaule * 1000) );
       if(new_minVaule > 0.10000)
       {
           emit processValue(100) ;
           return errpoint;
       }
       else
       {
           cvRectangle(srcResult, new_minLoc, cvPoint(new_minLoc.x + templatW, new_minLoc.y+ templatH), cvScalar(0,0,255),2,8,0);
           sum ++;
           getNextMinLoc(srcResult,result, new_minLoc, new_maxValue,templatW,templatH);
       }
       return new_minLoc;
}

void autowork::paintEvent(QPaintEvent *event)
{
    int a = this->width();
    int b = this->height();
   // int c = this->menuBar()->height();
    int d = this->ui->label->height();
    QRect rect(-1,-1,a+1,d+100);
    QPainter p(this);
    p.setPen(QColor(255,255,255));
    //QColor b(4c93ef);
    QLinearGradient linear(QPointF(-1, -1), QPointF(-1, b/2+100));
        linear.setColorAt(0, QColor(76, 147, 239));
        linear.setColorAt(1, Qt::white);

        // 设置显示模式
        linear.setSpread(QGradient::PadSpread);

    p.setBrush(linear);
    //p.setBrush(QColor(76, 147, 239));
    p.drawRect(rect);

    p.setBrush(QColor(255,255,255));
    QRect rect2(-1,d+14,a+1,b);//292
    p.drawRect(rect2);
}



//IplImage *srcResult = cvLoadImage("./temp.png", 3);  //用来显示
//IplImage *srcResult;///= cvCreateImage(cvSize(inframe->width,inframe->height), 32, 3);
//srcResult = (IplImage*)malloc(sizeof(*inframe));
//if(!srcResult)
//    return;
//*srcResult = *inframe;




//IplImage *result;  // 用来存放结果
//if(!src || !templat)
//{

//    return ;
//}

//src = (IplImage*)malloc(sizeof(*inframe));
//if( !src)
//    return;
//*src=*inframe;
