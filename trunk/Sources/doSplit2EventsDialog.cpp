#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "../Forms/Split2EventsDialog/Split2EventsDialog.h"

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void MainWindow::doSplit2EventsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    gb_createMetadataTemplate	= FALSE;

    Split2EventsDialog *dialog	= new Split2EventsDialog();

    if ( gsl_FilenameIn.isEmpty() == TRUE )
        chooseFiles();

    if ( gsl_FilenameIn.isEmpty() == FALSE )
        dialog->setWindowTitle( tr( "Split2Events - Settings for [%1]" ).arg( QDir::toNativeSeparators( gsl_FilenameIn.at( 0 ) ) ) );
    else
    {
        delete dialog;
        return;
    }

// ***********************************************************************************************************************

    QRegExp regExpPI( "[0-9]{6}" );
    QRegExp regExpSource( "[0-9]{6}" );
    QRegExp regExpNumberList( "[0-9,; ]{1024}" );
    QRegExp regExpURL( "[a-zA-Z0-9/:-()_\\x0024\\x0026\\x007E\\x003D\\x003F\\x002E]{1024}" );

    dialog->PrincipalInvestigatorLineEdit->setValidator( new QRegExpValidator( regExpPI, this ) );
    dialog->AuthorLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog->SourceLineEdit->setValidator( new QRegExpValidator( regExpSource, this ) );
    dialog->ReferenceLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog->ProjectLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog->URLDataDetailsLineEdit->setValidator( new QRegExpValidator( regExpURL, this ) );
    dialog->URLOtherVersionLineEdit->setValidator( new QRegExpValidator( regExpURL, this ) );
    dialog->URLOtherVersionLineEdit->setValidator( new QRegExpValidator( regExpURL, this ) );

    dialog->OKPushButton->setWhatsThis( "Start splitting" );
    dialog->CreateMetadataTemplatePushButton->setWhatsThis( "Create a metadata template" );
    dialog->CancelPushButton->setWhatsThis( "Cancel dialog" );
    dialog->NewProjectPushButton->setWhatsThis( "Clear all settings" );
    dialog->SaveProjectPushButton->setWhatsThis( "Load project settings" );
    dialog->LoadProjectPushButton->setWhatsThis( "Save project settings" );

    dialog->PrincipalInvestigatorLineEdit->setWhatsThis(
        "Principal Investigator ID"
        "<ul>"
        "<li><b>Example:</b> 123</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @PI@Event label@</li>"
        "</ul>" );

    dialog->AuthorLineEdit->setWhatsThis(
        "Staff ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 123, 456, 789</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @A@Event label@</li>"
        "</ul>" );

    dialog->SourceLineEdit->setWhatsThis(
        "Institute ID"
        "<ul>"
        "<li><b>Example:</b> 32<li>"
        "<li><b>Tip:</b> 999999 will be replaced by @I@Event label@</li>"
        "</ul>" );

    dialog->ReferenceLineEdit->setWhatsThis(
        "Reference ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 134, 567, 890</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @R@Event label@</li>"
        "</ul>" );

    dialog->ProjectLineEdit->setWhatsThis(
        "Project ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 40, 4080, 1</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @P@Event label@</li>"
        "</ul>" );

    dialog->DatasetTitleTextEdit->setWhatsThis(
        "Dataset titel or citation"
        "<ul>"
        "<li><b>Example:</b> Hydrochemistry measured on water bottle samples at station $E</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @D@Event label@</li>"
        "</ul>" );

    dialog->ExportFilenameLineEdit->setWhatsThis(
        "Export filename: max. 80 characters, without path, without extension"
        "<ul>"
        "<li><b>Example:</b> $E_hydrochem</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @E@Event label@</li>"
        "</ul>" );

    dialog->DataSetCommentTextEdit->setWhatsThis(
        "Dataset comment"
        "<ul>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @C@Event label@</li>"
        "</ul>" );

    dialog->URLDataDetailsLineEdit->setWhatsThis(
        "Link to further details: max. 1000 characters"
        "<ul>"
        "<li><b>Example:</b> hdl:10013/epic.31430.d001</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @F@Event label@</li>"
        "</ul>" );

    dialog->URLOtherVersionLineEdit->setWhatsThis(
        "Link to other version of dataset"
        "<ul>"
        "<li><b>Example:</b> http://store.pangaea.de/Projects/</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @O@Event label@</li>"
        "</ul>" );

    dialog->TopologicTypeComboBox->setWhatsThis( "Topologic type of data set" );
    dialog->StatusComboBox->setWhatsThis( "Status of data set" );
    dialog->LoginOnRadioButton->setWhatsThis( "Login on" );
    dialog->LoginOffRadioButton->setWhatsThis( "Login off" );

    dialog->UserLineEdit->setWhatsThis(
        "User ID1, ID2, ... , IDn. Default user is the principal investigator."
        "<ul>"
        "<li><b>Example:</b> 123, 543, 27643</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @U@Event label@</li>"
        "</ul>" );

// ***********************************************************************************************************************
// Mandatory

    dialog->PrincipalInvestigatorLineEdit->setText( gs_PI );
    dialog->AuthorLineEdit->setText( gs_Author );
    dialog->ReferenceLineEdit->setText( gs_Reference );
    dialog->SourceLineEdit->setText( gs_Source );
    dialog->ProjectLineEdit->setText( gs_Project );
    dialog->DatasetTitleTextEdit->setPlainText( gs_DatasetTitle );
    dialog->ExportFilenameLineEdit->setText( gs_ExportFilename );
    dialog->DatasetTitleTextEdit->setTabChangesFocus( TRUE );

    dialog->PrincipalInvestigatorLineEdit->setMaximumWidth( 7*fontMetrics().width( '9' )+4 );
    dialog->SourceLineEdit->setMaximumWidth( 7*fontMetrics().width( '9' )+5 );

    dialog->PrincipalInvestigatorLineEdit->setFocus();
    dialog->PrincipalInvestigatorLineEdit->selectAll();

// ***********************************************************************************************************************
// Optional

    dialog->DataSetCommentTextEdit->setPlainText( gs_DataSetComment );
    dialog->DataSetCommentTextEdit->setTabChangesFocus( TRUE );

    dialog->URLDataDetailsLineEdit->setText( gs_URLDataDetails );
    dialog->URLOtherVersionLineEdit->setText( gs_URLOtherVersion );
    dialog->UserLineEdit->setText( gs_User );

    dialog->TopologicTypeComboBox->setCurrentIndex( gi_TopologicType );
    dialog->StatusComboBox->setCurrentIndex( gi_Status );

    if ( gb_Login == ON )
        dialog->LoginOnRadioButton->setChecked( TRUE );
    else
        dialog->LoginOffRadioButton->setChecked( TRUE );

// ***********************************************************************************************************************
// Options

    if ( gb_splitFile == TRUE )
        dialog->splitFileCheckBox->setChecked( TRUE );
    else
        dialog->splitFileCheckBox->setChecked( FALSE );

    if ( gb_writeHeader == TRUE )
        dialog->writeHeaderCheckBox->setChecked( TRUE );
    else
        dialog->writeHeaderCheckBox->setChecked( FALSE );

    if ( gb_useFilenameInAsEventLabel == TRUE )
        dialog->useFilenameInAsEventLabelCheckBox->setChecked( TRUE );
    else
        dialog->useFilenameInAsEventLabelCheckBox->setChecked( FALSE );

    if ( gb_makeFilenameUnique == TRUE )
        dialog->makeFilenameUniqueCheckBox->setChecked( TRUE );
    else
        dialog->makeFilenameUniqueCheckBox->setChecked( FALSE );

    if ( gb_markSmallFile == TRUE )
        dialog->markSmallFileCheckBox->setChecked( TRUE );
    else
        dialog->markSmallFileCheckBox->setChecked( FALSE );

    if ( gb_overwriteDataset == TRUE )
        dialog->OverwriteDatasetCheckBox->setChecked( TRUE );
    else
        dialog->OverwriteDatasetCheckBox->setChecked( FALSE );

    switch ( gi_MetadataFileMode )
    {
    case NOTUSED:
        dialog->dontUseMetadataFileRadioButton->setChecked( TRUE );
        break;
    case AUTO:
        dialog->useAutoMetadataFileRadioButton->setChecked( TRUE );
        break;
    case BYPOSITION:
        dialog->findByPosisitionRadioButton->setChecked( TRUE );
        break;
    case BYNAME:
        dialog->findByNameRadioButton->setChecked( TRUE );
        break;
    }

    if ( gb_writeParameterImportFile == TRUE )
        dialog->writeParameterImportFileCheckBox->setChecked( TRUE );
    else
        dialog->writeParameterImportFileCheckBox->setChecked( FALSE );

    dialog->adjustSize();

    dialog->resize( sizeDialog );
    dialog->move( posDialog );

// ******************************************************************************************************

    i_DialogResult = dialog->exec();

    sizeDialog	= dialog->size();
    posDialog	= dialog->pos();

    switch ( i_DialogResult )
    {
    case QDialog::Accepted:
    case CREATEMETADATATEMPLATE:
        gs_PI				= dialog->PrincipalInvestigatorLineEdit->text();
        gs_Author			= dialog->AuthorLineEdit->text();
        gs_Reference		= dialog->ReferenceLineEdit->text();
        gs_Source			= dialog->SourceLineEdit->text();
        gs_Project			= dialog->ProjectLineEdit->text();
        gs_DatasetTitle		= dialog->DatasetTitleTextEdit->toPlainText();
        gs_ExportFilename	= dialog->ExportFilenameLineEdit->text();
        gs_User				= dialog->UserLineEdit->text();

        gs_DatasetTitle.replace( "\n", " " );

    // ****************************************************************************

        gs_DataSetComment	= dialog->DataSetCommentTextEdit->toPlainText();
        gs_URLDataDetails	= dialog->URLDataDetailsLineEdit->text();
        gs_URLOtherVersion	= dialog->URLOtherVersionLineEdit->text();
        gi_TopologicType	= dialog->TopologicTypeComboBox->currentIndex();
        gi_Status			= dialog->StatusComboBox->currentIndex();

        gs_DataSetComment.replace( "\n", " " );

    // ****************************************************************************

        if ( dialog->splitFileCheckBox->isChecked() )
            gb_splitFile = TRUE;
        else
            gb_splitFile = FALSE;

        if ( dialog->writeHeaderCheckBox->isChecked() )
            gb_writeHeader = TRUE;
        else
            gb_writeHeader = FALSE;

        if ( dialog->useFilenameInAsEventLabelCheckBox->isChecked() )
            gb_useFilenameInAsEventLabel = TRUE;
        else
            gb_useFilenameInAsEventLabel = FALSE;

        if ( dialog->makeFilenameUniqueCheckBox->isChecked() )
            gb_makeFilenameUnique = TRUE;
        else
            gb_makeFilenameUnique = FALSE;

        if ( dialog->markSmallFileCheckBox->isChecked() )
            gb_markSmallFile = TRUE;
        else
            gb_markSmallFile = FALSE;

        if ( dialog->OverwriteDatasetCheckBox->isChecked() )
            gb_overwriteDataset = TRUE;
        else
            gb_overwriteDataset = FALSE;

        if ( dialog->LoginOnRadioButton->isChecked() )
            gb_Login = ON;
        else
            gb_Login = OFF;

// ****************************************************************************

        if ( dialog->dontUseMetadataFileRadioButton->isChecked() )
            gi_MetadataFileMode = NOTUSED;
        if ( dialog->useAutoMetadataFileRadioButton->isChecked() )
            gi_MetadataFileMode = AUTO;
        if ( dialog->findByPosisitionRadioButton->isChecked() )
            gi_MetadataFileMode = BYPOSITION;
        if ( dialog->findByNameRadioButton->isChecked() )
            gi_MetadataFileMode = BYNAME;

// ****************************************************************************

        if ( dialog->writeParameterImportFileCheckBox->isChecked() )
            gb_writeParameterImportFile = TRUE;
        else
            gb_writeParameterImportFile = FALSE;

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    delete dialog;

// ****************************************************************************

    if ( i_DialogResult == CREATEMETADATATEMPLATE )
        doCreateMetadataTemplate();

    if ( i_DialogResult == QDialog::Accepted )
        doSplit2Events();
}
