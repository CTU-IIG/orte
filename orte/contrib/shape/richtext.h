/****************************************************************************
** $Id: richtext.h,v 1.6 2008/10/07 21:19:07 smolik Exp $
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
#include <QPushButton>
#include <QTextBrowser>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MyRichText : public QWidget
{
    Q_OBJECT

public:
    MyRichText( QWidget *parent = 0, Qt::WindowFlags f = 0);
    void setTextPublisher();
    void setTextSubscriber();
    const char **sayings;

protected:
    QTextBrowser *view;
    QPushButton *bClose, *bNext, *bPrev;
    int num;

protected slots:
    void prev();
    void next();

};

#endif
