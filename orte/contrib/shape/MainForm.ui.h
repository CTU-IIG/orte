/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qmessagebox.h>
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
#include "FPublisher.h"
#include "FSubscriber.h"
#include "richtext.h"
#if (QT_VERSION-0 >= 0x040000)
#include <QCloseEvent>
#include <QPixmap>
#include <q3ptrlist.h>
#else
#include <qptrlist.h>
#endif


void MainForm::addPublisher()
{

    FPublisher *fp;
    fp = new FPublisher;
    connect( this, SIGNAL( sigClose() ), fp, SLOT( destroy() ) );
    fp->initPublisher(publGroup->id(publGroup->selected()),rand()%3);
#if (QT_VERSION-0 >= 0x040000)
    fp->setWindowIcon(QPixmap("FPublisherIcon.png"));
#endif
    fp->show();
}

void MainForm::addSubscriber()
{
    FSubscriber *fs;
    fs = new FSubscriber;
    connect( this, SIGNAL( sigClose() ), fs, SLOT( destroy() ) );
    fs->initSubscribers(
	clBlue->isChecked(),
        clGreen->isChecked(),
        clRed->isChecked(),
        clBlack->isChecked(),
        clYellow->isChecked());
#if (QT_VERSION-0 >= 0x040000)
    fs->setWindowIcon(QPixmap("FSubscriberIcon.png"));
#endif    
    fs->show();
}

void MainForm::about()
{
    QMessageBox::about( this, "ORTE application demo",
                        "This application demonstrates usege of ORTE library\n\n"
			"Petr Smolik (c) 2004\n"
			"OCERA team");
}

void MainForm::viewPublSource()
{
    MyRichText *richtext;
    richtext=new MyRichText;
    richtext->resize( 450, 350 );
    richtext->setCaption( "Publisher example" );
    richtext->setTextPublisher();
    richtext->show();
}

void MainForm::viewSubsSource()
{
    MyRichText *richtext;
    richtext=new MyRichText;
    richtext->resize( 450, 350 );
    richtext->setCaption( "Subscriber example" );
    richtext->setTextSubscriber();
    richtext->show();
}

void MainForm::closeEvent( QCloseEvent *e )
{
    sigClose();
    e->accept();
}
