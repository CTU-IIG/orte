#include <qapplication.h>
#include "MulticlipForm.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
     MulticlipForm *w = new MulticlipForm();
    w->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
