
#include "word.h"
#include <set>
#include <vector>
#include <fstream>
#include "dictionary.h"
Word::Word(std::string str)
{
    word=str;
}

std::set<std::string> Word::editFirstOrder(std::string word_){
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::pair<std::string, std::string>> splits;
    for (int i = 0; i <= word_.length(); i++) {
        splits.push_back({word_.substr(0, i), word_.substr(i)});
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
std::set<std::string> Word::editSecondOrder(std::string w){
    std::set<std::string> result;
    std::set<std::string> editsOfFirstOrder_ = editFirstOrder(w);
    for(auto it : editsOfFirstOrder_){
        for(auto iterator : editFirstOrder(it)){
            result.insert(iterator);
        }
    }
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
double Word::errorModel(std::string w){
    double alpha=0.95;
    std::set<std::string> candidates = possibleCandidates(w);

    if(word==w){
        return alpha;
    }
    else if(checkWordInDictionary(w)){
        return double(1-alpha)/double(candidates.size());
    }
    else{
        return 0;
    }
}
std::set<std::string> Word::possibleCandidates(std::string w){
    std::set<std::string> candidates;
    for(auto it : editFirstOrder(w)){
        if(checkWordInDictionary(it)){
            candidates.insert(it);
        }
    }
    for(auto it : editSecondOrder(w)){
        if(checkWordInDictionary(it)){
            candidates.insert(it);
        }
    }
    if(checkWordInDictionary(w)){
        candidates.insert(w);
    }
    return candidates;
}
std::string Word::spellTest(){
    std::set<std::string> candidates = possibleCandidates(word);
    auto oneMove = *(candidates.begin());
    std::string res = oneMove;
    double maxProbability = (Dictionary::GetInstance()->getProbability(res)) * (errorModel(res));
    for(auto it:candidates){
        std::cout<<it<<std::endl;
        double temp =  (Dictionary::GetInstance()->getProbability(it)) * errorModel(it);
        if(temp > maxProbability){
            res = it;
            maxProbability = temp;
        }
    }
    std::cout<<maxProbability<<std::endl;
    return res;
}
