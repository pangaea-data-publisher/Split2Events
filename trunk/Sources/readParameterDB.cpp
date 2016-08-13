/* readParameterDB.cpp		  */
/* 2008-11-29                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::readParameterDB( const QString &s_FilenamePDB, structParameter *p_Parameter )
{
    int         n                   = 0;

    int         i_NumOfParameters   = 0;
    int         i_ParameterID       = 0;

    QString     s_Unit              = "";

    QStringList sl_Input;

    QByteArray  HashOfFile;
//------------------------------------------------------------------

    setWaitCursor();
    setStatusBar( tr( "Reading parameter database..." ) );

    if ( ( n = readFile( s_FilenamePDB, sl_Input, 0 ) ) < 1 ) // UTF-8
    {
        setNormalCursor();
        setStatusBar( tr( "" ) );

        return( -101 );
    }

    i_NumOfParameters = sl_Input.at( n-1 ).section( "\t", 0, 0 ).toInt();

    if ( i_NumOfParameters > _MAX_NUM_OF_PARAMETERS_ )
    {
        setNormalCursor();
        setStatusBar( tr( "" ) );

        return( -141 );
    }

//------------------------------------------------------------------

    p_Parameter[0].ParameterID			    = QString( "%1" ).arg( i_NumOfParameters );
    p_Parameter[0].ParameterName			= "Number of parameters";
    p_Parameter[0].ParameterNameL           = "";
    p_Parameter[0].ParameterAbbreviationL	= "";

    for ( int i=1; i <= i_NumOfParameters; i++ )
    {
        p_Parameter[i].ParameterID			    = QString( "%1" ).arg( i );
        p_Parameter[i].ParameterName			= QString( "Parameter %1 has been deleted" ).arg( i );
        p_Parameter[i].ParameterNameL           = "";
        p_Parameter[i].ParameterAbbreviationL	= "";
    }

    for ( int i=1; i < n; i++ )
    {
        i_ParameterID = sl_Input.at( i ).section( "\t", 0, 0 ).toInt();

        s_Unit = " [" + sl_Input.at( i ).section( "\t", 3, 3 ) + "]";
        s_Unit.replace( "[ ]", "[]" );

        p_Parameter[i_ParameterID].ParameterID			    = sl_Input.at( i ).section( "\t", 0, 0 );
        p_Parameter[i_ParameterID].ParameterName			= sl_Input.at( i ).section( "\t", 1, 1 ) + s_Unit;
        p_Parameter[i_ParameterID].ParameterNameL           = sl_Input.at( i ).section( "\t", 1, 1 ).toLower() + s_Unit.toLower();
        p_Parameter[i_ParameterID].ParameterAbbreviationL	= sl_Input.at( i ).section( "\t", 2, 2 ).toLower() + s_Unit.toLower();
    }

    setNormalCursor();
    setStatusBar( tr( "" ) );

    return( i_NumOfParameters );
}
