#include "mainwin.h"
#include "dwwtool.h"
#include <iostream>
#include <any>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

//qt widgets
#include <QMainWindow>
#include <QTableWidget>
#include <QString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFileDialog>
#include <QLabel>
#include <QDialog>
//#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QStackedWidget>

using namespace std;
using namespace boost::filesystem;

MainWin::MainWin()

    : mainWidget(new QStackedWidget(this)),
      errorDialog(new QDialog(this))
{

    QGridLayout *baseLayout = new QGridLayout;
    QGridLayout *subLayout = new QGridLayout;

    //swap between these two to change from base dir to subdirs in the gui
    QWidget *baseWidget = new QWidget();
    QWidget *subWidget = new QWidget();
    baseWidget->setLayout(baseLayout);
    subWidget->setLayout(subLayout);
    mainWidget->addWidget(baseWidget);
    mainWidget->addWidget(subWidget);

    setCentralWidget(mainWidget);

    //your monitor should hopefully be able to handle this
    setMinimumSize(1280, 720);

    //setup error dialog widgets
    QGridLayout *errLayout = new QGridLayout;
    QLabel *popupText = new QLabel("Your WAD path is not set. Please set it now.");


    QPushButton *browseBtn = new QPushButton("Browse...");
    QObject::connect(browseBtn, &QPushButton::clicked, this, &MainWin::openDirDialog);
    QPushButton *acceptBtn = new QPushButton("OK");
    QObject::connect(acceptBtn, &QPushButton::clicked, errorDialog, &QDialog::accept);
    QObject::connect(errorDialog, &QDialog::accepted, this, &MainWin::updateCfg);
    QObject::connect(errorDialog, &QDialog::accepted, errorDialog, &QDialog::hide);
    //this isn't actually a textbox yet, it will be changed to QLineEdit eventually
    QLabel *textBox = new QLabel();
    textBox->setObjectName("textbox");


    errLayout->addWidget(popupText);
    errLayout->addWidget(textBox);
    errLayout->addWidget(browseBtn);
    errLayout->addWidget(acceptBtn);

    errorDialog->setLayout(errLayout);

}

void MainWin::initTable(){

    QTableWidget *table = new QTableWidget;
    table->setColumnCount(1);
    table->setRowCount(0);
    QStringList colLabels = {"File Path"};
    table->setHorizontalHeaderLabels(colLabels);

    int seq = 0;
    for(vector<path>::iterator it = wadpath_content.begin(); it != wadpath_content.end(); ++it){

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
                if(item){
                //create new subwindow, add elements to it
                //SubWin *newWin = new SubWin;
                QTableWidget *newTable = new QTableWidget;
                newTable->setColumnCount(1);
                QStringList colLabels = {"Filename"};
                newTable->setHorizontalHeaderLabels(colLabels);
                string path = item->text().toStdString();
                int seq = 0;
                for(directory_entry& file : directory_iterator(path)){
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
                mainWidget->widget(1)->layout()->addWidget(newTable);
                mainWidget->widget(1)->layout()->addWidget(backbtn);
                mainWidget->widget(0)->hide();
                mainWidget->widget(1)->show();
            }
}


void MainWin::cfgError(){

    if(!exists("../cfg/config.cfg")){
        ofstream("../cfg/config.cfg");
    }

    if(!(errorDialog->isVisible())){
        errorDialog->open();
    }

    else{
        //todo: make it possible to type a directory in instead of using the browser
        QLabel *textbox = errorDialog->findChild<QLabel*>("textbox");
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

    boost::property_tree::ptree pt;
    pt.add<string>("paths.wadpath", dir.toStdString());
    boost::property_tree::write_ini("../cfg/config.cfg", pt);

    //todo: this doesn't work, find a way to refresh the UI after updating the wadpath
    initTable();

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
    mainWidget->widget(1)->hide();
    mainWidget->widget(0)->show();

}

void MainWin::showItem(QTableWidgetItem *item){

    string extension = path(item->text().toStdString()).extension().string();
    QWidget *subWidget = mainWidget->widget(1);

    if(boost::iequals(extension, ".txt")){
        QTextBrowser *txtDisplay = new QTextBrowser;
        subWidget->layout()->addWidget(txtDisplay);
    }
    else if(boost::iequals(extension, ".wad")){

    }
}