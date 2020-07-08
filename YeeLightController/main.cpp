#include <QApplication>
#include <iostream>
#include "header/window.h"



int main(int argc, char *argv[]) 
{

	QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
