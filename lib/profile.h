#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QPair>
#include <QVector>

struct Profile
{
    QString splitter = "\\n";
    QString encoding = "ANSI";

    QString nameStart = "\"0 ";
    QString nameEnd = "_000";
    int nameOffsets[2] = {0, 0}; // 0-nameStartOffset, 1-nameEndOffset
    QVector <QPair <QString, bool>> filters;

    bool allowDubs = true;
    bool orderInverted = false;

    QString countStart = "_";
    QString countEnd = "шт";
    int countOffsets[2] = {0, 0}; // 0-countStartOffset, 1-countEndOffset
    bool countsEnabled = true;

    bool fromJson(const QString file);
    bool toJson(const QString file);

};

#endif // PROFILE_H
