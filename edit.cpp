#include "edit.h"
#include "notes.h"

edit::edit(notes *parent) :
    QDialog(parent),
    note(parent)
{
    namelabel = new QLabel("Name", this);
    namelabel->move(10, 10);
    namebox = new QLineEdit(this);
    namebox->setGeometry(60, 10, 430, 20);
    block = new QTextEdit(this);
    block->setGeometry(10, 40, 480, 320);
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    cancel->setGeometry(10, 365, 60, 25);
    ok->setGeometry(430, 365, 60, 25);
}

edit::~edit()
{
    delete this;
}

void edit::okpressed()
{
    note->scrollArea->layout()->addWidget(new QPushButton(note->editbox->namebox->text()));
}
