QT += widgets core

HEADERS += dwwtool.h mainwin.h subwin.h
SOURCES += main.cpp mainwin.cpp subwin.cpp

INCLUDEPATH += /usr/include/qt/QtWidgets
INCLUDEPATH += /usr/include/qt

QMAKE_LFLAGS += -lQt5Core -lQt5Widgets -lQt5Gui -lboost_filesystem -lboost_system