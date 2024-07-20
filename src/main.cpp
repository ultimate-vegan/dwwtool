#include "mainwin.h"
#include "dwwtool.h"

//qt widgets
#include <QApplication>

DWWTool dwwtool;

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    MainWin *win = new MainWin;
    dwwtool.getCfg(win);
    win -> initUI();
    win -> show();

    app.exec();

}
