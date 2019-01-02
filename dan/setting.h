#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

private:
    Ui::setting *ui;
    void paintEvent();
};

#endif // SETTING_H
