#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
private slots:
    void on_SettingsDialog_accepted();
    void reloadSettings();

    void on_SettingsDialog_rejected();

private:
    Ui::SettingsDialog *ui;

    void populatePlugins();
};

#endif // SETTINGSDIALOG_H
