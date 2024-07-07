#include <QtWidgets>

#ifndef SUBWIN
#define SUBWIN

class SubWin : public QWidget{

    Q_OBJECT

    public:
        SubWin();
    
    void goBack(){
        delete this;
    }
};

#endif