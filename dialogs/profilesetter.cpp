#include "profilesetter.h"
#include "ui_profilesetter.h"

#include <QFile>
#include <QFileDialog>

#include <QStringList>

ProfileSetter::ProfileSetter(QWidget *parent, Profile *pro, Config *cfg) :
    QDialog(parent),
    ui(new Ui::ProfileSetter)
{
    ui->setupUi(this);
    profile = pro;
    config = cfg;

    updateGUI();
}

ProfileSetter::~ProfileSetter()
{
    delete ui;
}

void ProfileSetter::updateGUI()
{
    ui->lineEdit_splitter->setText(profile->splitter);

    ui->lineEdit_nameStart->setText(profile->nameStart);
    ui->lineEdit_nameEnd->setText(profile->nameEnd);
    QString temp = "";
    for (auto filter : profile->filters) {
        if (filter.second)
            temp += "!";
        temp += filter.first;
        temp += ";";
    }
    ui->lineEdit_filter->setText(temp);
    ui->checkBox_allowDub->setChecked(profile->allowDubs);
    ui->checkBox_findMarks->setChecked(profile->lookForMarkers);

    ui->checkBox_countEnabled->setChecked(profile->countsEnabled);
    ui->lineEdit_countStart->setText(profile->countStart);
    ui->lineEdit_countEnd->setText(profile->countEnd);

    ui->spinBox_offNS->setValue(profile->nameOffsets[0]);
    ui->spinBox_offNE->setValue(profile->nameOffsets[1]);
    ui->spinBox_offCS->setValue(profile->countOffsets[0]);
    ui->spinBox_offCE->setValue(profile->countOffsets[1]);

    ui->checkBox_invertedOrder->setChecked(profile->orderInverted);
    if (config->lastUsedProfile != "")
        ui->label_loaded->setText("Загруженный профиль: " + config->lastUsedProfile);
}

void ProfileSetter::updateProfile()
{
    profile->filters.clear();

    profile->splitter = ui->lineEdit_splitter->text();

    profile->nameStart = ui->lineEdit_nameStart->text();
    profile->nameEnd = ui->lineEdit_nameEnd->text();
    QString temp = ui->lineEdit_filter->text();
    QStringList filters = temp.split(";");
    for (auto filter : filters) {
        if (filter == "")
            continue;
        bool inverted = filter.startsWith("!");
        if (inverted) filter = filter.remove(0, 1);
        profile->filters.push_back( {filter, inverted} );
    }
    profile->allowDubs = ui->checkBox_allowDub->isChecked();
    profile->lookForMarkers = ui->checkBox_findMarks->isChecked();

    profile->countsEnabled = ui->checkBox_countEnabled->isChecked();
    profile->countStart = ui->lineEdit_countStart->text();
    profile->countEnd = ui->lineEdit_countEnd->text();

    profile->nameOffsets[0] = ui->spinBox_offNS->value();
    profile->nameOffsets[1] = ui->spinBox_offNE->value();
    profile->countOffsets[0] = ui->spinBox_offCS->value();
    profile->countOffsets[1] = ui->spinBox_offCE->value();

    profile->orderInverted = ui->checkBox_invertedOrder->isChecked();
}

void ProfileSetter::on_pushButton_apply_clicked()
{
    updateProfile();
    emit(applied());
    this->close();
}

void ProfileSetter::on_pushButton_load_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Выберите файл шаблона", QDir::homePath(), "JSON (*.json)");
    if (file == "")
        return;

    profile->fromJson(file);
    updateGUI();
    config->lastUsedProfile = file;
}

void ProfileSetter::on_pushButton_save_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Сохранить шаблон", QDir::homePath(), "JSON (*.json)");
    if (file == "")
        return;

    updateProfile();
    profile->toJson(file);
    config->lastUsedProfile = file;
}
