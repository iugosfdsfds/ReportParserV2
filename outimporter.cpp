#include "outimporter.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>

OutImporter::OutImporter() {}

QVector<LogLine> OutImporter::import(QWidget *parent, QString path)
{
    QString fileName = QFileDialog::getOpenFileName(parent, "Выберите файл", path, "CSV | *.csv");
    if (fileName.isEmpty())
        return QVector <LogLine> ();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return QVector <LogLine> ();

    QTextStream in(&file);

    QStringList textLines;
    while (!in.atEnd()) {
        textLines.push_back(in.readLine());
    }

    QVector <LogLine> lines;

    for (QString& line : textLines) {
        QStringList l = line.split(';');
        LogLine temp(l.at(0), l.at(1).toInt());
        temp.reprint = l.at(2) == "1" ? true : false;
        temp.urgent = l.at(3) == "1" ? true : false;
        temp.info = l.at(4) == "0" ? true : false; // !!!!! inverted !!!!!!

        lines.push_back(temp);
    }

    return lines;
}
