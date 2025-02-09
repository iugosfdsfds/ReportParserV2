#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>
#include "./lib/config.h"

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr, Config *config = nullptr, QString configFileName = "");
    ~ConfigWindow();

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_selectOut_clicked();

private:
    Ui::ConfigWindow *ui;

    Config *_config;
    QString configFile;

    void applySettings();

    void updateLines();
};

#endif // CONFIGWINDOW_H
