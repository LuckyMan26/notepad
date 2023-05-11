#include "qmutex.h"
#include "checkspellingthread.h"
#include <iostream>
#include <stdexcept>
#include <thread>

CheckSpellingThread::CheckSpellingThread(std::vector<std::string>& wordVec_,Word w,int beg_,int end_) :

    vecOfWords(wordVec_)
{
    word = w;
    beg = beg_;
    end = end_;

}
void CheckSpellingThread::setWord(Word w){

}
CheckSpellingThread::~CheckSpellingThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

}
void CheckSpellingThread::checkSpell(Word w,int beg_,int end_){

    mutex.lock();
    //m_isAwake=true;
    word = w;
    beg = beg_;
    end = end_;
    mutex.unlock();


}
void CheckSpellingThread::RunThread(Word w){
    mutex.lock();
    word = w;
    mutex.unlock();
    condition.wakeOne();

}
void CheckSpellingThread::run(){

    std::vector<std::string> correction_ = word.spellTest();
    std::vector<QString> res;
    for(int i=0;i<correction_.size();i++){
        res.push_back(QString::fromStdString(correction_[i]));
    }


        //vecOfWords.push_back(correction_);
        emit finishedComputing(res,QString::fromStdString(word.getWord()),beg,end);

    //emit Finished();
}
