#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QFile>
#include "logindialog.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *gbk = QTextCodec::codecForName("UTF-8");
   // QTextCodec::setCodecForTr(gbk);
    QTextCodec::setCodecForLocale(gbk);
   // QTextCodec::setCodecForCStrings(gbk);
   a.setFont(QFont("DejaVuSansMono.ttf"));

   QString styleName = "blue";
   QFile file(QString(":/image/%1.css").arg(styleName));
   file.open(QFile::ReadOnly);
   QString qss = QLatin1String(file.readAll());
   qApp->setStyleSheet(qss);
   qApp->setPalette(QPalette(QColor("#F0F0F0")));



    MainWindow w;

    w.show();



    return a.exec();
}
