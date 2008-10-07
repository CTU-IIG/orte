#include <QMessageBox>
#include "MainForm.h"
#include "richtext.h"

#include "FPublisher.h"
#include "FSubscriber.h"

MainForm::MainForm(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}

void MainForm::about()
{
    QMessageBox::about( this, "ORTE application demo",
                        "This application demonstrates usege of ORTE library\n\n"
			"Petr Smolik (c) 2004-2008\n"
			"OCERA team");
}

void MainForm::viewPublSource()
{
    MyRichText *richtext;
    richtext=new MyRichText;
    richtext->resize( 450, 350 );
    richtext->setWindowTitle( "Publisher example");
    richtext->setTextPublisher();
    richtext->show();
    connect( this, SIGNAL( rejected() ), richtext, SLOT( close() ) );
}

void MainForm::viewSubsSource()
{
    MyRichText *richtext;
    richtext=new MyRichText;
    richtext->resize( 450, 350 );
    richtext->setWindowTitle( "Subscriber example");
    richtext->setTextSubscriber();
    connect( this, SIGNAL( rejected() ), richtext, SLOT( close() ) );
    richtext->show();
}

void MainForm::addPublisher()
{
    FPublisher *fp;
    int color=0;

    if (radioButton2_3->isChecked())
      color=1;
    if (radioButton2_4->isChecked())
      color=2;
    if (radioButton1->isChecked())
      color=3;
    if (radioButton2->isChecked())
      color=4;
    fp = new FPublisher();
    fp->initPublisher(color,rand()%3);
    connect( this, SIGNAL( rejected() ), fp, SLOT( destroy() ) );
    fp->setWindowIcon(QPixmap(":/FPublisherIcon.png"));
    fp->show();
}

void MainForm::addSubscriber()
{
    FSubscriber *fs;
    fs = new FSubscriber;
    connect( this, SIGNAL( rejected() ), fs, SLOT( destroy() ) );
    fs->initSubscribers(
	clBlue->isChecked(),
        clGreen->isChecked(),
        clRed->isChecked(),
        clBlack->isChecked(),
        clYellow->isChecked());
    fs->setWindowIcon(QPixmap(":/FSubscriberIcon.png"));
    fs->show();
}
