#include "mainwin.h"
#include "dwwtool.h"

//qt widgets
#include <QApplication>

DWWTool dwwtool;

int main(int argc, char* argv[]){

    QApplication app(argc, argv);
    app.setOrganizationName("DOOM WAD World");
    app.setApplicationName("DWWTool");

    MainWin *win = new MainWin;
    dwwtool.getCfg(win);
    wadpath_content = dwwtool.iterDir(wadpath);
    win -> initTable(wadpath_content);
    win -> show();
    
    app.exec();

}