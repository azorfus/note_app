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
    ~notes();
    void cancel();
    QWidget *scrollArea;
    edit *editbox;

private slots:
    void addNew();
};

#endif // NOTES_H
