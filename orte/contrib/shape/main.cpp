#include <qapplication.h>
#if (QT_VERSION-0 >= 0x040000)
#include <QPixmap>
#endif
#include "MainForm.h"

QApplication *a;

int main( int argc, char ** argv )
{
    a=new QApplication(argc,argv);
    MainForm w;
    a->setMainWidget( &w);
#if (QT_VERSION-0 >= 0x040000)
    w.setWindowIcon(QPixmap("FMainIcon.png"));
#endif
    w.show();
    a->connect( a, SIGNAL( lastWindowClosed() ), a, SLOT( quit() ) );
    return a->exec();
}
