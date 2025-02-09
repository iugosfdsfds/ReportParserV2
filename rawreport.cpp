#include "rawreport.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QDebug>

const QString urgentMark = "ср";
const QString reprintMark = "доп";

RawReport::RawReport(QString logFile, Profile* profile)
{
    this->profile = profile;
    ok = parseFile(logFile);
}

bool RawReport::parsed() const
{
    return ok;
}

QVector<LogLine> RawReport::getLines() const
{
    return lines;
}

LogLine RawReport::getLine(int line) const
{
    return lines.at(line);
}

bool RawReport::filtersOK(QString text)
{
    bool ok = true;
    for (auto filter : profile->filters) {
        if (filter.second) {
            if (text.contains(filter.first))
                ok = false;
        } else
        {
            if (!text.contains(filter.first))
                ok = false;
        }
    }
    return ok;
}

bool RawReport::parseFile(QString file)
{
    QFile log(file);
    if (!log.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&log);
    if (profile->encoding == "ANSI")
        in.setCodec("ANSI");
    if (profile->encoding == "UTF-8")
        in.setCodec("UTF-8");

    QString data = in.readAll();

    log.close();
    QStringList dataLines;
    if (profile->splitter == "\\n")
        dataLines = data.split("\n");
    else
        dataLines = data.split(profile->splitter);
    if (dataLines.size() == 0)
        return false;

    bool markUrgent = false;
    bool markReprint = false;
    for (QString line : dataLines) { //magick goes here

        if ( (!line.contains(profile->nameStart, Qt::CaseInsensitive)) || (!line.contains(profile->nameEnd, Qt::CaseInsensitive)) )
            continue;
        if (!filtersOK(line))
            continue;

        //find name indexes
        int nameStartIndex, nameEndIndex, countStartIndex, countEndIndex;
        QString name, count;
        nameStartIndex = line.lastIndexOf(profile->nameStart, -1, Qt::CaseInsensitive);
        nameStartIndex += + profile->nameStart.size() + 1 + profile->nameOffsets[0]; //get name start index
        if (profile->nameEnd == "")
            nameEndIndex = line.size()-1;
        else
            nameEndIndex = line.indexOf(profile->nameEnd, nameStartIndex, Qt::CaseInsensitive) + profile->nameOffsets[1];  //get name end index, or endl index
        name = line.section("", nameStartIndex, nameEndIndex).remove("\n"); //get name

        if (!profile->allowDubs) // do check if sensetive for dublicats
            if (hasDubs(name))
                continue;

        //look for count if enabled
        if (profile->countsEnabled) {
            if (line.contains(profile->countEnd, Qt::CaseInsensitive)) {
                QString clearLine = line;
                QString clearEnding = profile->countEnd;
                clearLine.remove("_");
                clearEnding.remove("_");
                QChar lastSym = clearLine.at( clearLine.lastIndexOf(clearEnding, -1, Qt::CaseInsensitive)-1 ); //looking for last symbol before end key, excluding "_"
                countEndIndex = line.lastIndexOf(lastSym, line.lastIndexOf(clearEnding, -1, Qt::CaseInsensitive));
                countStartIndex = line.lastIndexOf(profile->countStart, countEndIndex, Qt::CaseInsensitive); //then look for rightmost include of mark before end mark
                countEndIndex += profile->countOffsets[0] +1;
                countStartIndex += profile->countOffsets[1] +1; //some adjustments

                count = line.section("", countStartIndex, countEndIndex).replace("_", ""); //get count
            } else
                count = "1";
        } else
            count = "1";

        //create line entry
        LogLine order( name, count.toInt() );

        //get reprints and urgent statuses
        QRegularExpression reprSign(reprintMark + "\\S*_", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression urgSign(urgentMark + "\\S*_", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression reprTrig(reprintMark + "\\S*\\d{1,}_", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression urgTrig(urgentMark + "\\S*\\d{1,}_", QRegularExpression::CaseInsensitiveOption);

        if (order.index != "0") {
            if (reprSign.match(order.name).hasMatch() || markReprint ) {
                order.reprint = true;
                if ( reprTrig.match( order.name).hasMatch() )
                    markReprint ^= 1;
            }
            if (urgSign.match(order.name).hasMatch() || markUrgent) {
                order.urgent = true;
                if ( urgTrig.match( order.name).hasMatch() )
                    markUrgent ^= 1;
            }
        } else {

            if (reprSign.match(order.name).hasMatch())
                markReprint = true;
            else if (markReprint)
                markReprint = false;

            if (urgSign.match(order.name).hasMatch())
                markUrgent = true;
            else if (markUrgent)
                markUrgent = false;

        }

        //save line data
        if (profile->orderInverted)
            lines.push_front(order);
        else
            lines.push_back(order);
    }
    if (lines.size() == 0)
        return false;

    return true;
}

bool RawReport::hasDubs(const QString lineName) const
{
    for (LogLine line : lines) {
        if (line.name == lineName)
            return true;
    }
    return false;
}




