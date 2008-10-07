#include <QApplication>

#include "MainForm.h"


int main( int argc, char ** argv )
{
    QApplication app(argc, argv);
    MainForm mf;
    mf.connect(&app, SIGNAL( lastWindowClosed()), &app, SLOT(quit()));
    mf.setWindowIcon(QPixmap(":/FMainIcon.png"));
    mf.show();
    return app.exec();
}
