#include "mainwin.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <QMainWindow>
#include <QSettings>

#ifndef DWWTOOL
#define DWWTOOL

extern std::vector<boost::filesystem::path> wadpath_content;
extern std::string wadpath;

class DWWTool {
    public:
    void getCfg(MainWin *win);
    std::vector<boost::filesystem::path> iterDir(std::string dir);
    QSettings settings;
};

extern DWWTool dwwtool;

#endif