/****************************************************************
**
** Implementation View_Frame class
**
****************************************************************/

#include "ViewFrm.h"
#include <qpainter.h>
#include <qstring.h>

View_Frame::View_Frame( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    	setBackgroundColor(QColor( 255, 255, 255)  );
	m_mousePressed=0;
}

void View_Frame::SetShape(char shape)
{
    	m_shape=shape;
}

void View_Frame::SetShapeColor(char color)
{
	switch(color)
	{
	case BLUE:
		m_shapeColor=QColor(0,0,255);
		break;
	case GREEN:
		m_shapeColor=QColor(0,255,0);
		break;
	case RED:
		m_shapeColor=QColor(255,0,0);
		break;
	case BLACK:
		m_shapeColor=QColor(0,0,0);
		break;
	}
}

void View_Frame::SetShapeRect(QRect rect)
{
	m_shapeRect=rect;
	repaint();
}

void View_Frame::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	
	p.setBrush(m_shapeColor);
	p.setPen(NoPen);


	switch(m_shape)
	{
	case RECTANGLE:
		p.drawRect(m_shapeRect);
		break;
	case ELLIPSE:
		p.drawEllipse(m_shapeRect);
		break;
	case TRIANGLE:
		QPointArray pt(3);
		pt.putPoints(0,3, m_shapeRect.center().x(),m_shapeRect.top(), m_shapeRect.right(),m_shapeRect.bottom(), m_shapeRect.left(),m_shapeRect.bottom());
		p.drawPolygon(pt);
		break;
	}

	QString strPos;
	
	strPos="X:" + QString::number(m_shapeRect.center().x())+"  Y:"+QString::number(m_shapeRect.center().y());
	
	p.drawText(m_shapeRect.left()-(70-m_shapeRect.width())/2,m_shapeRect.top()-1,strPos);
}

void View_Frame::mouseMoveEvent(QMouseEvent *e)
{
	if(m_mousePressed==0) return;
	
	m_mouseX=e->pos().x();
	m_mouseY=e->pos().y();
}

void View_Frame::mousePressEvent ( QMouseEvent * e)
{
	m_mousePressed=1;
	m_mouseX=e->pos().x();
	m_mouseY=e->pos().y();
}

void View_Frame::mouseReleaseEvent ( QMouseEvent * )
{
	m_mousePressed=0;
}
