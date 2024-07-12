//boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

//qt widgets
#include <QMainWindow>
#include <QDialog>
#include <QTableWidgetItem>
#include <QStackedWidget>
#include <QProcess>


#ifndef MAINWIN
#define MAINWIN

using namespace boost::filesystem;
using namespace std;

class MainWin : public QMainWindow{

    Q_OBJECT

    public:
        MainWin();
        //sets up initial table with directory listing
        void initTable(vector<path> dirList);
        void enterDir(QTableWidgetItem *item);
        //handle config errors
        void cfgError();
        void openDirDialog();
        //update config file
        void updateCfg();
        void goBack();
        //display information about selected file
        void showItem(QTableWidgetItem *item);
        //clear the layout of a widget, but don't delete it
        void clrLayout(QLayout *layout);
        //save config values on close
        void closeEvent(QCloseEvent *event);
        void openExternal(QString filePath);
        void openCfgMenu();
        QDialog *errDialog = new QDialog;
        QProcess *extApp = new QProcess;
    
    private:
        QString dir;
        string subDir;
        QStackedWidget *mainWidget;

};

#endif