#include <qapplication.h>
#include "MainForm.h"

QApplication *a;

int main( int argc, char ** argv )
{
    a=new QApplication(argc,argv);
    MainForm w;
    a->setMainWidget( &w);
    w.show();
    a->connect( a, SIGNAL( lastWindowClosed() ), a, SLOT( quit() ) );
    return a->exec();
}
