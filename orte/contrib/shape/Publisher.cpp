#include "Publisher.h"
#include <orte.h>
#include <qtimer.h>
#include "ViewFrm.h"

Publisher::Publisher( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
	m_stepx=rand()%2+1;
	m_stepy=rand()%2+1;
	m_incx=0;
	m_incy=0;

	while(m_incx==0) m_incx=(rand()%3-1)*m_stepx;
    	while(m_incy==0) m_incy=(rand()%3-1)*m_stepy;

	m_shapeRect.setRect(0,0,25,45);
}

void Publisher::Create(QString name, char shape, char color, long strength)
{
  NtpTime	timePersistence;
	//init main frame
	m_mainFrm = new MainForm;
	Text = new QLabel( m_mainFrm, "Text" );
    Text->setGeometry( QRect(10 , 0, 60, 15 ) );
    Text->setText( trUtf8( "Strength : " ) );
	str = new QSlider(m_mainFrm, "str" );
    str->setGeometry( QRect( 70, 0, 130, 15 ) );
	str->setMaxValue( 10 );
    str->setOrientation( QSlider::Horizontal );
	connect( str, SIGNAL( valueChanged(int) ), this, SLOT(changeStrenght() ) );
	m_mainFrm->show();
	m_mainFrm->SetProperties(shape,color);
	m_shapeRect.moveBy(rand()%m_mainFrm->MaxX(),rand()%m_mainFrm->MaxY());


	//init caption

 QString topic;
	QString type;
	top=shape;
	typ=color;
	strTitle="Publisher : "+name+" (Topic=";

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

	ORTEAppCreate(&app1);
  NtpTimeAssembFromMs(timePersistence, 5, 0);
  h_pub=ORTEAppPublAdd(app1,topic,type,
                       &timePersistence,strength);

	strTitle+=", Strength="+QString::number(strength)+" )";
	m_mainFrm->setCaption(strTitle);

	//create the timer
	QTimer *timer = new QTimer(this);
    	connect( timer, SIGNAL(timeout()), SLOT(Timer()));
    	timer->start( 50, FALSE );
}

void Publisher::Timer()
{
	char		msg[128];
	memset(msg,0,sizeof(msg));
	MoveShape();
	sprintf(msg,"%i %i %i %i",m_shapeRect.left(),m_shapeRect.top(),m_shapeRect.right(),m_shapeRect.bottom());
	//Don't forget add to length last zero char !!!
    ORTEAppPublSend(app1,h_pub,msg,strlen(msg)+1);
}

void Publisher::MoveShape()
{
    	if(m_shapeRect.left()<=0) m_incx=m_stepx;
    	if(m_shapeRect.top()<=0) m_incy=m_stepy;
    	if((m_shapeRect.right())>=m_mainFrm->MaxX()) m_incx=-m_stepx;
    	if((m_shapeRect.bottom())>=m_mainFrm->MaxY()) m_incy=-m_stepy;

    	if(m_mainFrm->View->m_mousePressed==1)
	{
		int tmpW=m_shapeRect.width();
		int tmpH=m_shapeRect.height();

		m_shapeRect.setRect(m_mainFrm->View->m_mouseX-tmpW/2,m_mainFrm->View->m_mouseY-tmpH/2,tmpW,tmpH);
	}
	else
	{
		m_shapeRect.moveBy(m_incx,m_incy);
	}

	m_mainFrm->SetShapeRect(m_shapeRect);
}

void Publisher::Destroy()
{	
	m_mainFrm->WantClose();
	m_mainFrm->close();
	ORTEAppDestroy(app1);
}
void Publisher::changeStrenght()
{
	ORTEPublProp  publ_prop;
	ORTEAppPublPropGet(this->app1,this->h_pub,&publ_prop);
	publ_prop.strength=str->value();
	ORTEAppPublPropSet(this->app1,this->h_pub,&publ_prop);
	QString name=QString::number(str->value());
	if((str->value())<10) name+=" ";
	(this->strTitle).replace((this->strTitle).length()-3,2,name);
	(this->m_mainFrm)->setCaption(this->strTitle);
}
