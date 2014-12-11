/* createImportParameterFile.cpp	*/
/* 2008-11-29						*/
/* Dr. Rainer Sieger				*/

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::createImportParameterFile( const QString& s_FilenameParameterImport )
{
    QFile fout( s_FilenameParameterImport );

    if ( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return( -121 );

    QTextStream tout( &fout );

//  tout << tr( "// See http://wiki.pangaea.de/wiki/Parameter for details" ) << endl << endl;
    tout << tr( "ParameterName\tAbbreviation\tUnit\tParameterGroupID\tDataGroup\tLowerLimit\tUpperLimit\tDefaultFormat\tDefaultMethodID\tDefaultDataType\tReferenceID\tDescription\tURL Parameter" ) << endl;

    fout.close();

    return( _NOERROR_ );
}
