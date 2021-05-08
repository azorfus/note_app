#include "edit.h"
#include "notes.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QDir>

edit::edit(notes *parent) :
    QDialog(parent),
    note(parent)
{
    this->setFixedSize(500, 420);
    this->setWindowTitle("Edit note - AZNotes");
    namelabel = new QLabel("Name", this);
    namelabel->move(10, 30);
    namebox = new QLineEdit(this);
    namebox->setGeometry(60, 30, 430, 20);
    block = new QTextEdit(this);
    block->setGeometry(10, 60, 480, 320);
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    cancel->setGeometry(10, 385, 60, 25);
    ok->setGeometry(430, 385, 60, 25);
    savebutton = new QPushButton("Save", this);
    savebutton->setGeometry(360, 385, 60, 25);
    QMenuBar *menubar = new QMenuBar(this);
    menubar->setNativeMenuBar(false);
    QAction *delete_note = menubar->addAction("Delete note");
    connect(delete_note, &QAction::triggered, this, &edit::deletenote);
}

void edit::deletenote()
{
    QMessageBox::StandardButton sure;
    sure = QMessageBox::question(this, "Delete Note", "Sure with note deletion?\nIt cannot be recovered again.", QMessageBox::Yes|QMessageBox::No);
    if(sure == QMessageBox::Yes)
    {
        QFile jsondat("azndata.json");
        QJsonObject obj;
        if(!jsondat.open(QFile::WriteOnly|QFile::Text))
        {
             QMessageBox::information(this, "Error", "Unable to open database.");
             close();
        }
        else
        {
            QString del_note = note->editbox->namebox->text();
            foreach(const QString &name, note->map.keys())
            {
                if(del_note == name)
                {
                    note->map.remove(name);
                    foreach(const QString &names, note->map.keys())
                    {
                        obj.insert(names, note->map[names]);
                    }
                    QJsonDocument jdoc;
                    jdoc.setObject(obj);
                    jsondat.write(jdoc.toJson());
                    jsondat.close();
                    note->clear();
                    note->read_hash();
                    note->reset();
                    break;
                }
            }
        }
        not_okay = false;
        close();
    }
}

void edit::read_vec()
{
    if(note->vec_vals.size() != 0)
    {
        note->vec_vals.clear();
    }
    foreach(const QString &key, note->map.keys())
    {

        note->vec_vals.push_back(key);
    }
}

void edit::replace()
{
    QFile jsondat("azndata.json");
    QJsonObject obj;
    bool same = false;
    if(!jsondat.open(QFile::WriteOnly|QFile::Text))
    {
         QMessageBox::information(this, "Error", "Unable to open database.");
         close();
    }
    else
    {
        if(note->editbox->namebox->text().isEmpty() || note->editbox->namebox->text()[0] == ' ')
        {
            QMessageBox::information(this, "Error", "Name is not entered! Please enter a name");
        }
        else
        {
            read_vec();
            for(unsigned long i=0;i<note->vec_vals.size();i++)
            {
                if(note->editbox->namebox->text() == note->vec_vals[i] && note->editbox->namebox->text() != note->old_name)
                {
                    QMessageBox::information(this, "Error", "Note with the same name already exists!");
                    same = true;
                    break;
                }
            }
            if(same == false)
            {
                QString rpl_note = note->editbox->namebox->text();
                foreach(const QString &name, note->map.keys())
                {
                    if(note->old_name == name)
                    {
                        note->map[name] = rpl_note;
                        note->map[note->map[name]] = note->editbox->block->toPlainText();
                        if(rpl_note != note->old_name)
                        {
                            note->map.remove(note->old_name);
                        }
                        foreach(const QString &name, note->map.keys())
                        {
                            obj.insert(name, note->map[name]);
                        }
                        QJsonDocument jdoc;
                        jdoc.setObject(obj);
                        jsondat.write(jdoc.toJson());
                        jsondat.close();
                        note->old_name = note->editbox->namebox->text();
                        note->read_hash();
                        note->clear();
                        note->reset();
                        break;
                    }
                }
            }
        }
        not_okay = false;
        if(same == false)
        {
            close();
            not_okay = false;
        }
    }

}

void edit::savefile()
{
    QFile jsondat("azndata.json");
    QJsonObject obj;
    if(!jsondat.open(QFile::WriteOnly|QFile::Text))
    {
         QMessageBox::information(this, "Error", "Unable to open database.");
         close();
    }
    else
    {
        bool same = false;
        read_vec();
        for(unsigned long i=0;i<note->vec_vals.size();i++)
        {
            if(note->editbox->namebox->text() == note->vec_vals[i] && note->editbox->namebox->text() != note->old_name)
            {
                QMessageBox::information(this, "Error", "Note with the same name already exists!");
                same = true;
                break;
            }
        }
        if(same == false)
        {
            if(note->newnote==false)
            {
                QString rpl_note = note->editbox->namebox->text();
                foreach(const QString &name, note->map.keys())
                {
                    if(note->old_name == name)
                    {
                        note->map[name] = rpl_note;
                        note->map[note->map[name]] = note->editbox->block->toPlainText();
                        if(rpl_note != note->old_name)
                        {
                            note->map.remove(note->old_name);
                        }
                        foreach(const QString &name, note->map.keys())
                        {
                            obj.insert(name, note->map[name]);
                        }
                        QJsonDocument jdoc;
                        jdoc.setObject(obj);
                        jsondat.write(jdoc.toJson());
                        jsondat.close();
                        note->old_name = note->editbox->namebox->text();
                        note->read_hash();
                        note->clear();
                        note->reset();
                        break;
                    }
                }
            }
            else
            {
                note->map.insert(note->editbox->namebox->text(), note->editbox->block->toPlainText());
                foreach(const QString &name, note->map.keys())
                {
                    obj.insert(name, note->map[name]);
                }
                QJsonDocument jdoc;
                jdoc.setObject(obj);
                jsondat.write(jdoc.toJson());
                jsondat.close();
                note->clear();
                note->read_hash();
                note->reset();
            }
        }
    }
}

void edit::okpressed()
{
    if(note->editbox->namebox->text().isEmpty() || note->editbox->namebox->text()[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Name is not entered! Please enter a name");
    }
    else
    {

        bool same = false;
        read_vec();
        note->old_name = note->editbox->namebox->text();
        for(unsigned long i=0;i<note->vec_vals.size();i++)
        {
            if(note->editbox->namebox->text() == note->vec_vals[i] && note->old_name != note->editbox->namebox->text())
            {
                QMessageBox::information(this, "Error", "Note with the same name already exists!");
                same = true;
                break;
            }
        }
        if(same == false)
        {
            QFile jsondat("azndata.json");
            QJsonObject obj;
            if(!jsondat.open(QFile::WriteOnly|QFile::Text))
            {
                 QMessageBox::information(this, "Error", "Unable to open database.");
                 close();
            }
            else
            {
                note->map.insert(note->editbox->namebox->text(), note->editbox->block->toPlainText());
                foreach(const QString &name, note->map.keys())
                {
                    obj.insert(name, note->map[name]);
                }
                QJsonDocument jdoc;
                jdoc.setObject(obj);
                jsondat.write(jdoc.toJson());
                jsondat.close();
                note->clear();
                note->read_hash();
                note->reset();
            }
        }
        if(same == false)
        {
            not_okay = false;
            close();
        }
    }
}
void edit::closeEvent (QCloseEvent *event)
{
    if(not_okay == true && (note->editbox->namebox->text() != note->old_name || note->editbox->block->toPlainText() != note->map[note->editbox->namebox->text()] || note->editbox->block->toPlainText() != note->map[note->old_name]))
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Quit", tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            event->accept();
        }
    }
}
void edit::cancelpress()
{
    close();
}
