#include "mainwin.h"
#include "dwwtool.h"

//qt widgets
#include <QApplication>

int main(int argc, char* argv[]){

    DWWTool dwwtool;
    QApplication app(argc, argv);
    MainWin *win = new MainWin;
    dwwtool.getCfg(win);
    dwwtool.iterDir();
    win -> initTable();
    win -> show();
    app.exec();

}