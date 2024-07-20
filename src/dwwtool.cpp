#include "dwwtool.h"
#include "mainwin.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

//boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

//qt widgets
#include <QMainWindow>

using namespace std;
using namespace boost::filesystem;

string wadpath{};
vector<path> wadpath_content{};

void DWWTool::getCfg(MainWin *win){

    if(!settings.value("paths/wadpath").toString().isEmpty()){
        wadpath = settings.value("paths/wadpath").toString().toStdString();
    }
    else{
        win->cfgError();
    }

}
