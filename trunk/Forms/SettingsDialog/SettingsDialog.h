#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "ui_settingsdialog.h"

class SettingsDialog : public QDialog, public Ui::SettingsDialog
{
Q_OBJECT

public:
    SettingsDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseDirDialog();
};

#endif
