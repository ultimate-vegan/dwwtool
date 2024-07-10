#include "mainwin.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

#include <QMainWindow>
#include <QSettings>
#include <QApplication>

#ifndef DWWTOOL
#define DWWTOOL

extern std::vector<boost::filesystem::path> wadpath_content;
extern std::string wadpath;

class DWWTool {
    public:
    void getCfg(MainWin *win);
    std::vector<boost::filesystem::path> iterDir(std::string dir);
    QSettings settings{"DOOM WAD World", "DWWTool"};
};

extern DWWTool dwwtool;

#endif