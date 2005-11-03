/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <stdio.h>
#include <qtimer.h> 
#include <qapplication.h>
#if (QT_VERSION-0 >= 0x040000)
#include <QCloseEvent>
#endif

extern QApplication *a;

void FSubscriber::init()
{
    ORTEInit(); 
    domain=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
    BoxType_type_register(domain);
    subscriberBlue=subscriberGreen=subscriberRed=NULL;
    subscriberBlack=subscriberYellow=NULL;
}

void FSubscriber::closeEvent( QCloseEvent *e )
{
    destroy();
    e->accept();
}

void FSubscriber::destroy()
{
    if (domain) {
      ORTEDomainAppDestroy(domain);
      domain=NULL;
    }
}

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  BoxType *boxType=(BoxType*)vinstance;
  FSubscriber *s=(FSubscriber*)recvCallBackParam;
  QRect   rect;

  a->lock();
  switch (info->status) {
    case NEW_DATA:
      rect.setCoords(
          boxType->rectangle.top_left_x,
	  boxType->rectangle.top_left_y,
	  boxType->rectangle.bottom_right_x,
	  boxType->rectangle.bottom_right_y);
      s->view->activateObject(boxType->color,boxType->color,boxType->shape);
      s->view->setPosition(boxType->color,rect);
      break;
    case DEADLINE:
      if (strcmp(info->topic,"Blue")==0)
        s->view->deactivateObject(CL_BLUE);
      if (strcmp(info->topic,"Green")==0)
        s->view->deactivateObject(CL_GREEN);
      if (strcmp(info->topic,"Red")==0)
        s->view->deactivateObject(CL_RED);
      if (strcmp(info->topic,"Black")==0)
        s->view->deactivateObject(CL_BLACK);
      if (strcmp(info->topic,"Yellow")==0)
        s->view->deactivateObject(CL_YELLOW);
      break;
  }
  a->unlock();
}


void FSubscriber::initSubscribers( int iBlue, int iGreen, int iRed, int iBlack, int iYellow )
{
    NtpTime deadline;
    
    NtpTimeAssembFromMs(msBlue, 0, 0);
    NtpTimeAssembFromMs(msGreen, 0, 0);
    NtpTimeAssembFromMs(msRed, 0, 0);
    NtpTimeAssembFromMs(msBlack, 0, 0);
    NtpTimeAssembFromMs(msYellow, 0, 0);
    NtpTimeAssembFromMs(deadline, 6, 0);
    if (iBlue) {
	subscriberBlue=ORTESubscriptionCreate(
	    domain,
	    IMMEDIATE,
	    BEST_EFFORTS,
	    "Blue",
	    "BoxType",
	    &boxTypeBlue,
	    &deadline,
	    &msBlue,
	    recvCallBack,
	    this,
            IPADDRESS_INVALID);
	combo->insertItem("Blue",combo->count());
    }
    if (iGreen) {
	subscriberGreen=ORTESubscriptionCreate(
    	    domain,
            IMMEDIATE,
            BEST_EFFORTS,
            "Green",
            "BoxType",
            &boxTypeGreen,
            &deadline,
            &msGreen,
            recvCallBack,
            this,
	    IPADDRESS_INVALID);
	combo->insertItem("Green",combo->count());
    }
    if (iRed) {
	subscriberRed=ORTESubscriptionCreate(
    	    domain,
            IMMEDIATE,
            BEST_EFFORTS,
            "Red",
            "BoxType",
            &boxTypeRed,
            &deadline,
            &msRed,
            recvCallBack,
            this,
	    IPADDRESS_INVALID);
	combo->insertItem("Red",combo->count());
    }
    if (iBlack) {
	subscriberBlack=ORTESubscriptionCreate(
    	    domain,
            IMMEDIATE,
            BEST_EFFORTS,
            "Black",
            "BoxType",
            &boxTypeBlack,
            &deadline,
            &msBlack,
            recvCallBack,
            this,
	    IPADDRESS_INVALID);
	combo->insertItem("Black",combo->count());
    }
    if (iYellow) {
	subscriberYellow=ORTESubscriptionCreate(
    	    domain,
            IMMEDIATE,
            BEST_EFFORTS,
            "Yellow",
            "BoxType",
            &boxTypeYellow,
            &deadline,
            &msYellow,
            recvCallBack,
            this,
	    IPADDRESS_INVALID);
	combo->insertItem("Yellow",combo->count());
     }
}


void FSubscriber::comboActivated( int )
{
    a->lock();
    if  (combo->currentText()==QString("Blue")) 
	slider->setValue(msBlue.seconds);
    if  (combo->currentText()==QString("Green")) 
	slider->setValue(msGreen.seconds);
    if  (combo->currentText()==QString("Red")) 
	slider->setValue(msRed.seconds);
    if  (combo->currentText()==QString("Black")) 
	slider->setValue(msBlack.seconds);
    if  (combo->currentText()==QString("Yellow")) 
	slider->setValue(msYellow.seconds);
    a->unlock();
}


void FSubscriber::sliderValueChanged( int  value)
{
    NtpTime minSep;
    ORTESubsProp  sp;
  
    a->lock();
    NtpTimeAssembFromMs(minSep, value, 0);
    if  (combo->currentText()==QString("Blue")) {
	 msBlue=minSep;
	 ORTESubscriptionPropertiesGet(subscriberBlue,&sp);
	 sp.minimumSeparation=msBlue;
	 ORTESubscriptionPropertiesSet(subscriberBlue,&sp);    
    }
    if  (combo->currentText()==QString("Green")) {
	 msGreen=minSep;
	 ORTESubscriptionPropertiesGet(subscriberGreen,&sp);
	 sp.minimumSeparation=msGreen;
	 ORTESubscriptionPropertiesSet(subscriberGreen,&sp);    
    }
    if  (combo->currentText()==QString("Red")) {
	 msRed=minSep;
	 ORTESubscriptionPropertiesGet(subscriberRed,&sp);
	 sp.minimumSeparation=msRed;
	 ORTESubscriptionPropertiesSet(subscriberRed,&sp);    
    }
    if  (combo->currentText()==QString("Black")) { 
	 msBlack=minSep;
	 ORTESubscriptionPropertiesGet(subscriberBlack,&sp);
	 sp.minimumSeparation=msBlack;
	 ORTESubscriptionPropertiesSet(subscriberBlack,&sp);    
    }
    if  (combo->currentText()==QString("Yellow")) {
	 msYellow=minSep;
	 ORTESubscriptionPropertiesGet(subscriberYellow,&sp);
	 sp.minimumSeparation=msYellow;
	 ORTESubscriptionPropertiesSet(subscriberYellow,&sp);    
    }
    a->unlock();
}
