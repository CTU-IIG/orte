/****************************************************************
**
** Definition of MyQFrame class
**
****************************************************************/

#ifndef MYQFRAME_H
#define MYQFRAME_H

#include <qframe.h>
#include <qrect.h>
#include <qpainter.h>
#include <qstring.h>

class MyQFrame : public QFrame
{
    Q_OBJECT
public:
    MyQFrame( QWidget *parent=0, const char *name=0 );
    
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent*);

public:
    void activateObject(int object,int color, int shape);
    void deactivateObject(int object);
    void setPosition(int object,QRect position);
    
protected:

private:
    int objects[5];
    int shapes[5];
    QColor colors[5];
    QRect positions[5];

public:
    int mousePressed;
    int mouseX;
    int mouseY;
};

#endif // MyQFRAME_H
