// 2011-06-09, Dr. Rainer Sieger

#include <QtWidgets>

#include "Application.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    QRegExp regExp("[0-9]\\d{0,2}");

    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( browseDirParameterDB_pushButton, SIGNAL( clicked() ), this, SLOT( browseDirDialog() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void SettingsDialog::browseDirDialog()
{
    QString	sd_Path 	= "";
    QString s_Path 		= this->DirParameterDB_lineEdit->text();
    QString s_PathStart	= getDocumentDir();

// ***********************************************************************************************************************

    s_PathStart.replace( "//", "/" );

    if ( s_Path.isEmpty() == false )
        s_PathStart = s_Path;

    sd_Path = QFileDialog::getExistingDirectory( this, tr( "Choose a folder for the parameter database" ), s_PathStart, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );

    if ( sd_Path.isEmpty() == false )
        s_Path = sd_Path;

    this->DirParameterDB_lineEdit->setText( QDir::toNativeSeparators( s_Path ) );

    DirParameterDB_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString SettingsDialog::getDocumentDir()
{
#if defined(Q_OS_LINUX)
    return( QDir::homePath() );
#endif

#if defined(Q_OS_MAC)
    return( QDir::homePath() );
#endif

#if defined(Q_OS_WIN)
    return( QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) );
#endif
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doSettingsDialog()
{
    QFileInfo fi( gs_FilenamePDB );

    QString s_Path = fi.absolutePath();

    SettingsDialog dialog( this );

    dialog.setCodecComboBox->setCurrentIndex( gi_Codec );

    switch ( gi_Extension )
    {
    case _CSV_:
        dialog.CSV_radioButton->setChecked( true );
        break;
    default:
        dialog.TXT_radioButton->setChecked( true );
        break;
    }

    if ( gb_showParameterImportFileCreatedMessage == true )
        dialog.showParameterImportFileCreatedMessage_checkBox->setChecked( true );
    else
        dialog.showParameterImportFileCreatedMessage_checkBox->setChecked( false );

    if ( gb_mergeNewParameters == true )
        dialog.mergeNewParameters_checkBox->setChecked( true );
    else
        dialog.mergeNewParameters_checkBox->setChecked( false );

    if ( gb_refreshOnStart == true )
        dialog.refreshOnStart_checkBox->setChecked( true );
    else
        dialog.refreshOnStart_checkBox->setChecked( false );

    if ( gb_createAdditionMetadataOptions == true )
        dialog.createAdditionMetadataOptions_checkBox->setChecked( true );
    else
        dialog.createAdditionMetadataOptions_checkBox->setChecked( false );

    dialog.DirParameterDB_lineEdit->setText( QDir::toNativeSeparators( s_Path ) );

    dialog.DirParameterDB_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.browseDirParameterDB_pushButton->setWhatsThis( "Browse for the parameter database directory" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    if ( dialog.exec() == QDialog::Accepted )
    {
        gi_Codec     = dialog.setCodecComboBox->currentIndex();
        gi_Extension = _TXT_;

        if ( dialog.CSV_radioButton->isChecked() )
            gi_Extension = _CSV_;

        if ( dialog.showParameterImportFileCreatedMessage_checkBox->isChecked() )
            gb_showParameterImportFileCreatedMessage = true;
        else
            gb_showParameterImportFileCreatedMessage = false;

        if ( dialog.mergeNewParameters_checkBox->isChecked() )
        {
            gb_mergeNewParameters = true;
            mergeParameterDBAction->setVisible( true );
        }
        else
        {
            gb_mergeNewParameters = false;
            mergeParameterDBAction->setVisible( false );
        }

        if ( dialog.refreshOnStart_checkBox->isChecked() )
            gb_refreshOnStart = true;
        else
            gb_refreshOnStart = false;

        if ( dialog.createAdditionMetadataOptions_checkBox->isChecked() )
            gb_createAdditionMetadataOptions = true;
        else
            gb_createAdditionMetadataOptions = false;

        gs_FilenamePDB = QDir::toNativeSeparators( dialog.DirParameterDB_lineEdit->text() + "/ParameterDB.pdb" );
    }

    posDialog = dialog.pos();
}
