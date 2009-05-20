#ifndef FSUBSCRIBER_H
#define FSUBSCRIBER_H

#include <QDialog>
#include <QLabel>
#include <QCloseEvent>
#include "ui_FSubscriber.h"
#include "BoxType.h"

#include <orte.h>

class FSubscriber : public QDialog , public Ui::FSubscriber
{
    Q_OBJECT

    BoxType boxTypeYellow;
    BoxType boxTypeBlack;
    BoxType boxTypeRed;
    BoxType boxTypeBlue;
    BoxType boxTypeGreen;
    ORTEDomain *domain;
    ORTESubscription *subscriberGreen;
    ORTESubscription *subscriberBlue;
    ORTESubscription *subscriberRed;
    ORTESubscription *subscriberBlack;
    ORTESubscription *subscriberYellow;
    NtpTime msGreen;
    NtpTime msBlue;
    NtpTime msRed;
    NtpTime msBlack;
    NtpTime msYellow;

 protected:
     void closeEvent(QCloseEvent *event);

public:
    FSubscriber(QWidget *parent = 0);

    bool initSubscribers( int iBlue, int iGreen, int iRed, int iBlack, int iYellow );


private slots:
    void destroy();
    void comboActivated( int );
    void sliderValueChanged( int  value);

};

#endif
