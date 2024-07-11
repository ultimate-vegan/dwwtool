QT += widgets core

HEADERS += src/dwwtool.h src/mainwin.h src/subwin.h
SOURCES += src/main.cpp src/mainwin.cpp src/dwwtool.cpp src/subwin.cpp

INCLUDEPATH += /usr/include/qt/QtWidgets
INCLUDEPATH += /usr/include/qt
INCLUDEPATH += libs/dsda-doom/prboom2/src/**
INCLUDEPATH += libs/dsda-doom/prboom2/build
INCLUDEPATH += /usr/include/SDL

QMAKE_LFLAGS += -lQt5Core -lQt5Widgets -lQt5Gui -lboost_filesystem -lboost_system

QMAKE_CXXFLAGS += -g