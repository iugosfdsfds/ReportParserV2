#include "config.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>

#define OUT_FILE "outFile"
#define LAST_USED "lastUsed"
#define LAST_FILES "lastUsedFiles"
#define LAST_PROFILE "lastUsedProfile"
#define COUNT_SIGN "countSign"
#define SPLITTER "splitter"
#define OUT_TO_CLIP "outToClip"
#define OUT_NAME "outName"
#define OUT_COUNT "outCount"
#define OUT_ATTRIBS "outAttribs"
#define LEFT_SHOWN "leftBoxShown"
#define PRESETS "presets"

bool Config::toJson(QString file)
{
    QFile config(file);

    if (!config.open(QIODevice::WriteOnly))
        return false;

    QJsonObject root;

    root.insert(OUT_FILE, outFile);

    QJsonArray lastFiles = QJsonArray::fromStringList(lastUsedFiles);

    root.insert(LAST_FILES, lastFiles);
    root.insert(LAST_PROFILE, lastUsedProfile);
    root.insert(COUNT_SIGN, countSign);
    root.insert(SPLITTER, splitter);
    root.insert(OUT_TO_CLIP, outToClip);
    root.insert(OUT_NAME, outName);
    root.insert(OUT_COUNT, outCount);
    root.insert(OUT_ATTRIBS, outAttribs);
    root.insert(LEFT_SHOWN, leftShown);

    QJsonArray Jpresets;
    for (QString preset : presets) {
        Jpresets.append(preset);
    }
    root.insert(PRESETS, Jpresets);

    QJsonDocument Jdoc(root);
    config.write(Jdoc.toJson());
    config.close();

    return true;
}

bool Config::fromJson(QString file)
{
    QFile config(file);

    if (!config.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument Jdoc = QJsonDocument::fromJson(config.readAll());
    config.close();

    QJsonObject root = Jdoc.object();

    if (root.contains(OUT_FILE))
        outFile = root.value(OUT_FILE).toString();

    if (root.contains((LAST_FILES))) {
        for (QVariant &v : root.value(LAST_FILES).toArray().toVariantList()) {
            lastUsedFiles.push_back(v.toString());
        }
    }

    //----- will be removed later
    if (root.contains(LAST_USED))
        lastUsedFiles.push_back(root.value(LAST_USED).toString());
    //-----

    if (root.contains(LAST_PROFILE))
        lastUsedProfile = root.value(LAST_PROFILE).toString();
    if (root.contains(COUNT_SIGN))
        countSign = root.value(COUNT_SIGN).toString();
    if (root.contains(SPLITTER))
        splitter = root.value(SPLITTER).toString();
    if (root.contains(OUT_TO_CLIP))
        outToClip = root.value(OUT_TO_CLIP).toBool();
    if (root.contains(OUT_NAME))
        outName = root.value(OUT_NAME).toBool();
    if (root.contains(OUT_COUNT))
        outCount = root.value(OUT_COUNT).toBool();
    if (root.contains(OUT_ATTRIBS))
        outAttribs = root.value(OUT_ATTRIBS).toBool();
    if (root.contains(LEFT_SHOWN))
        leftShown = root.value(LEFT_SHOWN).toBool();
    if (root.contains(PRESETS)) {
        QJsonArray Jpresets = root.value(PRESETS).toArray();
        for (int i = 0; i < Jpresets.size(); i++) {
            presets[i] = Jpresets.at(i).toString();
        }
    }

    return true;
}
