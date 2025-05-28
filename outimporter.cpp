#include "outimporter.h"

#include <QFile>
#include <QDataStream>
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

    QDataStream in(&file);

    QStringList textLines;
    QByteArray data;
    char buf;
    while (!in.atEnd()) {
        in.readRawData(&buf, sizeof(buf));
        if (buf == '\n') {
            QString temp = QString::fromUtf8(data);
            temp.remove('\r');
            textLines.push_back(temp);
            data.clear();
        } else {
            data.push_back(buf);
        }
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
