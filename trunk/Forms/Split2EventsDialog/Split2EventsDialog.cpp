#include <QtWidgets>

#include "Application.h"
#include "Split2EventsDialog.h"

Split2EventsDialog::Split2EventsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( NewProjectPushButton, SIGNAL( clicked() ), this, SLOT( NewProject() ) );
    connect( SaveProjectPushButton, SIGNAL( clicked() ), this, SLOT( SaveProject() ) );
    connect( LoadProjectPushButton, SIGNAL( clicked() ), this, SLOT( LoadProject() ) );
    connect( CreateMetadataTemplatePushButton, SIGNAL( clicked() ), this, SLOT( CreateMetadataTemplatePushButtonClicked() ) );

    connect( PrincipalInvestigatorLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( AuthorLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( ReferenceLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( ProjectLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( FurtherDetailsReferenceLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( FurtherDetailsDatasetLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( OtherVersionReferenceLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( OtherVersionDatasetLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( SourceReferenceLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( SourceDatasetLineEdit, SIGNAL( selectionChanged() ), this, SLOT( enableOKButton() ) );
    connect( writeHeaderCheckBox, SIGNAL( clicked() ), this, SLOT( enableOKButton() ) );
    connect( writeParameterImportFileCheckBox, SIGNAL( toggled( bool ) ), this, SLOT( enableAddSpeciesColumnCheckbox() ) );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::CreateMetadataTemplatePushButtonClicked()
{
    emit done( 99 );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::enableAddSpeciesColumnCheckbox()
{
    if ( this->writeParameterImportFileCheckBox->isChecked() == true )
        this->addSpeciesNamesCheckBox->setEnabled( true );
    else
        this->addSpeciesNamesCheckBox->setEnabled( false );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::enableOKButton()
{
    bool b_OK = true;

    if ( this->writeHeaderCheckBox->isChecked() == true )
    {
        if ( this->DatasetTitleTextEdit->toPlainText().length() > 255 )
            b_OK = false;

        if ( this->DataSetCommentTextEdit->toPlainText().length() > 1000 )
            b_OK = false;
    }

    if ( b_OK == true )
    {
        this->OK_pushButton->setEnabled( true );
        this->OK_pushButton->setDefault( true );
        this->Cancel_pushButton->setDefault( false );
    }
    else
    {
        this->OK_pushButton->setEnabled( false );
        this->OK_pushButton->setDefault( false );
        this->Cancel_pushButton->setDefault( true );
    }
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::setLineEditText()
{
    AuthorLineEdit->setText( makeNumberList( AuthorLineEdit->text() ) );
    ProjectLineEdit->setText( makeNumberList( ProjectLineEdit->text() ) );
    ReferenceLineEdit->setText( makeNumberList( ReferenceLineEdit->text() ) );
    FurtherDetailsReferenceLineEdit->setText( makeNumberList( FurtherDetailsReferenceLineEdit->text() ) );
    FurtherDetailsDatasetLineEdit->setText( makeNumberList( FurtherDetailsDatasetLineEdit->text() ) );
    OtherVersionReferenceLineEdit->setText( makeNumberList( OtherVersionReferenceLineEdit->text() ) );
    OtherVersionDatasetLineEdit->setText( makeNumberList( OtherVersionDatasetLineEdit->text() ) );
    SourceReferenceLineEdit->setText( makeNumberList( SourceReferenceLineEdit->text() ) );
    SourceDatasetLineEdit->setText( makeNumberList( SourceDatasetLineEdit->text() ) );

    if ( StatusComboBox->currentIndex() < 2 )
        AccessRightsNeededRadioButton->setChecked( true );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

QString Split2EventsDialog::makeNumberList( const QString s_NumberList )
{
    int i					= 0;

    QString s_newNumberList   = "";
    QString s_temp            = s_NumberList;

// ****************************************************************************

    s_temp.replace( " ", "," );
    s_temp.replace( ";", "," );
    s_temp.replace( ",,", "," );

    for ( i=0; i<=s_temp.count( "," ); i++ )
    {
        if ( s_temp.section( ",", i, i ).length() > 0 )
            s_newNumberList.append( s_temp.section( ",", i, i ) + "," );
    }

    return ( s_newNumberList.section( ",", 0, s_newNumberList.count( "," )-1 ) );
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::LoadProject()
{
    QString     s_Filename			= "";

// ****************************************************************************

    enableOKButton();

// ****************************************************************************

    s_Filename = this->windowTitle().section( "[", 1, 1 ).section( "]", 0, 0 );

    QFileInfo fi( s_Filename );

    s_Filename = QFileDialog::getOpenFileName( this, tr( "Select a project (*.pro)" ), fi.path() + "/" + fi.baseName() + QLatin1String( ".pro" ), tr( "Project (*.pro)" ), 0, QFileDialog::DontUseNativeDialog );

    if ( s_Filename.isEmpty() == false )
    {
        QSettings settings( s_Filename, QSettings::IniFormat );

        settings.beginGroup( "Settings" );

        if ( settings.value( "ProjectFileVersion", "ERROR" ).toString() != _PROVERSION_ )
        {
            NewProject();
            return;
        }

        this->PrincipalInvestigatorLineEdit->setText( settings.value( "PI", "" ).toString() );
        this->AuthorLineEdit->setText( settings.value( "Author", "" ).toString() );
        this->ReferenceLineEdit->setText( settings.value( "Reference", "" ).toString() );
        this->SourceLineEdit->setText( settings.value( "Source", "" ).toString() );
        this->ProjectLineEdit->setText( settings.value( "Project", "" ).toString() );
        this->DatasetTitleTextEdit->setPlainText( settings.value( "DatasetTitle", "" ).toString() );
        this->ExportFilenameLineEdit->setText( settings.value( "ExportFilename", "" ).toString() );

        this->DataSetCommentTextEdit->setPlainText( settings.value( "DatasetComment", "" ).toString() );
        this->FurtherDetailsReferenceLineEdit->setText( settings.value( "FurtherDetailsReference", "" ).toString() );
        this->FurtherDetailsDatasetLineEdit->setText( settings.value( "FurtherDetailsDataset", "" ).toString() );
        this->OtherVersionReferenceLineEdit->setText( settings.value( "OtherVersionReference", "" ).toString() );
        this->OtherVersionDatasetLineEdit->setText( settings.value( "OtherVersionDataset", "" ).toString() );
        this->SourceReferenceLineEdit->setText( settings.value( "SourceReference", "" ).toString() );
        this->SourceDatasetLineEdit->setText( settings.value( "SourceDataset", "" ).toString() );
        this->ParentLineEdit->setText( settings.value( "Parent", "" ).toString() );
        this->IssueLineEdit->setText( settings.value( "Issue", "" ).toString() );
        this->UserLineEdit->setText( settings.value( "User", "" ).toString() );

        this->TopologicTypeComboBox->setCurrentIndex( settings.value( "TopologicType", 1 ).toInt() );
        this->StatusComboBox->setCurrentIndex( settings.value( "Status", 2 ).toInt() );

        switch ( settings.value( "MetadataFileMode", _BYNAME_ ).toInt() )
        {
//      case _NOTUSED_:
//          this->dontUseMetadataFileRadioButton->setChecked( true );
//          break;
        case _AUTO_:
            this->useAutoMetadataFileRadioButton->setChecked( true );
            break;
        case _BYPOSITION_:
            this->findByPosisitionRadioButton->setChecked( true );
            break;
        case _BYNAME_:
            this->findByNameRadioButton->setChecked( true );
            break;
        }

        switch ( settings.value( "OutOfRangeValue", _IGNORE_ ).toInt() )
        {
        case _IGNORE_:
            this->ignoreRange_radioButton->setChecked( true );
            break;
        case _REMOVE_:
            this->removeValue_radioButton->setChecked( true );
            break;
        case _BAD_:
            this->badValue_radioButton->setChecked( true );
            break;
        case _QUESTIONABLE_:
            this->questionableValue_radioButton->setChecked( true );
            break;
        }

        switch ( settings.value( "Login", _UNRESTRICTED_ ).toInt() )
        {
        case _UNRESTRICTED_:
            this->UnrestrictedRadioButton->setChecked( true );
            break;
        case _SIGNUPREQUIRED_:
            this->SignupRequiredRadioButton->setChecked( true );
            break;
        case _ACCESSRIGHTSNEEDED_:
            this->AccessRightsNeededRadioButton->setChecked( true );
            break;
        default:
            this->LoginEventRadioButton->setChecked( true );
            break;
        }

        this->writeHeaderCheckBox->setChecked( settings.value( "WriteHeader", true ).toBool() );
        this->splitFileCheckBox->setChecked( settings.value( "SplitFile", true ).toBool() );
        this->markSmallFileCheckBox->setChecked( settings.value( "MarkSmallFile", false ).toBool() );
        this->makeFilenameUniqueCheckBox->setChecked( settings.value( "MakeFilenameUnique", false ).toBool() );
        this->OverwriteDatasetCheckBox->setChecked( settings.value( "OverwriteDataset", false ).toBool() );
        this->useFilenameInAsEventLabelCheckBox->setChecked( settings.value( "UseFilenameInAsEventLabel", false ).toBool() );
        this->writeParameterImportFileCheckBox->setChecked( settings.value( "WriteParameterImportFile", false ).toBool() );

        settings.endGroup();
    }
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::SaveProject()
{
    QString s_Dummy             = "";
    QString s_Filename          = "";

// ****************************************************************************

    enableOKButton();

// ****************************************************************************

    s_Filename = this->windowTitle().section( "[", 1, 1 ).section( "]", 0, 0 );

    QFileInfo fi( s_Filename );

    s_Filename = QFileDialog::getSaveFileName( this, tr( "Choose a filename to save under" ), fi.path() + "/" + fi.baseName() + QLatin1String( ".pro" ), tr( "Project (*.pro)" ), 0, QFileDialog::DontUseNativeDialog );

    if ( s_Filename.right( 4 ).toLower() != ".pro" )
        s_Filename.append( ".pro" );

    if ( s_Filename.isEmpty() == false )
    {
        QSettings settings( s_Filename, QSettings::IniFormat );

        settings.beginGroup( "Settings" );

        settings.setValue( "ProjectFileVersion", _PROVERSION_ );

        settings.setValue( "PI", this->PrincipalInvestigatorLineEdit->text() );
        settings.setValue( "Author", this->AuthorLineEdit->text() );
        settings.setValue( "Reference", this->ReferenceLineEdit->text() );
        settings.setValue( "Source", this->SourceLineEdit->text() );
        settings.setValue( "Project", this->ProjectLineEdit->text() );

        s_Dummy = DatasetTitleTextEdit->toPlainText();
        s_Dummy.replace( "\n", "<*?*>" );
        settings.setValue( "DatasetTitle", s_Dummy );

        settings.setValue( "ExportFilename", this->ExportFilenameLineEdit->text() );

        s_Dummy = this->DataSetCommentTextEdit->toPlainText();
        s_Dummy.replace( "\n", "<newline>" );
        settings.setValue( "DatasetComment", s_Dummy );

        settings.setValue( "FurtherDetailsReference", this->FurtherDetailsReferenceLineEdit->text() );
        settings.setValue( "FurtherDetailsDataset", this->FurtherDetailsDatasetLineEdit->text() );
        settings.setValue( "OtherVersionReference", this->OtherVersionReferenceLineEdit->text() );
        settings.setValue( "OtherVersionDataset", this->OtherVersionDatasetLineEdit->text() );
        settings.setValue( "SourceReference", this->SourceReferenceLineEdit->text() );
        settings.setValue( "SourceDataset", this->SourceDatasetLineEdit->text() );
        settings.setValue( "Parent", this->ParentLineEdit->text() );
        settings.setValue( "Issue", this->IssueLineEdit->text() );
        settings.setValue( "User", this->UserLineEdit->text() );

        settings.setValue( "TopologicType", this->TopologicTypeComboBox->currentIndex() );
        settings.setValue( "Status", this->StatusComboBox->currentIndex() );

//      if ( this->dontUseMetadataFileRadioButton->isChecked() )
//          settings.setValue( "MetadataFileMode", _NOTUSED_ );

        if ( this->useAutoMetadataFileRadioButton->isChecked() )
            settings.setValue( "MetadataFileMode", _AUTO_ );

        if ( this->findByPosisitionRadioButton->isChecked() )
            settings.setValue( "MetadataFileMode", _BYPOSITION_ );

        if ( this->findByNameRadioButton->isChecked() )
            settings.setValue( "MetadataFileMode", _BYNAME_ );

        if ( this->ignoreRange_radioButton->isChecked() )
            settings.setValue( "OutOfRangeValue", _IGNORE_ );

        if ( this->removeValue_radioButton->isChecked() )
            settings.setValue( "OutOfRangeValue", _REMOVE_ );

        if ( this->badValue_radioButton->isChecked() )
            settings.setValue( "OutOfRangeValue", _BAD_ );

        if ( this->questionableValue_radioButton->isChecked() )
            settings.setValue( "OutOfRangeValue", _QUESTIONABLE_ );

        if ( this->UnrestrictedRadioButton->isChecked() )
            settings.setValue( "Login", _UNRESTRICTED_ );

        if ( this->SignupRequiredRadioButton->isChecked() )
            settings.setValue( "Login", _SIGNUPREQUIRED_ );

        if ( this->AccessRightsNeededRadioButton->isChecked() )
            settings.setValue( "Login", _ACCESSRIGHTSNEEDED_ );

        if ( this->LoginEventRadioButton->isChecked() )
            settings.setValue( "Login", _EVENT_ );

        settings.setValue( "WriteHeader", this->writeHeaderCheckBox->isChecked() );
        settings.setValue( "SplitFile", this->splitFileCheckBox->isChecked() );
        settings.setValue( "MarkSmallFile", this->markSmallFileCheckBox->isChecked() );
        settings.setValue( "MakeFilenameUnique", this->makeFilenameUniqueCheckBox->isChecked() );
        settings.setValue( "OverwriteDataset", this->OverwriteDatasetCheckBox->isChecked() );
        settings.setValue( "UseFilenameInAsEventLabel", this->useFilenameInAsEventLabelCheckBox->isChecked() );
        settings.setValue( "WriteParameterImportFile", this->writeParameterImportFileCheckBox->isChecked() );

        settings.endGroup();
    }
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void Split2EventsDialog::NewProject()
{
// ***********************************************************************************************************************
// Mandatory

    this->PrincipalInvestigatorLineEdit->setText( "" );
    this->AuthorLineEdit->setText( "" );
    this->ReferenceLineEdit->setText( "" );
    this->SourceLineEdit->setText( "" );
    this->ProjectLineEdit->setText( "" );
    this->DatasetTitleTextEdit->setPlainText( "" );
    this->ExportFilenameLineEdit->setText( "" );

    this->PrincipalInvestigatorLineEdit->setFocus();

// ***********************************************************************************************************************
// Optional

    this->DataSetCommentTextEdit->setPlainText( "" );
    this->FurtherDetailsReferenceLineEdit->setText( "" );
    this->FurtherDetailsDatasetLineEdit->setText( "" );
    this->OtherVersionReferenceLineEdit->setText( "" );
    this->OtherVersionDatasetLineEdit->setText( "" );
    this->SourceReferenceLineEdit->setText( "" );
    this->SourceDatasetLineEdit->setText( "" );
    this->ParentLineEdit->setText( "" );
    this->IssueLineEdit->setText( "" );
    this->UserLineEdit->setText( "" );

    this->TopologicTypeComboBox->setCurrentIndex( _NOTSPECIFIED_ );
    this->StatusComboBox->setCurrentIndex( _PUBLISHED_ );

    this->UnrestrictedRadioButton->setChecked( true );

// ***********************************************************************************************************************
// Options

    this->splitFileCheckBox->setChecked( true );
    this->writeHeaderCheckBox->setChecked( true );
    this->useFilenameInAsEventLabelCheckBox->setChecked( false );
    this->makeFilenameUniqueCheckBox->setChecked( false );
    this->markSmallFileCheckBox->setChecked( false );
    this->OverwriteDatasetCheckBox->setChecked( false );
    this->findByNameRadioButton->setChecked( true );
    this->writeParameterImportFileCheckBox->setChecked( false );
    this->ignoreRange_radioButton->setChecked( true );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int Split2EventsDialog::readFile( const QString &s_FilenameIn, QStringList &sl_Input, const int i_Codec, const qint64 i_Bytes )
{
    QByteArray ba;

    sl_Input.clear();

// **********************************************************************************************
// read data

    QFile fpdb( s_FilenameIn );

    if ( fpdb.open( QIODevice::ReadOnly ) == false )
        return( -10 );

    if ( i_Bytes == 0 )
        ba = fpdb.readAll();

    if ( i_Bytes > 0 )
        ba = fpdb.read( i_Bytes );

    if ( i_Bytes < 0 )
        ba = qUncompress( fpdb.readAll() );

    fpdb.close();

// **********************************************************************************************
// replace End-Of-Line character

    ba.replace( "\r\r\n", "\n" ); // Windows -> Unix
    ba.replace( "\r\n", "\n" ); // Windows -> Unix
    ba.replace( "\r", "\n" ); // MacOS -> Unix

// **********************************************************************************************
// split

    if ( ba.size() < 500000000 ) // 500 MB; not tested
    {
        switch ( i_Codec )
        {
        case _SYSTEM_: // nothing
            sl_Input = QString( ba ).split( "\n" );
            break;

        case _APPLEROMAN_: // Apple Roman
            sl_Input = QTextCodec::codecForName( "Apple Roman" )->toUnicode( ba ).split( "\n" );
            break;

        case _LATIN1_: // Latin-1 = ISO 8859-1
            sl_Input = QTextCodec::codecForName( "ISO 8859-1" )->toUnicode( ba ).split( "\n" );
            break;

        default: // UTF-8
            sl_Input = QTextCodec::codecForName( "UTF-8" )->toUnicode( ba ).split( "\n" );
            break;
        }
    }
    else
    {
        sl_Input.append( tr( "File to big" ) );
    }

// **********************************************************************************************
// remove empty lines at the end of the file

    while ( ( sl_Input.count() > 0 ) && ( sl_Input.at( sl_Input.count()-1 ).isEmpty() == true ) )
        sl_Input.removeLast();

// **********************************************************************************************

    return( sl_Input.count() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doSplit2EventsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    gb_createMetadataTemplate	= false;

// ***********************************************************************************************************************

    Split2EventsDialog dialog( this );

    if ( gsl_FilenameList.isEmpty() == true )
        chooseFiles();

    if ( gsl_FilenameList.isEmpty() == false )
        dialog.setWindowTitle( tr( "Split2Events - Settings for [%1]" ).arg( QDir::toNativeSeparators( gsl_FilenameList.at( 0 ) ) ) );
    else
        return;

// ***********************************************************************************************************************

//  QRegExp regExpURL( "[a-zA-Z0-9/:-()_\\x0024\\x0026\\x007E\\x003D\\x003F\\x002E]{1024}" );

    QRegExp regExpPI( "[0-9]{6}" );
    QRegExp regExpSource( "[0-9]{6}" );
    QRegExp regExpNumberList( "[0-9,; ]{1024}" );

    dialog.PrincipalInvestigatorLineEdit->setValidator( new QRegExpValidator( regExpPI, this ) );
    dialog.AuthorLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.SourceLineEdit->setValidator( new QRegExpValidator( regExpSource, this ) );
    dialog.ReferenceLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.ProjectLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.FurtherDetailsReferenceLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.FurtherDetailsDatasetLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.OtherVersionReferenceLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.OtherVersionDatasetLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.SourceReferenceLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );
    dialog.SourceDatasetLineEdit->setValidator( new QRegExpValidator( regExpNumberList, this ) );

    dialog.OK_pushButton->setWhatsThis( "Start splitting" );
    dialog.CreateMetadataTemplatePushButton->setWhatsThis( "Create a metadata template" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.NewProjectPushButton->setWhatsThis( "Clear all settings" );
    dialog.SaveProjectPushButton->setWhatsThis( "Load project settings" );
    dialog.LoadProjectPushButton->setWhatsThis( "Save project settings" );

    dialog.PrincipalInvestigatorLineEdit->setWhatsThis(
        "Principal Investigator ID"
        "<ul>"
        "<li><b>Example:</b> 123</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @PI@Event label@</li>"
        "</ul>" );

    dialog.AuthorLineEdit->setWhatsThis(
        "Staff ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 123, 456, 789</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @A@Event label@</li>"
        "</ul>" );

    dialog.SourceLineEdit->setWhatsThis(
        "Institute ID"
        "<ul>"
        "<li><b>Example:</b> 32<li>"
        "<li><b>Tip:</b> 999999 will be replaced by @I@Event label@</li>"
        "</ul>" );

    dialog.ReferenceLineEdit->setWhatsThis(
        "Reference ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 134, 567, 890</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @R@Event label@</li>"
        "</ul>" );

    dialog.ProjectLineEdit->setWhatsThis(
        "Project ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 40, 4080, 1</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @P@Event label@</li>"
        "</ul>" );

    dialog.DatasetTitleTextEdit->setWhatsThis(
        "Dataset titel or citation"
        "<ul>"
        "<li><b>Example:</b> Hydrochemistry measured on water bottle samples at station $E</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @D@Event label@</li>"
        "</ul>" );

    dialog.ExportFilenameLineEdit->setWhatsThis(
        "Export filename: max. 80 characters, without path, without extension"
        "<ul>"
        "<li><b>Example:</b> $E_hydrochem</li>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @E@Event label@</li>"
        "</ul>" );

    dialog.DataSetCommentTextEdit->setWhatsThis(
        "Dataset comment"
        "<ul>"
        "<li><b>Tip 1:</b> $E will be replaced by the event label</li>"
        "<li><b>Tip 2:</b> $@ will be replaced by the string behind the @ given in the event label</li>"
        "<li><b>Tip 3:</b> 999999 will be replaced by @C@Event label@</li>"
        "</ul>" );

    dialog.FurtherDetailsReferenceLineEdit->setWhatsThis(
        "Reference ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 134, 567, 890</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @FR@Event label@</li>"
        "</ul>" );

    dialog.FurtherDetailsDatasetLineEdit->setWhatsThis(
        "Dataset ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 234134, 56997, 890234</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @FD@Event label@</li>"
        "</ul>" );

    dialog.OtherVersionReferenceLineEdit->setWhatsThis(
        "Reference ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 134, 567, 890</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @OR@Event label@</li>"
        "</ul>" );

    dialog.OtherVersionDatasetLineEdit->setWhatsThis(
        "Dataset ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 234134, 56997, 890234</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @RD@Event label@</li>"
        "</ul>" );

    dialog.SourceReferenceLineEdit->setWhatsThis(
        "Reference ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 134, 567, 890</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @SR@Event label@</li>"
        "</ul>" );

    dialog.SourceDatasetLineEdit->setWhatsThis(
        "Dataset ID1, ID2, ... , IDn"
        "<ul>"
        "<li><b>Example:</b> 234134, 56997, 890234</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @SD@Event label@</li>"
        "</ul>" );

    dialog.TopologicTypeComboBox->setWhatsThis( "Topologic type of data set" );
    dialog.StatusComboBox->setWhatsThis( "Status of data set" );

    dialog.UserLineEdit->setWhatsThis(
        "User ID1, ID2, ... , IDn. Default user is the principal investigator."
        "<ul>"
        "<li><b>Example:</b> 123, 543, 27643</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @U@Event label@</li>"
        "</ul>" );

    dialog.ParentLineEdit->setWhatsThis(
        "Parent ID"
        "<ul>"
        "<li><b>Example:</b> 123</li>"
        "<li><b>Tip:</b> 999999 will be replaced by @Par@Event label@</li>"
        "</ul>" );


// ***********************************************************************************************************************
// Mandatory

    dialog.PrincipalInvestigatorLineEdit->setText( gs_PI );
    dialog.AuthorLineEdit->setText( gs_Author );
    dialog.ReferenceLineEdit->setText( gs_Reference );
    dialog.SourceLineEdit->setText( gs_Source );
    dialog.ProjectLineEdit->setText( gs_Project );
    dialog.DatasetTitleTextEdit->setPlainText( gs_DatasetTitle );
    dialog.ExportFilenameLineEdit->setText( gs_ExportFilename );
    dialog.DatasetTitleTextEdit->setTabChangesFocus( true );

    dialog.PrincipalInvestigatorLineEdit->setMaximumWidth( 7*fontMetrics().width( '9' )+4 );
    dialog.SourceLineEdit->setMaximumWidth( 7*fontMetrics().width( '9' )+5 );

    dialog.PrincipalInvestigatorLineEdit->setFocus();
    dialog.PrincipalInvestigatorLineEdit->selectAll();

// ***********************************************************************************************************************
// Optional

    dialog.DataSetCommentTextEdit->setPlainText( gs_DataSetComment );
    dialog.DataSetCommentTextEdit->setTabChangesFocus( true );

    dialog.FurtherDetailsReferenceLineEdit->setText( gs_FurtherDetailsReference );
    dialog.FurtherDetailsDatasetLineEdit->setText( gs_FurtherDetailsDataset );
    dialog.OtherVersionReferenceLineEdit->setText( gs_OtherVersionReference );
    dialog.OtherVersionDatasetLineEdit->setText( gs_OtherVersionDataset );
    dialog.SourceReferenceLineEdit->setText( gs_SourceReference );
    dialog.SourceDatasetLineEdit->setText( gs_SourceDataset );
    dialog.ParentLineEdit->setText( gs_Parent );
    dialog.IssueLineEdit->setText( gs_Issue );
    dialog.UserLineEdit->setText( gs_User );

    dialog.TopologicTypeComboBox->setCurrentIndex( gi_TopologicType );
    dialog.StatusComboBox->setCurrentIndex( gi_Status );

    switch ( gi_Login )
    {
    case _UNRESTRICTED_:
        dialog.UnrestrictedRadioButton->setChecked( true );
        break;
    case _SIGNUPREQUIRED_:
        dialog.SignupRequiredRadioButton->setChecked( true );
        break;
    case _ACCESSRIGHTSNEEDED_:
        dialog.AccessRightsNeededRadioButton->setChecked( true );
        break;
    default:
        dialog.LoginEventRadioButton->setChecked( true );
        break;
    }

// ***********************************************************************************************************************
// Options

    if ( gb_splitFile == true )
        dialog.splitFileCheckBox->setChecked( true );
    else
        dialog.splitFileCheckBox->setChecked( false );

    if ( gb_writeHeader == true )
        dialog.writeHeaderCheckBox->setChecked( true );
    else
        dialog.writeHeaderCheckBox->setChecked( false );

    if ( gb_useFilenameInAsEventLabel == true )
        dialog.useFilenameInAsEventLabelCheckBox->setChecked( true );
    else
        dialog.useFilenameInAsEventLabelCheckBox->setChecked( false );

    if ( gb_makeFilenameUnique == true )
        dialog.makeFilenameUniqueCheckBox->setChecked( true );
    else
        dialog.makeFilenameUniqueCheckBox->setChecked( false );

    if ( gb_markSmallFile == true )
        dialog.markSmallFileCheckBox->setChecked( true );
    else
        dialog.markSmallFileCheckBox->setChecked( false );

    if ( gb_overwriteDataset == true )
        dialog.OverwriteDatasetCheckBox->setChecked( true );
    else
        dialog.OverwriteDatasetCheckBox->setChecked( false );

    switch ( gi_MetadataFileMode )
    {
//  case _NOTUSED_:
//      dialog.dontUseMetadataFileRadioButton->setChecked( true );
//      break;
    case _AUTO_:
        dialog.useAutoMetadataFileRadioButton->setChecked( true );
        break;
    case _BYPOSITION_:
        dialog.findByPosisitionRadioButton->setChecked( true );
        break;
    case _BYNAME_:
        dialog.findByNameRadioButton->setChecked( true );
        break;
    }

    switch ( gi_OutOfRangeValue )
    {
    case _IGNORE_:
        dialog.ignoreRange_radioButton->setChecked( true );
        break;
    case _REMOVE_:
        dialog.removeValue_radioButton->setChecked( true );
        break;
    case _BAD_:
        dialog.badValue_radioButton->setChecked( true );
        break;
    case _QUESTIONABLE_:
        dialog.questionableValue_radioButton->setChecked( true );
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

    dialog.adjustSize();
    dialog.move( posDialog );
    dialog.resize( sizeDialog );
    dialog.show();

// ******************************************************************************************************

    i_DialogResult = dialog.exec();

    switch ( i_DialogResult )
    {
    case QDialog::Accepted:
    case _CREATEMETADATATEMPLATE_:
        dialog.setLineEditText();

        gs_PI                       = dialog.PrincipalInvestigatorLineEdit->text();
        gs_Author                   = dialog.AuthorLineEdit->text();
        gs_Reference                = dialog.ReferenceLineEdit->text();
        gs_Source                   = dialog.SourceLineEdit->text();
        gs_Project                  = dialog.ProjectLineEdit->text();
        gs_DatasetTitle             = dialog.DatasetTitleTextEdit->toPlainText();
        gs_ExportFilename       	= dialog.ExportFilenameLineEdit->text();
        gs_Parent     				= dialog.ParentLineEdit->text();
        gs_Issue                    = dialog.IssueLineEdit->text();
        gs_User     				= dialog.UserLineEdit->text();
        gs_DataSetComment           = dialog.DataSetCommentTextEdit->toPlainText();
        gs_FurtherDetailsReference  = dialog.FurtherDetailsReferenceLineEdit->text();
        gs_FurtherDetailsDataset    = dialog.FurtherDetailsDatasetLineEdit->text();
        gs_OtherVersionReference    = dialog.OtherVersionReferenceLineEdit->text();
        gs_OtherVersionDataset      = dialog.OtherVersionDatasetLineEdit->text();
        gs_SourceReference          = dialog.SourceReferenceLineEdit->text();
        gs_SourceDataset            = dialog.SourceDatasetLineEdit->text();
        gi_TopologicType            = dialog.TopologicTypeComboBox->currentIndex();
        gi_Status                   = dialog.StatusComboBox->currentIndex();

// ****************************************************************************

        if ( dialog.splitFileCheckBox->isChecked() )
            gb_splitFile = true;
        else
            gb_splitFile = false;

        if ( dialog.writeHeaderCheckBox->isChecked() )
            gb_writeHeader = true;
        else
            gb_writeHeader = false;

        if ( dialog.useFilenameInAsEventLabelCheckBox->isChecked() )
            gb_useFilenameInAsEventLabel = true;
        else
            gb_useFilenameInAsEventLabel = false;

        if ( dialog.makeFilenameUniqueCheckBox->isChecked() )
            gb_makeFilenameUnique = true;
        else
            gb_makeFilenameUnique = false;

        if ( dialog.markSmallFileCheckBox->isChecked() )
            gb_markSmallFile = true;
        else
            gb_markSmallFile = false;

        if ( dialog.OverwriteDatasetCheckBox->isChecked() )
            gb_overwriteDataset = true;
        else
            gb_overwriteDataset = false;

        if ( dialog.UnrestrictedRadioButton->isChecked() )
            gi_Login = _UNRESTRICTED_;
        if ( dialog.SignupRequiredRadioButton->isChecked() )
            gi_Login = _SIGNUPREQUIRED_;
        if ( dialog.AccessRightsNeededRadioButton->isChecked() )
            gi_Login = _ACCESSRIGHTSNEEDED_;
        if ( dialog.LoginEventRadioButton->isChecked() )
            gi_Login = _EVENT_;

// ****************************************************************************

//      if ( dialog.dontUseMetadataFileRadioButton->isChecked() )
//          gi_MetadataFileMode = _NOTUSED_;
        if ( dialog.useAutoMetadataFileRadioButton->isChecked() )
            gi_MetadataFileMode = _AUTO_;
        if ( dialog.findByPosisitionRadioButton->isChecked() )
            gi_MetadataFileMode = _BYPOSITION_;
        if ( dialog.findByNameRadioButton->isChecked() )
            gi_MetadataFileMode = _BYNAME_;

// ****************************************************************************

        if ( dialog.ignoreRange_radioButton->isChecked() )
            gi_OutOfRangeValue = _IGNORE_;

        if ( dialog.removeValue_radioButton->isChecked() )
            gi_OutOfRangeValue = _REMOVE_;

        if ( dialog.badValue_radioButton->isChecked() )
            gi_OutOfRangeValue = _BAD_;

        if ( dialog.questionableValue_radioButton->isChecked() )
            gi_OutOfRangeValue = _QUESTIONABLE_;

// ****************************************************************************

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

// ****************************************************************************

        if ( gs_PI.isEmpty() == true )
            gs_PI = num2str( 506 );

        if ( gs_Project.isEmpty() == true )
            gs_Project = num2str( 15 );

        gs_DatasetTitle.replace( "\n", "<*?*>" );
        gs_DataSetComment.replace( "\n", "<newline>" );

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

// ****************************************************************************

    sizeDialog	= dialog.size();
    posDialog	= dialog.pos();

    if ( i_DialogResult == _CREATEMETADATATEMPLATE_ )
        doCreateMetadataTemplate();

    if ( i_DialogResult == QDialog::Accepted )
        doSplit2Events();
}
