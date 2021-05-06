#include "edit.h"
#include "notes.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

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

void edit::read_vec()
{
    Names.clear();
    QFile name_str("DatNM.txt");
    if(!name_str.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error", "Unable to open database.");
    }
    else
    {
        QTextStream stream(&name_str);
        while(!stream.atEnd())
        {
            Names.push_back(stream.readLine());
        }
    }
}

void edit::okpressed()
{
    if(note->editbox->namebox->text().isEmpty() || note->editbox->namebox->text()[0] == " ")
    {
        QMessageBox::information(this, "Error", "Name is not entered! Please enter a name");
    }
    else
    {
        QFile name_str("DatNM.txt");
        if(!name_str.open(QFile::Append | QFile::Text))
        {
            QMessageBox::information(this, "Error", "Unable to open database.");
        }
        else
        {
            bool same = false;
            read_vec();
            for(unsigned long i=0;i<Names.size();i++)
            {
                if(note->editbox->namebox->text() == Names[i])
                {
                    QMessageBox::information(this, "Error", "Note with the same name already exists!");
                    same = true;
                    break;
                }
            }
            if(same == false)
            {
                QFile jsondat("Data.json");
                if(!jsondat.open(QFile::Append|QFile::Text))
                {
                     QMessageBox::information(this, "Error", "Unable to open database.");
                }
                else
                {
                    QJsonObject datajson;
                    QJsonArray array;
                    //note->scrollArea->layout()->addWidget(new QPushButton(note->editbox->namebox->text()));
                    QTextStream in(&name_str);
                    in << note->editbox->namebox->text() << "\n";
                    datajson.insert()
                    QJsonDocument jsonDoc;
                    jsonDoc.setObject(datajson);
                    jsondat.write(jsonDoc.toJson());
                    jsondat.close();
                    name_str.close();
                    note->clear();
                    note->reset();
                }
            }
        }
    }
}
void edit::cancelpress()
{
    close();
}
