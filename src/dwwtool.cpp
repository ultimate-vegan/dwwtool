#include "dwwtool.h"
#include "mainwin.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

//boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

//qt widgets
#include <QMainWindow>

using namespace std;
using namespace boost::filesystem;

string wadpath{};
vector<path> wadpath_content{};

void DWWTool::getCfg(MainWin *win){

    boost::property_tree::ptree pt;

    try{
        //make sure you're building from dwwtool/build or else this won't work
        boost::property_tree::ini_parser::read_ini("../cfg/config.cfg", pt);
        
        try{wadpath = pt.get<string>("paths.wadpath");}
        catch(const exception& e){
            win -> cfgError();
        }
    }
    catch (const exception& e){
        cerr << "Error: " << e.what() << "\n";
        win -> cfgError();
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