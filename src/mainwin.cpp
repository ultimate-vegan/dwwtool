#include "mainwin.h"
#include "dwwtool.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <any>

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

using namespace std;
using namespace boost::filesystem;

MainWin::MainWin()

    : mainWidget(new QStackedWidget(this)),
      errDialog(new QDialog(this))
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

    QGridLayout *baseLayout = new QGridLayout;
    QGridLayout *subLayout = new QGridLayout;

    //basewidget is for the main directory listing (mainWidget->widget(0))
    QWidget *baseWidget = new QWidget();
    //subwidget is for subfolders (mainWidget->widget(1))
    QWidget *subWidget = new QWidget();
    baseWidget->setLayout(baseLayout);
    subWidget->setLayout(subLayout);
    mainWidget->addWidget(baseWidget);
    mainWidget->addWidget(subWidget);

    setCentralWidget(mainWidget);

    setMinimumSize(scrw/2, scrh/2);

    //setup error dialog widgets
    QGridLayout *errLayout = new QGridLayout;
    QLabel *popupText = new QLabel("Your WAD path is not set. Please set it now.");

    QPushButton *browseBtn = new QPushButton("Browse...");
    QObject::connect(browseBtn, &QPushButton::clicked, this, &MainWin::openDirDialog);
    QPushButton *acceptBtn = new QPushButton("OK");
    QObject::connect(acceptBtn, &QPushButton::clicked, errDialog, &QDialog::accept);
    QObject::connect(errDialog, &QDialog::accepted, this, &MainWin::updateCfg);

    QLineEdit *textBox = new QLineEdit();
    textBox->setObjectName("textbox");

    errLayout->addWidget(popupText);
    errLayout->addWidget(textBox);
    errLayout->addWidget(browseBtn);
    errLayout->addWidget(acceptBtn);

    errDialog->setLayout(errLayout);

}

void MainWin::initTable(vector<path> dirList){

    QTableWidget *table = new QTableWidget;
    table->setColumnCount(1);
    table->setRowCount(0);
    QStringList colLabels = {"File Path"};
    table->setHorizontalHeaderLabels(colLabels);

    int seq = 0;
    for(vector<path>::iterator it = dirList.begin(); it != dirList.end(); ++it){

        QTableWidgetItem *path = new QTableWidgetItem;
        QString text = QString::fromStdString(it->string());

        path->setFlags(path->flags().setFlag(Qt::ItemIsEditable, false));
        path->setText(text);

        table->insertRow(seq);
        table->setItem(seq, 0, path);
        seq++;
    }

    QObject::connect(table, &QTableWidget::itemDoubleClicked, this, &MainWin::enterDir);

    table->sortItems(0);
    table->horizontalHeader()->setDefaultSectionSize(400);
    mainWidget->widget(0)->layout()->addWidget(table);
    mainWidget->widget(0)->show();

}

void MainWin::enterDir(QTableWidgetItem *item){

    QTableWidget *newTable = new QTableWidget;
    newTable->setColumnCount(1);
    QStringList colLabels = {"Filename"};
    newTable->setHorizontalHeaderLabels(colLabels);
    string dirPath = item->text().toStdString();
    subDir = dirPath;
    int seq = 0;
    for(directory_entry& file : directory_iterator(dirPath)){
            if(
                //don't show files generated from video output
                file.path().filename() == "sound_stderr.txt" ||
                file.path().filename() == "mux_stderr.txt" ||
                file.path().filename() == "mux_stdout.txt" ||
                file.path().filename() == "sound_stdout.txt" ||
                file.path().filename() == "video_stderr.txt" ||
                file.path().filename() == "video_stdout.txt"
            ){
                continue;
            }
            else{
                QTableWidgetItem *content = new QTableWidgetItem;
                QString text = QString::fromStdString(file.path().filename().string());
                content->setFlags(content->flags().setFlag(Qt::ItemIsEditable, false));
                content->setText(text);
                newTable->insertRow(seq);
                newTable->setItem(seq, 0, content);
                seq++;
                }
            }
                
    newTable->sortItems(0);
    newTable->horizontalHeader()->setDefaultSectionSize(200);
    QPushButton *backbtn = new QPushButton("Back");
    QObject::connect(backbtn, &QPushButton::clicked, this, &MainWin::goBack);
    QObject::connect(newTable, &QTableWidget::itemClicked, this, &MainWin::showItem);

    //new container widget for easy clearing of display elements created by showItem
    QWidget *container = new QWidget;
    container->setObjectName("container");
    QGridLayout *contLayout = new QGridLayout;
    container->setLayout(contLayout);

    mainWidget->widget(1)->layout()->addWidget(newTable);
    mainWidget->widget(1)->layout()->addWidget(container);
    mainWidget->widget(1)->layout()->addWidget(backbtn);
    mainWidget->setCurrentWidget(mainWidget->widget(1));

}


void MainWin::cfgError(){

    if(!(errDialog->isVisible())){
        errDialog->open();
    }

    else{
        QLineEdit *textbox = errDialog->findChild<QLineEdit*>("textbox");
        if(textbox){
            textbox->setText(dir);
        }
        else{
            cout<<"Textbox not found"<<'\n';
        }
    }

}

void MainWin::openDirDialog(){

    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
    cfgError();

}

void MainWin::updateCfg(){

    QLineEdit *textbox = errDialog->findChild<QLineEdit*>("textbox");

    if(!textbox->text().isEmpty()){
        dir = textbox -> text();
        dwwtool.settings.setValue("paths/wadpath", dir);

        wadpath = dir.toStdString();
        wadpath_content = dwwtool.iterDir(dir.toStdString());
        clrLayout(mainWidget->widget(0)->layout());
        initTable(wadpath_content);
    }
    else{
        QDialog *popup = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *text = new QLabel("Please set a path.");
        QPushButton *btn = new QPushButton("OK");
        QObject::connect(btn, &QPushButton::clicked, popup, &QDialog::accept);
        layout->addWidget(text);
        layout->addWidget(btn);
        popup->setLayout(layout);
        popup->exec();
        cfgError();
    }

}

void MainWin::clrLayout(QLayout *layout){
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != nullptr){
        if(item->widget()){
            delete item->widget();
        }
        delete item;
    }
}

void MainWin::goBack(){

    //clear subwidget layout and return to main directory
    clrLayout(mainWidget->widget(1)->layout());
    mainWidget->setCurrentWidget(mainWidget->widget(0));

}

void MainWin::showItem(QTableWidgetItem *item){

    QWidget *subWidget = mainWidget->widget(1);
    clrLayout(subWidget->findChild<QWidget*>("container")->layout());
    QWidget *container = subWidget->findChild<QWidget*>("container");
    string ext = path(item->text().toStdString()).extension().string();
    string filePath = subDir + "/" + item->text().toStdString();

    if(
        //wad makers in the 90s used stupid text file extensions
        boost::iequals(ext, ".txt")||
        boost::iequals(ext, ".nfo")||
        boost::iequals(ext, ".1st")||
        boost::iequals(ext, ".add")||
        boost::iequals(ext, ".me")||
        boost::iequals(ext, ".diz")||
        boost::iequals(ext, ".ion")||
        boost::iequals(ext, ".btm")||
        boost::iequals(ext, ".mnu")||
        boost::iequals(ext, ".hi")||
        boost::iequals(ext, ".doc")||
        //todo: maybe add functionality to run .bat files in dosbox?
        boost::iequals(ext, ".bat")
    ){

        QTextBrowser *txtDisplay = new QTextBrowser;

        ifstream file(filePath);
        stringstream buff;
        buff<<file.rdbuf();
        string text = buff.str();

        txtDisplay->setText(QString::fromStdString(text));
        container->layout()->addWidget(txtDisplay);

    }
    else if(
        boost::iequals(ext, ".gif")||
        boost::iequals(ext, ".png")||
        boost::iequals(ext, ".jpg")||
        boost::iequals(ext, ".jpeg")||
        boost::iequals(ext, ".bmp")
    ){
        QImage img(QString::fromStdString(filePath));
        QLabel *imgDisplay = new QLabel;
        imgDisplay->setPixmap(QPixmap::fromImage(img));
        container->layout()->addWidget(imgDisplay);
        container->layout()->setAlignment(imgDisplay, Qt::AlignCenter);
    }

    else if(boost::iequals(ext, ".wad")){
        //wad file handling goes here
    }

    else if(boost::iequals(ext, ".deh")){
        //deh file handling goes here
    }
}

void MainWin::closeEvent(QCloseEvent *event){
    QPoint winPos = this->pos();
    dwwtool.settings.setValue("window/position", winPos);
    QSize winSize = this->size();
    dwwtool.settings.setValue("window/size", winSize);
    QMainWindow::closeEvent(event);
}