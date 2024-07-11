#ifndef SUBWIN
#define SUBWIN

#include "mainwin.h"

//qt widgets
//#include <QWidget>

class SubWin : public QWidget{

    Q_OBJECT

    public:
        SubWin(MainWin *parent = (MainWin *)nullptr);
    
    void goBack(){
        //https://www.youtube.com/watch?v=N5TWbeav7hI
        delete this;
    }
};

#endif