#include "edit.h"
#include "notes.h"
#include <QMessageBox>

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


void edit::okpressed()
{
    if(note->editbox->namebox->text().isEmpty() || note->editbox->namebox->text()[0] == " ")
    {
        QMessageBox::information(this, "Error", "Name is not entered! Please enter a name");
    }
    else
    {
        note->scrollArea->layout()->addWidget(new QPushButton(note->editbox->namebox->text()));
    }
}
void edit::cancelpress()
{
    close();
}
