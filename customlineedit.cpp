#include "customlineedit.h"

void CustomLineEdit::focusInEvent(QFocusEvent *event)
{
    emit focused();

    QLineEdit::focusInEvent(event);

}

void CustomLineEdit::focusOutEvent(QFocusEvent *event)
{
    emit unfocused();

    QLineEdit::focusOutEvent(event);

}
