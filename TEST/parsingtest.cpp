#include "parsingtest.h"

#include "profile.h"
#include "rawreport.h"

void parsingTest::checkMS() {
    Profile* profile = new Profile();
    profile->fromJson("src/MS.json");

    RawReport rp("src/MS_log.xml", profile);

    QString ref = "04234_1_BABOCHKI_I_VETVI_ROZ_GABARDIN_CH2_2_267__GEORGE_711";
    QString sample = rp.getLine(0).name;

    QCOMPARE(ref, sample);

    delete profile;
}

void parsingTest::checkTJ() {
    Profile* profile = new Profile();
    profile->fromJson("src/TJ.json");

    RawReport rp("src/TJ_log.txt", profile);

    QString ref = "00936_6_babochki_u_vodi_cherno_belie_gabardin_ch2-2_267__oleg_.tif";
    QString sample = rp.getLine(0).name;

    QCOMPARE(ref, sample);

    delete profile;
}

void parsingTest::checkWS() {
    Profile* profile = new Profile();
    profile->fromJson("src/Wasatch.json");

    RawReport rp("src/Wasatch_log.2", profile);

    QString ref = "146_мп_1";
    QString sample = rp.getLine(0).name;

    QCOMPARE(ref, sample);

    delete profile;
}
