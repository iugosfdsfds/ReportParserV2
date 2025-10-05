#ifndef LOGLINE_H
#define LOGLINE_H

#include <QString>

struct LogLine
{
    LogLine();
    LogLine(QString Name, int Count);

    QString index = "0";
    QString name = "Default";
    int count = 1;

    QString clearName = "Info";
    bool reprint = false;
    bool urgent = false;
    bool info = false;

    bool out = true;

    bool operator ==(const LogLine other) const;

    void updateData();
};

#endif // LOGLINE_H
