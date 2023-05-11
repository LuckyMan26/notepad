#include "qdebug.h"
#include "qmutex.h"
#include "checkspellingthread.h"
#include <iostream>
CheckSpellingThread::CheckSpellingThread(Word w,int beg_,int end_,QObject *parent) :
    QThread(parent)
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
    wait();
}
void CheckSpellingThread::checkSpell(Word w,int beg_,int end_){

    mutex.lock();
    //m_isAwake=true;
    word = w;
    beg = beg_;
    end = end_;
    mutex.unlock();
    if (!isRunning()) {
        start(HighPriority);
    }

}
void CheckSpellingThread::RunThread(Word w){
    mutex.lock();
    word = w;
    mutex.unlock();
    condition.wakeOne();

}
void CheckSpellingThread::run(){


    forever{

    mutex.lock();
    std::string correction_ = word.spellTest();
    QString res;
    if(correction_==""){
        res = QString::fromStdString(word.getWord());
    }
    else
        res = QString::fromStdString(correction_);


    emit finishedComputing(res,QString::fromStdString(word.getWord()),beg,end);

    emit Finished();
    if(!restart)
       condition.wait(&mutex);
    restart = false;
    mutex.unlock();
    }

}
