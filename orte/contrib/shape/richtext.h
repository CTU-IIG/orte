/****************************************************************************
** $Id: richtext.h,v 1.3 2004/04/03 11:33:16 smolik Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <qvbox.h>

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
