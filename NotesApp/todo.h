#ifndef TODO_H
#define TODO_H

#include <QDialog>
#include <QMainWindow>
#include <QScrollArea>
#include <QLineEdit>

class notes;
class todo : public QDialog
{
    Q_OBJECT

public:
    explicit todo(notes *parent);
    void addtask();
    void listtask();

    notes *note;
    QLineEdit *name;
    QDialog *newtask;
    QScrollArea* scroller;
    QWidget *scrollArea;
};

#endif // TODO_H
