#include <qapplication.h>
#include "MainForm.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MainForm w;
    a.setMainWidget( &w);
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
