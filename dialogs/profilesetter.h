#ifndef PROFILESETTER_H
#define PROFILESETTER_H

#include <QDialog>

#include "./lib/profile.h"
#include "./lib/config.h"
#include <QDir>

namespace Ui {
class ProfileSetter;
}

class ProfileSetter : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileSetter(QWidget *parent = nullptr, Profile *pro = nullptr, Config *cfg = nullptr);
    ~ProfileSetter();

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_load_clicked();

    void on_pushButton_save_clicked();

signals:
    void applied();

private:
    Ui::ProfileSetter *ui;

    Profile *profile;
    Config *config;

    void updateGUI();

    void updateProfile();

};

#endif // PROFILESETTER_H
