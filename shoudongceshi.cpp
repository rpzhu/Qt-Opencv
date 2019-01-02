#include "shoudongceshi.h"
#include "ui_shoudongceshi.h"
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include "chuangjian.h"


int a=0;

shoudongceshi::shoudongceshi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shoudongceshi)
{

    ui->setupUi(this);
     this->setFont(QFont("DejaVuSansMono.ttf"));
     setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);//去除窗体边框
//    templat = 0;
//    src = 0;
    qDebug()<<"open camera666666";
    QFile file("./a.png");
    if(file.exists())
    {

    }
    else
    {
        file.open( QIODevice::ReadWrite  );
        file.close();
    }
    watchtem.addPath("./a.png");
    connect(&watchtem,SIGNAL(fileChanged(QString)),this,SLOT(templateSlot()));
  //  a = 0;
    setAttribute(Qt::WA_DeleteOnClose);
    QImage image ;
    cj = NULL;
    template_num = NULL;
    connect(ui->pb_quit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pb_qutu,SIGNAL(clicked(bool)),this,SLOT(qutuSlot()));
    connect(ui->pb_cuangjian,SIGNAL(clicked(bool)),this,SLOT(chuangjianSlot()));
    connect(ui->pb_save,SIGNAL(clicked(bool)),this,SLOT(saveSlot()));

    imageList = GetAllImages();
    for(int i = 0;i<imageList.size();i++)
    {

        ui->comboBox->addItem(imageList.at(i));
    }
    connect(ui->comboBox,SIGNAL(activated(QString)),this,SLOT(showTemplateSlot(QString)));
    //connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(showTemplateSlot(QString)));
    //connect(ui->comboBox,SIGNAL())

    connect(ui->pb_delete,SIGNAL(clicked(bool)),this,SLOT(deleteTemplateSlot()));

    imageList.clear();
    connect(ui->pb_test,SIGNAL(clicked(bool)),this,SLOT(testSlot()));
    connect(this,SIGNAL(processValue(int)),ui->progressBar,SLOT(setValue(int)));
}

shoudongceshi::~shoudongceshi()
{
    delete ui;

    if(cj != NULL)
        delete cj;

    disconnect(&watchtem,SIGNAL(fileChanged(QString)),this,SLOT(templateSlot()));

    qDebug()<<"close camera 1 ";
}

QStringList shoudongceshi::GetAllImages()
{
    QStringList imageList;
    QDirIterator it("template",QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while (it.hasNext())
    {
         QString imageFile=it.next();
         QStringList list=imageFile.split('.');//把单个文件名拆分开。以 . 为分隔。不包含点。
         QString qian=list.takeFirst();//移除第一个，并返回名称。
         QString hou=list.takeFirst();//文件后缀名
         if(hou=="jpg"||hou=="JPG"||hou=="png"||hou=="PNG"||hou=="bmp"||hou=="BMP")
             imageList<<imageFile;//如果后缀名符合，把文件全名加入到链表中。
    }

    return imageList;
}

void shoudongceshi::qutuSlot()
{
    if(ui->lb_camera->cam == NULL)
    {
   // QString filename;
       filename=QFileDialog::getOpenFileName(this,
                                             tr("选择图像"),
                                             "",
                                             tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
       if(filename.isEmpty())
       {
            return;
       }
       else
       {
           QImage* img=new QImage;

           if(! ( img->load(filename) ) ) //加载图像
           {
               QMessageBox::information(this,
                                        tr("打开图像失败"),
                                        tr("打开图像失败!"));
               delete img;
               return;
           }
//
           ui->lb_image->setPixmap(QPixmap::fromImage(*img));
          img->save("./temp.png");
           delete img;
          ui->tabWidget->setCurrentWidget(ui->tab_3);
       }
    }
    else
    {
        image = ui->lb_camera->takingPictures();
        ui->lb_image->setPixmap((QPixmap::fromImage(image)));
        image.save("./temp.png");

        ui->tabWidget->setCurrentWidget(ui->tab_3);
    }

}


void shoudongceshi::chuangjianSlot()
{

    cj = new chuangjian();
    cj->m_image = image;
    cj->sendimage();

    cj->setWindowModality(Qt::ApplicationModal);
    cj->show();
}

void shoudongceshi::templateSlot()
{

    QPixmap p;
    for(int i =0;i<=10;i++);
    p.load("./a.png");
    ui->lb_template->setPixmap(p);
    ui->tabWidget->setCurrentWidget(ui->tab_2);
    ui->comboBox->setCurrentIndex(-1);


}

void shoudongceshi::saveSlot()
{
    template_num = template_num + 1;
    bool a = isDirExist("template");

    QString fn = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "./template/untitled.png",
                                tr("Images (*.png *.xpm *.jpg)"));
    if(fn.isEmpty())
    {
         return;
    }
    qDebug()<<fn;
    QDir dir;
    if(dir.exists(fn))
    {
         qDebug()<<"fn"<<"is exists";
         QFile::remove(fn);
    }
    QFile::copy("./a.png",fn)  ;

    ui->comboBox->clear();
    imageList = GetAllImages();
    for(int i = 0;i<imageList.size();i++)
    {

        ui->comboBox->addItem(imageList.at(i));
    }

}

void shoudongceshi::showTemplateSlot(QString string)
{
    QPixmap p;
    //qDebug()<<string;
    p.load(QString("./%1").arg(string));
    ui->lb_template->setPixmap(p);


}

void shoudongceshi::deleteTemplateSlot()
{

    QString string = ui->comboBox->currentText();
    QFile::remove(QString("./%1").arg(string)) ;
    ui->comboBox->clear();
    imageList.clear();
    imageList = GetAllImages();
    for(int i = 0;i<imageList.size();i++)
    {

        ui->comboBox->addItem(imageList.at(i));
    }
    showTemplateSlot(ui->comboBox->currentText());
}

void shoudongceshi::testSlot()
{
    emit processValue(0);

    //newminvalue.clear();
    minvalue_index = 0;
    newminvalue[0] =-1;
    newminvalue[1] =-1;
    QTime time;
    time.start();
    sum = 0 ;
    CvPoint errpoint;
    errpoint.x = -1;
    errpoint.y = -1;
    IplImage *src = cvLoadImage("./temp.png", 1);//灰度图
    IplImage *srcResult = cvLoadImage("./temp.png", 3);  //用来显示
    IplImage *templat = cvLoadImage("./a.png", 1);//灰度图
    IplImage *result;  // 用来存放结果
    if(!src || !templat)
    {
        cout << "打开图片失败" << endl;
        return ;
    }
   // long count = 0;
    //cvTemplateMatch(src,templat,&count);





    int srcW, srcH, templatW, templatH, resultH, resultW;
    srcW = src->width;
    srcH = src->height;
    templatW = templat->width;
    templatH = templat->height;
    if(srcW < templatW || srcH < templatH)
    {
        cout << "模板不能比原图小" << endl;
        return ;
    }
    resultW = srcW - templatW + 1;
    resultH = srcH - templatH + 1;
    result = cvCreateImage(cvSize(resultW, resultH), 32, 1);    //  匹配方法计算的结果最小值为float
    cvMatchTemplate(src, templat, result, CV_TM_SQDIFF);     //方差最小，匹配最好
    //cvMatchTemplate(src, templat, result, CV_TM_SQDIFF_NORMED);
    //CV_TM_SQDIFF_NORMED

    double minValue, maxValue;
    CvPoint minLoc, maxLoc;
    cvMinMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
    double cpmax = maxValue;
    cvRectangle(srcResult, minLoc, cvPoint(minLoc.x + templatW, minLoc.y+ templatH), cvScalar(0,0,255));
    CvPoint new_minLoc,new_maxLoc;
    //////////////////
new_minLoc = minLoc;
new_maxLoc = maxLoc;
        ////////////////////
    qDebug()<<"minValue = "<<minValue;

    cvNormalize(result, result, 0, 1, CV_MINMAX,NULL);
    sum = 1;
    minvalue_index = 0;
    // 计算下一个最小值
    newminvalue[1] =-1;
    count = 0;
////////////////
again:
    if(minValue <ui->doubleSpinBox->value())//0.085
    {
       // count ++;
//        newminvalue[minvalue_index] = minValue;

//        if(minvalue_index > 1)
//            minvalue_index = 0;

//        if(newminvalue[0] == newminvalue[1])
//            count ++;
//        qDebug()<<"[0]"<<newminvalue[0]<<"[1]"<<newminvalue[1]<<"count"<<count;
//        minvalue_index ++;
        //////////////////////
        new_minLoc = getNextMinLoc(srcResult,result, new_minLoc, maxValue, templatW, templatH ,minValue, new_maxLoc);
        cvRectangle(srcResult, new_minLoc, cvPoint(new_minLoc.x + templatW, new_minLoc.y+ templatH), cvScalar(255,0,0));
        sum ++;
        emit processValue((int)( minValue *1000) );
        if(minValue < ui->doubleSpinBox->value() /*&& newminvalue[0] != newminvalue[1]*/ && count < 3)
            goto again;

    }
///////////////////
    emit processValue(100);

    cv::Mat resul=cv::Mat(srcResult);
    QImage Qimg_pic_resul;
    Qimg_pic_resul = QImage((const unsigned char*)(resul.data),
                            resul.cols,resul.rows,QImage::Format_RGB888).rgbSwapped();
    ui->lb_image->setPixmap(QPixmap::fromImage(Qimg_pic_resul));
    ui->lb_image->resize(ui->lb_image->pixmap()->size());

    ui->tabWidget->setCurrentWidget(ui->tab_3);
    ui->le_currentnum->setText(QString::number(sum,10));
    int etime = time.elapsed();
    ui->le_usedtime->setText(QString::number(etime,10));
    // delete srcResult;
    // delete src;
}

void shoudongceshi::paintEvent(QPaintEvent* event)
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

void shoudongceshi::cvTemplateMatch(IplImage *src, IplImage *tmp,long *count)
{

    *count = 0;
    int w,h;
    int m_ThresholdValue=10;
    CvRect rect;
    IplImage* result=NULL;
    IplImage* srcGray=NULL;
    IplImage* tmpGray=NULL;

    double min_val;
    double max_val;
    CvPoint min_loc;
    CvPoint max_loc;
    CvPoint pt1;
    CvPoint pt2;

    w=src->width-tmp->width;
    h=src->height-tmp->height;
    result=cvCreateImage(cvSize(w+1,h+1),IPL_DEPTH_32F,1);      //必须为32位浮点型，IPL_DEPTH_32F
    srcGray=cvCreateImage(cvGetSize(src),8,1);
    tmpGray=cvCreateImage(cvGetSize(tmp),8,1);
    cvCvtColor(src,srcGray,CV_BGR2GRAY);
    cvCvtColor(tmp,tmpGray,CV_BGR2GRAY);
    cvMatchTemplate(srcGray,tmpGray,result,CV_TM_CCOEFF_NORMED);
    //CV_TM_SQDIFF CV_TM_SQDIFF_NORMED CV_TM_CCORR (这三种Method的匹配效果不佳)
    //CV_TM_CCORR_NORMED CV_TM_CCOEFF CV_TM_CCOEFF_NORMED (这三种Method的匹配效果完美)

    cvNamedWindow("result",1);
    cvShowImage("result",result);

    printf("result的宽：%d\n",w);
    printf("result的高：%d\n",h);


    cvMinMaxLoc(result,&min_val,&max_val,&min_loc,&max_loc,NULL);//找出图片或一组数据中最大值及最小值的数据,以及最大值及最小值的位置

    CvPoint point=cvPoint(0,0);
    rect=cvRect(max_loc.x,max_loc.y,tmp->width,tmp->height);

    for(int cx=0;cx<w;cx++)
    {
        for(int cy=0;cy<h;cy++)
        {
            float fTemp=CV_IMAGE_ELEM(result,float,cy,cx);//得到(cx,cy)处的值

            //printf("%f\n",fTemp);

            if(fTemp>0.7)

            {
                //	point=cvPoint(cx+rect.x,cy+rect.y);//记录位置
            point=cvPoint(cx,cy);//记录位置
                //	CvPoint point2=cvPoint(point.x+rect.width,point.y+rect.height);//对角位置
            CvPoint point2=cvPoint(point.x+rect.width,point.y+rect.height);//对角位置
                cvRectangle(src,point,point2,cvScalar(0,0,255),1,8,0);
                *count =*count +1;
            }
        }
    }
    //pt1=cvPoint(rect.x,rect.y);
    //pt2=cvPoint(rect.x+rect.width,rect.y+rect.height);
    //cvRectangle( src,pt1, pt2, cvScalar(255,0,0),1, 8, 0 );

    cvReleaseImage(&srcGray);
    cvReleaseImage(&tmpGray);
    cvReleaseImage(&result);
}



//double shoudongceshi::getNextMinLoc(IplImage *srcResult, IplImage *result, CvPoint minLoc,
//                                     int maxVaule, int templatW, int templatH)
CvPoint shoudongceshi::getNextMinLoc(IplImage *srcResult, IplImage *result, CvPoint minLoc,
                                     int maxVaule, int templatW,
                                     int templatH ,double &minValue ,CvPoint &new_maxLoc)
{
    // 先将第一个最小值点附近两倍模板宽度和高度的都设置为最大值防止产生干扰
    CvPoint errpoint;
    errpoint.x = -1;
    errpoint.y = -1;
    double lastvalue = 0;
       int startX = minLoc.x - templatW;
       int startY = minLoc.y - templatH;
       int endX = minLoc.x + templatW;
       int endY = minLoc.y + templatH;

//    int startX = minLoc.x;
//    int startY = minLoc.y;
//    int endX = minLoc.x + templatW;
//    int endY = minLoc.y + templatH;
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
              // cvSetReal2D(result, y, x, 10);
           }
       }
       // 然后得到下一个最小值并且返回
       double new_minVaule, new_maxValue;
       CvPoint new_minLoc;
      // new_minVaule = minvalue;
       cvMinMaxLoc(result, &minValue, &new_maxValue, &new_minLoc, &new_maxLoc);
       qDebug()<<"minValue = "<<minValue;
       qDebug()<<"maxValue = "<<new_maxValue;

///////////////

        return new_minLoc;
////////////////


//       if(new_minVaule > 0.074 )
//       {
//           emit processValue(100) ;
//           return 0;
//       }


//       else
//       {

//           cvRectangle(srcResult, new_minLoc, cvPoint(new_minLoc.x + templatW, new_minLoc.y+ templatH), cvScalar(0,0,255),2,8,0);
//           sum ++;
//           getNextMinLoc(srcResult,result, new_minLoc, new_maxValue,templatW,templatH);
//       }
//       return 0;
}

bool shoudongceshi::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
        return true;
    }
    else
    {
        bool ok = dir.mkpath("./");//只创建一级子目录，即必须保证上级目录存在
        return ok;
    }
}










////////////建立数组存临时minvalue。
//           newminvalue[minvalue_index] = new_minVaule;
//           minvalue_index++;
//           if(minvalue_index == 2)
//           {
//              // qDebug()<<"minValue []= "<<newminvalue.at(newminvalue.capacity());
//               if(newminvalue[0] == newminvalue[1])
//               {
//                  // qDebug()<<"minValue []= "<<newminvalue[0];
//                  // qDebug()<<"minValue []= "<<newminvalue[1];
//                  // newminvalue.clear();
//                    //minvalue_index=0;
//                   return 0;
//               }
//               minvalue_index=0;
//           }

//lastvalue = new_minVaule;

//           cvRectangle(srcResult, new_minLoc, cvPoint(new_minLoc.x + templatW, new_minLoc.y+ templatH), cvScalar(0,0,255),2,8,0);


//         /* if (*/ getNextMinLoc(srcResult,result, new_minLoc, new_maxValue,templatW,templatH) /*== 0)*/
////               return 0;
//               //qDebug()<<"lastvalue = "<<new_minVaule;;// new_minVaule = new_minVaule + 0.01;
//            sum ++;
