
#ifndef WORD_H
#define WORD_H



#include "qthread.h"
#include <set>
#include <string>
class Word
{


    std::string word;
    std::set<std::string> possibleCandidates(std::string w);
    std::set<std::string> candidates;

public:
    std::set<std::string> editFirstOrder(std::string word);
    std::set<std::string> editSecondOrder(std::string w);
    Word(std::string str="");
    void setWord(std::string str);
    std::string getWord();
    bool checkWordInDictionary(std::string w);
    double errorModel(std::string w);
    std::string spellTest();
public slots:
    void handleResults(std::set<std::string>& s);

};

#endif // WORD_H
