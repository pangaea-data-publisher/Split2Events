/* ConcatenateFiles.cpp       */
/* 2008-05-13                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// 2008-05-13

/*! @brief Verbindet 1 bis n Dateien zu einer neuen mit dem Namen FilenameOut. Die Inputdateien werden geloescht, wenn b_RemoveFile = true.
*
*   Routine wird im Moment nicht genutzt.
*
*   @param s_FilenameOut Name der neuen Datei.
*   @param sl_FilenameIn Liste aller zu verbindenen Dateien.
*   @param s_ProgressMessage Ausgabetext fuer ProgressDialog->
*   @param i_SkipNFirstLines Anzahl der Zeilen die ab der zweiten Datei ueberlesen werden sollen.
*   @param b_deleteOriginalFiles Bestimmt ob die Inputdatei geloescht werden soll oder nicht.
*
*   @return Fehlercode
*/

int MainWindow::concatenateFiles( const QString& s_FilenameOut, const QStringList sl_FilenameIn,
                                  const QString& s_ProgressMessage, const int i_SkipNFirstLines,
                                  const bool b_deleteOriginalFiles )
{
    int         err             = _NOERROR_;

    int         i				= 0;
    int         j               = 0;
    int         n               = 0;

    int         i_stopProgress	= 0;

    QStringList	sl_Input;

    QTime       timestamp;

// ************************************************************************************************

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );
    tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );

// ************************************************************************************************

    initFileProgress( sl_FilenameIn.count(), sl_FilenameIn.at( 0 ), s_ProgressMessage );

// ************************************************************************************************

    while ( ( j < sl_FilenameIn.count() ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        setStatusBarFileInProgress( sl_FilenameIn.at( j ) );

        if ( ( n = readFile( sl_FilenameIn.at( j ), sl_Input, _UTF8_ ) ) > 0 ) // encoding: UTF-8
        {
            initProgress( n );

            if ( j > 0 )
                i = i_SkipNFirstLines;
            else
                i = 0;

            timestamp.start();

            while ( i < n )
            {
                tout << sl_Input.at( i++ ) << endl;

                if ( timestamp.elapsed() > 100 )
                {
                    i_stopProgress = incProgress( sl_FilenameIn.count(), i );
                    QApplication::processEvents();
                    timestamp.start();
                }
            }

            resetProgress( sl_FilenameIn.count() );

            if ( b_deleteOriginalFiles == true )
                QFile::remove(sl_FilenameIn.at( j ) );
        }
        else
        {
            err = -143;
        }

        i_stopProgress = incFileProgress( sl_FilenameIn.count(), ++j );
    }

    fout.close();

    resetFileProgress( sl_FilenameIn.count() );

    if ( i_stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( err != _NOERROR_ )
        fout.remove();

    return( err );
}
