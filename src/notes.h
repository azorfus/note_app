#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include "edit.h"
class notes;
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

    QString old_name;
    std::vector <QString> vec_vals;
    QWidget *scrollArea;
    edit *editbox;
    QDialog *about_win;
    QHash <QString, QString> map;

private slots:
    void addNew();
    void noteclicked();
};

#endif // NOTES_H
