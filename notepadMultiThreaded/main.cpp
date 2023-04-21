
#include "mainwindow.h"
#include "dictionary.h"
#include <QApplication>
#include <iostream>
#include "word.h"

int main(int argc, char *argv[])
{

    std::string str="speling";

    std::string correction;
    Word w(str);
    correction = w.spellTest();
    std::cout<<correction<<std::endl;
   /* QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/
    return 0;
}
