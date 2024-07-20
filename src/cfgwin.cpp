#include "cfgwin.h"
#include "dwwtool.h"

//qt widgets
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

CfgWin::CfgWin(){

    setWindowTitle("Preferences");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);

    //buttons
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *cancelbtn = new QPushButton("Cancel");
    QPushButton *okbtn = new QPushButton("OK");
    connect(cancelbtn, &QPushButton::clicked, this, &CfgWin::cancel);
    connect(okbtn, &QPushButton::clicked, this, &CfgWin::apply);
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(cancelbtn);
    btnLayout->addWidget(okbtn);

    //path prefs
    QWidget *pathWidget = new QWidget;
    QVBoxLayout *pathBox = new QVBoxLayout;
    QLabel *dirLabel = new QLabel("Default Directory");
    QLineEdit *dirEdit = new QLineEdit(dwwtool.settings.value("paths/wadpath").toString());
    dirEdit->setObjectName("dirEdit");
    pathBox->addWidget(dirLabel);
    pathBox->addWidget(dirEdit);
    pathWidget->setLayout(pathBox);

    //doom prefs
    QWidget *doomWidget = new QWidget;
    QVBoxLayout *doomBox = new QVBoxLayout;

    QLabel *defDoomLabel = new QLabel("Default Source Port");
    QComboBox *defDoomBox = new QComboBox;
    defDoomBox->insertItem(0, "DSDA-Doom");
    defDoomBox->insertItem(1, "Woof");
    defDoomBox->insertItem(2, "Chocolate Doom");

    doomBox->addWidget(defDoomLabel);
    doomBox->addWidget(defDoomBox);
    doomWidget->setLayout(doomBox);

    //add sections to main widget
    layout->addWidget(pathWidget);
    layout->addWidget(doomWidget);

    //keep this on the bottom
    layout->addLayout(btnLayout);

    this->setLayout(layout);

}

void CfgWin::cancel(){
    //https://www.youtube.com/watch?v=N5TWbeav7hI
    delete this;
}

void CfgWin::apply(){
    dwwtool.settings.setValue("paths/wadpath", this->findChild<QLineEdit*>("dirEdit")->text());
    delete this;
}
