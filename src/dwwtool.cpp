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

vector<path> DWWTool::iterDir(string dir){

    vector<path> dirList;

    try{
        for(directory_entry& folder : directory_iterator(dir)){

            //skip hidden dirs
            if(
                folder.path().filename() == ".coop" ||
                folder.path().filename() == ".dm" || 
                folder.path().filename() == ".doesntwork" ||
                folder.path().filename() == ".unrecorded" ||
                //skip loose wads
                boost::iequals(folder.path().extension().string(), ".wad")
            )
            {
                continue;
            }
            dirList.push_back(folder.path());
        }
    }
    catch(exception& e){
        cerr << "Error: " << e.what() << '\n';
    }
    return dirList;
}