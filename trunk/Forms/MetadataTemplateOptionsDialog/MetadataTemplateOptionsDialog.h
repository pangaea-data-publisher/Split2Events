#ifndef METADATATEMPLATEOPTIONSDIALOG_H
#define METADATATEMPLATEOPTIONSDIALOG_H

#include <QDialog>

#include "ui_metadatatemplateoptionsdialog.h"

class MetadataTemplateOptionsDialog : public QDialog, public Ui::MetadataTemplateOptionsDialog
{
    Q_OBJECT

public:
    MetadataTemplateOptionsDialog( QWidget *parent = 0 );
};

#endif
