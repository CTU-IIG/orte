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

void FSubscriber::init()
{
    ORTEInit(); 
    domain=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
    ORTETypeRegisterBoxType(domain);
    subscriberBlue=subscriberGreen=subscriberRed=NULL;
    subscriberBlack=subscriberYellow=NULL;
    pthread_mutex_init(&mutex,NULL);
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
      pthread_mutex_destroy(&mutex);
    }
}

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  BoxType *boxType=(BoxType*)vinstance;
  FSubscriber *s=(FSubscriber*)recvCallBackParam;
  QRect   rect;

  switch (info->status) {
    case NEW_DATA:
      pthread_mutex_lock(&s->mutex);
      rect.setCoords(
          boxType->rectangle.top_left_x,
	  boxType->rectangle.top_left_y,
	  boxType->rectangle.bottom_right_x,
	  boxType->rectangle.bottom_right_y);
      s->view->activateObject(boxType->color,boxType->color,boxType->shape);
      s->view->setPosition(boxType->color,rect);
      pthread_mutex_unlock(&s->mutex);
      break;
    case DEADLINE:
      if (strcmp(info->topic,"Blue")==0)
        s->view->deactivateObject(0);
      if (strcmp(info->topic,"Green")==0)
        s->view->deactivateObject(1);
      if (strcmp(info->topic,"Red")==0)
        s->view->deactivateObject(2);
      if (strcmp(info->topic,"Black")==0)
        s->view->deactivateObject(3);
      if (strcmp(info->topic,"Yellow")==0)
        s->view->deactivateObject(4);
      break;
  }
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
	    this);
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
            this);
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
            this);
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
            this);
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
            this);
	combo->insertItem("Yellow",combo->count());
     }
}


void FSubscriber::comboActivated( int )
{
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
}


void FSubscriber::sliderValueChanged( int  value)
{
    NtpTime minSep;
    ORTESubsProp  sp;
  
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
}
