#include <iostream>
#include "subwin.h"

//boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

//qt widgets
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QString>
#include <QDialog>


#ifndef MAINWIN
#define MAINWIN

using namespace boost::filesystem;
using namespace std;

class MainWin : public QMainWindow{

    Q_OBJECT

    public:
        MainWin();
        void initUI();
        void enterDir(QTableWidgetItem *item);
        void cfgError();
        void openDirDialog();
        void updateCfg();
        QDialog *errorDialog = new QDialog;
    
    private:
        QString dir;

};

#endif