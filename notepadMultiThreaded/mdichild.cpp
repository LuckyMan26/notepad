#include <QtWidgets>
#include <iostream>
#include "QtConcurrent/qtconcurrentrun.h"
#include "mdichild.h"
#include "dictionary.h"
#include "word.h"
#include <QToolTip>
#include <algorithm>
#include <QTextFragment>

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
    std::cout<<"Correct Spelling: " << word.toStdString()<< " " << correctSpelling << std::endl;
    if(correctSpelling==""){
        std::cout<<"pizda\n";
        QTextEdit::contextMenuEvent(event);
        return;
    }
    else{
    QMenu* menu = this->createStandardContextMenu();
    QAction* actionCorrectWord = new QAction(QString::fromStdString(correctSpelling),this);
    menu->addAction(actionCorrectWord);
    connect(actionCorrectWord,&QAction::triggered,this,[this,&correctSpelling,&wordStd,&beg]{ CorrectWord(correctSpelling,wordStd,beg); });
    menu->popup(viewport()->mapToGlobal(pos()));
    menu->exec(event->globalPos());
    }


}
void MdiChild::CorrectWord(std::string correctSpelling,std::string wrongSpelling,int beg){


    QString text = toPlainText();
    std::string textStd = text.toStdString();

    QTextCharFormat underlineFormat;
    underlineFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
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

    //end = textStd.size();
    //setFocus();
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
    std::string textStd = text.toStdString();


    int index = text.size()-1;
    int lastIndex = text.lastIndexOf(' ');
    end = lastIndex;
    QString lastWord = text.mid(beg,end-beg+1);
    QTextCharFormat underlineFormat;
    std::string correction;
    std::string lastWordStr = lastWord.toLower().toStdString();
    lastWordStr.erase(remove(lastWordStr.begin(), lastWordStr.end(), ' '), lastWordStr.end());



    Word w(lastWordStr);
    CheckSpellingThread *workerThread = new CheckSpellingThread(vecOfCorrectWords,w,beg,end);
    beg = end+1;
    connect(workerThread, &CheckSpellingThread::finishedComputing, this, &MdiChild::updateText);
    connect(workerThread, &CheckSpellingThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
    prevText = text.toStdString();
    //else{
    /*underlineFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    QTextCursor cursor = textCursor();
    cursor.setPosition((lastWordPos>0 ? lastWordPos+1:0));
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, index - (lastWordPos>0 ? lastWordPos+1:0));
    cursor.mergeCharFormat(underlineFormat);
    setTextCursor(cursor);
    lastWordPos = index;*/

    //return lastWord.toStdString();
    // }
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
