#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "./lib/logline.h"
#include "rawreport.h"
#include "./lib/config.h"

class ReportModel : public QAbstractTableModel
{
    Q_OBJECT
signals:
    void editingFinished();

public:
    ReportModel(QObject* parent = nullptr);

    QVector <LogLine> getLines() const;
    void setLines(RawReport *report);
    void setLines(QVector <LogLine> newLines);
    const LogLine getAt(int index) const;
    void changeReprint(int index);
    void changeUrgent(int index);
    void changeInfo(int index);

    void addLines(QVector <LogLine> newLines);
    void addLines(LogLine line);
    void clearAll();

    void resetStyles();

    void updateView();

    bool toOut(const Config *config, const QVector <int> indexes = {});
    QPair <int, int> mergeLines(QVector <int> indexes);

    //void moveLine(int from, int to);

    void moveLines(QVector <int> indexes, bool moveUp, bool maxed = false);

    int getSize() const;

private:
    QVector <LogLine> lines;

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool removeRows(int row, int count, const QModelIndex &parent);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
    */

private slots:
    //void lineMoved(int vFrom, int from, int to);
};

#endif // REPORTMODEL_H
