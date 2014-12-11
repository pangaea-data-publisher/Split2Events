#include <QtGui>

#include "Application.h"
#include "Globals.h"
#include "ParameterDBOptionsDialog.h"

ParameterDBOptionsDialog::ParameterDBOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    QRegExp regExp("[0-9]\\d{0,2}");

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( browseDirParameterDB_pushButton, SIGNAL( clicked() ), this, SLOT( browseDirDialog() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ParameterDBOptionsDialog::browseDirDialog()
{
    QString	sd_Path 	= "";
    QString s_Path 		= this->DirParameterDB_lineEdit->text();
    QString s_PathStart	= getPersonalDir();

// ***********************************************************************************************************************

    s_PathStart.replace( "//", "/" );

    if ( s_Path.isEmpty() == FALSE )
        s_PathStart = s_Path;

    sd_Path = QFileDialog::getExistingDirectory( this,  tr( "Choose a folder for the parameter database" ), s_PathStart, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( sd_Path.isEmpty() == FALSE )
        s_Path = sd_Path;

    this->DirParameterDB_lineEdit->setText( QDir::toNativeSeparators( s_Path ) );

    DirParameterDB_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString ParameterDBOptionsDialog::getPersonalDir()
{
    QString s_PersonalDir	= QDir::homePath();
    QString s_HomeDir		= QDir::homePath();

    #if defined(Q_WS_WIN)
        QSettings settings( "Microsoft", "Windows" );
        s_PersonalDir = settings.value( "/CurrentVersion/Explorer/Shell Folders/Personal" ).toString();
    #endif

    #if defined(Q_WS_MACX)
        s_PersonalDir = s_HomeDir;
    #endif

    return( QDir::toNativeSeparators( s_PersonalDir ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doParameterDBOptionsDialog()
{
    QFileInfo fi( gs_FilenamePDB );

    QString s_Path = fi.absolutePath();

    int i_DialogResult = QDialog::Rejected;

    ParameterDBOptionsDialog *dialog = new ParameterDBOptionsDialog();

    if ( gb_mergeNewParameters == TRUE )
        dialog->mergeNewParameters_checkBox->setChecked( TRUE );
    else
        dialog->mergeNewParameters_checkBox->setChecked( FALSE );

    if ( gb_refreshOnStart == TRUE )
        dialog->refreshOnStart_checkBox->setChecked( TRUE );
    else
        dialog->refreshOnStart_checkBox->setChecked( FALSE );

    dialog->DirParameterDB_lineEdit->setText( QDir::toNativeSeparators( s_Path ) );

    dialog->okButton->setWhatsThis( "Close dialog" );
    dialog->cancelButton->setWhatsThis( "Cancel dialog" );
    dialog->browseDirParameterDB_pushButton->setWhatsThis( "Browse for the parameter database directory" );

    dialog->DirParameterDB_lineEdit->selectAll();

    dialog->move( posDialog );

// ******************************************************************************************************

    if ( dialog->exec() == QDialog::Accepted )
    {
        posDialog = dialog->pos();

        if ( dialog->mergeNewParameters_checkBox->isChecked() )
        {
            gb_mergeNewParameters = TRUE;
            mergeParameterDBAction->setVisible( TRUE );
        }
        else
        {
            gb_mergeNewParameters = FALSE;
            mergeParameterDBAction->setVisible( FALSE );
        }

        if ( dialog->refreshOnStart_checkBox->isChecked() )
            gb_refreshOnStart = TRUE;
        else
            gb_refreshOnStart = FALSE;

        gs_FilenamePDB = QDir::toNativeSeparators( dialog->DirParameterDB_lineEdit->text() + "/ParameterDB.pdb" );

        i_DialogResult = QDialog::Accepted;
    }

    delete dialog;
}
