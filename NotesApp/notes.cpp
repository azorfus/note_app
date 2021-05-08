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
#include <QCloseEvent>
#include "edit.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QTextBrowser>
#include <QDir>
#include <QSpacerItem>

void notes::read_hash()
{
    vec_vals.clear();
    QFile jfile("azndata.json");
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
    this->setWindowTitle("AZNotes Beta 2.0");
    this->setFixedSize(400, 300);
    QWidget* mainLayout=new QWidget;
    mainLayout->setLayout(new QVBoxLayout);
    setCentralWidget(mainLayout);
    scroller=new QScrollArea;
    scrollArea = new QWidget;
    scrollArea->setLayout(new QVBoxLayout);
    scrollArea->layout()->setAlignment(Qt::AlignTop);
    scroller->setWidgetResizable(true);
    scroller->setWidget(scrollArea);
    mainLayout->layout()->addWidget(scroller);
    QPushButton* button= new QPushButton("New Note");
    mainLayout->layout()->addWidget(button);
    connect(button,&QPushButton::released,this,&notes::addNew);
    menuBar()->setNativeMenuBar(false);
    QMenu *fileMenu = menuBar()->addMenu(tr("&Menu"));
    lightmode = fileMenu->addAction("Light Theme");
    lightmode->setCheckable(true);
    QFile config("config.txt");
    if(!config.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::information(this, "Error", "Unable to open/find config file in "+ QDir::currentPath());
    }
    else
    {
        std::string config_txt = config.readAll().toStdString();
        if(config_txt.substr(0, 5) == "light")
        {
            light = true;
            lightmode->setChecked(true);
        }
    }

    connect(lightmode, &QAction::triggered, this, &notes::light_checked);
    QAction *about = fileMenu->addAction("About");
    connect(about, &QAction::triggered, this, &notes::about);
    read_hash();
    reset();
}

void notes::light_checked()
{
    if(lightmode->isChecked())
    {
        QFile config("config.txt");
        if(!config.open(QFile::WriteOnly|QFile::Text))
        {
            QMessageBox::information(this, "Error", "Unable to open/find config file in " + QDir::currentPath());
        }
        else
        {
            config.write("light");
            light = true;
            lightmode->setChecked(true);
        }
        config.close();
    }
    else
    {
        QFile config("config.txt");
        if(!config.open(QFile::Truncate|QFile::Text|QFile::WriteOnly))
        {
            QMessageBox::information(this, "Error", "Unable to open/find config file in "+ QDir::currentPath());
        }
        config.close();
    }
    QMessageBox::information(this, "Restart", "Changes will be applied once app is restarted.");
}

void notes::addNew()
{
    static int n=1;
    editbox = new edit(this);
    editbox->show();
    newnote = true;
    QObject::connect(editbox->ok, &QPushButton::clicked, editbox, &edit::okpressed);
    QObject::connect(editbox->savebutton, &QPushButton::clicked, editbox, &edit::savefile);
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
            QFile data("azndata.json");
            //qDebug() << current << "\n";
            old_name = current;
            if(!data.open(QFile::ReadOnly|QFile::Text))
            {
                 QMessageBox::information(this, "Error", "Unable to open database.");
                 break;
            }
            else
            {
                document = QJsonDocument::fromJson(data.readAll());
                data.close();
                QJsonObject obj = document.object();
                QString content = obj[current].toString();
                editbox = new edit(this);
                editbox->namebox->setText(current);
                editbox->block->setText(content);
                editbox->show();
                newnote = false;
                QObject::connect(editbox->cancel, &QPushButton::clicked, editbox, &edit::cancelpress);
                QObject::connect(editbox->savebutton, &QPushButton::clicked, editbox, &edit::savefile);
                QObject::connect(editbox->ok, &QPushButton::clicked, editbox, &edit::replace);
            }
        }
    }
}

