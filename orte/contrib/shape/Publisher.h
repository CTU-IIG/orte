#if !defined PUBLISHER_H
#define PUBLISHER_H

#include <define.h>
#include "MainFrm.h"
#include <qwidget.h>
#include <orte.h>
#include <qslider.h>
#include <qlabel.h>
class Publisher : public QWidget
{
    Q_OBJECT
public:
 	Publisher(QWidget *parent=0, const char *name=0);

public:
	void Create(QString name, char shape, char color, long strength);
	void Destroy();

public slots:
	void Timer();
	void changeStrenght();

public:
	MainForm* m_mainFrm;
	QSlider* str;
	QLabel *Text;
	 ManagedApp    *app1;
 	 int           h_pub;
	 int top,typ;
	QString strTitle;

protected:
	QRect m_shapeRect;
	char m_stepx;
	char m_stepy;
	char m_incx;
	char m_incy;

	void MoveShape();

};

#endif
