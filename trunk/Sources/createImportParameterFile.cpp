/* createImportParameterFile.cpp	*/
/* 2008-11-29						*/
/* Dr. Rainer Sieger				*/

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::createImportParameterFile( const QString &s_FilenameParameterImport, const bool b_match_against_WoRMS )
{
    QFile fout( s_FilenameParameterImport );

    if ( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return( -121 );

    QTextStream tout( &fout );

    tout << tr( "ParameterName\tAbbreviation\tUnit\tDataGroup\tLowerLimit\tUpperLimit\tDefaultFormat\tDefaultMethodID\tDefaultDataType\tReferenceID\tDescription\tURL Parameter" );

    if ( b_match_against_WoRMS == true )
        tout << tr( "\tSpecies name" );

    tout << endl;

    fout.close();

    return( _NOERROR_ );
}
