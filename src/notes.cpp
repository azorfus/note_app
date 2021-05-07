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
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QTextBrowser>

#define bug(x) qDebug() << x << "\n"

void notes::read_hash()
{
    vec_vals.clear();
    QFile jfile("Data.json");
    if(!jfile.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::information(this, "Error", "Could not open database!");
        close();
    }
    else
    {
        QString bytes = jfile.readAll();
        jfile.close();

        QJsonDocument document = QJsonDocument::fromJson(bytes.toUtf8());

        QJsonObject obj = document.object();
        foreach(const QString &key, obj.keys())
        {
            vec_vals.push_back(key);
            map.insert(key, obj[key].toString());
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
    int i = 0;
    if(map.size() > 0 && vec_vals.size() > 0)
    {
        QPushButton *note_buttons[map.size()];
        foreach(const QString &name, map.keys())
        {
            QString butname = vec_vals[i];
            note_buttons[i] = new QPushButton(butname); // notes::findChild<QPushButton *>(butname);
            this->scrollArea->layout()->addWidget(note_buttons[i]);
            connect(note_buttons[i], &QPushButton::released, this, &notes::noteclicked);
            i++;
        }
    }
}

notes::notes()
{
    this->setWindowTitle("AZNotes");
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
    menuBar()->setNativeMenuBar(false);
    QMenu *fileMenu = menuBar()->addMenu(tr("&Help"));
    QAction *about = fileMenu->addAction("About");
    connect(about, &QAction::triggered, this, &notes::about);
    read_hash();
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

void notes::about()
{
    about_win = new QDialog(this);
    about_win->setFixedSize(250, 200);
    about_win->setWindowTitle("About");
    about_win->setModal(true);
    QPushButton *close = new QPushButton(about_win);
    close->setGeometry(95, 170, 60, 20);
    close->setText("Close");
    connect(close, &QPushButton::clicked, this, &notes::close_about);
    QTextBrowser *about_txt = new QTextBrowser(about_win);
    about_txt->setGeometry(10, 10, 230, 150);
    about_txt->setAlignment(Qt::AlignCenter);
    about_txt->append("AZNotes is a note-taking app\n");
    about_txt->setAlignment(Qt::AlignCenter);
    about_txt->append("Written by Azorfus\n");
    about_txt->setAlignment(Qt::AlignCenter);
    about_txt->append("Contact me or report bugs at azorfus@gmail.com");
    about_txt->setAlignment(Qt::AlignCenter);
    about_win->show();
}

void notes::close_about()
{
    about_win->close();
}

void notes::noteclicked()
{
    QPushButton *button = (QPushButton *)sender();
    QString current = button->text();
    QString content;
    QJsonDocument document;
    old_name = current;
    foreach(const QString &name, map.keys())
    {
        if(current == name)
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
                document = QJsonDocument::fromJson(data.readAll());
                QJsonObject obj = document.object();
                QString content = obj[current].toString();
                editbox = new edit(this);
                editbox->namebox->setText(current);
                editbox->block->setText(content);
                editbox->show();
                QObject::connect(editbox->cancel, &QPushButton::clicked, editbox, &edit::cancelpress);
                QObject::connect(editbox->ok, &QPushButton::clicked, editbox, &edit::replace);
            }
        }
    }
}
