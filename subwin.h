#include <QtWidgets>

#ifndef SUBWIN
#define SUBWIN

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