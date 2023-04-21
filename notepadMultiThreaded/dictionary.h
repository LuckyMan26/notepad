
#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QHash>
#include <iostream>

class Dictionary
{
    long long numOfWords;
    QHash<std::string, long long int> hash;
    std::string file_name;
protected:
    static Dictionary* dictionary_;
    Dictionary();
public:
    double getProbability(std::string word);
    Dictionary(Dictionary &other) = delete;

    void operator=(const Dictionary &) = delete;
    static Dictionary *GetInstance();
    bool checkWordInDictionary(std::string word);
    int getNumOfWords();
};

#endif // DICTIONARY_H
