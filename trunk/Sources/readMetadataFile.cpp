/****************************************************************
**
** readMetadataFile
** 01.06.2003, Dr. Rainer Sieger
** last change: 19.01.2005
**
****************************************************************/

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::getMetadataFilename( const QString& s_firstFilenameIn, const int i_Extension, const int i_NumOfFiles )
{
    QString s_FilenameMetadata        = "";
    QString s_FilenameMetadataDefault = "";

// *************************************************************************************

    QFileInfo fi( s_firstFilenameIn );

    if ( ( fi.exists() == true ) && ( fi.isFile() == true ) )
    {
        switch ( i_Extension )
        {
        case _CSV_:
            s_FilenameMetadataDefault = fi.absolutePath() + "/" + tr( "default" ) + tr( "_metadata.csv" );
            s_FilenameMetadata        = fi.absolutePath() + "/" + fi.baseName() + tr( "_metadata.csv" );
            break;
        default:
            s_FilenameMetadataDefault = fi.absolutePath() + "/" + tr( "default" ) + tr( "_metadata.txt" );
            s_FilenameMetadata        = fi.absolutePath() + "/" + fi.baseName() + tr( "_metadata.txt" );
            break;
        }

        if ( i_NumOfFiles == 1 )
        {
            fi.setFile( s_FilenameMetadata );

            if ( ( fi.exists() == false ) || ( fi.isFile() == false ) )
                fi.setFile( s_FilenameMetadataDefault );
        }
        else
        {
            fi.setFile( s_FilenameMetadataDefault );
        }

        if ( ( fi.exists() == false ) || ( fi.isFile() == false ) )
        {

            #if defined(Q_OS_LINUX)
                switch ( i_Extension )
                {
                case _CSV_:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.csv)\nMetadata files (*_metadata.txt)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                default:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.txt)\nMetadata files (*_metadata.csv)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                }
            #endif

            #if defined(Q_OS_WIN)
                switch ( i_Extension )
                {
                case _CSV_:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.csv)\nMetadata files (*_metadata.txt)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                default:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.txt)\nMetadata files (*_metadata.csv)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                }
            #endif

            #if defined(Q_OS_MAC)
                switch ( i_Extension )
                {
                case _CSV_:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.csv)\nMetadata files (*_metadata.txt)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                default:
                    s_FilenameMetadata = QFileDialog::getOpenFileName( this, tr( "Choose metadata file" ), fi.absolutePath(), tr( "Metadata files (*_metadata.txt)\nMetadata files (*_metadata.csv)" ), 0, QFileDialog::DontUseNativeDialog );
                    break;
                }
            #endif

            fi.setFile( s_FilenameMetadata );
        }
    }

// *************************************************************************************

    if ( ( fi.exists() == false ) || ( fi.isFile() == false ) )
        s_FilenameMetadata = "";
    else
        s_FilenameMetadata = fi.absoluteFilePath();

// *************************************************************************************

    return( s_FilenameMetadata );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::readMetadataFile( const QString& s_FilenameMetadata, QStringList& sl_ParameterList, QStringList& sl_DataSetComment,
                                  QStringList& sl_FurtherDetailsReference, QStringList& sl_FurtherDetailsDataset,
                                  QStringList& sl_OtherVersionReference, QStringList& sl_OtherVersionDataset )
{
    int         i                   = 0;
    int         n                   = 0;

    int         i_stopProgress      = 0;

    QString     s_EventLabel        = "";
    QString     s_Dummy             = "";

    QStringList sl_Input;

//------------------------------------------------------------------

    if ( s_FilenameMetadata.isEmpty() == true )
        return( _APPBREAK_ );

//------------------------------------------------------------------

    sl_ParameterList.clear();
    sl_DataSetComment.clear();
    sl_FurtherDetailsReference.clear();
    sl_FurtherDetailsDataset.clear();
    sl_OtherVersionReference.clear();
    sl_OtherVersionDataset.clear();

//------------------------------------------------------------------

    if ( ( n = readFile( s_FilenameMetadata, sl_Input, gi_Codec ) ) < 1 )
        return( -112 );

//------------------------------------------------------------------

    initProgress( 1, s_FilenameMetadata, tr( "Reading metadata file..." ), n );

//------------------------------------------------------------------
// Parameters

    if ( sl_Input.at( 0 ).startsWith( tr( "[Parameter]" ) ) == false )
        return( -30 ); // Wrong format

    i = 2;

    while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).isEmpty() == false )
            sl_ParameterList.append( trimParameter( sl_Input.at( i ) ) );

        i_stopProgress = incProgress( 1, ++i );
    }

//------------------------------------------------------------------
// Dataset comments

    while ( ( i < n ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        if ( ( i < n ) && ( sl_Input.at( i ).startsWith( tr( "[DataSet comment]" ) ) == true ) )
        {
            i++;

            while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
            {
                if ( sl_Input.at( i ).isEmpty() == false )
                    sl_DataSetComment.append( sl_Input.at( i ) );

                i_stopProgress = incProgress( 1, ++i );
            }
        }

//------------------------------------------------------------------
// ID(s) of reference(s) for further details

        if ( ( i < n ) && ( sl_Input.at( i ).startsWith( tr( "[ID(s) of reference(s) for further details]" ) ) == true ) )
        {
            i++;

            while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
            {
                if ( sl_Input.at( i ).isEmpty() == false )
                {
                    s_EventLabel = sl_Input.at( i ).section( "\t", 0, 0 );
                    s_Dummy      = sl_Input.at( i ).section( "\t", 1, 1 );

                    s_Dummy.replace( " ", "" );
                    s_Dummy.replace( ";", "," );

                    for( int j=0; j<=s_Dummy.count( "," ); j++ )
                        sl_FurtherDetailsReference.append( s_EventLabel + "\t" + s_Dummy.section( ",", j, j ) );
                }

                i_stopProgress = incProgress( 1, ++i );
            }
        }

//------------------------------------------------------------------
// ID(s) of dataset(s) for further details

        if ( ( i < n ) && ( sl_Input.at( i ).startsWith( tr( "[ID(s) of dataset(s) for further details]" ) ) == true ) )
        {
            i++;

            while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
            {
                if ( sl_Input.at( i ).isEmpty() == false )
                {
                    s_EventLabel = sl_Input.at( i ).section( "\t", 0, 0 );
                    s_Dummy      = sl_Input.at( i ).section( "\t", 1, 1 );

                    s_Dummy.replace( " ", "" );
                    s_Dummy.replace( ";", "," );

                    for( int j=0; j<=s_Dummy.count( "," ); j++ )
                        sl_FurtherDetailsDataset.append( s_EventLabel + "\t" + s_Dummy.section( ",", j, j ) );
                }

                i_stopProgress = incProgress( 1, ++i );
            }
        }

//------------------------------------------------------------------
// ID(s) of reference(s) for other version

        if ( ( i < n ) && ( sl_Input.at( i ).startsWith( tr( "[ID(s) of reference(s) for other version]" ) ) == true ) )
        {
            i++;

            while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
            {
                if ( sl_Input.at( i ).isEmpty() == false )
                {
                    s_EventLabel = sl_Input.at( i ).section( "\t", 0, 0 );
                    s_Dummy      = sl_Input.at( i ).section( "\t", 1, 1 );

                    s_Dummy.replace( " ", "" );
                    s_Dummy.replace( ";", "," );

                    for( int j=0; j<=s_Dummy.count( "," ); j++ )
                        sl_OtherVersionReference.append( s_EventLabel + "\t" + s_Dummy.section( ",", j, j ) );
                }

                i_stopProgress = incProgress( 1, ++i );
            }
        }

//------------------------------------------------------------------
// ID(s) of dataset(s) for other version

        if ( ( i < n ) && ( sl_Input.at( i ).startsWith( tr( "[ID(s) of dataset(s) for other version]" ) ) == true ) )
        {
            i++;

            while ( ( i < n ) && ( sl_Input.at( i ).startsWith( "[" ) == false ) && ( i_stopProgress != _APPBREAK_ ) )
            {
                if ( sl_Input.at( i ).isEmpty() == false )
                {
                    s_EventLabel = sl_Input.at( i ).section( "\t", 0, 0 );
                    s_Dummy      = sl_Input.at( i ).section( "\t", 1, 1 );

                    s_Dummy.replace( " ", "" );
                    s_Dummy.replace( ";", "," );

                    for( int j=0; j<=s_Dummy.count( "," ); j++ )
                        sl_OtherVersionDataset.append( s_EventLabel + "\t" + s_Dummy.section( ",", j, j ) );
                }

                i_stopProgress = incProgress( 1, ++i );
            }
        }

        i++;
    }

//-----------------------------------------------------------------------------------------------------------

    resetProgress( 1 );

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::trimParameter( const QString& ParameterInStr )
{
    int     i_ParameterID   = 0;

    QString s_dummy         = ParameterInStr;
    QString ParameterOutStr = "";

// *************************************************************************************

    s_dummy.replace( "\"", "" );

    i_ParameterID = s_dummy.section( "\t", 1, 1 ).toInt();

    if ( s_dummy.section( "\t", 0, 0 ).isEmpty() == false ) // real name
        ParameterOutStr = s_dummy.section( "\t", 0, 0 ) + "\t";
    else
        ParameterOutStr = "\t";

    if ( i_ParameterID > 0 )	// PANGAEA ID
        ParameterOutStr += QString( "%1\t" ).arg( i_ParameterID );
    else
        ParameterOutStr += s_dummy.section( "\t", 1, 1 ) + "\t";

    if ( s_dummy.section( "\t", 2, 2 ).toInt() > 0 ) // PI ID
        ParameterOutStr += QString( "%1\t" ).arg( s_dummy.section( "\t", 2, 2 ).toInt() );
    else
        ParameterOutStr += "506\t"; // not given

    if ( s_dummy.section( "\t", 3, 3 ).toInt() > 0 ) // Method ID
        ParameterOutStr += QString( "%1\t" ).arg( s_dummy.section( "\t", 3, 3 ).toInt() );
    else
        if ( s_dummy.section( "\t", 3, 3 ) == "default" )
            ParameterOutStr += "\t"; // default method as set in 4D
        else
            ParameterOutStr += "43\t"; // not given

    switch ( i_ParameterID )
    {   // Geocodes
        case 1:     // Depth, sediment/rock
        case 1599:  // Date/Time
        case 1600:  // Latitude
        case 1601:  // Longitude
        case 1619:  // Depth, water
        case 2205:  // Age
        case 2920:  // Distance
        case 4607:  // Altitude
        case 5059:  // Depth, ice/snow
        case 6262:  // Oridinal Number
        case 8128:  // Elevation
        case 25539: // Section
        case 26097: // Point distance
        case 56349: // Height above ground
            ParameterOutStr += "\t"; // no comment for geocodes
            break;

        default:
            if ( s_dummy.section( "\t", 4, 4 ).isEmpty() == false ) // Comment
                ParameterOutStr += s_dummy.section( "\t", 4, 4 ) + "\t";
            else
                ParameterOutStr += "\t"; // not given
            break;
    }

    if ( s_dummy.section( "\t", 5, 5 ).isEmpty() == false ) // Format
        ParameterOutStr += s_dummy.section( "\t", 5, 5 ) + "\t";
    else
        ParameterOutStr += "\t"; // not given

    if ( s_dummy.section( "\t", 6, 6 ).isEmpty() == false ) // Factor
        ParameterOutStr += s_dummy.section( "\t", 6, 6 ) + "\t";
    else
        ParameterOutStr += "1\t"; // not given

    if ( s_dummy.section( "\t", 7, 7 ).isEmpty() == false ) // missing value
        ParameterOutStr += s_dummy.section( "\t", 7, 7 ) + "\t";
    else
        ParameterOutStr += "\t"; // not given

    if ( s_dummy.section( "\t", 8, 8 ).isEmpty() == false ) // Range min
        ParameterOutStr += s_dummy.section( "\t", 8, 8 ) + "\t";
    else
        ParameterOutStr += "-10E30\t"; // not given

    if ( s_dummy.section( "\t", 9, 9 ).isEmpty() == false ) // Range max
        ParameterOutStr += s_dummy.section( "\t", 9, 9 );
    else
        ParameterOutStr += "+10E30"; // not given

    return( ParameterOutStr );
}
