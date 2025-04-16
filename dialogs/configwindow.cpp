#include "configwindow.h"
#include "ui_configwindow.h"

#include <QFileDialog>
#include <QStandardPaths>

#include "version.h"

ConfigWindow::ConfigWindow(QWidget *parent, Config *config, QString configFileName) :
    QDialog(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    _config = config;
    configFile = configFileName;
    ui->label_verison->setText("v"+QString(APP_VERSION));

    updateLines();
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::updateLines()
{
    ui->lineEdit_outFile->setText(_config->outFile);
    ui->lineEdit_splitter->setText(_config->splitter);
    ui->lineEdit_countSign->setText(_config->countSign);
    if (_config->outToClip) {
        ui->radioButton_fileOut->setChecked(false);
        ui->radioButton_clipOut->setChecked(true);
    } else {
        ui->radioButton_fileOut->setChecked(true);
        ui->radioButton_clipOut->setChecked(false);
    }
    ui->checkBox_name->setChecked(_config->outName);
    ui->checkBox_count->setChecked(_config->outCount);
    ui->checkBox_atrib->setChecked(_config->outAttribs);
}

void ConfigWindow::applySettings()
{
    _config->outFile = ui->lineEdit_outFile->text();
    _config->splitter = ui->lineEdit_splitter->text();
    _config->countSign = ui->lineEdit_countSign->text();
    if (ui->radioButton_fileOut->isChecked()) _config->outToClip = false;
    else if (ui->radioButton_clipOut->isChecked()) _config->outToClip = true;
    _config->outName = ui->checkBox_name->isChecked();
    _config->outCount = ui->checkBox_count->isChecked();
    _config->outAttribs = ui->checkBox_atrib->isChecked();
}

void ConfigWindow::on_pushButton_apply_clicked()
{
    applySettings();
    if (configFile != "")
        _config->toJson(configFile);
    this->close();
}

void ConfigWindow::on_pushButton_selectOut_clicked()
{
    QString _outFileName = QFileDialog::getSaveFileName(this, "Укажите размещение out файла", QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first(), "CSV (*.csv);;Текстовый файл (*.txt);;Иное");
    if (_outFileName != "") {
        ui->lineEdit_outFile->setText(_outFileName);
    }
}
