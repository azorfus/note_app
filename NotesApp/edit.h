#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QMainWindow>
#include <QCloseEvent>

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
    void closeEvent(QCloseEvent *event);

    notes* note;
    bool not_okay = true;
    QLabel *namelabel;
    QLineEdit *namebox;
    QTextEdit *block;
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *savebutton;

};

#endif // EDIT_H
