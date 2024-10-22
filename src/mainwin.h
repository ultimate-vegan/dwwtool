//qt widgets
#include <QMainWindow>
#include <QDialog>
#include <QTableWidgetItem>
#include <QStackedWidget>
#include <QProcess>

#include "globals.h"


#ifndef MAINWIN
#define MAINWIN

using namespace boost::filesystem;
using namespace std;

class MainWin : public QMainWindow{

    Q_OBJECT

    public:
        MainWin();
        //to be called on startup only
        void initUI();
        //sets up table with directory listing
        void initTable(string dir);
        void enterDir(QTableWidgetItem *item);
        void goBack();
        //display information about selected file
        void showItem(QTableWidgetItem *item);
        //clear the layout of a widget, but don't delete it
        void clrLayout(QLayout *layout);
        //save config values on close
        void closeEvent(QCloseEvent *event);
        void openExternal(QString filePath);
        void openCfgMenu();
        QProcess *extApp = new QProcess;
    
    private:
        QString errDir;
        string currDir;
        string lastDir;
        QWidget *mainWidget;

};

#endif
