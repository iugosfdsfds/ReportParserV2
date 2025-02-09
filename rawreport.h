#ifndef RAWREPORT_H
#define RAWREPORT_H

#include <QVector>

#include ".\lib\logline.h"
#include ".\lib\profile.h"

class RawReport
{
public:
    RawReport(QString logFile, Profile* profile);

    bool parsed() const;

    QVector <LogLine> getLines() const;
    LogLine getLine(int line) const;

private:
    Profile* profile;

    QVector <LogLine> lines;
    bool ok = false;

    bool filtersOK(QString text);

    bool parseFile(QString file);

    bool hasDubs(const QString lineName) const;

};

#endif // RAWREPORT_H
