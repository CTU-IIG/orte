#include "Subscriber.h"
#include <qtimer.h>
#include <qrect.h>
#include <orte_api.h>


Subscriber::Subscriber( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
}

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  BoxType *boxType=(BoxType*)vinstance;
  Subscriber *s=(Subscriber*)recvCallBackParam;
  QRect   rect;

  switch (info->status) {
    case NEW_DATA:
      rect.setCoords(
          boxType->rectangle.top_left_x,
	  boxType->rectangle.top_left_y,
	  boxType->rectangle.bottom_right_x,
	  boxType->rectangle.bottom_right_y);
      pthread_mutex_lock(&s->mutex);
      s->m_mainFrm->ShapeColorRect(boxType->color,boxType->shape,boxType->color,rect);
      pthread_mutex_unlock(&s->mutex);
      break;
    case DEADLINE:
//      printf("deadline occured\n");
      break;
  }
}

void Subscriber::Create(QString name, char eBlue,char eGreen,
	                char eRed,char eBlack,char eYellow )
{
	subBlue=NULL;
        NtpTimeAssembFromMs(minimumSeparationBlue, 0, 0);
	subGreen=NULL;
	NtpTimeAssembFromMs(minimumSeparationGreen, 0, 0);
	subRed=NULL;
	NtpTimeAssembFromMs(minimumSeparationRed, 0, 0);
	subBlack=NULL;
	NtpTimeAssembFromMs(minimumSeparationBlack, 0, 0);
	subYellow=NULL;
	NtpTimeAssembFromMs(minimumSeparationYellow, 0, 0);
        
	//init main frame
	m_mainFrm = new MainForm;
	Text = new QLabel( m_mainFrm, "Text" );
	Text->setGeometry( QRect(10 , 0, 110, 15 ) );
        Text->setText( trUtf8( "Minimum Separation : " ) );
	MinSep = new QSlider(m_mainFrm, "str" );
        MinSep->setGeometry( QRect( 120, 0, 130, 15 ) );
	MinSep->setMaxValue( 10 );
        MinSep->setOrientation( QSlider::Horizontal );
        MinSep->setTickmarks( QSlider::NoMarks );
	connect( MinSep, SIGNAL( valueChanged(int) ), this, SLOT(changeMinSep() ) );
	m_mainFrm->show();


	//init caption
        QString topic;

	strTitle="Subscriber : "+name;
	m_mainFrm->setCaption(strTitle);

        pthread_mutex_init(&mutex,NULL);
	
        ORTEInit(); 
	domain=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
	ORTETypeRegisterBoxType(domain);
	NtpTime deadline;
        NtpTimeAssembFromMs(deadline, 1, 0);
	if (eBlue) {
          subBlue=ORTESubscriptionCreate(
              domain,
              IMMEDIATE,
              BEST_EFFORTS,
              "Blue",
              "BoxType",
              &boxType,
              &deadline,
              &minimumSeparationBlue,
              recvCallBack,
              this);
  	   m_mainFrm->SetActiveObject(0);
         }
	if (eGreen) {
          subGreen=ORTESubscriptionCreate(
              domain,
              IMMEDIATE,
              BEST_EFFORTS,
              "Green",
              "BoxType",
              &boxType,
              &deadline,
              &minimumSeparationGreen,
              recvCallBack,
              this);
  	   m_mainFrm->SetActiveObject(1);
         }
	if (eRed) {
          subRed=ORTESubscriptionCreate(
              domain,
              IMMEDIATE,
              BEST_EFFORTS,
              "Red",
              "BoxType",
              &boxType,
              &deadline,
              &minimumSeparationRed,
              recvCallBack,
              this);
  	   m_mainFrm->SetActiveObject(2);
         }
	if (eBlack) {
          subBlack=ORTESubscriptionCreate(
              domain,
              IMMEDIATE,
              BEST_EFFORTS,
              "Black",
              "BoxType",
              &boxType,
              &deadline,
              &minimumSeparationBlack,
              recvCallBack,
              this);
  	   m_mainFrm->SetActiveObject(3);
         }
	if (eYellow) {
          subYellow=ORTESubscriptionCreate(
              domain,
              IMMEDIATE,
              BEST_EFFORTS,
              "Yellow",
              "BoxType",
              &boxType,
              &deadline,
              &minimumSeparationYellow,
              recvCallBack,
              this);
  	   m_mainFrm->SetActiveObject(4);
         }

}


void Subscriber::Destroy()
{
        m_mainFrm->WantClose();
	m_mainFrm->close();
	ORTEDomainAppDestroy(domain);
        pthread_mutex_destroy(&mutex);

}


void Subscriber::changeMinSep()
{
	ORTESubsProp  sp;

//        ORTESubscriptionPropertiesGet(subscriber,&sp);
//	NtpTimeAssembFromMs(sp.minimumSeparation,MinSep->value(),0);
//        ORTESubscriptionPropertiesSet(subscriber,&sp);
	QString name=QString::number(MinSep->value());
	if((MinSep->value())<10) name+=" ";
	(this->strTitle).replace((this->strTitle).length()-3,2,name);
	(this->m_mainFrm)->setCaption(this->strTitle);
}
