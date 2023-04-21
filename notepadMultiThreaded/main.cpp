
#include "mainwindow.h"
#include "dictionary.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    std::cout<<"Hello" << std::endl;
    Dictionary d;
    double res = d.getProbability("the");
    std::cout<<res<<std::endl;
   /* QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/
}
