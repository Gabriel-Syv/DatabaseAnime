#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "definespath.h"
#include "dbalocalization.h"
#include "mngrconnection.h"
#include "mngrquerys.h"
#include "dbasettings.h"

#include "xmldbareader.h"
#include "xmldbawriter.h"

#include <QLocale>
#include <QDir>
#include <QFileDialog>
//#include <QVariant>

//#include <QMessageBox>
//#include <QDebug>

//#include <QProgressDialog>
//#include <QAbstractButton>


namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
private:
    Ui::Settings *ui;
    bool restoreDefault;
    MngrConnection MngrConnect;

    QPalette stylePalette;
public:
    explicit Settings(MngrConnection &MngrCon, QWidget *parent = 0);
    ~Settings();

    Sort::sort getSort();
    Tables::UniformField::field getDisplaydField();
    bool getSwitchToDir();
    bool getRestoreDefault();
    QLocale::Language getLanguage();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void BtnBox_resetDefaults();
    void on_PBtn_Action_Export_clicked();
    void on_TBtn_Export_Path_Choose_clicked();
    void on_TBtn_Import_Path_Choose_clicked();
    void on_PBtn_Import_Append_clicked();
    void on_PBtn_Import_Replace_clicked();
    unsigned long long on_actionImport_triggered();
    bool on_actionDeleteRecords_triggered();
    void on_TBtn_WorkDir_Choose_clicked();
    void on_actionShowImportProgressBar_triggered(bool checked);
    void on_actionShowExportProgressBar_triggered(bool checked);
    void on_toolButton_Window_clicked();
    void on_toolButton_TextLabel_clicked();
    void on_toolButton_Base_clicked();
    void on_toolButton_AlternateBase_clicked();
    void on_toolButton_ToolTipBase_clicked();
    void on_toolButton_ToolTipText_clicked();
    void on_toolButton_Text_clicked();
    void on_toolButton_Button_clicked();
    void on_toolButton_ButtonText_clicked();
    void on_toolButton_BrightText_clicked();
    void on_toolButton_Link_clicked();
    void on_toolButton_Highlight_clicked();
    void on_toolButton_HighlightedText_clicked();
    void on_pushButton_clicked();
};

#endif // SETTINGS_H
