#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mdichild.h"

MainWindow::MainWindow(QWidget *parent)
    : mdiArea(new QMdiArea),
    ui(new Ui::MainWindow)


{
    std::cout<<"Hello\n";
    ui->setupUi(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    std::cout<<"Hello\n";

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
    connect(btnSave, SIGNAL(clicked()),this, SLOT(newFile()));
    connect(btnAdd, SIGNAL(clicked()),this, SLOT(addData()));

    editToolBar->addWidget(btnSave);
    editToolBar->addWidget(btnAdd);
    editToolBar->addWidget(btnRedo);
    editToolBar->addWidget(btnUndo);
    std::cout<<"Hello\n";


    std::cout<<"Hello\n";

    newFile();
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);


}
MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;

    mdiArea->addSubWindow(child);

    return child;
}
void MainWindow::newFile()
{

    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}



