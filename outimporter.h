#ifndef OUTIMPORTER_H
#define OUTIMPORTER_H

#include <QWidget>
#include "lib/logline.h"
#include <QVector>

class OutImporter
{
public:
    OutImporter();

    static QVector <LogLine> import(QWidget* parent, QString path);

};

#endif // OUTIMPORTER_H
