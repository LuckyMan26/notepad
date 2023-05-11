#ifndef WORD_H
#define WORD_H



#include "qthread.h"
#include <set>
#include <string>
struct Candidate{
    std::string word;
    double coef;
    bool operator < (const Candidate& c) const{
        return word < c.word;
    }
};
class Word
{

    std::string word;
    std::set<Candidate> possibleCandidates(std::string w);
    std::set<Candidate> candidates;

public:
    char nextSymbol;
    std::set<std::string> editFirstOrder(std::string word);
    std::set<std::string> editSecondOrder(std::string w);
    Word(std::string str="");
    void setWord(std::string str);
    std::string getWord();
    bool checkWordInDictionary(std::string w);
    double errorModel(Candidate w);
    std::string spellTest();
public slots:
    void handleResults(std::set<std::string>& s);

};
#endif // WORD_H
