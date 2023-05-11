#include <QtWidgets>
#include <iostream>
#include "QtConcurrent/qtconcurrentrun.h"
#include "mdichild.h"
#include "dictionary.h"
#include "word.h"
#include <QToolTip>
#include <algorithm>
#include <QTextFragment>
std::vector<std::pair<std::string, size_t>> breakTextIntoWords(const std::string& text) {
    std::vector<std::pair<std::string, size_t>> words;
    std::string word;
    size_t wordStartIndex = 0;

    for (size_t i = 0; i < text.length(); ++i) {
        char currentChar = text[i];

        // Check if the current character is a delimiter (space, punctuation, etc.)
        if (std::isspace(currentChar) || std::ispunct(currentChar)) {
            if (!word.empty()) {
                words.emplace_back(word, wordStartIndex);
                word.clear();
            }
        } else {
            // If it's not a delimiter, add the character to the current word
            if (word.empty()) {
                wordStartIndex = i;  // Start of a new word
            }
            word += currentChar;
        }
    }

    // Handle the last word if the text doesn't end with a delimiter
    if (!word.empty()) {
        words.emplace_back(word, wordStartIndex);
    }

    return words;
}

MdiChild::MdiChild()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    this->setFontPointSize(11);
    this->setFontFamily("Consolas");
    setAcceptDrops(false);
    curPos=0;
    beg = 0;
    end = 0;
    lastWordPos=0;
    prevText = "";
    connect(this, &MdiChild::spacePressed, this, &MdiChild::checkSpellingOfTheWord,Qt::QueuedConnection);

}

void MdiChild::contextMenuEvent(QContextMenuEvent *event)
{



    QTextCursor textCursor = cursorForPosition(event->pos());
    textCursor.select(QTextCursor::WordUnderCursor);
    setTextCursor(textCursor);
    int temp = textCursor.selectionEnd();

    QString word = textCursor.selectedText();
    int beg = temp - word.length();
    std::string wordStd = word.toStdString();
    QString text = toPlainText();
    std::string correction;
    word = word.toLower();
    std::string correctSpelling = map[word.toStdString()];
    if(correctSpelling==""){

        QTextEdit::contextMenuEvent(event);
        return;
    }
    else{
    QMenu* menu = this->createStandardContextMenu();
    QAction* actionCorrectWord = new QAction(QString::fromStdString(correctSpelling),this);
    menu->addAction(actionCorrectWord);
    menu->insertSeparator(actionCorrectWord);
    connect(actionCorrectWord,&QAction::triggered,this,[this,&correctSpelling,&wordStd,&beg]{ CorrectWord(correctSpelling,wordStd,beg); });
    menu->popup(viewport()->mapToGlobal(pos()));
    menu->exec(event->globalPos());
    }


}
void MdiChild::CorrectWord(std::string correctSpelling,std::string wrongSpelling,int beg){


    QString text = toPlainText();
    std::string textStd = text.toStdString();

    QTextCursor cursor = textCursor();
    cursor.setPosition(beg);
    textCursor().beginEditBlock();
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, wrongSpelling.length());
    cursor.removeSelectedText();

    QString html = "<span>" + QString::fromStdString(correctSpelling) + "</span>";
    cursor.insertHtml(html);
    cursor.setPosition(QTextCursor::End);
    textCursor().endEditBlock();
    setTextCursor(cursor);
    text = toPlainText();



}
void MdiChild::updateText(QString correction,QString word,int beg_,int end_){
    QString word_ = word.toLower();

    QString correction_ = correction.toLower();
    if(word_!=correction_){
    QTextCursor cursor = textCursor();
    QTextCharFormat underlineFormat;
    underlineFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);

    cursor.setPosition(beg_);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end_ - beg_);
    cursor.mergeCharFormat(underlineFormat);
    setTextCursor(cursor);
    underlineFormat.setUnderlineColor(QTextCharFormat::NoUnderline);
    QTextCursor cursor_ = textCursor();
    cursor_.movePosition(QTextCursor::End);
    setTextCursor(cursor_);
    map[word_.toStdString()]=correction_.toStdString();
    }

    update();
}
void MdiChild::keyPressEvent(QKeyEvent *event) {
    curPos++;
    QTextEdit::keyPressEvent(event);
    if (event->key() == Qt::Key_Space) {
        emit spacePressed();
    }


}
void MdiChild::newFile()
{
    static int sequenceNumber = 1;


    isUntitled = true;
    curFile = tr("untitled%1").arg(sequenceNumber++); //curFile = tr("untitled%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QT Notepad"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);

    return true;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    //QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QT Notepad"),
                             tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool MdiChild::maybeSave()
{
    if (!document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("QT Notepad"),
                               tr("%1 has been modified.\n"
                                  "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                               QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
std::string MdiChild::checkSpellingOfTheWord(){


    QString text = this->toPlainText();
    text = text.toLower();
    std::string textStd = text.toStdString();


    end = text.size()-1;
    int lastIndex = text.lastIndexOf(' ');

    QString lastWord = text.mid(beg,end-beg+1);
    QTextCharFormat underlineFormat;
    std::string correction;
    std::string lastWordStr = lastWord.toLower().toStdString();
    lastWordStr.erase(remove(lastWordStr.begin(), lastWordStr.end(), ' '), lastWordStr.end());

    std::vector<std::pair<std::string,size_t>> wordsOfText = breakTextIntoWords(textStd);
    int counter=0;
        for(auto it : wordsOfText){

            if(!map.contains(it.first)){
                Word w(it.first);

                CheckSpellingThread *workerThread = new CheckSpellingThread(vecOfCorrectWords,w,it.second,it.second+it.first.length());

                QThreadPool::globalInstance()->start(workerThread);
                connect(workerThread, &CheckSpellingThread::finishedComputing, this, &MdiChild::updateText);
            }
            else{
                updateText(QString::fromStdString(map[it.first]),QString::fromStdString(it.first),it.second,it.second+it.first.length());
            }
    }

    beg = end+1;
    return "";
}
void MdiChild::SelectedWord(QString& str){

}
void MdiChild::correctMistakes(){


}
void MdiChild::mousePressEvent(QMouseEvent * event)
{

    QTextEdit::mousePressEvent(event);
}
