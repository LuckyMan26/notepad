
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QToolBar* editToolBar ;
    QStatusBar* statusBar;
    QPushButton* btnSave;
    QPushButton* btnAdd;
    QPushButton* btnRedo;
    QPushButton* btnUndo;


    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
