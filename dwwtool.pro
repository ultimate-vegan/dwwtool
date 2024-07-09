QT += widgets core

HEADERS += src/dwwtool.h src/mainwin.h src/subwin.h
SOURCES += src/main.cpp src/mainwin.cpp src/subwin.cpp src/dwwtool.cpp

INCLUDEPATH += /usr/include/qt/QtWidgets
INCLUDEPATH += /usr/include/qt
INCLUDEPATH += libs/dsda-doom/prboom2

QMAKE_LFLAGS += -lQt5Core -lQt5Widgets -lQt5Gui -lboost_filesystem -lboost_system