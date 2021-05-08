#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include "edit.h"
#include "todo.h"
#include <QCloseEvent>

class todo;
class edit;
class notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit notes();
    //~notes();
    edit *edits;
    void cancel();
    void read_vec();
    void reset();
    void clear();
    void read_hash();
    void about();
    void close_about();
    void light_checked();
    void show_todo();

    QString old_name;
    QAction *lightmode;
    QScrollArea* scroller;
    std::vector <QString> vec_vals;
    QWidget *scrollArea;
    bool newnote = false;
    bool light = false;
    edit *editbox;
    todo *todo_list;
    QDialog *about_win;
    QHash <QString, QString> map;

private slots:
    void addNew();
    void noteclicked();
};

#endif // NOTES_H
