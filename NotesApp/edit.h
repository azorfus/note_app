#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QMainWindow>
class edit;
class notes;
class edit : public QDialog
{
    Q_OBJECT

public:
    explicit edit(notes *parent);
    //~edit();
    void okpressed();
    void cancelpress();
    void read_vec();
    void deletenote();
    void replace();
    void savefile();

    notes* note;
    QLabel *namelabel;
    QLineEdit *namebox;
    QTextEdit *block;
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *savebutton;

};

#endif // EDIT_H
