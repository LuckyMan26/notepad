
#include "mainwindow.h"
#include "dictionary.h"
#include <QApplication>
#include <iostream>
#include "word.h"

int main(int argc, char *argv[])
{

    /*std::string str="sfdgfdsgsdfgsdfgrefwerfsxdfasdfsfgsfdgfd";

    std::vector<std::string> correction;
    Word w(str);
    correction = w.spellTest();
    std::cout<<correction.size()<<std::endl;
    for(auto it : correction){
        std::cout<<it<<std::endl;
    }*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //return 0;
}
