#include <QtWidgets>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "subwin.h"

#ifndef MAINWIN
#define MAINWIN

using namespace boost::filesystem;
using namespace std;

class MainWin : public QMainWindow{

    Q_OBJECT

    public:
        MainWin();
    
    void enterDir(QTableWidgetItem *item){
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

};

#endif