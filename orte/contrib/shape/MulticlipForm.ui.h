/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include <orte_api.h>
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

void MulticlipForm::GotoS(  )
{
    QString name =QString::number(ns);
    ns++;
    tabSub[NSubscriber]=new Subscriber();
    tabSub[NSubscriber]->Create(name,
                                clBlue->isChecked(),
                                clGreen->isChecked(),
                                clRed->isChecked(),
                                clBlack->isChecked(),
                                clYellow->isChecked());
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
