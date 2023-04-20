
#ifndef MDICHILD_H
#define MDICHILD_H
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
signals:
    void spacePressed();
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void documentWasModified();
    std::string getLastWord();

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void keyPressEvent(QKeyEvent *event) override;
    QString curFile;
    bool isUntitled;

};

#endif // MDICHILD_H
