#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "./lib/profile.h"
#include "./lib/config.h"
#include "reportmodel.h"
#include "./dialogs/configwindow.h"
#include "./dialogs/profilesetter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector <int> getSelected(QTableView *table);

    void initTables();
    void deleteSelected(QTableView *table);

private slots:
    void needUpdate();

    void presetSelected();

    void on_pushButton_curClear_clicked();

    void on_pushButton_allClear_clicked();

    void on_pushButton_curDelete_clicked();

    void on_pushButton_allDelete_clicked();

    void on_pushButton_curAdd_clicked();

    void on_action_appCfg_triggered();

    void on_action_profileCfg_triggered();

    void on_checkBox_showLeft_stateChanged(int arg1);

    void on_pushButton_reload_clicked();

    void on_pushButton_selectFile_clicked();

    void on_pushButton_savePreset_clicked();

    void on_pushButton_reprint_clicked();

    void on_pushButton_urdent_clicked();

    void on_pushButton_info_clicked();

    void on_pushButton_clearStyle_clicked();

    void on_pushButton_curExport_clicked();

    void on_pushButton_allExport_clicked();

    void on_lineEdit_inFile_textChanged(const QString &arg1);

    void on_pushButton_join_clicked();

    void on_tableView_all_doubleClicked(const QModelIndex &index);

    void on_tableView_current_doubleClicked(const QModelIndex &index);

    void permitDelete();

    void on_pushButton_moveTop_clicked();

    void on_pushButton_moveUp_clicked();

    void on_pushButton_moveDown_clicked();

    void on_pushButton_moveBottom_clicked();

    void on_pushButton_insertNote_clicked();

    void on_action_open_triggered();

private:
    Ui::MainWindow *ui;

    ReportModel *currentOrders;
    ReportModel *allOrders;

    Profile *profile;
    Config *config;

    ConfigWindow *cfgWin;
    ProfileSetter *prfWin;

    QString configFileName;

    bool updateCurrent();

    int selectedPreset = -1;

    bool preventDelete = false;

    bool loadCSVFile();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
};
#endif // MAINWINDOW_H
