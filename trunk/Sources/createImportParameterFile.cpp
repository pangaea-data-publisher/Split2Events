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

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2016-01-08

QString MainWindow::buildNewParameterEntry( const QString &s_Parameter, const bool b_match_against_WoRMS )
{
    int     i_DataType              = 2;  // Parameter is a text parameter
    int     i_NumOfSections         = 0;

    QString s_ParameterName         = "";
    QString s_ParameterAbbreviation = "";
    QString s_Unit                  = "";
    QString s_ParameterNew          =  s_Parameter.section( "@", 0, 0 ).simplified();

// *************************************************************************************

    s_ParameterNew.replace( " [", "\t" );
    s_ParameterNew.replace( "[", "\t" );
    s_ParameterNew.replace( "]", "" );

    s_ParameterName			= s_ParameterNew.section( "\t", 0, 0 );
    s_ParameterAbbreviation	= s_ParameterNew.section( "\t", 0, 0 );
    s_Unit					= s_ParameterNew.section( "\t", 1, 1 );

// *************************************************************************************
// special parameter abbreviations

    s_ParameterAbbreviation.replace( "forma", "f@" );
    s_ParameterAbbreviation.replace( "length", "l@" );
    s_ParameterAbbreviation.replace( "width", "w@" );
    s_ParameterAbbreviation.replace( "biomass as carbon", "C@" );
    s_ParameterAbbreviation.replace( "biomass", "biom@" );
    s_ParameterAbbreviation.replace( "dry mass", "dm@" );
    s_ParameterAbbreviation.replace( "wet mass", "wm@" );
    s_ParameterAbbreviation.replace( "mass", "m@" );
    s_ParameterAbbreviation.replace( "fractionated", "frac@" );
    s_ParameterAbbreviation.replace( "fragments", "fragm@" );

    s_ParameterAbbreviation.replace( "copepodites", "c@" );
    s_ParameterAbbreviation.replace( "c1", "c1@" );
    s_ParameterAbbreviation.replace( "c2", "c2@" );
    s_ParameterAbbreviation.replace( "c3", "c3@" );
    s_ParameterAbbreviation.replace( "c4", "c4@" );
    s_ParameterAbbreviation.replace( "c5", "c5@" );
    s_ParameterAbbreviation.replace( "nauplii", "naup@" );
    s_ParameterAbbreviation.replace( "adult", "ad@" );
    s_ParameterAbbreviation.replace( "female", "f@" );
    s_ParameterAbbreviation.replace( "male", "m@" );
    s_ParameterAbbreviation.replace( "larvae", "larv@" );
    s_ParameterAbbreviation.replace( "juvenile", "juv@" );

    s_ParameterAbbreviation.replace( "-type", "-T@" );
    s_ParameterAbbreviation.replace( "planktic", "plankt@" );
    s_ParameterAbbreviation.replace( "bentic", "bent@" );
    s_ParameterAbbreviation.replace( "other", "oth@" );
    s_ParameterAbbreviation.replace( "cover", "cov@" );
    s_ParameterAbbreviation.replace( "cf.", "cf.@" );
    s_ParameterAbbreviation.replace( "aff.", "aff.@" );
    s_ParameterAbbreviation.replace( "d13C", "d13C@" );
    s_ParameterAbbreviation.replace( "d18O", "d18O@" );

    s_ParameterAbbreviation.replace( "fraction", "fr@@" );
    s_ParameterAbbreviation.replace( "per unit mass organic carbon", "OC@@" );
    s_ParameterAbbreviation.replace( "per unit mass total organic carbon", "/TOC@@" );
    s_ParameterAbbreviation.replace( "per unit sediment mass", "/sed@@" );
    s_ParameterAbbreviation.replace( "spp.", "spp.@@" );
    s_ParameterAbbreviation.replace( "sp.", "sp.@@" );
    s_ParameterAbbreviation.replace( "cyst", "cyst@@" );
    s_ParameterAbbreviation.replace( "maximal", "max@@" );
    s_ParameterAbbreviation.replace( "minimal", "min@@" );
    s_ParameterAbbreviation.replace( "maximum", "max@@" );
    s_ParameterAbbreviation.replace( "minimum", "min@@" );
    s_ParameterAbbreviation.replace( "standard deviation", "std dev@@" );
    s_ParameterAbbreviation.replace( "indeterminata", "indet@@" );

// *************************************************************************************
// data type

    if ( s_Unit.isEmpty() == false )
        i_DataType = 1;

    if ( s_ParameterName.endsWith( "ratio") == true )
    {
        s_ParameterAbbreviation.replace( "ratio", "@@" );
        i_DataType = 1;
    }

// *************************************************************************************

    s_ParameterAbbreviation.replace( ", ", "@" );
    s_ParameterAbbreviation.replace( " /", "/" );
    s_ParameterAbbreviation.replace( "/ ", "/" );

    s_ParameterAbbreviation.replace( " ", "\t" );

    i_NumOfSections = NumOfSections( s_ParameterAbbreviation );

    if ( i_NumOfSections == 2 )
        s_ParameterAbbreviation.append( "\t@" );
    else
        s_ParameterAbbreviation.append( "\t" );

    s_ParameterNew = s_ParameterName;

    if ( s_ParameterAbbreviation.section( "\t", 1, 1 ).contains( "@@")  == true )
    {
        s_ParameterNew.append( QString( "\t%1").arg( s_ParameterAbbreviation.section( "\t", 0, 0 ) ) );
    }
    else
    {
        if ( ( s_ParameterAbbreviation.contains( "@" ) == false ) || ( i_NumOfSections == 1 ) || ( s_ParameterAbbreviation.section( "\t", 0, 0 ).length() < 6 ) )
            s_ParameterNew.append( QString( "\t%1").arg( s_ParameterAbbreviation.section( "\t", 0, 0 ) ) );
        else
            s_ParameterNew.append( QString( "\t%1.").arg( s_ParameterAbbreviation.section( "\t", 0, 0 ).left( 1 ) ) );
    }

    for ( int i=1; i<i_NumOfSections; i++ )
        s_ParameterNew.append( QString( " %1").arg( s_ParameterAbbreviation.section( "\t", i, i ) ) );

    s_ParameterNew.append( QString( "\t%1\t\t\t\t\t\t%2\t\t\t").arg( s_Unit ).arg( i_DataType ) );

    s_ParameterNew.replace( "@", "" );
    s_ParameterNew.replace( " \t", "\t" );

    if ( b_match_against_WoRMS == true )
        s_ParameterNew.append( QString( "http://www.marinespecies.org/aphia.php?p=taxdetails&id=xxx\t%1" ).arg( s_ParameterName.section( ",", 0, 0 ) ) );

    return( s_ParameterNew );
}
