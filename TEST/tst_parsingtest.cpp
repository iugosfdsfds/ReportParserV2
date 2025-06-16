#include <QTest>

#include "../lib/profile.h"
#include "../rawreport.h"

class ParsingTest : public QObject
{
    Q_OBJECT

public:
    ParsingTest() {};
    ~ParsingTest() {
        delete profile;
    };

private:
    Profile* profile;

private slots:
    void checkMS() {
        profile = new Profile();
        profile->fromJson("../../profiles/MS.json");

        RawReport rp("../src/MS_log.xml", profile);

        QString ref = "04234_1_BABOCHKI_I_VETVI_ROZ_GABARDIN_CH2_2_267__GEORGE_711";
        QString sample = QString();
        if (rp.parsed())
            sample = rp.getLine(0).name;

        QCOMPARE(ref, sample);
    }

    void checkTJ() {
        profile = new Profile();
        profile->fromJson("../../profiles/TruJet.json");

        RawReport rp("../src/TJ_log.txt", profile);

        QString ref = "00936_6_babochki_u_vodi_cherno_belie_gabardin_ch2-2_267__oleg_.tif";
        QString sample = QString();
        if (rp.parsed())
            sample = rp.getLine(0).name;

        QCOMPARE(ref, sample);
    }

    void checkWS() {
        profile = new Profile();
        profile->fromJson("../../profiles/Wasatch.json");

        RawReport rp("../src/Wasatch_log.2", profile);

        QString ref = "146_мп_1";
        QString sample = QString();
        if (rp.parsed())
            sample = rp.getLine(0).name;

        QCOMPARE(ref, sample);
    }
};

QTEST_APPLESS_MAIN(ParsingTest)

#include "tst_parsingtest.moc"
