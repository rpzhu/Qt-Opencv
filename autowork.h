#ifndef AUTOWORK_H
#define AUTOWORK_H

#include <QWidget>
#include <QDirIterator>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QTextEdit>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>
#include <QPainter>

namespace Ui {
class autowork;
}

class autowork : public QWidget
{
    Q_OBJECT

public:
    explicit autowork(QWidget *parent = 0);
    ~autowork();
   QStringList GetAllImages();
   QStringList imageList;
signals:
   void printSignal();
    void processValue(int);
private slots:
   void manualPrintingSlot(bool);
   void printSlot();
   void startSlot();
   void clearSlot();
   void clearLintSlot();
private:
   CvPoint getNextMinLoc(IplImage *srcResult, IplImage *result, CvPoint minLoc, int maxVaule, int templatW, int templatH);
   void paintEvent(QPaintEvent *event);
   long sum; //当前
    long sumnum;//累计值

    Ui::autowork *ui;
};

#endif // AUTOWORK_H
