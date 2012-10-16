#include <QtGui/QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("UoYCS");
    a.setOrganizationDomain("cs.york.ac.uk");
    a.setApplicationName("GP Developer");

    MainWindow w;
    w.show();
    
    return a.exec();
}
