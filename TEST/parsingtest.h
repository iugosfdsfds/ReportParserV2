#ifndef PARSINGTEST_H
#define PARSINGTEST_H

#include <QObject>

#include <QTest>

class parsingTest : public QObject
{
    Q_OBJECT

private slots:
    void checkMS();
    void checkTJ();
    void checkWS();

};

#endif // PARSINGTEST_H
