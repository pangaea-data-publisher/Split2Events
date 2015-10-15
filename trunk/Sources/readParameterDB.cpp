/* readParameterDB.cpp		  */
/* 2008-11-29                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::readParameterDB( const QString& s_FilenamePDB, structParameter *p_Parameter )
{
    int             i                   = 0;
    int             n                   = 0;
    int             i_stopProgress      = 0;

    int             i_NumOfParameters   = 0;
    int             i_ParameterID       = 0;

    QString         s_Unit              = "";

    QStringList     sl_Input;

    QTime           timestamp;

//------------------------------------------------------------------

    if ( ( n = readFile( s_FilenamePDB, sl_Input, 0 ) ) < 1 ) // UTF-8
        return( -101 );

    i_NumOfParameters = sl_Input.at( n-1 ).section( "\t", 0, 0 ).toInt();

    if ( i_NumOfParameters > _MAX_NUM_OF_PARAMETERS_ )
        return( -141 );

//------------------------------------------------------------------

    setWaitCursor();
    initProgress( 1, s_FilenamePDB, tr( "Reading parameter database..." ), 2*i_NumOfParameters );

    p_Parameter[0].ParameterID			    = QString( "%1" ).arg( i_NumOfParameters );
    p_Parameter[0].ParameterName			= "Number of parameters";
    p_Parameter[0].ParameterNameL           = "";
    p_Parameter[0].ParameterAbbreviationL	= "";

    i = 1;

    timestamp.start();

    while ( ( i < i_NumOfParameters ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        p_Parameter[i].ParameterID			    = QString( "%1" ).arg( i );
        p_Parameter[i].ParameterName			= QString( "Parameter %1 has been deleted" ).arg( i );
        p_Parameter[i].ParameterNameL           = "";
        p_Parameter[i].ParameterAbbreviationL	= "";

        ++i;

        if ( timestamp.elapsed() > 100 )
        {
            i_stopProgress = incProgress( 1, i );
            QApplication::processEvents();
            timestamp.start();
        }
    }

    i = 1;

    timestamp.start();

    while ( ( i < n ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        i_ParameterID = sl_Input.at( i ).section( "\t", 0, 0 ).toInt();

        s_Unit = " [" + sl_Input.at( i ).section( "\t", 3, 3 ) + "]";
        s_Unit.replace( "[ ]", "[]" );

        p_Parameter[i_ParameterID].ParameterID			    = sl_Input.at( i ).section( "\t", 0, 0 );
        p_Parameter[i_ParameterID].ParameterName			= sl_Input.at( i ).section( "\t", 1, 1 ) + s_Unit;
        p_Parameter[i_ParameterID].ParameterNameL           = sl_Input.at( i ).section( "\t", 1, 1 ).toLower() + s_Unit.toLower();
        p_Parameter[i_ParameterID].ParameterAbbreviationL	= sl_Input.at( i ).section( "\t", 2, 2 ).toLower() + s_Unit.toLower();

        ++i;

        if ( timestamp.elapsed() > 100 )
        {
            i_stopProgress = incProgress( 1, i + i_NumOfParameters );
            QApplication::processEvents();
            timestamp.start();
        }
    }

    resetProgress( 1 );
    setNormalCursor();
    setStatusBar( tr( "" ) );

//------------------------------------------------------------------

    if ( i_stopProgress == _APPBREAK_ )
        i_NumOfParameters = -140;

    return( i_NumOfParameters );
}
