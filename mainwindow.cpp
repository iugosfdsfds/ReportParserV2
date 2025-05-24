#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QMessageBox>

#include "rawreport.h"
#include "outimporter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    configFileName = QCoreApplication::applicationDirPath() + "/config.json";
    config = new Config();
    if (config->fromJson(configFileName))
        ui->statusbar->showMessage("Настройки загружены", 3000);
    else
        ui->statusbar->showMessage("Ошибка загрузки настроек", 3000);

    profile = new Profile();
    if (config->lastUsedProfile.isEmpty() || config->lastUsedProfile.isNull()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Профиль не был загружен");
        msgBox.exec();
    } else {
        if (!profile->fromJson(config->lastUsedProfile)) {
            QMessageBox msgBox;
            msgBox.setText("Внимание! Профиль был загружен с ошибкой");
            msgBox.exec();
        }
    }

    cfgWin = new ConfigWindow(this, config, configFileName);
    prfWin = new ProfileSetter(this, profile, config);

    connect(prfWin, SIGNAL(applied()), this, SLOT(needUpdate()));

    currentOrders = new ReportModel();
    allOrders = new ReportModel();

    initTables();

    ui->checkBox_showLeft->setChecked(config->leftShown);
    if (!config->leftShown)
        ui->leftBox->hide();

    if (config->lastUsedFile != "") {
        ui->lineEdit_inFile->setText(config->lastUsedFile);
        updateCurrent();
    }

    connect(ui->radioButton_1, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_4, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_5, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_6, SIGNAL(clicked()), this, SLOT(presetSelected()));
    connect(ui->radioButton_7, SIGNAL(clicked()), this, SLOT(presetSelected()));

    connect(allOrders, SIGNAL(editingFinished()), this, SLOT(permitDelete()));
    connect(currentOrders, SIGNAL(editingFinished()), this, SLOT(permitDelete()));
}

MainWindow::~MainWindow()
{
    config->toJson(configFileName);

    delete currentOrders;
    delete allOrders;
    delete profile;
    delete config;
    delete cfgWin;
    delete prfWin;
    delete ui;
}

void MainWindow::initTables()
{
    ui->tableView_current->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_current->setModel(currentOrders);
    ui->tableView_current->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableView_current->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    ui->tableView_all->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_all->setModel(allOrders);
    ui->tableView_all->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableView_all->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

}

void MainWindow::deleteSelected(QTableView *table)
{
    QVector <int> indexes = getSelected(table);
    if (indexes.size() == 0) {
        ui->statusbar->showMessage("Необходимо выбрать строки для удаления", 3000);
        return;
    }
    for (int i = indexes.last(); i >= indexes.first(); i--) {
        table->model()->removeRows(i, 1, QModelIndex());
    }
    ui->statusbar->showMessage("Удалено строк: " + QString::number(indexes.size()), 3000);
}

QVector<int> MainWindow::getSelected(QTableView *table)
{
    QItemSelectionModel * selection = table->selectionModel();

    QVector <int> indexes;
    for (auto index : selection->selectedRows()) {
        if (!indexes.contains(index.row()))
            indexes.push_back(index.row());
    }
    return indexes;
}

bool MainWindow::updateCurrent()
{
    RawReport rep(config->lastUsedFile, profile);
    ui->lineEdit_inFile->setText(config->lastUsedFile);
    if (!rep.parsed()) {
        currentOrders->clearAll();
        return false;
    }
    currentOrders->setLines(&rep);
    return true;
}

bool MainWindow::loadCSVFile()
{


    QVector <LogLine> lines = OutImporter::import(this, config->outFile);

    allOrders->clearAll();
    allOrders->addLines(lines);
    allOrders->updateView();

    return true;
}

//slots

void MainWindow::needUpdate()
{
    if (updateCurrent())
        ui->statusbar->showMessage("Файл прочитан", 3000);
}

void MainWindow::presetSelected()
{
    selectedPreset = sender()->objectName().replace("radioButton_", "").toInt()-1;
    if (config->presets->size() == 0)
        return;
    config->lastUsedFile = config->presets[selectedPreset];
    if (updateCurrent())
        ui->statusbar->showMessage("Файл прочитан", 3000);
    else
        ui->statusbar->showMessage("Файл не прочитан", 3000);
}

void MainWindow::on_pushButton_curClear_clicked()
{
    currentOrders->clearAll();
}

void MainWindow::on_pushButton_allClear_clicked()
{
    allOrders->clearAll();
}

void MainWindow::on_pushButton_curDelete_clicked()
{
    deleteSelected(ui->tableView_current);
}

void MainWindow::on_pushButton_allDelete_clicked()
{
    deleteSelected(ui->tableView_all);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        if (!preventDelete) {
            deleteSelected(ui->tableView_current);
            deleteSelected(ui->tableView_all);
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        config->lastUsedFile = event->mimeData()->urls()[0].toLocalFile();
        if (updateCurrent())
            ui->statusbar->showMessage("Файл прочитан", 3000);
        else
            ui->statusbar->showMessage("Файл не прочитан", 3000);
    }
}

void MainWindow::on_pushButton_curAdd_clicked()
{
    QVector <int> indexes = getSelected(ui->tableView_current);
    if (indexes.size() == 0)
        allOrders->addLines(currentOrders->getLines());
    else
    {
        QVector <LogLine> newLines;
        for (int index : indexes) {
            newLines.push_back(currentOrders->getAt(index));
        }
        allOrders->addLines(newLines);
    }
}

void MainWindow::on_action_appCfg_triggered()
{
    cfgWin->exec();
}

void MainWindow::on_action_profileCfg_triggered()
{
    prfWin->exec();
}

void MainWindow::on_checkBox_showLeft_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if (ui->checkBox_showLeft->isChecked())
        ui->leftBox->show();
    else
        ui->leftBox->hide();
    config->leftShown = ui->checkBox_showLeft->isChecked();
}

void MainWindow::on_pushButton_reload_clicked()
{
    if (updateCurrent())
        ui->statusbar->showMessage("Файл прочитан", 3000);
}

void MainWindow::on_pushButton_selectFile_clicked()
{
    QString dir = QString();
    if (config->lastUsedFile != "")
        dir = QFileInfo(config->lastUsedFile).filePath();
    QString file = QFileDialog::getOpenFileName(this, "Открыть файл", dir);
    if (file == "")
        return;

    config->lastUsedFile = file;
    if (updateCurrent()) {
        ui->statusbar->showMessage("Файл прочитан", 3000);
    } else
        ui->statusbar->showMessage("Файл не прочитан", 3000);
}

void MainWindow::on_pushButton_savePreset_clicked()
{
    if (selectedPreset != -1)
        config->presets[selectedPreset] = config->lastUsedFile;
    ui->statusbar->showMessage("Пресет записан", 3000);
}

void MainWindow::on_pushButton_reprint_clicked()
{
    QVector <int> indexes;
    indexes = getSelected(ui->tableView_all);
    for (int i : indexes) {
        allOrders->changeReprint(i);
    }

    indexes = getSelected(ui->tableView_current);
    for (int i : indexes) {
        currentOrders->changeReprint(i);
    }

    ui->statusbar->showMessage("Отмечено как ДОП", 3000);
}

void MainWindow::on_pushButton_urdent_clicked()
{
    QVector <int> indexes;
    indexes = getSelected(ui->tableView_all);
    for (int i : indexes) {
        allOrders->changeUrgent(i);
    }

    indexes = getSelected(ui->tableView_current);
    for (int i : indexes) {
        currentOrders->changeUrgent(i);
    }

    ui->statusbar->showMessage("Отмечено как СРОЧНО", 3000);
}

void MainWindow::on_pushButton_info_clicked()
{
    QVector <int> indexes;
    indexes = getSelected(ui->tableView_all);
    for (int i : indexes) {
        allOrders->changeInfo(i);
    }

    indexes = getSelected(ui->tableView_current);
    for (int i : indexes) {
        currentOrders->changeInfo(i);
    }
}

void MainWindow::on_pushButton_clearStyle_clicked()
{
    allOrders->resetStyles();
    currentOrders->resetStyles();

    currentOrders->updateView();
    allOrders->updateView();
}

void MainWindow::on_pushButton_curExport_clicked()
{
    int status = currentOrders->toOut(config, getSelected(ui->tableView_current));
    if (status == 0)
        ui->statusbar->showMessage("Файл сгенерирован", 3000);
    else if (status == 2)
        ui->statusbar->showMessage("Выводной файл не указан", 3000);
}

void MainWindow::on_pushButton_allExport_clicked()
{
    int status = allOrders->toOut(config, getSelected(ui->tableView_all));
    if (status == 0)
        ui->statusbar->showMessage("Файл сгенерирован", 3000);
    else if (status == 2)
        ui->statusbar->showMessage("Выводной файл не указан", 3000);
}

void MainWindow::on_lineEdit_inFile_textChanged(const QString &arg1)
{
    config->lastUsedFile = arg1;
    updateCurrent();
}

void MainWindow::on_pushButton_join_clicked()
{
    QPair <int, int> doneAll;
    QPair <int, int> doneCur = currentOrders->mergeLines( getSelected(ui->tableView_current) );
    if (config->leftShown)
        doneAll = allOrders->mergeLines( getSelected(ui->tableView_all) );

    QString out = "Объединено: ";
    if (config->leftShown)
        out.push_back( QString::number(doneAll.first) + " -> " + QString::number(doneAll.second) + "  |  ");
    out.push_back( QString::number(doneCur.first) + " -> " + QString::number(doneCur.second) );
    ui->statusbar->showMessage(out, 3000);
}

void MainWindow::on_tableView_all_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    preventDelete = true;
}

void MainWindow::on_tableView_current_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    preventDelete = true;
}

void MainWindow::permitDelete()
{
    preventDelete = false;
}

void MainWindow::on_pushButton_moveTop_clicked()
{
    QVector <int> indexes = getSelected(ui->tableView_all);
    if (indexes.first() == 0) return;
    allOrders->moveLines(getSelected(ui->tableView_all), true, true);
    ui->tableView_all->selectionModel()->select(QItemSelection(allOrders->index(0, 0), allOrders->index(indexes.last()-indexes.first(), 1)), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_pushButton_moveBottom_clicked()
{
    QVector <int> indexes = getSelected(ui->tableView_all);
    if (indexes.last() == allOrders->getSize()-1) return;
    allOrders->moveLines(getSelected(ui->tableView_all), false, true);
    ui->tableView_all->selectionModel()->select(QItemSelection(allOrders->index(allOrders->getSize()-1-(indexes.last()-indexes.first()), 0), allOrders->index(allOrders->getSize()-1, 1)), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_pushButton_moveUp_clicked()
{
    QVector <int> indexes = getSelected(ui->tableView_all);
    if (indexes.first() == 0) return;
    allOrders->moveLines(indexes, true);
    ui->tableView_all->selectionModel()->select(QItemSelection(allOrders->index(indexes.first()-1, 0), allOrders->index(indexes.last()-1, 1)), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_pushButton_moveDown_clicked()
{
    QVector <int> indexes = getSelected(ui->tableView_all);
    if (indexes.last() == allOrders->getSize()-1) return;
    allOrders->moveLines(getSelected(ui->tableView_all), false);
    ui->tableView_all->selectionModel()->select(QItemSelection(allOrders->index(indexes.first()+1, 0), allOrders->index(indexes.last()+1, 1)), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_pushButton_insertNote_clicked()
{
    if (ui->lineEdit_addNote->text().isEmpty()) return;
    LogLine temp(ui->lineEdit_addNote->text(), 1);
    allOrders->addLines(temp);
}

void MainWindow::on_action_open_triggered()
{
    loadCSVFile();
}

