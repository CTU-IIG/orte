/****************************************************************
**
** Implementation MyQFrame class
**
****************************************************************/

#include "MyQFrame.h"

MyQFrame::MyQFrame( QWidget *parent, const char *name )
        : QFrame( parent, name )
{
    mousePressed=0;
    objects[0]=objects[1]=objects[2]=objects[3]=objects[4]=0;
}


void MyQFrame::activateObject(int object,int color, int shape) {
    if (object>4) return;
    objects[object]=1;
    switch(color) {
	case 0:colors[object]=QColor(0,0,255);break;
    	case 1:colors[object]=QColor(0,255,0);break;
	case 2:colors[object]=QColor(255,0,0);break;
	case 3:colors[object]=QColor(0,0,0);break;
	case 4:colors[object]=QColor(255,255,0);break;
    }
    shapes[object]=shape;
}

void MyQFrame::deactivateObject(int object) {
    if (object>4) return;
    objects[object]=0;
    repaint();
}

void MyQFrame::setPosition(int object,QRect position) {
    positions[object]=position;
    repaint();
}

void MyQFrame::paintEvent(QPaintEvent*)
{
    QPainter p(this);
	
    for(int i=0;i<5;i++) {
	if (!objects[i]) continue;
	p.setBrush(colors[i]);
	p.setPen(NoPen);
  	switch(shapes[i]){
	    case 0://rectangle
		p.drawRect(positions[i]);
		break;
	    case 1: //ellipse
		p.drawEllipse(positions[i]);
		break;
	    case 2: //triangle
		QPointArray pt(3);
		pt.putPoints(0,3, positions[i].center().x(),positions[i].top(), 
		                  positions[i].right(),positions[i].bottom(), 
				  positions[i].left(),positions[i].bottom());
		p.drawPolygon(pt);
		break;
	}
	QString strPos;
	strPos="X:" + QString::number(positions[i].center().x())+" " +
	       "Y:"+QString::number(positions[i].center().y());
	p.drawText(positions[i].left()-(70-positions[i].width())/2,positions[i].top()-1,strPos);
    }

}

void MyQFrame::mouseMoveEvent(QMouseEvent *e)
{
    if(mousePressed==0) return;	
    mouseX=e->pos().x();
    mouseY=e->pos().y();
}

void MyQFrame::mousePressEvent (QMouseEvent *e)
{
    mousePressed=1;
    mouseX=e->pos().x();
    mouseY=e->pos().y();
}

void MyQFrame::mouseReleaseEvent (QMouseEvent *)
{
    mousePressed=0;
}
