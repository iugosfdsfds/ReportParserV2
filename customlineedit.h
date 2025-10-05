#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>

class CustomLineEdit : public QLineEdit
{
Q_OBJECT

public:
    CustomLineEdit() : QLineEdit() {};
    CustomLineEdit(QWidget *parent) : QLineEdit(parent) {};

protected:
    virtual void focusInEvent(QFocusEvent *event) override;

    virtual void focusOutEvent(QFocusEvent *event) override;

signals:
    void focused();
    void unfocused();

};

#endif // CUSTOMLINEEDIT_H
