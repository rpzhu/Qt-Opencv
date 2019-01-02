#ifndef SHOUDONGCESHI_H
#define SHOUDONGCESHI_H

#include <QWidget>
#include "chuangjian.h"
#include "camaraget.h"
#include <QFile>
#include <QDirIterator>
#include <QFileSystemWatcher>
#include <iostream>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTime>
#include <vector>
#include <iostream>

using namespace std ;
using namespace cv;
namespace Ui {
class shoudongceshi;
}

class shoudongceshi : public QWidget
{
    Q_OBJECT

public:
    explicit shoudongceshi(QWidget *parent = 0);
    ~shoudongceshi();

    chuangjian *cj;
    QImage image;
     QString filename;
    long int template_num ;

    QStringList GetAllImages();
    QStringList imageList;
    QFileSystemWatcher watchtem;
signals:
   void toNewTemplateSignal(QImage *image);
   void processValue(int value);

Q_SIGNALS:
   void fileChanged(const QString &path);

public slots:
    void qutuSlot();
    void chuangjianSlot();
    void templateSlot();
    void saveSlot();
    void showTemplateSlot(QString string);
    void deleteTemplateSlot();
    void testSlot();
private:

    void paintEvent(QPaintEvent* event);
    void cvTemplateMatch(IplImage* src,IplImage* tmp,long *count);

   CvPoint getNextMinLoc(IplImage *srcResult, IplImage *result,
                         CvPoint minLoc, int maxVaule,
                         int templatW, int templatH, double &minValue,CvPoint &new_maxLoc);
  //Cvpoint
   long  sum;
   double newminvalue[2];
   int minvalue_index;
   int count;
    Ui::shoudongceshi *ui;
    bool isDirExist(QString fullPath);

};

#endif // SHOUDONGCESHI_H
