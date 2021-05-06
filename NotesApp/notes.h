#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include "edit.h"
class notes;
class notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit notes();
    //~notes();
    void cancel();
    void read_vec();
    void reset();
    void clear();

    std::vector<QString> Names;
    QWidget *scrollArea;
    edit *editbox;

private slots:
    void addNew();
    void noteclicked();
};

#endif // NOTES_H
