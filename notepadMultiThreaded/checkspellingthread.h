
#ifndef CHECKSPELLINGTHREAD_H
#define CHECKSPELLINGTHREAD_H


#include "qmutex.h"
#include "qthread.h"
#include "qwaitcondition.h"
#include "word.h"

class CheckSpellingThread : public QThread
{
    Q_OBJECT

public:
    CheckSpellingThread(Word w=Word(""),int beg_=0,int end_=0,QObject *parent = nullptr);
    ~CheckSpellingThread();
    void setWord(Word w);

signals:
    void finishedComputing(QString,QString,int,int);
    void Finished();
public slots:
    void checkSpell(Word w,int,int);
    void RunThread(Word w);
protected:
    void run() override;

private:

    int beg,end;

    Word word;
    QMutex mutex;
    QWaitCondition condition;
    bool restart = false;
    bool abort = false;
    std::string correction;
    bool m_isAwake;
};
#endif // CHECKSPELLINGTHREAD_H
