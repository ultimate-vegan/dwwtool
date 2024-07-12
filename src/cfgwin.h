#include "mainwin.h"

#ifndef CFGWIN
#define CFGWIN

class CfgWin : public QWidget{

        Q_OBJECT

        public:
            CfgWin();
            void cancel();
            void apply();
};

#endif