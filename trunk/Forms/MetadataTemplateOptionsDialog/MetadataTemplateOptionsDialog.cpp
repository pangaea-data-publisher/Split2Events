#include <QtWidgets>

#include "Application.h"
#include "MetadataTemplateOptionsDialog.h"

MetadataTemplateOptionsDialog::MetadataTemplateOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT(accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT(reject() ) );
    connect( writeParameterImportFileCheckBox, SIGNAL( toggled( bool ) ), this, SLOT( enableAddSpeciesColumnCheckbox() ) );

}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void MetadataTemplateOptionsDialog::enableAddSpeciesColumnCheckbox()
{
    if ( this->writeParameterImportFileCheckBox->isChecked() == true )
        this->addSpeciesNamesCheckBox->setEnabled( true );
    else
        this->addSpeciesNamesCheckBox->setEnabled( false );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void MainWindow::doMetadataTemplateOptionsDialog()
{
    MetadataTemplateOptionsDialog dialog( this );

    if ( gsl_FilenameList.isEmpty() == true )
        chooseFiles();

    if ( gsl_FilenameList.isEmpty() == false )
        dialog.setWindowTitle( tr( "Split2Events - create metadata template for [%1]" ).arg( QDir::toNativeSeparators( gsl_FilenameList.at( 0 ) ) ) );
    else
        return;

    switch ( gi_MetadataFileMode )
    {
    case _AUTO_:
    case _BYPOSITION_:
        dialog.findByPositionRadioButton->setChecked( true );
        break;
    case _BYNAMEABBR_:
        dialog.findByNameOrAbbreviationRadioButton->setChecked( true );
        break;
    case _BYNAME_:
        dialog.findByNameOnlyRadioButton->setChecked( true );
        break;
    }

    if ( gb_writeParameterImportFile == true )
    {
        dialog.writeParameterImportFileCheckBox->setChecked( true );
        dialog.addSpeciesNamesCheckBox->setEnabled( true );
    }
    else
    {
        dialog.writeParameterImportFileCheckBox->setChecked( false );
        dialog.addSpeciesNamesCheckBox->setEnabled( false );
    }

    if ( gb_match_against_WoRMS == true )
        dialog.addSpeciesNamesCheckBox->setChecked( true );
    else
        dialog.addSpeciesNamesCheckBox->setChecked( false );

    dialog.OK_pushButton->setWhatsThis( "Create metadata template" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        if ( dialog.findByPositionRadioButton->isChecked() )
            gi_MetadataFileMode = _BYPOSITION_;

        if ( dialog.findByNameOrAbbreviationRadioButton->isChecked() )
            gi_MetadataFileMode = _BYNAMEABBR_;

        if ( dialog.findByNameOnlyRadioButton->isChecked() )
            gi_MetadataFileMode = _BYNAME_;

        if ( dialog.writeParameterImportFileCheckBox->isChecked() )
            gb_writeParameterImportFile = true;
        else
            gb_writeParameterImportFile = false;

        if ( gb_writeParameterImportFile == true )
        {
            if ( dialog.addSpeciesNamesCheckBox->isChecked() )
                gb_match_against_WoRMS = true;
            else
                gb_match_against_WoRMS = false;
        }

        doCreateMetadataTemplate();
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();
}
