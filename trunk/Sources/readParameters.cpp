/* readParameters.cpp		  */
/* 11.01.2004                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-06

int MainWindow::readDataDescription( const QStringList& sl_Input, QStringList& sl_DataDescription, int& i_HeaderLine, int& i_DataLine )
{
    int i = 0;
    int n = sl_Input.count();

    sl_DataDescription.clear();

// *************************************************************************************

    if ( sl_Input.at( 0 ).startsWith( "/*" ) == true )
    {
        while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "*/" ) == false ) )
        {
            sl_DataDescription.append( sl_Input.at( i ) );

            if ( ++i >= n )
                return( -130 );
        }
    }

    if ( i > 0 )
    {
        i_HeaderLine = i+1;
        i_DataLine   = i+2;
    }
    else
    {
        i_HeaderLine = 0;
        i_DataLine   = 1;
    }

    return( i );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::checkEventEntry( const QString &s_EventEntry )
{
    if ( s_EventEntry == "eventlabel" )  return( true );
    if ( s_EventEntry == "labelevent" )  return( true );
    if ( s_EventEntry == "event" )       return( true );
    if ( s_EventEntry == "station" )     return( true );
    if ( s_EventEntry == "cruise" )      return( true );
    if ( s_EventEntry == "campaign" )    return( true );
    if ( s_EventEntry == "scientist" )   return( true );

    return( false );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::readDataHeaderLine( const QString& s_Header, QStringList& sl_ListParameter )
{
    QString s_Event     = "";
    QString s_Parameter = "";

// *************************************************************************************

    s_Event = s_Header.section( "\t", 0, 0 ).toLower().trimmed();
    s_Event.replace( "\"", "" );
    s_Event.replace( " ", "" );

    if ( checkEventEntry( s_Event ) == false )
        return( -131 );

// *************************************************************************************

    sl_ListParameter.clear();

    sl_ListParameter.append( tr( "Event label" ) );

    for ( int i=1; i<=s_Header.count( "\t" ); i++ )
    {
        s_Parameter = s_Header.section( "\t", i, i ).trimmed();

        s_Parameter.replace( "\"", "" );
        s_Parameter.replace( "  ", " " );
        s_Parameter.replace( "] (", "]@(" );
        s_Parameter.replace( "Salinity (", "Salinity []@(" );

        sl_ListParameter.append( s_Parameter );
    }

    return( sl_ListParameter.count() );
}
