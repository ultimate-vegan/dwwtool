#include "mainwin.h"
#include "dwwtool.h"
#include <iostream>
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

    //get user's screen dimensions
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect res = screen->geometry();
    int scrw = res.width();
    int scrh = res.height();

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

    //todo: replace these with some QSettings to remember window size and position
    setMinimumSize(scrw/2, scrh/2);
    move(res.center() - this->rect().center());

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