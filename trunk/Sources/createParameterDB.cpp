#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-07-05

int MainWindow::getNumParameterQuerys()
{
    int             n                      = 0; // Number of lines
    int             i_NumOfParameterQuerys = 0; // Number of parameter querys

    QStringList     sl_Input;

// **********************************************************************************************
// get number of parameter querys

    QFile outfile( getParameterDDIFilename() );

    if ( outfile.exists() == true )
        outfile.remove();

    if ( outfile.open( QIODevice::WriteOnly  | QIODevice::Text ) == true )
    {
        webfile m_webfile;

        m_webfile.setUrl( QLatin1String( "http://www.pangaea.de/curator/Parameter/ParameterDDI.txt" ) );

        if ( m_webfile.open() == true )
        {
            char    buffer[1024];
            qint64  nSize = 0;

            while ( ( nSize = m_webfile.read( buffer, sizeof( buffer ) ) ) > 0 )
                outfile.write( buffer, nSize );

            m_webfile.close();
        }

        outfile.close();
    }

    if ( ( n = readFile( getParameterDDIFilename(), sl_Input ) ) > 0 ) // System encoding
    {
        if ( sl_Input.at( 0 ).startsWith( "Split2Events" ) == true )
            i_NumOfParameterQuerys = sl_Input.at( 0 ).section( ": ", 1, 1 ).toInt();
    }

    return( i_NumOfParameterQuerys );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 02.09.2008

/*! @brief Erstellt eine neue Parameterdatenbank */

void MainWindow::doCreateParameterDB()
{
    int         i				= 0;

    int         err				= _NOERROR_;

    int         i_stopProgress	= 0;

    QStringList sl_FilenameIn;

// **********************************************************************************************

    gi_NumOfParameterFiles = getNumParameterQuerys();

    if ( gi_NumOfParameterFiles > 0 )
    {
        QFileInfo fi( gs_FilenamePDB );

        if ( fi.exists() == false )
            QDir().mkdir( QDir::toNativeSeparators( fi.absolutePath() ) );
        else
        {
            QFile PDBfile( gs_FilenamePDB );
            PDBfile.remove();
        }

        initFileProgress( gi_NumOfParameterFiles, tr( "ParameterDB.pdb" ), tr( "Refreshing parameter database (download)..." ) );

        while ( ( i < gi_NumOfParameterFiles ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
        {
            QString s_FilenameParameterQuery = QString( "Parameter%1" ).arg( i+1 );
            QString s_FilenameParameter      = fi.absolutePath() + "/" + s_FilenameParameterQuery + ".txt";

            QFile outfile( s_FilenameParameter );

            if ( outfile.open( QIODevice::WriteOnly  | QIODevice::Text ) == true )
            {
                QString s_DDI_URL = QLatin1String( "http://www.pangaea.de/ddi/xxx.tab?retr=/curator/Parameter/" ) + s_FilenameParameterQuery + QLatin1String( ".retr&conf=/curator/Parameter/ParameterListOrderByID.conf&format=textfile&charset=UTF-8" );

                webfile m_webfile;

                m_webfile.setUrl( s_DDI_URL );

                if ( m_webfile.open() == true )
                {
                    char    buffer[1024];
                    qint64  nSize = 0;

                    while ( ( nSize = m_webfile.read( buffer, sizeof( buffer ) ) ) > 0 )
                        outfile.write( buffer, nSize );

                    m_webfile.close();

                    sl_FilenameIn.append( s_FilenameParameter );
                }

                outfile.close();
            }

            i_stopProgress = incFileProgress( gi_NumOfParameterFiles, ++i );
        }

        resetFileProgress( gi_NumOfParameterFiles );
    }

// **********************************************************************************************

    if ( ( sl_FilenameIn.count() > 0 ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        err = concatenateFiles( gs_FilenamePDB, sl_FilenameIn, tr( "Refreshing parameter database (concatenate)..." ), 1, false );

        gi_NumOfParametersInPDB = 0;

        setStatusBar( tr( "Done" ), 2 );
    }
    else
    {
        setStatusBar( tr( "Refreshing parameter database was canceled" ), 2 );
    }

// **********************************************************************************************

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-09-02

/*! @brief Fuegt einer bestehenden Parameterdatenbank neue Parameter hinzu */

void MainWindow::doMergeParameterDB()
{
    int		i					= 0;
    int		err					= 0;
    int		i_stopProgress		= 0;

    QStringList sl_FilenameIn;

// **********************************************************************************************

    QFileInfo fi( gs_FilenamePDB );

    if ( fi.exists() == false )
        QDir().mkdir( QDir::toNativeSeparators( fi.absolutePath() ) );

// **********************************************************************************************

    initFileProgress( gi_NumOfParameterFiles, tr( "ParameterDB.pdb" ), tr( "Refreshing parameter database (download)..." ) );

    while ( ( i < gi_NumOfParameterFiles ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        QString s_FilenameParameterQuery = QString( "Parameter%1" ).arg( i+1 );
        QString s_FilenameParameter      = fi.absolutePath() + "/" + s_FilenameParameterQuery + ".txt";

        if ( i < gi_NumOfParameterFiles-1 )
        {
            QFile outfile( s_FilenameParameter );

            if ( outfile.exists() == false )
                err = -142;
            else
                sl_FilenameIn.append( s_FilenameParameter );
        }
        else
        {
            QFile outfile( s_FilenameParameter );

            if ( outfile.exists() == true )
                outfile.remove();

            if ( outfile.open( QIODevice::WriteOnly  | QIODevice::Text ) == true )
            {
                QString s_DDI_URL = QLatin1String( "http://www.pangaea.de/ddi/xxx.tab?retr=/curator/Parameter/" ) + s_FilenameParameterQuery + QLatin1String( ".retr&conf=/curator/Parameter/ParameterListOrderByID.conf&format=textfile&charset=UTF-8" );

                webfile m_webfile;

                m_webfile.setUrl( s_DDI_URL );

                if ( m_webfile.open() == true )
                {
                    char    buffer[1024];
                    qint64  nSize = 0;

                    while ( ( nSize = m_webfile.read( buffer, sizeof( buffer ) ) ) > 0 )
                        outfile.write( buffer, nSize );

                    m_webfile.close();

                    sl_FilenameIn.append( s_FilenameParameter );
                }

                outfile.close();
            }
        }

        i_stopProgress = incFileProgress( gi_NumOfParameterFiles, ++i );
    }

    resetFileProgress( gi_NumOfParameterFiles );

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        err = concatenateFiles( gs_FilenamePDB, sl_FilenameIn, tr( "Refreshing parameter database (concatenate)..." ), 1, false );

        gi_NumOfParametersInPDB = 0;

        setStatusBar( tr( "Done" ), 2 );
    }
    else
    {
        setStatusBar( tr( "Refreshing parameter database was canceled" ), 2 );
    }

// **********************************************************************************************

    onError( err );
}
