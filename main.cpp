#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <any>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "libs/dsda-doom/prboom2/src/doomdata.h"

using namespace std;
using namespace boost::filesystem;


string wadpath = "/home/vegan/games/games_ssd/wads/doom/";
vector<path> wadpath_content;

void iterDir(){

    vector<path> contents;

    for(directory_entry& folder : directory_iterator(wadpath)){

        //skip hidden dirs
        if(
        folder.path().filename() == ".coop" ||
        folder.path().filename() == ".dm" || 
        folder.path().filename() == ".doesntwork" ||
        folder.path().filename() == ".unrecorded" ||
        //skip loose wads
        boost::iequals(folder.path().extension().string(), ".wad")
        ){
            continue;
        }

        wadpath_content.push_back(folder.path());
        cout << folder << '\n';

        //get contents of subdirs
        /*for(vector<directory_entry>::iterator it = folders.begin(); it != folders.end(); ++it){
            for(directory_entry& content : directory_iterator(it -> path())){
                cout << content << '\n';
            }
        }*/

        //skip text files generated from video output
        if(
        folder.path().filename() == "sound_stderr.txt" ||
        folder.path().filename() == "mux_stderr.txt" ||
        folder.path().filename() == "mux_stdout.txt" ||
        folder.path().filename() == "sound_stdout.txt" ||
        folder.path().filename() == "video_stderr.txt" ||
        folder.path().filename() == "video_stdout.txt"
        ){
            continue;
        }

        //cout << content << '\n';
        contents.push_back(folder.path());
    }
}

int main(){

    iterDir();

}