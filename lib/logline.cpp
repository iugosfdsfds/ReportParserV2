#include "logline.h"

#include <QStringList>

LogLine::LogLine()
{
}

LogLine::LogLine(QString Name, int Count)
{
    name = Name;
    count = Count;

    updateData();
}

bool LogLine::operator ==(const LogLine other) const
{
    return ((this->index == other.index) && (this->clearName == other.clearName) && (this->index == other.index));
}

void LogLine::updateData()
{
    QString normName = name;
    normName.replace(" ", "_");
    index = normName.split('_').at(0);
    bool ok = false;
    QString temp = index;
    temp.toInt(&ok);
    if (ok) {
        clearName = name;
        clearName.remove(index+'_');
        clearName = clearName.section("", 0, clearName.indexOf("__"));
        info = false;
    } else {
        index = "0";
        clearName = "Info";
        info = true;
    }
}
