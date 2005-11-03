/****************************************************************************
** $Id: richtext.h,v 1.5 2005/11/03 09:27:47 smolik Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <qglobal.h>

#if (QT_VERSION-0 >= 0x040000)
#define QVBox Q3VBox
#define QHBox Q3HBox
#define QTextView Q3TextView
#include <q3vbox.h>
#else
#include <qvbox.h>
#endif

class QTextView;
class QPushButton;

class MyRichText : public QVBox
{
    Q_OBJECT

public:
    MyRichText( QWidget *parent = 0, const char *name = 0);
    void setTextPublisher();
    void setTextSubscriber();
    const char **sayings;

protected:
    QTextView *view;
    QPushButton *bClose, *bNext, *bPrev;
    int num;

protected slots:
    void prev();
    void next();

};

#endif
