#if !defined PUBLISHER_H
#define PUBLISHER_H

#include <ortedemo_types.h>
#include "MainFrm.h"
#include <qwidget.h>
#include <qslider.h>
#include <qlabel.h>
class Publisher : public QWidget
{
    Q_OBJECT
public:
 	Publisher(QWidget *parent=0, const char *name=0);

public:
	void Create(QString name, int shape, int color, long strength);
	void Destroy();

public slots:
	void Timer();
	void changeStrenght();

public:
	MainForm* m_mainFrm;
	QSlider* str;
	QLabel *Text;
	QString strTitle;
	ORTEDomain *domain;
	ORTEPublication *publisher;

protected:
	QRect m_shapeRect;
	char m_stepx;
	char m_stepy;
	char m_incx;
	char m_incy;
	BoxType boxType;

	void MoveShape();

};

#endif
