
#ifndef WORD_H
#define WORD_H



#include <set>
#include <string>
class Word
{
    std::string word;
    std::set<std::string> possibleCandidates(std::string w);
    std::set<std::string> editFirstOrder(std::string word);
    std::set<std::string> editSecondOrder(std::string w);
public:
    Word(std::string str="");
    void setWord(std::string str);
    std::string getWord();
    bool checkWordInDictionary(std::string w);
    double errorModel(std::string w);
    std::string spellTest();


};

#endif // WORD_H
