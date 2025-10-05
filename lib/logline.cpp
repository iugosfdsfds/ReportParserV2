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
    return ((this->index == other.index) && (this->clearName == other.clearName));
}

void LogLine::updateData()
{
    name.replace(" ", "_");
    name.replace("(", "_");
    name.replace(")", "_");

    index = name.split('_').at(0);
    bool ok = false;
    index.toInt(&ok);
    if (ok && !index.isEmpty()) {
        clearName = name;
        clearName.remove(index+"_");
        clearName = clearName.section("", 0, clearName.indexOf("__"));
    } else {
        index = "0";
        clearName = "Info";
    }
}
