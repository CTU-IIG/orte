#include "Publisher.h"
#include <orte_api.h>
#include <qtimer.h>
#include <unistd.h>
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

void Publisher::Create(QString name, int shape, int color, long strength)
{
  NtpTime	persistence;
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
	m_mainFrm->SetActiveObject(0);
	m_shapeRect.moveBy(rand()%m_mainFrm->MaxX(),rand()%m_mainFrm->MaxY());


	//init caption
        QString topic;
	boxType.shape=shape;
	boxType.color=color;

	strTitle="Publisher : "+name+" (Topic=";
	switch(color)
	{
	case BLUE:
		topic="Blue";
		break;
	case GREEN:
		topic="Green";
		break;
	case RED:
		topic="Red";
		break;
	case BLACK:
		topic="Black";
		break;
	}
	strTitle+=topic;

        ORTEInit(); 
	domain=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
	ORTETypeRegisterBoxType(domain);
        NtpTimeAssembFromMs(persistence, 5, 0);
        publisher=ORTEPublicationCreate(
	             domain,
                     topic,
	             "BoxType",
	             &boxType,
		     &persistence,
		     strength,
		     NULL,
		     NULL,
		     NULL);

	strTitle+=", Strength="+QString::number(strength)+" )";
	m_mainFrm->setCaption(strTitle);

	//create the timer
	QTimer *timer = new QTimer(this);
    	connect( timer, SIGNAL(timeout()), SLOT(Timer()));
    	timer->start( 50, FALSE );
}

void Publisher::Destroy()
{	
	m_mainFrm->WantClose();
	m_mainFrm->close();
	ORTEDomainAppDestroy(domain);
}

void Publisher::Timer()
{
        MoveShape();
        boxType.rectangle.top_left_x=m_shapeRect.left();
        boxType.rectangle.top_left_y=m_shapeRect.top();
        boxType.rectangle.bottom_right_x=m_shapeRect.right();
        boxType.rectangle.bottom_right_y=m_shapeRect.bottom();
        ORTEPublicationSend(publisher);
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

	m_mainFrm->ShapeColorRect(0,boxType.shape,boxType.color,m_shapeRect);
}

void Publisher::changeStrenght()
{
	ORTEPublProp  pp;

        ORTEPublicationPropertiesGet(publisher,&pp);
	pp.strength=str->value();
        ORTEPublicationPropertiesSet(publisher,&pp);
	QString name=QString::number(str->value());
	if((str->value())<10) name+=" ";
	(this->strTitle).replace((this->strTitle).length()-3,2,name);
	(this->m_mainFrm)->setCaption(this->strTitle);
}
