/****************************************************************
**
** Definition of View_Frame class
**
****************************************************************/

#ifndef VIEW_FRAME_H
#define VIEW_FRAME_H

#include <qwidget.h>
#include <qrect.h>
#include <qcolor.h>
#include "define.h"

class View_Frame : public QWidget
{
    Q_OBJECT
public:
    View_Frame( QWidget *parent=0, const char *name=0 );
    
protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);


public:
    void SetShape(char shape);
    void SetShapeColor(char color);
    void paintEvent(QPaintEvent*);
    void SetShapeRect(QRect rect);

protected:
    QRect m_shapeRect;
    QColor m_shapeColor;

private:
    char m_shape;

public:
	char m_mousePressed;
	int m_mouseX;
	int m_mouseY;

};


#endif // VIEW_FRAME_H
