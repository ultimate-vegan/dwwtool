#include "mainwin.h"
#include "dwwtool.h"
#include <iostream>

//qt widgets
#include <QMainWindow>
#include <QTableWidget>
#include <QString>
#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

using namespace std;
using namespace boost::filesystem;

MainWin::MainWin(){
    QGridLayout *layout = new QGridLayout;

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

    QWidget *win = new QWidget();

    layout -> addWidget(table);
    win -> setLayout(layout);
    setCentralWidget(win);

    //your monitor should hopefully be able to handle this
    setMinimumSize(1280, 720);
}