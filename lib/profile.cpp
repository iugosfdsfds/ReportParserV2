#include "profile.h"

#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#define SPLITTER "splitter"

#define NAME_START "nameStart"
#define NAME_END "nameEnd"
#define NAME_OFFSETS "nameOffsets"
#define FILTERS "filters"

#define ALLOW_DUBS "allowDubs"

#define COUNT_START "countStart"
#define COUNT_END "countEnd"
#define COUNT_OFFSETS "countOffsets"
#define COUNTS_ENABLED "countsEnabled"
#define MARKERS_ENABLED "markersEnabled"

#define ORDER_INVERTED "invertedListOrder"

bool Profile::fromJson(const QString file)
{
    //reading file
    QFile configFile(file);

    if (!configFile.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument Jconfig = QJsonDocument::fromJson(configFile.readAll());
    configFile.close();

    //converting from json

    QJsonObject root(Jconfig.object());

    //general config
    if (root.contains(SPLITTER))
        splitter = root.value(SPLITTER).toString();

    //name parsing config
    if (root.contains(NAME_START))
        nameStart = root.value(NAME_START).toString();
    if (root.contains(NAME_END))
        nameEnd = root.value(NAME_END).toString();
    if (root.contains(NAME_OFFSETS)) {
        QJsonArray JnameOffsets;
        JnameOffsets = root.value(NAME_OFFSETS).toArray();
        nameOffsets[0] = JnameOffsets[0].toInt();
        nameOffsets[1] = JnameOffsets[1].toInt();
    }
    filters.clear();
    if (root.contains(FILTERS)) {
        QJsonArray Jfilters;
        Jfilters = root.value(FILTERS).toArray();
        for (const QJsonValue & jf : Jfilters) {
            QJsonArray Jfilt = jf.toArray();
            filters.push_back( {Jfilt[0].toString(), Jfilt[1].toBool() } );
        }
    }
    if (root.contains(ALLOW_DUBS))
        allowDubs = root.value(ALLOW_DUBS).toBool();

    if (root.contains(MARKERS_ENABLED))
        lookForMarkers = root.value(MARKERS_ENABLED).toBool();

    if (root.contains(ORDER_INVERTED))
        orderInverted = root.value(ORDER_INVERTED).toBool();

    //count config
    if (root.contains(COUNT_START))
        countStart = root.value(COUNT_START).toString();
    if (root.contains(COUNT_END))
        countEnd = root.value(COUNT_END).toString();
    QJsonArray JcountOffsets;
    if (root.contains(COUNT_OFFSETS))
    {
        JcountOffsets = root.value(COUNT_OFFSETS).toArray();
        countOffsets[0] = JcountOffsets[0].toInt();
        countOffsets[1] = JcountOffsets[1].toInt();
    }
    if (root.contains(COUNTS_ENABLED))
            countsEnabled = root.value(COUNTS_ENABLED).toBool();

    return true;
}

bool Profile::toJson(const QString file) //loading from file
{
    QFile configFile(file);

    if (!configFile.open(QIODevice::WriteOnly)) //failsafe
        return false;

    //creating json data
    QJsonObject root;

    //general config
    root.insert(SPLITTER, splitter);

    //name parsing config
    root.insert(NAME_START, nameStart);
    root.insert(NAME_END, nameEnd);

    QJsonArray JnameOffsets;
    JnameOffsets.append(nameOffsets[0]);
    JnameOffsets.append(nameOffsets[1]);
    root.insert(NAME_OFFSETS, JnameOffsets);

    QJsonArray Jfilters;
    for (QPair <QString, bool> filter : filters) {
        QJsonArray Jfilter;
        Jfilter.append(filter.first);
        Jfilter.append(filter.second);
        Jfilters.append(Jfilter);
    }
    root.insert(FILTERS, Jfilters);

    root.insert(ALLOW_DUBS, allowDubs);

    root.insert(MARKERS_ENABLED, lookForMarkers);

    root.insert(ORDER_INVERTED, orderInverted);

    //counts config
    root.insert(COUNT_START, countStart);
    root.insert(COUNT_END, countEnd);

    QJsonArray JcountOffsets;
    JcountOffsets.append(countOffsets[0]);
    JcountOffsets.append(countOffsets[1]);
    root.insert(COUNT_OFFSETS, JcountOffsets);

    root.insert(COUNTS_ENABLED, countsEnabled);

    //writing to file
    QJsonDocument Jconfig(root);

    configFile.write(Jconfig.toJson());
    configFile.close();

    return true;
}
