// ***********************************************************************************************
// *                                                                                             *
// * SetEncodingDialog.cpp - Sets encoding of datasets                                           *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-01-03                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "SetEncodingDialog.h"

SetEncodingDialog::SetEncodingDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doSetEncodingDialog()
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    SetEncodingDialog *dialog = new SetEncodingDialog();

    dialog->setCodecComboBox->setCurrentIndex( gi_Codec );

    dialog->OK_pushButton->setWhatsThis( "Close dialog" );
    dialog->Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog->adjustSize();
    dialog->move( posDialog );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        gi_Codec    	= dialog->setCodecComboBox->currentIndex();
        i_DialogResult  = QDialog::Accepted;
    }

    posDialog  = dialog->pos();

    delete dialog;
}
