/****************************************************************************
** $Id: richtext.cpp,v 1.3 2004/03/18 21:38:02 smolik Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "richtext.h"

#include <qhbox.h>
#include <qhbox.h>
#include <qpushbutton.h>
#include <qtextview.h>
#include <qbrush.h>
#include <qapplication.h>

static const char* publisherExamples[] = {
    "<b>Publisher Example 1:</b><br>"
    "<hr><br>"

    "<pre>ORTEInit();<br>"
    "d=<b>ORTEDomainAppCreate</b>(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);<br>"
    "<b>ORTETypeRegisterAdd</b>(d,\"HelloMsg\",NULL,NULL,64);<br>"
    "NTPTIME_BUILD(persistence,3);<br>"
    "p=<b>ORTEPublicationCreate</b>(<br>"
    "     d,<br>"
    "     \"Example HelloMsg\",    /* Topic */<br>"
    "     \"HelloMsg\",            /* Type */<br>"
    "     &instance2Send,        /* Instance */<br>"
    "     &persistence,          /* Persistance */<br>"
    "     1,                     /* Strength */<br>"
    "     NULL,<br>"
    "     NULL,<br>"
    "     NULL);<br></pre>",
  
    "<b>Example 2:</b><br>"
    "<hr><br><br>"
    "<big>A well-used door needs no oil on its hinges.<br>"
    "A swift-flowing steam does not grow stagnant.<br>"
    "Neither sound nor thoughts can travel through a vacuum.<br>"
    "Software rots if not used.<br><br>"
    "These are great mysteries.</big><br><br>"
    "<center><i>-- Geoffrey James, \"The Tao of Programming\"</i></center>",

    0
};

static const char* subscriberExamples[] = {
    "<b>Subscriber Example 1:</b><br>"
    "<hr><br>"

    "<pre>ORTEInit();<br>"
    "d=<b>ORTEDomainAppCreate</b>(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);<br>"
    "<b>ORTETypeRegisterAdd</b>(d,\"HelloMsg\",NULL,NULL,64);<br>"
    "NTPTIME_BUILD(persistence,3);<br>"
    "s=<b>ORTESubscriptionCreate</b>(<br>"
    "    d,<br>"
    "    IMMEDIATE,<br>"
    "    BEST_EFFORTS,<br>"
    "    \"Example HelloMsg\",     /* Topic */<br>"
    "    \"HelloMsg\",             /* Type */<br>"
    "    &instance2Recv,         /* Instance */<br>"
    "    &deadline,              /* Deadline */<br>"
    "    &minimumSeparation,     /* Minimum Separation */<br>"
    "    recvCallBack,           /* CallBack function */<br>"
    "    NULL);<br><br>"
    "void <b>recvCallBack</b>(const ORTERecvInfo *info,<br>"
    "                  void *vinstance, void *recvCallBackParam) {<br>"
    "  char *instance=(char*)vinstance;<br><br>"
    "  switch (info->status) {<br>"
    "    case NEW_DATA:<br>"
    "      printf(\"%s\",instance);<br>"
    "      break;<br>"
    "    case DEADLINE:<br>"
    "      printf(\"deadline occured\");<br>"
    "      break;<br>"
    "  }<br>"
    "}<br></pre>",

    "<b>Example 2:</b><br>"
    "<hr><br><br>"
    "<big>A well-used door needs no oil on its hinges.<br>"
    "A swift-flowing steam does not grow stagnant.<br>"
    "Neither sound nor thoughts can travel through a vacuum.<br>"
    "Software rots if not used.<br><br>"
    "These are great mysteries.</big><br><br>"
    "<center><i>-- Geoffrey James, \"The Tao of Programming\"</i></center>",

    0
};


MyRichText::MyRichText( QWidget *parent, const char *name )
    : QVBox( parent, name )
{
    setMargin( 5 );
    
    view = new QTextView( this );
    QBrush paper;
    paper.setPixmap( QPixmap( "marble.png" ) );
    if ( paper.pixmap() != 0 )
	view->setPaper( paper );
    else
	view->setPaper( white );

    view->setMinimumSize( 500, 250 );

    QHBox *buttons = new QHBox( this );
    buttons->setMargin( 5 );

    bClose = new QPushButton( "&Close", buttons );
    bPrev = new QPushButton( "<< &Prev", buttons );
    bNext = new QPushButton( "&Next >>", buttons );

    bPrev->setEnabled( FALSE );

    connect( bClose, SIGNAL(clicked()), this, SLOT(close()) );
    connect( bPrev, SIGNAL( clicked() ), this, SLOT( prev() ) );
    connect( bNext, SIGNAL( clicked() ), this, SLOT( next() ) );

}

void MyRichText::setTextPublisher()
{
    num = 0;
    sayings=publisherExamples;
    view->setText(sayings[0]);
}


void MyRichText::setTextSubscriber()
{
    num = 0;
    sayings=subscriberExamples;
    view->setText(sayings[0]);
}

void MyRichText::prev()
{
    if ( num <= 0 )
        return;

    num--;

    view->setText( sayings[num] );

    if ( num == 0 )
        bPrev->setEnabled( FALSE );

    bNext->setEnabled( TRUE );
}

void MyRichText::next()
{
    if ( !sayings[++num] )
        return;

    view->setText( sayings[num] );

    if ( !sayings[num + 1] )
        bNext->setEnabled( FALSE );

    bPrev->setEnabled( TRUE );
}





