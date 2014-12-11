/* createMetadataFile.cpp     */
/* 2008-11-29                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2011-12-07

int MainWindow::createMetadataTemplate( const QString& s_FilenameIn, const QString& s_FilenameMetadata, const QString& s_FilenameParameterImport,
                                        const int i_Codec, const structParameter p_ParameterList[], const QString& s_PI,
                                        const int i_MetadataFileMode, const bool b_createParameterImportFile, const bool b_createAdditionMetadataOptions,
                                        const int i_NumOfFiles )

{
    int			  err                               = _NOERROR_;

    int			  i                                 = 0;
    int			  j                                 = 0;
    int			  k                                 = 0;
    int           n                                 = 0;

    int           i_stopProgress                    = 0;

    int           i_NumOfDataDescriptionLines       = 0;
    int			  i_NumOfParameters                 = 0;
    int			  i_DataType                        = 0;
    int           i_HeaderLine                      = 0;

    QString       s_ParameterID                     = "";
    QString		  s_Parameter                       = "";
    QString		  s_ParameterName                   = "";
    QString		  s_ParameterAbbreviation           = "";
    QString		  s_Unit                            = "";
    QString		  s_Format                          = "";
    QString		  s_Factor                          = "";
    QString		  s_Comment                         = "";
    QString       s_Method                          = "";

    QStringList   sl_DataDescription;
    QStringList	  sl_ListParameter;
    QStringList   sl_Input;

    QTime         timer;

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

    timer.start();

    setWaitCursor();
    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Find IDs or names of parameters ..." ), i_NumOfParameters );

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
                    s_Format.append( "#0." );
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
            s_ParameterID = tr( "unknown" );

            if ( s_Parameter.toLower() == "latitude" )
            {
                s_ParameterID	= "1600";
                s_Factor		= "1";
            }

            if ( s_Parameter.toLower() == "longitude" )
            {
                s_ParameterID	= "1601";
                s_Factor		= "1";
            }

            if ( s_Parameter.toLower() == "depth, water [m]" )
            {
                s_ParameterID	= "1619";
                s_Factor		= "1";
            }

            if ( ( s_Parameter.toLower() == "m" ) || ( s_Parameter.toLower() == "depth, rock [m]" ) || ( s_Parameter.toLower() == "depth, sediment [m]" ) || ( s_Parameter.toLower() == "depth, sediment/rock [m]" )  || ( s_Parameter.toLower() == "depth, rock/sediment [m]" ) )
            {
                s_ParameterID	= "1";
                s_Factor		= "1";
            }

            if ( ( s_Parameter.toLower() == "cm" ) || ( s_Parameter.toLower() == "depth, sediment [cm]" ) )
            {
                s_ParameterID	= "1";
                s_Factor		= "0.01";

                if ( s_Format.contains( "." ) == true )
                    s_Format.append( "00" );
                else
                    s_Format.append( ".00" );
            }

            if ( s_Parameter.toLower() == "date/time" )
            {
                s_ParameterID	= "1599";
                s_Format		= "yyyy-MM-dd'T'HH:mm";
                s_Comment       = "for details see: http://icu-project.org/apiref/icu4j/com/ibm/icu/text/SimpleDateFormat.html";
            }

            if ( s_Parameter.startsWith( "depth, bathymetric [m]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "2268";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "depth, top [m]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "3";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "depth, bottom [m]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "4";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "volume [m**3]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "8247";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "label", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "790";
                s_Format		= "";
            }

            if ( s_Parameter.startsWith( "temperature, water [deg c]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "717";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "salinity (", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "716";
                s_Factor		= "1";
            }

            if ( s_Parameter.startsWith( "temperature, air [deg c]", Qt::CaseInsensitive ) == true )
            {
                s_ParameterID	= "4610";
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
                QString s_ParameterSearch = s_Parameter.section( "@", 0, 0 ).simplified().toLower();

                s_ParameterSearch.append( " []" );
                s_ParameterSearch.replace( "] []", "]" );

                s_ParameterID = findParameterByName( p_ParameterList, s_ParameterSearch );

                if ( ( s_ParameterID == "unknown" ) && ( b_createParameterImportFile == true ) )
                {
                    QString s_ParameterNew = s_Parameter.section( "@", 0, 0 ).simplified();

                    s_ParameterNew.replace( " [", "\t" );
                    s_ParameterNew.replace( "[", "\t" );
                    s_ParameterNew.replace( "]", "" );

                    s_ParameterName			= s_ParameterNew.section( "\t", 0, 0 );
                    s_ParameterAbbreviation	= s_ParameterNew.section( "\t", 0, 0 );
                    s_Unit					= s_ParameterNew.section( "\t", 1, 1 );

                    i_DataType = 2;  // Parameter is a text parameter

                    if ( s_Unit.isEmpty() == false )
                    {
                        i_DataType = 1;

                        s_Unit.replace( "+/-", "±" );
                        s_Unit.replace( "my", "µ" );
                        s_Unit.replace( "deg ", "°" );
                        s_Unit.replace( "deg", "°" );
                    }

                    tpar << s_ParameterName << "\t";

                    s_ParameterAbbreviation.replace( " ", "\t" );

                    int i_NumOfSections = NumOfSections( s_ParameterAbbreviation );

                    if ( i_NumOfSections > 1 )
                    {
                        if ( s_ParameterAbbreviation.section( "\t", 0, 0 ).endsWith( "," ) )
                        {
                            tpar << s_ParameterAbbreviation.section( "\t", 0, 0 );
                        }
                        else
                        {
                            if ( ( s_ParameterAbbreviation.section( "\t", 1, 1 ).length() < 5 ) || ( s_ParameterAbbreviation.section( "\t", 1, 1 ).endsWith( "p.," ) == true ) || ( s_ParameterAbbreviation.section( "\t", 1, 1 ).endsWith( "male" ) == true ) || ( s_ParameterAbbreviation.section( "\t", 1, 1 ) == "larvae" ) )
                                tpar << s_ParameterAbbreviation.section( "\t", 0, 0 );
                            else
                                tpar << s_ParameterAbbreviation.section( "\t", 0, 0 ).left( 1 ) << ".";
                        }

                        for ( j=1; j<i_NumOfSections; j++ )
                            tpar << " " << s_ParameterAbbreviation.section( "\t", j, j );
                    }
                    else
                    {
                        tpar << s_ParameterAbbreviation;
                    }

                    tpar << "\t" << s_Unit << "\t\t\t\t\t\t\t" << i_DataType << endl;
                }
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

        if ( ( i_stopProgress = incProgress( i_NumOfFiles, k ) ) == _APPBREAK_ )
            k = i_NumOfParameters;
    }

// **********************************************************************************************

    resetProgress( i_NumOfFiles );
    setNormalCursor();
    setStatusBar( tr( "Done" ), 2 );

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

    if ( ( i_MetadataFileMode != _AUTO_ ) && ( timer.elapsed() > 10000 ) )
        QMessageBox::information( this, getApplicationName( true ), tr( "Metadata template has been created." ) );

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::findParameterByName(const structParameter p_ParameterList[], const QString& s_ParameterIn )
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

QString MainWindow::findParameterByID( const structParameter p_ParameterList[], const QString& s_Parameter )
{
    int     i_ParameterID   = s_Parameter.section( "@", 0, 0 ).toInt();
    QString s_ParameterName	= p_ParameterList[i_ParameterID].ParameterName;

    if ( s_Parameter.section( "@", 1, 1 ).isEmpty() == false )
        s_ParameterName.append( QString( "@%1" ).arg( s_Parameter.section( "@", 1, 1 ) ) );

    return( s_ParameterName );
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

        if ( gb_writeParameterImportFile == true )
            err = createImportParameterFile( s_FilenameParameterImport );

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
                                          gs_PI, gi_MetadataFileMode, gb_writeParameterImportFile,
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
                                tr( "After completing this file (See http://wiki.pangaea.de/wiki/Parameter for details) " ) +
                                tr( "create an issue (http://issues.pangaea.de) and upload the parameter import file\n" ) +
                                tr( "to the issue." );

            QMessageBox::information( this, getApplicationName( true ), s_Message );
        }

        setStatusBar( tr( "Done" ), 2 );
    }
    else
    {
        setStatusBar( tr( "Creating metadata template files was canceled" ), 2 );
    }

    onError( err );
}
