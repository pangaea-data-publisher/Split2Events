#ifndef PARAMETERDBOPTIONSDIALOG_H
#define PARAMETERDBOPTIONSDIALOG_H

#include <QDialog>

#include "ui_ParameterDBOptionsDialog.h"

class ParameterDBOptionsDialog : public QDialog, public Ui::ParameterDBOptionsDialog
{
Q_OBJECT

public:
    ParameterDBOptionsDialog( QWidget *parent = 0 );
	QString getPersonalDir();

private slots:
    void browseDirDialog();
};

#endif
