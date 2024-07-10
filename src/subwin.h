//this file is currently unused, may repurpose later

#ifndef SUBWIN
#define SUBWIN

//qt widgets
#include <QWidget>

class SubWin : public QWidget{

    Q_OBJECT

    public:
        SubWin();
    
    void goBack(){
        //https://www.youtube.com/watch?v=N5TWbeav7hI
        delete this;
    }
};

#endif