#include "Subscriber.h"
#include <qtimer.h>
#include <qrect.h>
#include <orte.h>

Subscriber::Subscriber( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
}

void Subscriber::Create(QString name, char shape, char color)
{

	//init main frame
	m_mainFrm = new MainForm;
	Text = new QLabel( m_mainFrm, "Text" );
	Text->setGeometry( QRect(10 , 0, 110, 15 ) );
    Text->setText( trUtf8( "Minimum Separation : " ) );
	MinSep = new QSlider(m_mainFrm, "str" );
    MinSep->setGeometry( QRect( 120, 0, 130, 15 ) );
	MinSep->setMaxValue( 10 );
    MinSep->setOrientation( QSlider::Horizontal );
	connect( MinSep, SIGNAL( valueChanged(int) ), this, SLOT(changeMinSep() ) );
	m_mainFrm->show();
	m_mainFrm->SetProperties(shape,color);


	//init caption


	strTitle="Subscriber : "+name+" (Topic=";
typS=color;
topS=shape;
	switch(shape)
	{
	case RECTANGLE:
		strTitle+="RECTANGLE, Type=";
		topic="Rectangle";
		break;
	case ELLIPSE:
		strTitle+="ELLIPSE, Type=";
		topic="Ellipse";
		break;
	case TRIANGLE:
		strTitle+="TRIANGLE, Type=";
		topic="Triangle";
		break;
	}

	switch(color)
	{
	case BLUE:
		strTitle+="BLUE";
		type="Blue";
		break;
	case GREEN:
		strTitle+="GREEN";
		type="Green";
		break;
	case RED:
		strTitle+="RED";
		type="Red";
		break;
	case BLACK:
		strTitle+="BLACK";
		type="Black";
		break;
	}
	strTitle+=" Min.Sep.=0 )";
	m_mainFrm->setCaption(strTitle);
}


void Subscriber::Destroy()
{
                m_mainFrm->WantClose();
	m_mainFrm->close();

}

void Subscriber::changeMinSep()
{
	ORTESubsProp  sub_prop;
	ORTEAppSubsPropGet(app,1,&sub_prop);
	NtpTimeAssembFromMs(sub_prop.minimumSeparation,MinSep->value(),0);
	ORTEAppSubsPropSet(app,1,&sub_prop);
	QString name=QString::number(MinSep->value());
	if((MinSep->value())<10) name+=" ";
	(this->strTitle).replace((this->strTitle).length()-3,2,name);
	(this->m_mainFrm)->setCaption(this->strTitle);
}
