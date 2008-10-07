#include "FPublisher.h"

FPublisher::FPublisher(QWidget *parent)
    : QDialog(parent)
{
    stepx=rand()%2+1;stepy=rand()%2+1;
    incx=incy=0;
    while(incx==0) incx=(rand()%3-1)*stepx;
    while(incy==0) incy=(rand()%3-1)*stepy;
    rect.setRect(0,0,25,45);
    ORTEInit(); 
    domain=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
    BoxType_type_register(domain);
    publisher=NULL;
    /* setup UI */
    setupUi(this);
}

void FPublisher::initPublisher(int icolor,int istrength)
{
    NtpTime	persistence;
    const char 	*topic;

    color=icolor;
    strength=istrength;
    boxType.color=color;
    boxType.shape=strength;

    view->activateObject(0,color,strength);//color,shape
    NtpTimeAssembFromMs(persistence, 5, 0);
    switch(color) {
	case CL_BLUE:topic="Blue";break;
	case CL_GREEN:topic="Green";break;
	case CL_RED:topic="Red";break;
	case CL_BLACK:topic="Black";break;
	case CL_YELLOW:topic="Yellow";break;
    }
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

    timer = new QTimer();
    connect( timer, SIGNAL(timeout()), this, SLOT(Timer()));
    timer->start( 50 );
}


void FPublisher::closeEvent( QCloseEvent *e )
{
    if (domain!=NULL)
       destroy();
    e->accept();
}

void FPublisher::destroy()
{
    if (domain) {
      delete timer;
      ORTEDomainAppDestroy(domain);
      domain=NULL;
      close();
    }
}

void FPublisher::Timer()
{
    if(rect.left()<=0) incx=stepx;
    if(rect.top()<=0) incy=stepy;
    if((rect.right())>=view->width()) incx=-stepx;
    if((rect.bottom())>=view->height()) incy=-stepy;
    if(view->mousePressed==1) {
	int tmpW=rect.width();
	int tmpH=rect.height();
	rect.setRect(view->mouseX-tmpW/2,view->mouseY-tmpH/2,tmpW,tmpH);
    } else {
	rect.moveTo(rect.left()+incx,rect.top()+incy);
    }
    view->setPosition(0,rect);
    //prepare published data
    boxType.rectangle.top_left_x=rect.left();
    boxType.rectangle.top_left_y=rect.top();
    boxType.rectangle.bottom_right_x=rect.right();
    boxType.rectangle.bottom_right_y=rect.bottom();
    ORTEPublicationSend(publisher);    
}


void FPublisher::strengthChanged()
{
    ORTEPublProp  pp;
    
    if (!publisher) return;
    ORTEPublicationPropertiesGet(publisher,&pp);
    pp.strength=slider->value();
    ORTEPublicationPropertiesSet(publisher,&pp);
}

