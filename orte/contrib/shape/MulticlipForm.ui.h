/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include <orte.h>
#include <qwidget.h>
Subscriber *tabSub[50];
int NSubscriber;
Publisher *tabPub[50];
int NPublisher;
int np,ns;

void MulticlipForm::init()
{
	memset(tabSub,0,sizeof(tabSub));
	memset(tabPub,0,sizeof(tabPub));
	NPublisher=0;
	NSubscriber=0;
	np=ns=1;
	QTimer *timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), SLOT(Timer()));
 	timer->start( 500, FALSE );
}


void MulticlipForm::addPublisher()
{
	PublishersListBox->insertItem( QString::number(np), 0 );
	GotoP();
	NPublisher++;
}



void MulticlipForm::deletePublisher()
{
	QString  text=PublishersListBox->currentText();
	if(PublishersListBox->currentItem()!=-1 && !text.isEmpty()){
		int pub;
		pub=NPublisher-1-PublishersListBox->currentItem() ;
		PublishersListBox->removeItem( PublishersListBox->currentItem() );
		tabPub[pub]->Destroy();
		delete(tabPub[pub]);
		for(int i=pub;i+1<NPublisher;i++) tabPub[i]=tabPub[i+1];
		tabPub[NPublisher]=NULL;
		NPublisher--;
	}
}


void MulticlipForm::GotoP(  )
{
    QString name = QString::number(np);
	np++;
    int shap=ShapeP->currentItem();
    int color=ColorP->currentItem();
    int strength=0;
   tabPub[NPublisher]=new Publisher();
   tabPub[NPublisher]->Create(name,shap,color,strength);
}



void MulticlipForm::closeEvent( QCloseEvent *e )
{
	int i;
    for(i=0;i<NPublisher;i++){
	 tabPub[i]->Destroy();
	 delete(tabPub[i]);
     }
    for(i=0;i<NSubscriber;i++){
	 tabSub[i]->Destroy();
	 delete(tabSub[i]);
     }
e->accept();
}

void rcvCallBack(ORTERcvInfo *rcvInfo,u_char status)
{
	QRect rect;
	int a,b,c,d;
	char *topic,*type;
	int top=0,typ=0;
	switch (status) {
		case 0:  //Issue
			sscanf((char*)rcvInfo->data,"%i %i %i %i",&a,&b,&c,&d);
			rect.setCoords(a,b,c,d);
			topic=(char *)rcvInfo->subsTopic;
			type=(char *)rcvInfo->subsTypeName;
			if(strcmp(topic,"Ellipse")==0)top=0;
			if(strcmp(topic,"Rectangle")==0)top=1;
			if(strcmp(topic,"Triangle")==0)top=2;
			if(strcmp(type,"Blue")==0)typ=0;
			if(strcmp(type,"Green")==0)typ=1;
			if(strcmp(type,"Red")==0)typ=2;
			if(strcmp(type,"Black")==0)typ=3;

			for(int i=0;i<NSubscriber;i++){
			if(tabSub[i]->topS==top && tabSub[i]->typS==typ){
				(tabSub[i]->m_mainFrm)->setCaption(tabSub[i]->strTitle);
				(tabSub[i]->m_mainFrm)->SetShapeRect(rect);
			}
		}
		break;
		case 1:  //Data
			for(int i=0;i<NSubscriber;i++){
				int dead=0;
				if(NPublisher!=0){
					for(int j=0;j<NPublisher;j++){
						if(tabSub[i]->topS!=tabPub[j]->top || tabSub[i]->typS!=tabPub[j]->typ){
						dead++;
						}
					}
				}
			 if(NPublisher==0 || dead==NPublisher){
		 		QString name=tabSub[i]->strTitle+" deadline ";
		 		(tabSub[i]->m_mainFrm)->setCaption(name);
			}
		}
		break;
	}
}

void MulticlipForm::GotoS(  )
{
    QString name =QString::number(ns);
	ns++;
    int shap=ShapeS->currentItem();
    int color=ColorS->currentItem();
	QString topic;
	QString type;

switch(shap)
	{
	case RECTANGLE:
		topic="Rectangle";
		break;
	case ELLIPSE:
		topic="Ellipse";
		break;
	case TRIANGLE:
		topic="Triangle";
		break;
	}

	switch(color)
	{
	case BLUE:
		type="Blue";
		break;
	case GREEN:
		type="Green";
		break;
	case RED:
		type="Red";
		break;
	case BLACK:
		type="Black";
		break;
	}
NtpTime	minimumSeparation,deadline;
int h_sub;
   tabSub[NSubscriber]=new Subscriber();
   tabSub[NSubscriber]->Create(name,shap,color);
ORTEAppCreate(&(tabSub[NSubscriber]->app));
  NtpTimeAssembFromMs(minimumSeparation,0,0);
  NtpTimeAssembFromMs(deadline, 5, 0);
 h_sub=ORTEAppSubsAdd(tabSub[NSubscriber]->app,topic,type, &minimumSeparation,&deadline,rcvCallBack);
}

void MulticlipForm::addSubscriber()
{
	
	SubscribersListBox->insertItem( QString::number(ns), 0 );
	GotoS();
	NSubscriber++;
}


void MulticlipForm::deleteSubscriber()
{
QString  text=SubscribersListBox->currentText();
	if(SubscribersListBox->currentItem()!=-1 && !text.isEmpty()){
		int pub;
		pub=NSubscriber-1-SubscribersListBox->currentItem() ;
		SubscribersListBox->removeItem( SubscribersListBox->currentItem() );
		tabSub[pub]->Destroy();
		delete(tabSub[pub]);
		for(int i=pub;i+1<NSubscriber;i++) tabSub[i]=tabSub[i+1];
		tabSub[NSubscriber]=NULL;
		NSubscriber--;
	}
}



void MulticlipForm::keyPressEvent( QKeyEvent *e )
{    
    if(Qt::Key_Delete==e->key() ) {
		if( focusWidget ()==PublishersListBox)deletePublisher();
		if( focusWidget ()==SubscribersListBox)deleteSubscriber();
    }
}



void MulticlipForm::Timer()
{
	int i;
	for(i=0;i<NPublisher;i++){
		if((tabPub[i]->m_mainFrm)->getClose()==1){
			PublishersListBox->setCurrentItem (NPublisher-1-i);
			deletePublisher();
		}
	}
	for(i=0;i<NSubscriber;i++){
		if((tabSub[i]->m_mainFrm)->getClose()==1){
			SubscribersListBox->setCurrentItem (NSubscriber-1-i);
			deleteSubscriber();
		}
	}
}
