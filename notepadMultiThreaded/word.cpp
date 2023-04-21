
#include "word.h"
#include <set>
#include <vector>
#include <fstream>
#include "dictionary.h"
Word::Word()
{
    word="";
}

std::set<std::string> Word::edit(){
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::pair<std::string, std::string>> splits;
    for (int i = 0; i <= word.length(); i++) {
        splits.push_back({word.substr(0, i), word.substr(i)});
    }
    std::vector<std::string> deletes, transposes, replaces, inserts;
    for (auto split : splits) {
        std::string L = split.first, R = split.second;
        if (!R.empty()) {
            deletes.push_back(L + R.substr(1));
        }
        if (R.length() > 1) {
            transposes.push_back(L + R[1] + R[0] + R.substr(2));
        }
        for (char c : letters) {
            if (R.length() > 0) {
                replaces.push_back(L + c + R.substr(1));
            }
            inserts.push_back(L + c + R);
        }
    }
    std::set<std::string> result;
    result.insert(deletes.begin(), deletes.end());
    result.insert(transposes.begin(), transposes.end());
    result.insert(replaces.begin(), replaces.end());
    result.insert(inserts.begin(), inserts.end());
    return result;

}
bool Word::checkWordInDictionary(std::string w){
    std::ifstream f;
    std::string str;
    return Dictionary::GetInstance()->checkWordInDictionary(w);
}
void Word::setWord(std::string str){
    word=str;
}
std::string Word::getWord(){
    return word;
}
