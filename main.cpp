/*
 *  Custom Minecraft Launcher - ILMM - main.cpp
 *  by Hopskocz
 */

#include "ilmm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString version = "v0001";
    ILMM w( 0, version );
    w.show();
    
    return a.exec();
}
