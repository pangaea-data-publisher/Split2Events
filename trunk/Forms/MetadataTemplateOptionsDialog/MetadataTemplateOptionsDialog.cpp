#include <QtWidgets>

#include "Application.h"
#include "MetadataTemplateOptionsDialog.h"

MetadataTemplateOptionsDialog::MetadataTemplateOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

