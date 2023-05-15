
#include "mainwindow.h"
#include "dictionary.h"
#include <QApplication>
#include <iostream>
#include "word.h"

int main(int argc, char *argv[])
{
    /*int n = 100;
    std::clock_t clock = std::clock();
    for(int i=0;i<n;i++){
    std::string str="teh";

    std::vector<std::string> correction;
    Word w(str);
    correction = w.spellTest();
    }*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
