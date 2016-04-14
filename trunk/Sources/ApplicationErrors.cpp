/* ApplicationErrors.cpp      */
/* 2008-03-27                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! Errorhandling */

void MainWindow::onError( const int err )
{
    switch ( err )
    {
    case _APPBREAK_:  // Progress aborted
        break;
    case _ERROR_:  // Error
        break;
    case _NOERROR_:  // No error
        break;
    case -1: // File not exists
        QMessageBox::information( this, getApplicationName( true ), tr( "Import file not exists" ) );
        break;
    case -2: // Choose aborted
        break;
    case -10:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open import file.\nPossible locked by another application." ) );
        break;
    case -20:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create export file.\nFile is already open." ) );
        break ;
    case -30:
        QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format." ) );
        break ;
    case -40:
        QMessageBox::information( this, getApplicationName( true ), tr( "Only one file selected." ) );
        break ;
    case -101:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open parameter database file.\nPossible locked by another application." ) );
        break ;
    case -110:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open temp file.\nPossible locked by another application." ) );
        break;
    case -111:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open data file.\nPossible locked by another application." ) );
        break;
    case -112:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open metadata file.\nPossible locked by another application." ) );
        break;
    case -120:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create data file.\nFile is already open." ) );
        break ;
    case -121:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create parameter import file.\nFile is already open." ) );
        break ;
    case -122:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create temp file.\nFile is already open." ) );
        break ;
    case -123:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create info file.\nFile is already open." ) );
        break ;
    case -124:
        QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format. No data!" ) );
        break ;
    case -130:
        QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format of header." ) );
        break ;
    case -131:
        QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format. Event label is missing." ) );
        break ;
    case -132:
        QMessageBox::information( this, getApplicationName( true ), tr( "Too many parameters in input file." ) );
        break ;
    case -140:
        setStatusBar( tr( "Loading parameter database was aborted" ), 2 );
        break ;
    case -141:
        QMessageBox::information( this, getApplicationName( true ), tr( "Too many parameters in parameter database.\nPlease contact rsieger@pangaea.de" ) );
        break ;
    case -142:
        QMessageBox::information( this, getApplicationName( true ), tr( "Parameter database cannot be updated.\nPlease refresh parameter database." ) );
        break ;
    case -143:
        QMessageBox::information( this, getApplicationName( true ), tr( "One parameter file is missing or empty.\nParameter database cannot be updated.\n\nDont't worry, the old parameter database has be restored. Try again after 2 minutes." ) );
        break ;
    case -144:
        QMessageBox::information( this, getApplicationName( true ), tr( "One parameter file is missing or empty.\nParameter database cannot be updated.\nTry again after 2 minutes." ) );
        break ;
    case -145:
        QMessageBox::information( this, getApplicationName( true ), tr( "Ups. Parameter database hasn't changed. Please check the status of 4D update queue before calling the helpdesk." ) );
        break ;
    default :
        QMessageBox::information( this, getApplicationName( true ), tr( "Unknown error.\nPlease contact rsieger@pangaea.de" ) );
        break ;
    }
}

