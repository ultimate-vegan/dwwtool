#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

vector<path> iterDir(string dirpath){

    vector<path> folders;

    for(directory_entry& folder : recursive_directory_iterator(dirpath)){
        cout << folder << '\n';
        folders.push_back(folder.path());
    }

    return folders;
}

string wadpath = "/home/vegan/games/games_ssd/wads/doom/";
string playedpath = "/home/vegan/games/games_ssd/wads/doom/.played";
vector<path> wadfolders = iterDir(wadpath);
vector<path> playedfolders = iterDir(playedpath);

int main(){

    for(vector<path>::iterator it = wadfolders.begin(); it != wadfolders.end(); ++it){
        //erase hidden paths
        if(it -> extension() != "" && it -> extension() != ".wad" && it -> extension() != ".WAD"){
            wadfolders.erase(it);
            continue;
        }
    }



}