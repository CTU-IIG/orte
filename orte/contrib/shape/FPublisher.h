#ifndef FPUBLISHER_H
#define FPUBLISHER_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QCloseEvent>
#include "ui_FPublisher.h"
#include "BoxType.h"

#include <orte.h>

class FPublisher : public QDialog , public Ui::FPublisher
{
    Q_OBJECT

    int strength;
    ORTEPublication *publisher;
    ORTEDomain *domain;
    int color;
    int stepx;
    int stepy;
    int incx;
    int incy;
    int shape;
    BoxType boxType;
    QRect rect;

 protected:
     void closeEvent(QCloseEvent *event);

public:
    FPublisher(QWidget *parent = 0);

    bool initPublisher(int icolor,int istrength);

private slots:
    void Timer();
    void destroy();
    void strengthChanged();

private:
    QTimer *timer;
};

#endif
