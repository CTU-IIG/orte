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

void View_Frame::SetActiveObject(char object)
{
  active_object[object]=1;
}


void View_Frame::ShapeColorRect(char object,char shape,char color,QRect rect)
{
    	m_shape[object]=shape;

	switch(color)
	{
	case BLUE:
		m_shapeColor[object]=QColor(0,0,255);
		break;
	case GREEN:
		m_shapeColor[object]=QColor(0,255,0);
		break;
	case RED:
		m_shapeColor[object]=QColor(255,0,0);
		break;
	case BLACK:
		m_shapeColor[object]=QColor(0,0,0);
		break;
	}

	m_shapeRect[object]=rect;
	repaint();
}

void View_Frame::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	
        for(int i=0;i<5;i++) {
	    if (!active_object[i]) continue;
	    p.setBrush(m_shapeColor[i]);
	    p.setPen(NoPen);
  	    switch(m_shape[i])
	    {
	    case RECTANGLE:
		p.drawRect(m_shapeRect[i]);
		break;
	    case ELLIPSE:
		p.drawEllipse(m_shapeRect[i]);
		break;
	    case TRIANGLE:
		QPointArray pt(3);
		pt.putPoints(0,3, m_shapeRect[i].center().x(),m_shapeRect[i].top(), 
		                  m_shapeRect[i].right(),m_shapeRect[i].bottom(), 
				  m_shapeRect[i].left(),m_shapeRect[i].bottom());
		p.drawPolygon(pt);
		break;
	    }
	    QString strPos;
	    strPos="X:" + QString::number(m_shapeRect[i].center().x())+
	           "  Y:"+QString::number(m_shapeRect[i].center().y());
	    p.drawText(m_shapeRect[i].left()-(70-m_shapeRect[i].width())/2,m_shapeRect[i].top()-1,strPos);
	}

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
