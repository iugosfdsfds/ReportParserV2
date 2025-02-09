#include "dragmovetableview.h"

#include <QItemSelectionModel>
#include <QMetaObject>
#include <QDropEvent>

DragMoveTableView::DragMoveTableView(QWidget *parent) :  QTableView(parent), m_dropRow(0)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropOverwriteMode(false);
    setDropIndicatorShown(true);
}

int DragMoveTableView::selectedRow() const
{
    QItemSelectionModel *selection = selectionModel();
    return selection->hasSelection() ? selection->selectedRows().front().row() : -1;
}

void DragMoveTableView::reset()
{
    QTableView::reset();

    QObject::connect(model(), &QAbstractTableModel::rowsInserted, this, [this](const QModelIndex &parent, int first, int last) {m_dropRow = first;});
}

void DragMoveTableView::dropEvent(QDropEvent *e)
{
    if (e->source() != this || e->dropAction() != Qt::MoveAction)
              return;
           int dragRow = selectedRow();

           QTableView::dropEvent(e);  // m_dropRow is set by inserted row

           if (m_dropRow > dragRow)
              --m_dropRow;

           QMetaObject::invokeMethod(this, "selectRow", Qt::QueuedConnection, Q_ARG(int, m_dropRow));
}
