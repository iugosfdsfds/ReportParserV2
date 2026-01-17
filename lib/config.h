#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QStringList>
#include <QVector>

struct Config
{
    QString outFile = "";

    QString lastUsedFile = "";
    QStringList lastUsedFiles;
    QString lastUsedProfile = "";

    QString countSign = "";
    QString splitter = ";";

    bool outToClip = false;

    bool outName = true;
    bool outCount = true;
    bool outAttribs = true;

    bool leftShown = true;

    QString presets[7] = {"", "", "", "", "", "", ""};

    bool toJson(QString file);
    bool fromJson(QString file);

};

#endif // CONFIG_H
