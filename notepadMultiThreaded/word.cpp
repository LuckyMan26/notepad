#include "word.h"
#include <ctime>
#include <set>
#include <vector>
#include <fstream>
#include "dictionary.h"
#include <time.h>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>


Word::Word(std::string str)
{
    word=str;
    if(word[word.length()-1]==':' || word[word.length()-1]=='-' || word[word.length()-1]=='+' || word[word.length()-1]==','){
        std::cout<<"Suits\n";
        nextSymbol = word[word.length()-1];
        word.pop_back();
    }
    else{
    nextSymbol=char(0);
    }
     word.erase(remove(word.begin(), word.end(), ' '), word.end());
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
        std::set<std::string> editFirstOrderSet = editFirstOrder(it);
        for(auto iterator : editFirstOrderSet){
            result.insert(iterator);
        }
    }
    return result;
}
/*class WordThread : public QThread
{
    Q_OBJECT
    std::string word;
    std::set<std::string>& candidates;
public:
    explicit WordThread(std::set<std::string>& candidates_,std::string w):
        word(w), candidates(candidates_){
    }
    void run() override {
        Word w;
        std::set<std::string> editFirstOrderSet = w.editFirstOrder(word);
        for(auto it : editFirstOrderSet){
            if(w.checkWordInDictionary(it)){
                candidates.insert(it);
            }
        }
        emit(resultReady(candidates));
    }
signals:
    void resultReady(const std::set<std::string>& candidates);
};*/
extern std::set<Candidate> possibleCandidatsOfFirstOrder(std::string w){
    Word word;
    std::set<Candidate> candidates;
    std::set<std::string> editSecondOrderSet = word.editFirstOrder(w);
    for(auto it : editSecondOrderSet){
        if(word.checkWordInDictionary(it)){
            candidates.insert(Candidate{it,1});
        }
    }
    return candidates;
}
extern std::set<Candidate> possibleCandidatsOfSecondOrder(std::string w){
    Word word;
    std::set<Candidate> candidates;
    std::set<std::string> editFirstOrderSet = word.editFirstOrder(w);
    for(auto it : editFirstOrderSet){
        if(word.checkWordInDictionary(it)){
              candidates.insert(Candidate{it,0.5});
        }
    }
    return candidates;
}
bool Word::checkWordInDictionary(std::string w){
    return Dictionary::GetInstance()->checkWordInDictionary(w);
}
void Word::setWord(std::string str){
    word=str;
}
std::string Word::getWord(){
    return word;
}
double Word::errorModel(Candidate w){
    double alpha=0.95;

    std::clock_t clock = std::clock();

    if(word==w.word){
        return alpha;
    }
    else if(checkWordInDictionary(w.word)){
        return double(w.coef*(1-alpha))/double((candidates.size()));
    }
    else{
        return 0;
    }
}
void Word::handleResults(std::set<std::string>& s){

}
std::set<Candidate> Word::possibleCandidates(std::string w){
    std::set<Candidate> candidates1;
    std::set<Candidate> candidates2;
    std::set<Candidate> res;

    QFuture<std::set<Candidate>> future1 = QtConcurrent::run(possibleCandidatsOfFirstOrder, w);
    QFuture<std::set<Candidate>> future2 = QtConcurrent::run(possibleCandidatsOfSecondOrder, w);

    candidates1 = future1.result();
    candidates2 = future2.result();
    std::merge(candidates1.begin(), candidates1.end(), candidates2.begin(), candidates2.end(), std::inserter(res, res.begin()));
    if(checkWordInDictionary(w)){
        res.insert(Candidate{w,2});
    }

    return res;
}
std::string Word::spellTest(){
    std::cout<<"Word: "<<word<<std::endl;
    candidates = possibleCandidates(word);
    std::clock_t clock = std::clock();
    auto oneMove = *(candidates.begin());
    Candidate res = oneMove;
    double maxProbability = (Dictionary::GetInstance()->getProbability(res.word)) * (errorModel(res));

    clock = std::clock();
    for(auto it:candidates){
         std::cout<<"Check all words: "<< it.word<<std::endl;
        double temp =  (Dictionary::GetInstance()->getProbability(it.word)) * errorModel(it);
        if(temp > maxProbability){
            res = it;
            maxProbability = temp;
        }
    }
    std::cout<<res.word<<std::endl;
    return res.word;
}
