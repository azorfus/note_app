#include "notes.h"
//#include "notes_setup.h"
#include <QMenuBar>
#include <QMenuBar>
#include <QObject>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPalette>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QFile>
#include <QTextEdit>
#include "edit.h"

notes::notes()
{
    this->setWindowTitle("Notes");
    this->setFixedSize(400, 300);
    QWidget* mainLayout=new QWidget;
    mainLayout->setLayout(new QVBoxLayout);
    setCentralWidget(mainLayout);
    QScrollArea* scroller=new QScrollArea;
    scrollArea = new QWidget;
    scrollArea->setLayout(new QVBoxLayout);
    scroller->setWidgetResizable(true);
    scroller->setWidget(scrollArea);
    mainLayout->layout()->addWidget(scroller);
    QPushButton* button= new QPushButton("New Note");
    mainLayout->layout()->addWidget(button);
    connect(button,&QPushButton::released,this,&notes::addNew);
}

notes::~notes()
{
    delete this;
}

void notes::addNew()
{
    static int n=1;
    editbox = new edit();
    editbox->show();
    QObject::connect(editbox->ok, &QPushButton::clicked, editbox, &edit::okpressed);
    ++n;
}
