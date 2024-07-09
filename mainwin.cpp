#include "mainwin.h"
#include "dwwtool.h"
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

//qt widgets
#include <QMainWindow>
#include <QTableWidget>
#include <QString>
#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFileDialog>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

using namespace std;
using namespace boost::filesystem;

MainWin::MainWin(){

    QGridLayout *layout = new QGridLayout;
    QWidget *win = new QWidget();
    win -> setLayout(layout);
    setCentralWidget(win);

    //your monitor should hopefully be able to handle this
    setMinimumSize(1280, 720);

}

void MainWin::initUI(){

    QTableWidget *table = new QTableWidget;
    table -> setColumnCount(1);
    table -> setRowCount(0);
    QStringList colLabels = {"File Path"};
    table -> setHorizontalHeaderLabels(colLabels);

    int seq = 0;
    for(vector<path>::iterator it = wadpath_content.begin(); it != wadpath_content.end(); ++it){

        QTableWidgetItem *path = new QTableWidgetItem;
        QString text = QString::fromStdString(it -> string());

        path -> setFlags(path -> flags().setFlag(Qt::ItemIsEditable, false));
        path -> setText(text);

        table -> insertRow(seq);
        table -> setItem(seq, 0, path);
        seq++;
    }

    QObject::connect(table, &QTableWidget::itemDoubleClicked, this, &MainWin::enterDir);

    table -> sortItems(0);
    table -> horizontalHeader() -> setDefaultSectionSize(400);
    this -> centralWidget() -> layout() -> addWidget(table);

}

void MainWin::enterDir(QTableWidgetItem *item){
                if(item){
                //create new subwindow, add elements to it
                SubWin *newWin = new SubWin;
                QTableWidget *newTable = new QTableWidget;
                newTable -> setColumnCount(1);
                QGridLayout *layout = new QGridLayout;
                QStringList colLabels = {"Filename"};
                newTable -> setHorizontalHeaderLabels(colLabels);
                string path = item -> text().toStdString();
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
                        content -> setFlags(content -> flags().setFlag(Qt::ItemIsEditable, false));
                        content -> setText(text);
                        newTable -> insertRow(seq);
                        newTable -> setItem(seq, 0, content);
                        seq++;
                    }
                }
                
                newTable -> sortItems(0);
                newTable -> horizontalHeader() -> setDefaultSectionSize(200);
                layout -> addWidget(newTable);
                QPushButton *backbtn = new QPushButton("Back");
                QObject::connect(backbtn, &QPushButton::clicked, newWin, &SubWin::goBack);
                layout -> addWidget(backbtn);
                newWin -> setLayout(layout);
                newWin -> show();
            }
}


void MainWin::cfgError(){

    if(!exists("../cfg/config.cfg")){
        ofstream("../cfg/config.cfg");
    }

    if(!(errorDialog -> isVisible())){
        QGridLayout *layout = new QGridLayout;
        QLabel *popupText = new QLabel("Your WAD path is not set. Please set it now.");


        QPushButton *browseBtn = new QPushButton("Browse...");
        QObject::connect(browseBtn, &QPushButton::clicked, this, &MainWin::openDirDialog);
        QPushButton *acceptBtn = new QPushButton("OK");
        QObject::connect(acceptBtn, &QPushButton::clicked, errorDialog, &QDialog::accepted);
        QObject::connect(errorDialog, &QDialog::accepted, this, &MainWin::updateCfg);
        QObject::connect(errorDialog, &QDialog::accepted, errorDialog, &QDialog::hide);

        layout -> addWidget(popupText);
        layout -> addWidget(browseBtn);
        layout -> addWidget(acceptBtn);

        errorDialog -> setLayout(layout);
        errorDialog -> open();
    }

    else{

        //todo: make the layout not suck because of this workaround to a crash
        //also make it possible to type a directory in instead of using the browser
        QLabel *textBox = new QLabel(dir);
        errorDialog -> layout() -> addWidget(textBox);
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
    initUI();

}