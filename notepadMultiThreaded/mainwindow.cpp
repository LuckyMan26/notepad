
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    editToolBar = new QToolBar();
    editToolBar = addToolBar(tr("ToolBar"));
    statusBar =  new QStatusBar;
    setStatusBar(statusBar);
    btnSave = new QPushButton();
    btnAdd = new QPushButton();
    btnRedo = new QPushButton();
    btnUndo = new QPushButton();
    btnSave->setIcon(QIcon(":/images/save.png"));
    btnAdd->setIcon(QIcon(":/images/add.png"));
    btnRedo->setIcon(QIcon(":/images/redo.jpg"));
    btnUndo->setIcon(QIcon(":/images/undo.jpg"));
    connect(btnSave, SIGNAL(clicked()),this, SLOT(insertData()));
    connect(btnAdd, SIGNAL(clicked()),this, SLOT(addData()));

    editToolBar->addWidget(btnSave);
    editToolBar->addWidget(btnAdd);
    editToolBar->addWidget(btnRedo);
    editToolBar->addWidget(btnUndo);

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}




