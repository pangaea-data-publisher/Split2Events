/* convertEOL.cpp			*/
/* 2008-12-19               */
/* Dr. Rainer Sieger        */

#include <QtGui>

#include "Application.h"
#include "Globals.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::convertEOL( const QString& FilenameIn, const bool b_convertEOL, const int i_OS )
{
    bool	b_isWindowsEOL	= FALSE;
    bool	b_isMacOS9EOL	= FALSE;
    bool	b_isAllOSEOL	= FALSE;

// **********************************************************************************************

    if ( b_convertEOL == TRUE )
    {
        QFileInfo fi( FilenameIn );

        QFile fin( FilenameIn );
        if ( !fin.open( QIODevice::ReadOnly ) )
            return( -10 );

        QByteArray ba = fin.read( 10000 );

        switch ( i_OS )
        {
        case WIN:
            if ( ba.contains( "\r\n" ) == TRUE ) // Windows
                b_isWindowsEOL = TRUE;
            break;

        case MACOS:
            if ( ( ba.contains( "\r" ) == TRUE ) && ( ba.contains( "\n" ) == FALSE ) )  // MacOS 9
                b_isMacOS9EOL = TRUE;
            break;

        case ALLOS:
            if ( ba.contains( "\r" ) == TRUE ) // Windows or MacOS 9
                b_isAllOSEOL  = TRUE;
            break;

        default:
            break;
        }

        if ( ( b_isAllOSEOL == TRUE ) || ( b_isWindowsEOL == TRUE ) || ( b_isMacOS9EOL == TRUE ) )
        {
            fin.reset();
            ba.clear();
            ba.resize( fin.size() );
            ba = fin.readAll();
        }

        fin.close();

// **********************************************************************************************

        if ( b_isAllOSEOL == TRUE )
        {
            QFile fout( fi.absolutePath() + "/~" + fi.baseName() );
            if ( !fout.open( QIODevice::WriteOnly ) ) // | QIODevice::Text ) )
                return( -20 );

            ba.replace( "\r\n", "\n" ); // Windows -> Unix
            ba.replace( "\r", "\n" ); // MacOS -> Unix

            fout.write( ba );

            fout.close();

            fin.remove();
            fout.rename( FilenameIn );
        }

// **********************************************************************************************

        if ( b_isWindowsEOL == TRUE )
        {
            QFile fout( fi.absolutePath() + "/~" + fi.baseName() );
            if ( !fout.open( QIODevice::WriteOnly ) ) // | QIODevice::Text ) )
                return( -20 );

            ba.replace( "\r\n", "\n" ); // Windows -> Unix

            fout.write( ba );

            fout.close();

            fin.remove();
            fout.rename( FilenameIn );
        }

// **********************************************************************************************

        if ( b_isMacOS9EOL == TRUE )
        {
            QFile fout( fi.absolutePath() + "/~" + fi.baseName() );
            if ( !fout.open( QIODevice::WriteOnly ) ) // | QIODevice::Text ) )
                return( -20 );

            ba.replace( "\r", "\n" ); // MacOS 9 -> Unix

            fout.write( ba );

            fout.close();

            fin.remove();
            fout.rename( FilenameIn );
        }
    }

// **********************************************************************************************

    return( NOERROR );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

void MainWindow::doWinConvertEOL()
{
    int		err							= NOERROR;
    int		i							= 0;
    int		i_stopProgress				= 0;

// *************************************************************************************

    if ( gsl_FilenameIn.isEmpty() == TRUE )
        chooseFiles();

    if ( gsl_FilenameIn.isEmpty() == TRUE )
        return;

    initFileProgress( gsl_FilenameIn.count(), gsl_FilenameIn.at( 0 ), tr( "Converting end-of-line..." ) );

    while ( ( i<gsl_FilenameIn.count() ) && ( err == NOERROR ) && ( i_stopProgress == 0 ) )
    {
        err = convertEOL( gsl_FilenameIn.at( i ), TRUE, WIN );
        i_stopProgress = incFileProgress( gsl_FilenameIn.count(), ++i );
    }

    resetFileProgress( gsl_FilenameIn.count() );

    if ( i_stopProgress == APPBREAK )
        err = APPBREAK;

    if ( ( err == NOERROR ) && ( i_stopProgress != APPBREAK ) )
        setStatusBar( tr( "Done" ), 2 );
    else
        setStatusBar( tr( "Converting end-of-line was canceled" ), 2 );

    onError( err );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

void MainWindow::doMacOSConvertEOL()
{
    int		err							= NOERROR;
    int		i							= 0;
    int		i_stopProgress				= 0;

// *************************************************************************************

    if ( gsl_FilenameIn.isEmpty() == TRUE )
        chooseFiles();

    if ( gsl_FilenameIn.isEmpty() == TRUE )
        return;

    initFileProgress( gsl_FilenameIn.count(), gsl_FilenameIn.at( 0 ), tr( "Converting end-of-line..." ) );

    while ( ( i<gsl_FilenameIn.count() ) && ( err == NOERROR ) && ( i_stopProgress == 0 ) )
    {
        err = convertEOL( gsl_FilenameIn.at( i ), TRUE, MACOS );
        i_stopProgress = incFileProgress( gsl_FilenameIn.count(), ++i );
    }

    resetFileProgress( gsl_FilenameIn.count() );

    if ( i_stopProgress == APPBREAK )
        err = APPBREAK;

    if ( ( err == NOERROR ) && ( i_stopProgress != APPBREAK ) )
        setStatusBar( tr( "Done" ), 2 );
    else
        setStatusBar( tr( "Converting end-of-line was canceled" ), 2 );

    onError( err );
}
