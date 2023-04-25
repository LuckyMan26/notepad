#ifndef MDICHILD_H
#define MDICHILD_H
#include "checkspellingthread.h"
#include <QTextEdit>

class MdiChild : public QTextEdit
{
    Q_OBJECT
    int curPos;
    int lastWordPos;
public:
    MdiChild();

    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
    void mousePressEvent(QMouseEvent * event) override;
    void enterEvent(QMouseEvent *event);
    void SelectedWord(QString&);
    void correctMistakes();
signals:
    void spacePressed();
    void hoveredWord();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void documentWasModified();
    std::string checkSpellingOfTheWord();
    void updateText(QString,int,int);

private:
    std::vector<std::string> vecOfCorrectWords;
    QString correctText;
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void keyPressEvent(QKeyEvent *event) override;
    QString curFile;
    bool isUntitled;
    int beg;
    int end;
};

#endif // MDICHILD_H
