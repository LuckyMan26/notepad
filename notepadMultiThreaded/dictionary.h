
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
public:
    Dictionary();
    double getProbability(std::string word);

    Dictionary(Dictionary &other) = delete;

    void operator=(const Dictionary &) = delete;
    static Dictionary *GetInstance();
    bool checkWordInDictionary(std::string word);

};

#endif // DICTIONARY_H
