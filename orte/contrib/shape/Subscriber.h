#if !defined SUBSCRIBER_H
#define SUBSCRIBER_H

#include <define.h>
#include "MainFrm.h"
#include <qwidget.h>
#include <orte.h>
#include <qslider.h>
#include <qlabel.h>
class Subscriber : public QWidget
{
    Q_OBJECT
public:
 	Subscriber(QWidget *parent=0, const char *name=0);
	QString topic;
	QString type;
	QString strTitle;
	int typS,topS;
	ManagedApp    *app;
	MainForm* m_mainFrm;
	QSlider* MinSep;
	QLabel *Text;

public:
	void Create(QString name, char shape, char color);
	void Destroy();
	
public slots:
	void changeMinSep();

};
#endif
