/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>

void MainForm::init()
{
	m_wantClose=0;
	m_close=0;
    }

void MainForm::resizeEvent( QResizeEvent *)
{
    	View->setGeometry(0,15,size().width(),size().height()-15);
}

void MainForm::SetShapeRect(QRect rect)
{
	m_shapeRect=rect;
	View->SetShapeRect(m_shapeRect);
}

void MainForm::SetProperties(char shape, char color)
{
	View->SetShape(shape);
	View->SetShapeColor(color);
}

int MainForm::MaxX()
{
	return View->width();
}

int MainForm::MaxY()
{
	return View->height();
}


void MainForm::closeEvent( QCloseEvent * e )
{
	if(m_wantClose==1) e->accept();
	else {
	    e->ignore();
	    m_close=1;
	}
}


void MainForm::WantClose()
{
	m_wantClose=1;
}

int MainForm::getClose()
{
 return m_close;
}
