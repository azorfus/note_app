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
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtDebug>

#define bug(x) qDebug() << x << "\n"

void notes::read_vec()
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

void notes::clear()
{
    QList<QPushButton *> widgets = scrollArea->findChildren<QPushButton *>();
    foreach(QPushButton * widget, widgets)
    {
        delete widget;
    }
}

void notes::reset()
{
    read_vec();
    if(Names.size() > 0)
    {
        QPushButton *note_buttons[Names.size()];
        for(unsigned long i=0;i<Names.size();i++)
        {
            QString butname = Names[i];
            note_buttons[i] = new QPushButton(butname); // notes::findChild<QPushButton *>(butname);
            this->scrollArea->layout()->addWidget(note_buttons[i]);
            qDebug() << butname << "\n";
            connect(note_buttons[i], &QPushButton::released, this, &notes::noteclicked);
        }
    }
}

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
    reset();
}

void notes::addNew()
{
    static int n=1;
    editbox = new edit(this);
    editbox->show();
    QObject::connect(editbox->ok, &QPushButton::clicked, editbox, &edit::okpressed);
    QObject::connect(editbox->cancel, &QPushButton::clicked, editbox, &edit::cancelpress);
    ++n;
}

void notes::noteclicked()
{
    QPushButton *button = (QPushButton *)sender();
    QString current = button->text();
    QString content;
    QJsonDocument document;
    for(unsigned long i = 0;i<Names.size();i++)
    {
        if(current == Names[i])
        {
            QFile data("Data.json");
            //qDebug() << current << "\n";
            if(!data.open(QFile::ReadOnly|QFile::Text))
            {
                 QMessageBox::information(this, "Error", "Unable to open database.");
                 break;
            }
            else
            {
                bug(current);
                document = QJsonDocument::fromJson(data.readAll());
                QJsonObject obj = document.object();
                bug(obj[current]);
                //QString content = obj[current].toString();
                qDebug() << "----------------\n";
                QMessageBox::information(this, "info", content);
            }
        }
    }
}
