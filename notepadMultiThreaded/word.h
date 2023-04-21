
#ifndef WORD_H
#define WORD_H



#include <set>
#include <string>
class Word
{
    std::string word;

public:
    Word();
    std::set<std::string> edit();
    void setWord(std::string str);
    std::string getWord();
    bool checkWordInDictionary(std::string w);
};

#endif // WORD_H
