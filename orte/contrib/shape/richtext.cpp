/****************************************************************************
** $Id: richtext.cpp,v 1.1 2004/03/17 22:46:13 smolik Exp $
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
    "<b>Saying 1:</b><br>"
    "<hr><br><br>"
    "<big>Evil is that which one believes of others.  It is a sin to believe evil "
    "of others, but it is seldom a mistake.</big><br><br>"
    "<center><i>-- H.L. Mencken</i></center>",

    "<b>Saying 2:</b><br>"
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
    "<b>Example 1:</b><br>"
    "<hr><br><br>"
    "<big>Evil is that which one believes of others.  It is a sin to believe evil "
    "of others, but it is seldom a mistake.</big><br><br>"
    "<center><i>-- H.L. Mencken</i></center>",

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

    view->setMinimumSize( 450, 250 );

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





