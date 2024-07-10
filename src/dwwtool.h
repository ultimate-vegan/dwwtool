#include "mainwin.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <QMainWindow>

#ifndef DWWTOOL
#define DWWTOOL

extern std::vector<boost::filesystem::path> wadpath_content;
extern std::string wadpath;

class DWWTool {
    public:
    void getCfg(MainWin *win);
    std::vector<boost::filesystem::path> iterDir(std::string dir);
    bool wadpathChanged = false;
};

extern DWWTool dwwtool;

#endif