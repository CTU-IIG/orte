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
#include "ortedemo_types.h"

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
    void SetActiveObject(char object);
    void ShapeColorRect(char object,char shape,char color,QRect rect);
    void paintEvent(QPaintEvent*);

protected:
    int active_object[5];
    QRect m_shapeRect[5];
    QColor m_shapeColor[5];

private:
    char m_shape[5];

public:
    char m_mousePressed;
    int m_mouseX;
    int m_mouseY;

};


#endif // VIEW_FRAME_H
