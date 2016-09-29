/* createMetadataFile.cpp     */
/* 2008-11-29                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

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

    s_ParameterAbbreviation.replace( ",", "" );
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
        if ( ( s_ParameterAbbreviation.contains( "@" ) == false ) || ( i_NumOfSections == 1 ) )
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
        s_ParameterNew.append( QString( "\t%1" ).arg( s_ParameterName.section( ",", 0, 0 ) ) );

    return( s_ParameterNew );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2011-12-07

int MainWindow::createMetadataTemplate( const QString &s_FilenameIn, const QString &s_FilenameMetadata, const QString &s_FilenameParameterImport,
                                        const int i_Codec, const structParameter p_ParameterList[], const QString &s_PI, const int i_MetadataFileMode,
                                        const bool b_createParameterImportFile, const bool b_match_against_WoRMS,
                                        const bool b_createAdditionMetadataOptions, const int i_NumOfFiles )

{
    int			  err                               = _NOERROR_;

    int			  i                                 = 0;
    int			  j                                 = 0;
    int			  k                                 = 0;
    int           n                                 = 0;

    int           i_stopProgress                    = 0;

    int           i_NumOfDataDescriptionLines       = 0;
    int			  i_NumOfParameters                 = 0;
    int           i_HeaderLine                      = 0;
    int           i_minNumOfParameters              = 30;

    QString       s_ParameterID                     = "";
    QString		  s_Parameter                       = "";
    QString		  s_ParameterName                   = "";
    QString       s_ParameterSearch                 = "";
    QString		  s_Format                          = "";
    QString		  s_Factor                          = "";
    QString		  s_Comment                         = "";
    QString       s_Method                          = "";

    QStringList   sl_DataDescription;
    QStringList	  sl_ListParameter;
    QStringList   sl_Input;

    structPFormat F_ptr[_MAX_NUM_OF_COLUMNS_+1];

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, gi_Codec ) ) < 2 )
        return( -111 );

// *************************************************************************************

    if ( ( i_NumOfDataDescriptionLines = readDataDescription( sl_Input, sl_DataDescription, i_HeaderLine, i ) ) < 0 )
        return( i_NumOfDataDescriptionLines );

    if ( ( i_NumOfParameters = readDataHeaderLine( sl_Input.at( i_HeaderLine ), sl_ListParameter ) ) <= 0 )
        return( i_NumOfParameters );

    if ( ( err = findFormat( s_FilenameIn, sl_Input, i_NumOfParameters, F_ptr, i_NumOfFiles ) ) != _NOERROR_ )
        return( err );

// **********************************************************************************************

    QFile fmeta( s_FilenameMetadata );
    if ( !fmeta.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return( -120 );

    QFile fpar( s_FilenameParameterImport );
    if ( !fpar.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ) )
    {
        fmeta.close();
        return( -121 );
    }

    QTextStream tmeta( &fmeta );
    QTextStream tpar( &fpar );

    switch ( i_Codec )
    {
    case _SYSTEM_: // nothing
        break;

    case _APPLEROMAN_:
        tmeta.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        tpar.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;

    case _LATIN1_:
        tmeta.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        tpar.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;

    default:
        tmeta.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        tpar.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    setWaitCursor();

    if ( i_NumOfFiles > 1 )
        i_minNumOfParameters = 9999;

    if ( i_NumOfParameters > i_minNumOfParameters )
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Find IDs or names of parameters ..." ), i_NumOfParameters );
    else
        setStatusBarFileInProgress( s_FilenameIn );

// **********************************************************************************************

    tmeta << tr( "[Parameter]" ) << endl;
    tmeta << tr( "Parameter name\tParameter ID\tPI ID\tMethod ID\tComment\tFormat\tFactor\tfill empty cells with\t" );
    tmeta << tr( "Range min\tRange max\tNumber of integers influenced by line\tNumber of digits influenced by line" ) << endl;

// **********************************************************************************************

    i = 0;

    for ( k=1; k<i_NumOfParameters; k++ ) // k = 0 => Event label
    {
        s_Parameter	= sl_ListParameter.at( k );
        s_Comment	= "";
        s_Method    = "43";  // not given

        ++i;

        if ( F_ptr[i].integer + F_ptr[i].digits > 0 )
        {
            s_Format = "";
            s_Factor = "1";

            if ( F_ptr[i].digits > 0 )
            {
                if ( F_ptr[i].integer <= 1 )
                {
                    s_Format.append( "#0." );
                }
                else
                {
                    s_Format.fill( '#', F_ptr[i].integer - 1 );
                    s_Format.append( "0." );
                }

                for ( j=1; j<=F_ptr[i].digits; j++ )
                    s_Format.append( "0" );
            }
            else
            {
                s_Format.fill( '#', F_ptr[i].integer - 1 );
                s_Format.append( "0" );
            }
        }
        else
        {
            s_Format = "";
            s_Factor = "";
        }

        if ( ( s_Format == "#" ) || ( s_Format == "0" ) )
            s_Format = "#0";

        bool b_OK          = false;
        int  i_ParameterID = s_Parameter.section( "@", 0, 0 ).toInt( &b_OK );

        if ( b_OK == true )
        {	// Parameter is given by ID
            if ( ( i_MetadataFileMode == _AUTO_ ) || ( i_ParameterID <= 0 ) )
            {
                tmeta << s_Parameter << "\t" << s_Parameter.section( "@", 0, 0 ) << "\t" << s_PI << "\t" << s_Method << "\t" << s_Parameter.section( "@", 1, 1 ) << "\t";
                tmeta << s_Format << "\t" << s_Factor << "\t\t" << "-10E30" << "\t" << "10E30" << endl;
            }
            else
            {
                s_ParameterName = findParameterByID( p_ParameterList, s_Parameter );

                tmeta << s_ParameterName << "\t" << s_Parameter.section( "@", 0, 0 ) << "\t" << s_PI << "\t" << s_Method << "\t" << s_Parameter.section( "@", 1, 1 ) << "\t";
                tmeta << s_Format << "\t" << s_Factor << "\t\t";

                if ( F_ptr[i].min < 9.999E30 )
                    tmeta << F_ptr[i].min - qAbs( F_ptr[i].min/100. ) << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].max > -9.999E30 )
                    tmeta << F_ptr[i].max + qAbs( F_ptr[i].max/100. ) << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].lineInt > 0 )
                    tmeta << F_ptr[i].lineInt << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].lineDigi > 0 )
                    tmeta << F_ptr[i].lineDigi;

                tmeta << endl;
            }
        }
        else
        {   // Parameter is given as text

            s_ParameterID     = tr( "unknown" );
            s_ParameterSearch = s_Parameter.section( "@", 0, 0 ).toLower();

            if ( ( s_ParameterSearch == "latitude" ) || ( s_ParameterSearch == "latitude []" ) )
            {
                s_ParameterID	= "1600";
                s_Factor		= "1";
            }

            if ( ( s_ParameterSearch == "longitude" ) || ( s_ParameterSearch == "longitude []" ) )
            {
                s_ParameterID	= "1601";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "depth, water [m]" )
            {
                s_ParameterID	= "1619";
                s_Factor		= "1";
            }


            if ( ( s_ParameterSearch == "m" ) || ( s_ParameterSearch == "depth, rock [m]" ) || ( s_ParameterSearch == "depth, sediment [m]" ) || ( s_ParameterSearch == "depth, sediment/rock [m]" )  || ( s_ParameterSearch == "depth, rock/sediment [m]" ) )
            {
                s_ParameterID	= "1";
                s_Factor		= "1";
            }

            if ( ( s_ParameterSearch == "cm" ) || ( s_ParameterSearch == "depth, sediment [cm]" ) )
            {
                s_ParameterID	= "1";
                s_Factor		= "0.01";

                if ( s_Format.contains( "." ) == true )
                    s_Format.append( "00" );
                else
                    s_Format.append( ".00" );
            }

            if ( s_ParameterSearch == "date/time" )
            {
                s_ParameterID	= "1599";
                s_Format		= "yyyy-MM-dd'T'HH:mm";
                s_Comment       = "for details see: http://icu-project.org/apiref/icu4j/com/ibm/icu/text/SimpleDateFormat.html";
            }

            if ( s_ParameterSearch == "depth, bathymetric [m]")
            {
                s_ParameterID	= "2268";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "depth, top [m]" )
            {
                s_ParameterID	= "3";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "depth, bottom [m]" )
            {
                s_ParameterID	= "4";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "volume [m**3]" )
            {
                s_ParameterID	= "8247";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "label" )
            {
                s_ParameterID	= "790";
                s_Format		= "";
            }

            if ( s_ParameterSearch == "temperature, water [deg c]" )
            {
                s_ParameterID	= "717";
                s_Factor		= "1";
            }

            if ( ( s_ParameterSearch == "salinity" ) || ( s_ParameterSearch == "salinity []" ) || ( s_ParameterSearch == "sal" ) || ( s_ParameterSearch == "sal []" ) )
            {
                if ( s_Parameter.section( "@", 1, 1 ).isEmpty() == true )
                    s_Parameter = "Salinity []";
                else
                    s_Parameter = "Salinity []@" + s_Parameter.section( "@", 1, 1 ).simplified();

                s_ParameterID	= "716";
                s_Factor		= "1";
            }

            if ( ( s_ParameterSearch == "salinity (psu)" ) || ( s_ParameterSearch == "sal (psu)" ) )
            {
                if ( s_Parameter.section( "@", 1, 1 ).isEmpty() == true )
                    s_Parameter = "Salinity []@PSU";
                else
                    s_Parameter = "Salinity []@PSU, " + s_Parameter.section( "@", 1, 1 ).simplified();

                s_ParameterID	= "716";
                s_Factor		= "1";
            }

            if ( s_ParameterSearch == "temperature, air [deg c]" )
            {
                s_ParameterID	= "4610";
                s_Factor		= "1";
            }

            if ( ( s_ParameterSearch == "oxygen saturation [%]" ) || ( s_ParameterSearch == "oxygen, saturation [%]" ) )
            {
                s_ParameterID	= "1883";
                s_Factor		= "1";
            }

            s_Comment = s_Parameter.section( "@", 1, 1 ).simplified();

            if ( s_Comment.isEmpty() == false )
            {
                s_Method = findMethod( s_Comment );

                s_Comment.replace( "(CTD)", "" );
                s_Comment.replace( "(CTD with attached Oxygen sensor (unspecified))", "" );
                s_Comment.replace( "(CTD with attached oxygen sensor)", "" );

                s_Comment.replace( "(PSU)", "PSU" );
                s_Comment.replace( "(PSS-78)", "PSS-78" );

                s_Comment.replace( "(CTD - PSU)", "PSU" );
                s_Comment.replace( "(CTD - PSS-78)", "PSS-78" );
                s_Comment.replace( "(CTD, Neil Brown, Mark III B - PSU)", "PSU" );

                s_Comment.replace( "(Salinometer, inductive - PSU)", "PSU" );
                s_Comment.replace( "(Salinometer, inductive - PSS-78)", "PSS-78" );

                s_Comment.replace( "(CTD - ITS-90)", "ITS-90" );
                s_Comment.replace( "(Reversing thermometer - ITS-90)", "ITS-90" );

                if ( ( s_Comment.startsWith( "(") == true ) && ( s_Comment.endsWith( ")" ) == true ) )
                    s_Comment = s_Comment.mid( 1, s_Comment.length()-2 );
            }

            if ( s_ParameterID == "unknown" )
            {
                s_ParameterSearch = s_Parameter.section( "@", 0, 0 ).simplified().toLower();

                s_ParameterSearch.append( " []" );
                s_ParameterSearch.replace( "] []", "]" );

                s_ParameterID = findParameterByName( p_ParameterList, s_ParameterSearch );

                if ( ( s_ParameterID == "unknown" ) && ( b_createParameterImportFile == true ) )
                    tpar << buildNewParameterEntry( s_Parameter, b_match_against_WoRMS ) << endl;
            }

            if ( i_MetadataFileMode == _AUTO_ )
            {
                tmeta << s_Parameter << "\t" << s_ParameterID << "\t" << s_PI << "\t" << s_Method << "\t";
                tmeta << s_Comment << "\t" << s_Format << "\t" << s_Factor << "\t\t" << "-10E30" << "\t" << "10E30" << endl;
            }
            else
            {
                tmeta << s_Parameter << "\t" << s_ParameterID.replace( "unknown", "unknown Parameter ID" ) << "\t";
                tmeta << s_PI << "\t" << s_Method << "\t" << s_Comment << "\t" << s_Format << "\t" << s_Factor << "\t\t";

                if ( F_ptr[i].min < 9.999E30 )
                    tmeta << F_ptr[i].min - qAbs( F_ptr[i].min/100. ) << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].max > -9.999E30 )
                    tmeta << F_ptr[i].max + qAbs( F_ptr[i].max/100. ) << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].lineInt > 0 )
                    tmeta << F_ptr[i].lineInt << "\t";
                else
                    tmeta << "\t";

                if ( F_ptr[i].lineDigi > 0 )
                    tmeta << F_ptr[i].lineDigi;

                tmeta << endl;
            }
        }

        if ( i_NumOfParameters > i_minNumOfParameters )
        {
            if ( ( i_stopProgress = incProgress( i_NumOfFiles, k ) ) == _APPBREAK_ )
                k = i_NumOfParameters;
        }
    }

// **********************************************************************************************

    if ( i_NumOfParameters > i_minNumOfParameters )
        resetProgress( i_NumOfFiles );

    setNormalCursor();

// **********************************************************************************************

    if ( ( i_MetadataFileMode != _AUTO_ ) && ( b_createAdditionMetadataOptions == true ) )
    {
        tmeta << tr( "[DataSet comment]" ) << endl;
        tmeta << tr( "Event label\tComment" ) << endl;
        tmeta << tr( "[ID(s) of reference(s) for further details]" ) << endl;
        tmeta << tr( "Event label\t345,45" ) << endl;
        tmeta << tr( "[ID(s) of dataset(s) for further details]" ) << endl;
        tmeta << tr( "Event label\t234129,123432" ) << endl;
        tmeta << tr( "[ID(s) of reference(s) for other version]" ) << endl;
        tmeta << tr( "Event label\t125,8" ) << endl;
        tmeta << tr( "[ID(s) of dataset(s) for other version]" ) << endl;
        tmeta << tr( "Event label\t802375,76523" ) << endl;
    }

    tmeta << tr( "[EOF]" ) << endl;

    fmeta.close();
    fpar.close();

    if ( b_createParameterImportFile == false )
        fpar.remove();

    if ( i_stopProgress == _APPBREAK_ )
    {
        fmeta.remove();
        return( _APPBREAK_ );
    }

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::findParameterByName(const structParameter p_ParameterList[], const QString &s_ParameterIn )
{
    QString s_ParameterID           = "unknown";

    QString s_Parameter             = "";
    QString s_Unit                  = "[";

    int     i_NumOfParametersInPDB  = p_ParameterList[0].ParameterID.toInt();
    int     i_NumOfSteps            = 0;
    int     i_MaxNumOfSteps         = i_NumOfParametersInPDB/4 + 1;

    int     p1                      = 1;
    int     p2                      = i_NumOfParametersInPDB/2;
    int     p3                      = i_NumOfParametersInPDB/2;
    int     p4                      = i_NumOfParametersInPDB;

    bool    b_Stop                  = false;

// *************************************************************************************

    s_Unit.append( s_ParameterIn.section( "@", 0, 0 ).section( " [", 1, 1 ) );

    if ( s_Unit == "[+/-]" )
        s_Unit = "[±]";

    if ( s_Unit.contains( "my") == true )
        s_Unit.replace( "my", "µ" );

    if ( s_Unit.contains( "deg" ) == true )
    {
        if ( s_Unit.contains( "deg c" ) == true )
            s_Unit.replace( "deg c", "°c" );

        if ( ( s_Unit != "[deg]" ) && ( s_Unit != "[s*deg]" ) && ( s_Unit != "[deg/min]" ) && ( s_Unit != "[deg s]" ) )
            s_Unit.replace( "deg", "°" );
    }

    s_Parameter = s_ParameterIn.section( "@", 0, 0 ).section( " [", 0, 0 ) + " " + s_Unit;

    if ( s_ParameterIn.contains( "]@" ) == true )
        s_Parameter.append( "@" + s_ParameterIn.section( "@", 1, 1 ) );

// *************************************************************************************

    while ( b_Stop == false )
    {
        if ( ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p1].ParameterNameL ) || ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p1].ParameterAbbreviationL ) )
            s_ParameterID = p_ParameterList[p1].ParameterID;

        if ( ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p2].ParameterNameL ) || ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p2].ParameterAbbreviationL ) )
            s_ParameterID = p_ParameterList[p2].ParameterID;

        if ( ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p3].ParameterNameL ) || ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p3].ParameterAbbreviationL ) )
            s_ParameterID = p_ParameterList[p3].ParameterID;

        if ( ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p4].ParameterNameL ) || ( s_Parameter.section( "@", 0, 0 ) == p_ParameterList[p4].ParameterAbbreviationL ) )
            s_ParameterID = p_ParameterList[p4].ParameterID;

        ++p1; --p2; ++p3; --p4;

        if ( ( s_ParameterID != "unknown" ) || ( ++i_NumOfSteps > i_MaxNumOfSteps ) )
            b_Stop = true;
    }

    if ( ( s_Parameter != "unknown" ) && ( s_Parameter.section( "@", 1, 1 ).isEmpty() == false ) )
        s_ParameterID.append( QString( "@%1" ).arg( s_Parameter.section( "@", 1, 1 ) ) );

    return( s_ParameterID );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::findParameterByID( const structParameter p_ParameterList[], const QString &s_Parameter )
{
    int     i_NumOfParametersInPDB  = p_ParameterList[0].ParameterID.toInt();
    int     i_ParameterID           = s_Parameter.section( "@", 0, 0 ).toInt();

    QString s_ParameterName         = "Parameter ID xxx is out-of-range!";

// *************************************************************************************

    if ( ( 1 <= i_ParameterID ) && ( i_ParameterID <= i_NumOfParametersInPDB ) )
    {
        s_ParameterName = p_ParameterList[i_ParameterID].ParameterName;

        if ( s_Parameter.section( "@", 1, 1 ).isEmpty() == false )
            s_ParameterName.append( QString( "@%1" ).arg( s_Parameter.section( "@", 1, 1 ) ) );

        return( s_ParameterName );
    }
    else
    {
        s_ParameterName = QString( "Parameter ID %1 is out-of-range!" ).arg( i_ParameterID );

        return( s_ParameterName );
    }

    return( "ERROR" );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

void MainWindow::doCreateMetadataTemplate()
{
    int         err							= _NOERROR_;
    int     	i							= 0;
    int         i_stopProgress				= 0;

    QString     s_FilenameMetadata			= "";
    QString     s_FilenameParameterImport	= "";

    QTime       timer;

    QDateTime   datetime = QDateTime::currentDateTime();

// *************************************************************************************

    if ( gsl_FilenameList.isEmpty() == true )  // no data file selected
        chooseFiles();

    if ( gsl_FilenameList.isEmpty() == true )  // no data file selected, choose aborted
        return;

    gsl_FilenameList = renameFiles( gsl_FilenameList, "_metadata", "" );

    if ( ( gi_NumOfParametersInPDB <= 0 ) && ( err == _NOERROR_ ) )
    {
        QFileInfo fi( gs_FilenamePDB );

        if ( fi.exists() == false )
            doCreateParameterDB();

        if ( fi.exists() == true )
            gi_NumOfParametersInPDB = readParameterDB( gs_FilenamePDB, gp_Parameter );
    }

    if ( ( gi_NumOfParametersInPDB > 0 ) && ( err == _NOERROR_ ) )
    {
        QFileInfo fi = QFileInfo( gsl_FilenameList.at( 0 ) );

        switch ( gi_Extension )
        {
        case _CSV_:
            s_FilenameParameterImport = fi.absolutePath() + "/" + tr( "imp_parameter_" ) + datetime.toString( "yyyyMMddhhmmss" ) + ".csv";
            break;
        default:
            s_FilenameParameterImport = fi.absolutePath() + "/" + tr( "imp_parameter_" ) + datetime.toString( "yyyyMMddhhmmss" ) + ".txt";
            break;
        }

//      s_FilenameParameterImport = fi.absolutePath() + "/" + tr( "imp_parameter_test" ) + ".txt";

        if ( gb_writeParameterImportFile == true )
            err = createImportParameterFile( s_FilenameParameterImport, gb_match_against_WoRMS );

// *************************************************************************************

        timer.start();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating metadata template files..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( i_stopProgress == 0 ) )
        {
            fi.setFile( gsl_FilenameList.at( i ) );

            switch ( gi_Extension )
            {
            case _CSV_:
                s_FilenameMetadata = fi.absolutePath() + "/" + fi.baseName() + "_metadata.csv";
                break;
            default:
                s_FilenameMetadata = fi.absolutePath() + "/" + fi.baseName() + "_metadata.txt";
                break;
            }

            err = createMetadataTemplate( gsl_FilenameList.at( i ), s_FilenameMetadata, s_FilenameParameterImport, gi_Codec, gp_Parameter,
                                          gs_PI, gi_MetadataFileMode, gb_writeParameterImportFile, gb_match_against_WoRMS,
                                          gb_createAdditionMetadataOptions, gsl_FilenameList.count() );

            i_stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }

    if ( gi_NumOfParametersInPDB < 0 )
        err = gi_NumOfParametersInPDB;

    if ( i_stopProgress == _APPBREAK_ )
        err = _APPBREAK_;

    if ( ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        if ( ( gb_writeParameterImportFile == true ) && ( gb_showParameterImportFileCreatedMessage == true ) )
        {
            QFileInfo fi( s_FilenameParameterImport );

            QString s_Message = tr( "A list of missing parameter(s) has been created. See\n\n" ) +
                                fi.completeBaseName() + tr( "." ) + fi.suffix() + tr( "\n\n" ) +
                                tr( "After completing this list create an issue and upload " ) +
                                tr( "the parameter import file." );

            QMessageBox::information( this, getApplicationName( true ), s_Message );
        }
        else
        {
            if ( ( gi_MetadataFileMode != _AUTO_ ) && ( timer.elapsed() > 10000 ) )
            {
                if ( gsl_FilenameList.count() == 1 )
                    QMessageBox::information( this, getApplicationName( true ), tr( "Metadata template has been created." ) );
                else
                    QMessageBox::information( this, getApplicationName( true ), tr( "Metadata templates have been created." ) );
            }
        }

        setStatusBar( tr( "Done" ), 2 );
    }
    else
    {
        setStatusBar( tr( "Creating metadata template files was canceled" ), 2 );
    }

    onError( err );
}
