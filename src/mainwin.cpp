#include "mainwin.h"
#include "dwwtool.h"
#include "cfgwin.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

//qt widgets
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QLineEdit>
#include <QHeaderView>
#include <QGuiApplication>
#include <QTextBrowser>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QGroupBox>

//DOOM libraries
#include "wadfile.h"

using namespace std;
using namespace boost::filesystem;

MainWin::MainWin()

    : mainWidget(new QWidget(this)),
      extApp(new QProcess(this))
{

    setWindowTitle("DOOM WAD World Tool");

    //get user's screen dimensions
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect res = screen->geometry();
    int scrw = res.width();
    int scrh = res.height();


    //resize according to config file
    if(!dwwtool.settings.value("window/size").toSize().isNull()){
        resize(dwwtool.settings.value("window/size").toSize());
    }

    //start in center of screen if position isn't saved
    if(dwwtool.settings.value("window/position").toPoint().isNull()){
        move(res.center() - this->rect().center());
    }
    else{
        move(dwwtool.settings.value("window/position").toPoint());
    }

    QMenuBar *menuBar = new QMenuBar;

    //todo:add functionality for opening individual files/new directories through this menu
    QMenu *fileMenu = new QMenu("File");
    QAction *openDir = new QAction("Open Directory");
    QAction *openFile = new QAction("Open File");
    fileMenu->addActions(QList{openDir, openFile});

    QMenu *editMenu = new QMenu("Edit");
    QAction *prefs = new QAction("Preferences");
    editMenu->addAction(prefs);
    QObject::connect(prefs, &QAction::triggered, this, &MainWin::openCfgMenu);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);

    setMenuBar(menuBar);

    QGridLayout *layout = new QGridLayout;
    mainWidget->setLayout(layout);

    setCentralWidget(mainWidget);

    setMinimumSize(scrw/2, scrh/2);

}

void MainWin::initUI(){
    if(!dwwtool.settings.value("paths/wadpath").toString().isEmpty()){
        initTable(dwwtool.settings.value("paths/wadpath").toString().toStdString());
    }
    else{
        //will have to be changed for windows
        initTable("/home");
    }
}

void MainWin::initTable(string dir){

    clrLayout(mainWidget->layout());
    currDir = dir;

    vector<path> dirList;
    for(directory_entry& folder : directory_iterator(dir)){
        dirList.push_back(folder.path());
    }

    QTableWidget *table = new QTableWidget;
    table->setColumnCount(1);
    table->setRowCount(0);
    QStringList colLabels = {"Filename"};
    table->setHorizontalHeaderLabels(colLabels);

    int seq = 0;
    for(vector<path>::iterator it = dirList.begin(); it != dirList.end(); ++it){

        QTableWidgetItem *fName = new QTableWidgetItem;
        QString text = QString::fromStdString(it->filename().string());

        fName->setFlags(fName->flags().setFlag(Qt::ItemIsEditable, false));
        fName->setText(text);

        table->insertRow(seq);
        table->setItem(seq, 0, fName);
        seq++;
    }

    QObject::connect(table, &QTableWidget::itemDoubleClicked, this, &MainWin::enterDir);


    QPushButton *backbtn = new QPushButton("Back");
    QObject::connect(backbtn, &QPushButton::clicked, this, &MainWin::goBack);
    QObject::connect(table, &QTableWidget::itemClicked, this, &MainWin::showItem);

    //container widget for easy clearing of display elements created by showItem
    QWidget *container = new QWidget;
    container->setObjectName("container");
    QGridLayout *contLayout = new QGridLayout;
    container->setLayout(contLayout);

    table->sortItems(0);
    table->horizontalHeader()->setDefaultSectionSize(400);
    mainWidget->layout()->addWidget(table);
    mainWidget->layout()->addWidget(container);
    container->hide();
    mainWidget->layout()->addWidget(backbtn);
    mainWidget->show();

}

void MainWin::goBack(){

    if(is_directory(lastDir)){
        initTable(lastDir);
    }
    else{
        return;
    }

}

void MainWin::enterDir(QTableWidgetItem *item){

    QString dirPath = QString::fromStdString(currDir) + '/' + item->text();
    string strPath = dirPath.toStdString();
    if(is_directory(strPath)){
        lastDir = currDir;
        currDir = strPath;
        initTable(strPath);
    }
    else{
        return;
    }

}

void MainWin::showItem(QTableWidgetItem *item){


    string filePath = currDir + "/" + item->text().toStdString();
    QWidget *container = mainWidget->findChild<QWidget*>("container");
    cout<<filePath<<'\n';
    //don't show directories
    if(is_directory(filePath)){
        container->hide();
        return;
    }

    clrLayout(mainWidget->findChild<QWidget*>("container")->layout());
    container->show();
    string ext = path(item->text().toStdString()).extension().string();
    QString qfp = QString::fromStdString(filePath);

    //wad makers in the 90s used stupid text file extensions
    const regex txtPattern("^\\.(txt|nfo|1st|add|me|diz|ion|btm|mnu|hi|doc|bat|it|bbs)$", regex_constants::icase);
    const regex imgPattern("^\\.(gif|png|jpg|jpeg|bmp)$", regex_constants::icase);
    const regex wadPattern("^\\.(wad|pwd)$", regex_constants::icase);
    smatch match;

    if(regex_match(ext, match, txtPattern)){

        QTextBrowser *txtDisplay = new QTextBrowser;

        ifstream file(filePath);
        stringstream buff;
        buff<<file.rdbuf();
        string text = buff.str();

        QPushButton *extBtn = new QPushButton("Open in External App");
        QObject::connect(extBtn, &QPushButton::clicked, [this,qfp]{ openExternal(qfp); });

        txtDisplay->setText(QString::fromStdString(text));
        container->layout()->addWidget(extBtn);
        container->layout()->addWidget(txtDisplay);

    }
    else if(regex_match(ext, match, imgPattern)){
        QImage img(QString::fromStdString(filePath));
        QLabel *imgDisplay = new QLabel;
        imgDisplay->setPixmap(QPixmap::fromImage(img));
        container->layout()->addWidget(imgDisplay);
        container->layout()->setAlignment(imgDisplay, Qt::AlignCenter);
    }

    else if(regex_match(ext, match, wadPattern)){

        WadFile wad(filePath);
        QLabel *typeLabel = new QLabel(QString::fromStdString(wad.headers.wadtype));
        container->layout()->addWidget(typeLabel);
    }

    /*else if(boost::iequals(ext, ".deh")){
        //deh file handling goes here
    }
    else if(boost::iequals(ext, ".lmp")){
        //lmp file handling goes here
    }*/

    else{
        container->hide();
    }
}

void MainWin::clrLayout(QLayout *layout){
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != nullptr){
        if(QWidget *widget = item->widget()){
            widget->deleteLater();
        }
        delete item;
    }
}

void MainWin::closeEvent(QCloseEvent *event){
    QPoint winPos = this->pos();
    dwwtool.settings.setValue("window/position", winPos);
    QSize winSize = this->size();
    dwwtool.settings.setValue("window/size", winSize);
    // currently unused
    // dwwtool.settings.setValue("paths/exitdir", QString::fromStdString(currDir));
    QMainWindow::closeEvent(event);
}

//todo:detect user OS and run a command that actually works on windows
void MainWin::openExternal(QString filePath){
    extApp->start("xdg-open", QStringList()<<filePath);
}

void MainWin::openCfgMenu(){

    CfgWin *cfgWin = new CfgWin();
    cfgWin->setMinimumSize(this->size()/2);
    cfgWin->show();

}
