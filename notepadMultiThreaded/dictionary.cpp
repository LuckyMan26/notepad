
#include "dictionary.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
Dictionary* Dictionary::dictionary_= nullptr;
std::vector<std::string> removeDupWord(std::string str,long long& numOfWords)
{
    std::vector<std::string> res;
    size_t start = 0;
    size_t end = str.find(' ');
    std::string word;
    while (end != std::string::npos) {
        numOfWords++;
        res.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(' ', start);
    }

    res.push_back(str.substr(start));
    return res;
}
Dictionary::Dictionary()
{

    file_name = ":/big_text.txt";
    QFile file("../big_text.txt");
    QTextStream in(&file);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout<<"file isn`t opened\n";
        return;
    }
    std::vector<std::string> words;
    std::string str;
    while(!in.atEnd()) {
        QString line = in.readLine();
        line = line.toLower();
        str = line.toStdString();

        words = removeDupWord(str,numOfWords);

        for(int i=0;i<words.size();i++){


            if (hash.contains(words[i])){
                int temp = hash[words[i]];
                hash[words[i]] = temp+1;
            }
            else{
                hash[words[i]] = 1;
            }
        }
        words.clear();
    }
    file.close();
}
double Dictionary::getProbability(std::string word){
    return (double(hash[word]) / double(numOfWords));
}
Dictionary *Dictionary::GetInstance()
{
    if(dictionary_==nullptr){
        dictionary_ = new Dictionary();
    }
    return dictionary_;
}
bool Dictionary::checkWordInDictionary(std::string word){
    return (hash.contains(word));
}
int Dictionary::getNumOfWords(){
    return numOfWords;
}
