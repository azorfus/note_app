#include "todo.h"
#include "notes.h"
#include <QDialog>
#include <QPushButton>
#include <QMenuBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QFile>
#include <QMessageBox>

todo::todo(notes *parent) :
    QDialog(parent),
    note(parent)
{
    this->setFixedSize(300, 400);
    this->setWindowTitle("Todo list - AZNotes");
    QWidget* mainLayout=new QWidget(this);
    mainLayout->setGeometry(5, 20, 290, 380);
    mainLayout->setLayout(new QVBoxLayout);
    //setCentralWidget(mainLayout);
    this->setModal(true);
    scroller=new QScrollArea;
    scrollArea = new QWidget;
    scrollArea->setLayout(new QVBoxLayout);
    scrollArea->layout()->setAlignment(Qt::AlignTop);
    scroller->setWidgetResizable(true);
    scroller->setWidget(scrollArea);
    mainLayout->layout()->addWidget(scroller);
    QPushButton* add = new QPushButton("New task");
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setNativeMenuBar(false);
    mainLayout->layout()->addWidget(add);
    QAction *del_task = menuBar->addAction("Delete Done");
    connect(add, &QPushButton::clicked, this, &todo::addtask);
}

void todo::addtask()
{
    newtask = new QDialog(this);
    newtask->setFixedSize(300, 40);
    newtask->setWindowTitle("Add task");
    QLabel *namelabel = new QLabel(newtask);
    namelabel->setText("Name");
    namelabel->setGeometry(10, 10, 40, 20);
    name = new QLineEdit(newtask);
    name->setGeometry(60, 10, 180, 20);
    QPushButton *add = new QPushButton(newtask);
    add->setText("Ok");
    add->setGeometry(250, 10, 40, 20);
    connect(add, &QPushButton::clicked, this, &todo::listtask);

    newtask->show();
}

void todo::listtask()
{
    QFile tasks("tasks.txt");
    if(!tasks.open(QFile::Append|QFile::Text))
    {
        QMessageBox::information(newtask, "Error", "Error opening tasks database");
    }
    else
    {
        std::string taskdat = "0" + nae->text().toStdString();
        tasks.write(taskdat);
    }
}
