#include "Application.h"

#include "../Forms/MetadataTemplateOptionsDialog/MetadataTemplateOptionsDialog.h"

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
    case _NOTUSED_:
    case _AUTO_:
    case _BYPOSITION_:
        dialog.findByPosisitionRadioButton->setChecked( true );
        break;
    case _BYNAME_:
        dialog.findByNameRadioButton->setChecked( true );
        break;
    }

    if ( gb_writeParameterImportFile == true )
        dialog.writeParameterImportFileCheckBox->setChecked( true );
    else
        dialog.writeParameterImportFileCheckBox->setChecked( false );

    dialog.OK_pushButton->setWhatsThis( "Create metadata template" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        if ( dialog.findByPosisitionRadioButton->isChecked() )
            gi_MetadataFileMode = _BYPOSITION_;

        if ( dialog.findByNameRadioButton->isChecked() )
            gi_MetadataFileMode = _BYNAME_;

        if ( dialog.writeParameterImportFileCheckBox->isChecked() )
            gb_writeParameterImportFile = true;
        else
            gb_writeParameterImportFile = false;

        doCreateMetadataTemplate();
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();
}
