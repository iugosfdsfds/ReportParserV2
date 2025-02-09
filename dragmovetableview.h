#ifndef DRAGMOVETABLEVIEW_H
#define DRAGMOVETABLEVIEW_H

#include <QTableView>

class DragMoveTableView: QTableView
{

    Q_OBJECT

public:
    DragMoveTableView(QWidget *parent);

    int selectedRow() const;
    void reset();
    void dropEvent(QDropEvent *e);

private:
    int m_dropRow;
};

#endif // DRAGMOVETABLEVIEW_H
