#if !defined SUBSCRIBER_H
#define SUBSCRIBER_H

#include <ortedemo_types.h>
#include "MainFrm.h"
#include <qwidget.h>
#include <qslider.h>
#include <qlabel.h>
class Subscriber : public QWidget
{
    Q_OBJECT
public:
 	Subscriber(QWidget *parent=0, const char *name=0);
	QString strTitle;
	MainForm* m_mainFrm;
	QSlider* MinSep;
	QLabel *Text;
	ORTEDomain  *domain;
	ORTESubscription *subBlue;
	NtpTime minimumSeparationBlue;
	ORTESubscription *subGreen;
	NtpTime minimumSeparationGreen;
	ORTESubscription *subRed;
	NtpTime minimumSeparationRed;
	ORTESubscription *subBlack;
	NtpTime minimumSeparationBlack;
	ORTESubscription *subYellow;
	NtpTime minimumSeparationYellow;
	pthread_mutex_t mutex;

public:
	void Create(QString name, char eBlue,char eGreen,
	            char eRed,char eBlack,char eYellow );
	void Destroy();
	
public slots:
	void changeMinSep();

protected:
	BoxType boxType;
	
};
#endif
