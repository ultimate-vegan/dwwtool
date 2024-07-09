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

void DWWTool::iterDir(){

    //vector<path> contents;

    try{
    for(directory_entry& folder : directory_iterator(wadpath)){

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
        wadpath_content.push_back(folder.path());
    }
    }
    catch(exception& e){
        cerr << "Error: " << e.what() << '\n';
    }
        //cout << folder << '\n';

        //get contents of subdirs
        /*for(vector<directory_entry>::iterator it = folders.begin(); it != folders.end(); ++it){
            for(directory_entry& content : directory_iterator(it -> path())){
                cout << content << '\n';
            }
        }*/

        //skip text files generated from video output
        /*if(
        folder.path().filename() == "sound_stderr.txt" ||
        folder.path().filename() == "mux_stderr.txt" ||
        folder.path().filename() == "mux_stdout.txt" ||
        folder.path().filename() == "sound_stdout.txt" ||
        folder.path().filename() == "video_stderr.txt" ||
        folder.path().filename() == "video_stdout.txt"
        ){
            continue;
        }*/

        //cout << content << '\n';
        //contents.push_back(folder.path());
}