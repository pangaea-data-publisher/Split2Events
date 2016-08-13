/* findFormat.cpp		 	 */
/* 28.11.2004                */
/* Dr. Rainer Sieger         */

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::findFormat( const QString &s_FilenameIn, const QStringList &sl_Input,
                            const int i_NumOfParameters, structPFormat Format_ptr[],
                            const int i_NumOfFiles )
{
    int         i					= 1;
    int         n                   = sl_Input.count();

    int         i_stopProgress      = 0;

    int         integer				= 0;
    int         digits				= 0;

    int         i_NumOfDataItems    = 0;
    int         i_minDataItems      = 10000;

    QString     s_dataItem			= "";

// *************************************************************************************

    if ( n < 2 ) return( -124 ); // no data

// *************************************************************************************

    memset( Format_ptr, 0, ( _MAX_NUM_OF_COLUMNS_ + 1 )*sizeof( Format_ptr[0] ) );

    for ( int j=0; j<=i_NumOfParameters; j++ )
    {
        Format_ptr[j].min = (float) 10E30;
        Format_ptr[j].max = (float) -10E30;
    }

// *************************************************************************************

    i_NumOfDataItems = i_NumOfParameters * n;

    if ( i_NumOfDataItems > i_minDataItems )
    {
        setWaitCursor();
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Find best format..." ), n );
    }

// *************************************************************************************
// check all data items

    while ( ( i < n ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        for ( int j=1; j<i_NumOfParameters; j++ )
        {
            s_dataItem = sl_Input.at( i ).section( "\t", j, j ).trimmed();

            if ( ( s_dataItem.isEmpty() == false ) && ( s_dataItem.contains( QRegExp( "[a-zA-Z: ]" ) ) == false ) && ( s_dataItem.count( "." ) < 2 ) )
            {
                if ( s_dataItem.count( "." ) < 1 )
                {
                    integer = s_dataItem.length();
                }
                else
                {
                    integer = s_dataItem.indexOf( "." );
                    digits  = s_dataItem.length() - s_dataItem.indexOf( "." ) - 1;

                    if ( digits > Format_ptr[j].digits )
                    {
                        Format_ptr[j].digits   = digits;
                        Format_ptr[j].lineDigi = i;
                    }
                }

                if ( integer > Format_ptr[j].integer )
                {
                    Format_ptr[j].integer = integer;
                    Format_ptr[j].lineInt = i;
                }

                Format_ptr[j].min = qMin( Format_ptr[j].min, s_dataItem.toFloat() );
                Format_ptr[j].max = qMax( Format_ptr[j].max, s_dataItem.toFloat() );
            }
        }

        ++i;

        if ( i_NumOfDataItems > i_minDataItems )
            i_stopProgress = incProgress( i_NumOfFiles, i );
    }

//-----------------------------------------------------------------------------------------------------------

    if ( i_NumOfDataItems > i_minDataItems )
    {
        resetProgress( i_NumOfFiles );
        setNormalCursor();
        setStatusBar( tr( "Done" ), 2 );
    }

    if ( i_stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}
