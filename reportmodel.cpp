#include "reportmodel.h"

#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QGuiApplication>
#include <QClipboard>
#include <QPair>

//#include <QDebug>

ReportModel::ReportModel(QObject *parent)
{
    setParent(parent);
}

QVector<LogLine> ReportModel::getLines() const
{
    return lines;
}

void ReportModel::setLines(RawReport *report)
{
    emit layoutAboutToBeChanged();
    lines = report->getLines();
    emit layoutChanged();

}

void ReportModel::setLines(QVector<LogLine> newLines)
{
    beginResetModel();
    lines = newLines;
    endResetModel();
}

const LogLine ReportModel::getAt(int index) const
{
    return lines.at(index);
}

void ReportModel::changeReprint(int index)
{
    lines[index].reprint = !lines.at(index).reprint;

}

void ReportModel::changeUrgent(int index)
{
    lines[index].urgent = !lines.at(index).urgent;
}

void ReportModel::changeInfo(int index)
{
    lines[index].info = !lines[index].info;
}

void ReportModel::addLines(QVector<LogLine> newLines)
{
    beginResetModel();
    for (LogLine& line : newLines) {
        lines.push_back(line);
    }
    endResetModel();
}

void ReportModel::addLines(LogLine line)
{
    beginResetModel();
    lines.push_back(line);
    endResetModel();
}

void ReportModel::clearAll()
{
    beginResetModel();
    lines.clear();
    endResetModel();
}

void ReportModel::resetStyles()
{
    for (int i = 0; i < lines.size(); ++i) {
        lines[i].urgent = false;
        lines[i].reprint = false;
        lines[i].info = false;
    }
}

void ReportModel::updateView()
{
    beginResetModel();
    endResetModel();
}

/*Returns 0 - success, 1 - couldn't create out, 2 - no out file specified */
int ReportModel::toOut(const Config *config, const QVector <int> indexes)
{
    //combining model log lines
    QString outText = "";
    for (int i = 0; i < lines.size(); i++) {
        if (indexes.size() > 0) //if got vector of indexes, combine only them
            if (!indexes.contains(i))
                continue;

        if (config->outName)
            outText.push_back(lines.at(i).name);
        if (config->outCount)
            outText.push_back(config->splitter + QString::number(lines.at(i).count));
        if (config->outAttribs) {
            outText.push_back(config->splitter);

            if (lines.at(i).reprint)
                outText.push_back("1");
            else
                outText.push_back("0");
            outText.push_back(config->splitter);

            if (lines.at(i).urgent)
                outText.push_back("1");
            else
                outText.push_back("0");
            outText.push_back(config->splitter);

            outText.push_back( lines.at(i).info ? "0" : "1" );
        }

        outText.push_back("\n");
    }

    //out to clip / file
    if (config->outToClip) {
        QGuiApplication::clipboard()->setText(outText);
    } else {
        if (config->outFile.isEmpty()) {
            qWarning("Выводной файл не указан");
            return 2;
        }
        QFile file(config->outFile);

        if (!file.open(QIODevice::WriteOnly)) {
            qWarning("Не удалось создать выводной файл");
            return 1;
        }

        QTextStream out(&file);

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        out.setEncoding(QStringConverter::Utf8);
#else
        out.setCodec("UTF-8");
#endif

        out << outText;
        file.close();
    }

    return 0;
}

QPair<int, int> ReportModel::mergeLines(QVector<int> indexes)
{
    QPair <int, int> doneLines = {0, 0};
    if ( (indexes.size() < 2) || (indexes.size() > lines.size()))
        return {0, 0};
    //get selected lines indexes in pairs

    QVector <QPair <int, int>> sectors;

    QVector <int> temp;
    temp.push_back(indexes[0]);
    for (int i = 1; i < indexes.size(); i++) {
        if (temp.last() == indexes[i]-1 ) {
            temp.push_back(indexes[i]);
        } else {
            if (temp.size() > 1)
                sectors.push_back( {temp.first(), temp.last()} );
            temp.clear();
            temp.push_back(indexes[i]);
        }
    }
    if (temp.first() != temp.last())
        sectors.push_back( {temp.first(), temp.last()} );

    //for each selected group
    for (int i = sectors.size()-1; i >= 0; i--) {
        QVector <LogLine> tempLines;
        for (int index = sectors.at(i).first; index <= sectors.at(i).second; index++) { //from selection group start to end
            if (!tempLines.contains( lines.at(index) )) { //add to temp if entry doesn't exist
                tempLines.push_back( lines.at(index) );
            } else { //else counts sum
                tempLines[ tempLines.indexOf(lines.at(index)) ].count += lines.at(index).count;
            }
        }

        for (int del = sectors.at(i).second; del >= sectors.at(i).first; del--) { //removing selected group from end
            lines.remove(del);
            doneLines.first++;
        }
        QVector <LogLine>::iterator it = lines.begin() + sectors.at(i).first; //inserting from temp to the same place
        for (LogLine& line : tempLines) {
            lines.insert(it, line);
            doneLines.second++;
            ++it;
        }
    }

    updateView();
    return doneLines;

}

void ReportModel::moveLines(QVector<int> indexes, bool moveUp, bool maxed)
{
    if (indexes.isEmpty()) return;
    //beginResetModel();
    if (moveUp) {
        if (maxed) {
            QVector <LogLine> temp = lines.mid(indexes.first(), indexes.last()-indexes.first()+1);
            lines.erase(lines.begin()+indexes.first(), lines.begin()+indexes.last()+1);
            for (int i = temp.size()-1; i >= 0; --i) {
                lines.push_front(temp[i]);
            }
        } else lines.move(indexes.first()-1, indexes.last());
    }
    else {
        if (maxed) {
            QVector <LogLine> temp = lines.mid(indexes.first(), indexes.last()-indexes.first()+1);
            lines.erase(lines.begin()+indexes.first(), lines.begin()+indexes.last()+1);
            for (LogLine& l : temp) {
                lines.push_back(l);
            }
        } else lines.move(indexes.last()+1, indexes.first());
    }
    emit dataChanged( createIndex(0, 0), createIndex(lines.size()-1, 1 ) );
    //endResetModel();
}

int ReportModel::getSize() const
{
    return lines.size();
}

int ReportModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return lines.size();
}

int ReportModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ReportModel::data(const QModelIndex &index, int role) const
{
    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole)) {
        switch (index.column()) {
            case 0:
                return lines.at(index.row()).name;
                break;
            case 1:
                return lines.at(index.row()).count;
                break;
            default:
                break;
        }
    }
    if (role == Qt::ForegroundRole) {
        if (index.column() == 0) {
            if (lines.at(index.row()).reprint)
                return QColor(Qt::darkGreen);
            if (lines.at(index.row()).urgent)
                return QColor(Qt::red);
        }
    }
    if (role == Qt::BackgroundRole) {
        if (index.column() == 0) {
            if ( (lines.at(index.row()).urgent) && (lines.at(index.row()).reprint) )
                return QColor(Qt::yellow);

            if ( lines.at(index.row()).info )
                return QColor(180, 180, 255, 128);
        }
    }
    return QVariant();
}

bool ReportModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    emit editingFinished();
    if (role == Qt::EditRole) {
        switch (index.column()) {
            case 0:
                if (value.toString() == "")
                    return false;
                lines[index.row()].name = value.toString();
                lines[index.row()].updateData();
                emit dataChanged(index, index);
                return true;
                break;
            case 1:
                bool ok = true;
                value.toInt(&ok);
                if (!ok)
                    return false;
                lines[index.row()].count = value.toInt();
                emit dataChanged(index, index);
                return true;
                break;
        }
    }
    return false;
}

bool ReportModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    lines.erase(lines.begin() + row, lines.begin() + row + count );
    endRemoveRows();
    return true;
}

Qt::ItemFlags ReportModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return "Наименование";
                break;
            case 1:
                return "Количество";
                break;
        }
    } else {
        return section+1;
    }
    return QVariant();
}
