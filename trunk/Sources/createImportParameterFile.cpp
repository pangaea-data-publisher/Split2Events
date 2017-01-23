/* createImportParameterFile.cpp	*/
/* 2017-01-23						*/
/* Dr. Rainer Sieger				*/

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::createImportParameterFile( const QString &s_FilenameParameterImport, const int i_Codec, const bool b_match_against_WoRMS, QStringList &sl_ListParameterNew )
{
    QFile fout( s_FilenameParameterImport );

    if ( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return( -121 );

    QTextStream tout( &fout );

    switch ( i_Codec )
    {
    case _SYSTEM_: // nothing
        break;

    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;

    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;

    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    tout << tr( "ParameterName\tAbbreviation\tUnit\tDataGroup\tLowerLimit\tUpperLimit\tDefaultFormat\t" );
    tout << tr( "DefaultMethodID\tDefaultDataType\tReferenceID\tDescription\tURL Parameter" );

    if ( b_match_against_WoRMS == true )
        tout << tr( "\tSpecies name" );

    tout << endl;

    sl_ListParameterNew.sort();

    tout << sl_ListParameterNew.at( 0 ) << endl;

    if ( sl_ListParameterNew.count() > 1 )
    {
        for( int i=1; i<sl_ListParameterNew.count(); i++ )
        {
            if ( sl_ListParameterNew.at( i ) != sl_ListParameterNew.at( i-1 ) )
                tout << sl_ListParameterNew.at( i ) << endl;
        }
    }

    fout.close();

    return( _NOERROR_ );
}
