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
#include <qptrlist.h>
#include "FPublisher.h"
#include "FSubscriber.h"
#include "richtext.h"


void MainForm::addPublisher()
{

    FPublisher *fp;
    fp = new FPublisher(this);
    fp->initPublisher(publGroup->id(publGroup->selected()),rand()%3);
    fp->show();
}

void MainForm::addSubscriber()
{
    FSubscriber *fs;
    fs = new FSubscriber(this);
    fs->initSubscribers(
	clBlue->isChecked(),
        clGreen->isChecked(),
        clRed->isChecked(),
        clBlack->isChecked(),
        clYellow->isChecked());
    fs->show();
}

void MainForm::about()
{
    QMessageBox::about( this, "ORTE application demo",
                        "This application demonstrates use of ORTE library\n\n"
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
