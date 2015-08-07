/****************************************************************
**
** Class Split2Events
** 2003-06-01, Dr. Rainer Sieger
** last change: 2012-02-07
**
****************************************************************/

#include "Application.h"

int MainWindow::Split2Events( const QString& s_FilenameIn, const int i_Codec, const int i_Extension,
                              const QStringList& sl_MFParameter, const QStringList& sl_DataSetComment,
                              const QStringList& sl_FurtherDetailsReference, const QStringList& sl_FurtherDetailsDataset,
                              const QStringList& sl_OtherVersionReference, const QStringList& sl_OtherVersionDataset,
                              const QStringList& sl_SourceReference, const QStringList& sl_SourceDataset,
                              const QString& s_Author, const QString& s_Source, const QString& s_DatasetTitle, const QString& s_ExportFilename,
                              const QString& s_Reference, const QString& s_Project, const QString& s_DataSetComment,
                              const QString& s_FurtherDetailsReference, const QString& s_FurtherDetailsDataset,
                              const QString& s_OtherVersionReference, const QString& s_OtherVersionDataset,
                              const QString& s_SourceReference, const QString& s_SourceDataset,
                              const QString& s_PI, const QString& s_User, const QString& s_Parent, const int i_Status, const int i_Login,
                              const bool b_writeHeader, const bool b_splitFile, const int i_OutOfRangeValue,
                              const bool b_useFilenameInAsEventLabel, const bool b_makeFilenameUnique,
                              const int i_MetadataFileMode, const int i_TopologicType, const bool b_overwriteDataset,
                              const bool b_markSmallFile, const int i_FileNo, const int i_NumOfFiles )
{
    int         i                           = 0;
    int         j                           = 0;
    int         n                           = 0;

    int         err                         = _NOERROR_;

    int         i_stopProgress              = 0;
    int         i_NumOfParameters           = 0;
    int         i_emptyDataItem             = 0;
    int         i_NumOfDataDescriptionLines = 0;
    int         i_HeaderLine                = 0;
    int         i_NumOfSavedDataLines       = 0;

    bool        b_hasManyEvents             = false;

    QString     s_OutputPath                = "";

    QStringList sl_Input;
    QStringList sl_Data;
    QStringList sl_DSParameter;
    QStringList sl_DataDescription;

// *************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_Codec ) ) < 1 )
        return( -111 );

// *************************************************************************************

    if ( i_FileNo == 0 )
        s_OutputPath = createDir( s_FilenameIn, i_NumOfFiles, true );
    else
        s_OutputPath = createDir( s_FilenameIn, i_NumOfFiles, false );

// *************************************************************************************

    if ( ( i_NumOfDataDescriptionLines = readDataDescription( sl_Input, sl_DataDescription, i_HeaderLine, i ) ) < 0 )
        return( i_NumOfDataDescriptionLines );

// *************************************************************************************

    if ( ( i_NumOfParameters = readDataHeaderLine( sl_Input.at( i_HeaderLine ), sl_DSParameter ) ) <= 0 )
        return( i_NumOfParameters );

// *************************************************************************************

    QFileInfo fi( s_FilenameIn );

// *************************************************************************************

    sl_Input = addColumns( sl_Input, i_NumOfParameters+1 );

// *************************************************************************************

    if ( b_splitFile == false )
    {
        int k = i;

        while ( ++k < n )
        {
          if ( isNewEvent( sl_Input, k ) == true )
            b_hasManyEvents = true;
        }
    }

// *************************************************************************************

    if ( b_splitFile == true )
    {
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Splitting file..." ), 2*(n-i_NumOfDataDescriptionLines-1) );

        resetDataList( sl_Input, i_HeaderLine, i, sl_Data );

        while ( ( ++i < n ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
        {
            if ( isNewEvent( sl_Input, i ) == true )
            {
                err = writeDataImportFile( fi.baseName(), s_OutputPath, _UTF8_, i_Extension, sl_Data, sl_DSParameter, sl_MFParameter, sl_DataSetComment,
                                           sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset,
                                           sl_SourceReference, sl_SourceDataset, s_Author, s_Source, s_DatasetTitle, s_ExportFilename,
                                           s_Reference, s_Project, s_DataSetComment, s_FurtherDetailsReference, s_FurtherDetailsDataset, s_OtherVersionReference, s_OtherVersionDataset,
                                           s_SourceReference, s_SourceDataset, s_PI, s_User, s_Parent, i_Status, i_Login, b_writeHeader, b_splitFile,
                                           b_useFilenameInAsEventLabel, b_makeFilenameUnique, b_hasManyEvents, i_MetadataFileMode,
                                           i_TopologicType, b_overwriteDataset, b_markSmallFile, i_NumOfSavedDataLines+j, i_OutOfRangeValue, i_NumOfFiles );

                i_NumOfSavedDataLines += 2*(sl_Data.count()-1);
                i_stopProgress         = incProgress( i_NumOfFiles, i_NumOfSavedDataLines );
                j                      = 0;

                resetDataList( sl_Input, i_HeaderLine, i, sl_Data );
            }
            else
            {
                sl_Data.append( buildDataLine( sl_Input, i ) );

                i_stopProgress = incProgress( i_NumOfFiles, ++j+i_NumOfSavedDataLines );
            }
        }

        err = writeDataImportFile( fi.baseName(), s_OutputPath, _UTF8_, i_Extension, sl_Data, sl_DSParameter, sl_MFParameter, sl_DataSetComment,
                                   sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset,
                                   sl_SourceReference, sl_SourceDataset, s_Author, s_Source, s_DatasetTitle, s_ExportFilename,
                                   s_Reference, s_Project, s_DataSetComment, s_FurtherDetailsReference, s_FurtherDetailsDataset, s_OtherVersionReference, s_OtherVersionDataset,
                                   s_SourceReference, s_SourceDataset, s_PI, s_User, s_Parent, i_Status,
                                   i_Login, b_writeHeader, b_splitFile, b_useFilenameInAsEventLabel, b_makeFilenameUnique, b_hasManyEvents, i_MetadataFileMode,
                                   i_TopologicType, b_overwriteDataset, b_markSmallFile, i_NumOfSavedDataLines+j, i_OutOfRangeValue, i_NumOfFiles );

        resetProgress( i_NumOfFiles );
    }

// *************************************************************************************

    if ( b_splitFile == false )
    {
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Processing file..." ), 2*n );

        resetDataList( sl_Input, i_HeaderLine, i, sl_Data );

        while ( ( ++i < n ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
        {
            sl_Data.append( buildDataLine( sl_Input, i ) );

            i_stopProgress = incProgress( i_NumOfFiles, i );
        }

        err = writeDataImportFile( fi.baseName(), s_OutputPath, _UTF8_, i_Extension, sl_Data, sl_DSParameter, sl_MFParameter, sl_DataSetComment,
                                   sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset,
                                   sl_SourceReference, sl_SourceDataset, s_Author, s_Source, s_DatasetTitle, s_ExportFilename,
                                   s_Reference, s_Project, s_DataSetComment, s_FurtherDetailsReference, s_FurtherDetailsDataset, s_OtherVersionReference, s_OtherVersionDataset,
                                   s_SourceReference, s_SourceDataset, s_PI, s_User, s_Parent, i_Status,
                                   i_Login, b_writeHeader, b_splitFile, b_useFilenameInAsEventLabel, b_makeFilenameUnique, b_hasManyEvents, i_MetadataFileMode,
                                   i_TopologicType, b_overwriteDataset, b_markSmallFile, i_NumOfSavedDataLines+j, i_OutOfRangeValue, i_NumOfFiles );

        resetProgress( i_NumOfFiles );
    }

// *************************************************************************************

    if ( i_emptyDataItem > 0 )
    {
        setNormalCursor();
        QMessageBox::information( this, getApplicationName( true ), QString( tr( "Empty data item found in\nfirst data column of line %1." ).arg( i_emptyDataItem-1 ) ) );
        setWaitCursor();
    }

    if ( i_stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}


// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::writeDataImportFile( const QString& s_baseNameFilenameIn, const QString& s_OutputPath, const int i_Codec, const int i_Extension,
                                     const QStringList& sl_Data, const QStringList& sl_DSParameter, const QStringList& sl_MFParameter,
                                     const QStringList& sl_DataSetComment,
                                     const QStringList& sl_FurtherDetailsReference, const QStringList& sl_FurtherDetailsDataset,
                                     const QStringList& sl_OtherVersionReference, const QStringList& sl_OtherVersionDataset,
                                     const QStringList& sl_SourceReference, const QStringList& sl_SourceDataset,
                                     const QString& s_Author, const QString& s_Source, const QString& s_DatasetTitle,
                                     const QString& s_ExportFilename, const QString& s_Reference,
                                     const QString& s_Project, const QString& s_DataSetComment,
                                     const QString& s_FurtherDetailsReference, const QString& s_FurtherDetailsDataset,
                                     const QString& s_OtherVersionReference, const QString& s_OtherVersionDataset,
                                     const QString& s_SourceReference, const QString& s_SourceDataset,
                                     const QString& s_PI, const QString& s_User, const QString& s_Parent, const int i_Status, const int i_Login,
                                     const bool b_writeHeader, const bool b_splitFile, const bool b_useFilenameInAsEventLabel, const bool b_makeFilenameUnique,
                                     const bool b_hasManyEventsIn, const int i_MetadataFileMode, const int i_TopologicType, const bool b_overwriteDataset,
                                     const bool b_markSmallFile, const int i_NumOfSavedDataLines, const int i_OutOfRangeValue, const int i_NumOfFiles )
{
    QString s_OutputFile         = "";
    QString s_baseNameEventLabel = "";
    QString s_FileExtension      = "";

    QString s_EventHeader        = "";
    QString s_EventLabel         = sl_Data.at( 1 ).section( "\t", 0, 0 ).section( "@", 0, 0 );
    QString s_MinorLabel         = sl_Data.at( 1 ).section( "\t", 0, 0 ).section( "@", 1, 1 );

    int     i_NumOfColumns       = NumOfSections( buildHeaderOutputString( sl_Data.at( 0 ) ) );

    bool    b_JSON_Test          = false;
    bool    b_hasEmptyColumn     = false;
    bool    b_hasManyEvents      = false;

// *************************************************************************************
// Switch b_JSON_Test to true for testing Split2Events

    if ( QCoreApplication::applicationFilePath().contains( "Split2Events_JSON" ) == true )
        b_JSON_Test = true;

// *************************************************************************************

//  b_JSON_Test = true;

// *************************************************************************************

    bool	b_EmptyColumn[_MAX_NUM_OF_COLUMNS_+1];

    double	d_Factor[_MAX_NUM_OF_COLUMNS_+1];
    double	d_RangeMin[_MAX_NUM_OF_COLUMNS_+1];
    double	d_RangeMax[_MAX_NUM_OF_COLUMNS_+1];

    int		i_Digits[_MAX_NUM_OF_COLUMNS_+1];

    QString	s_defaultValue[_MAX_NUM_OF_COLUMNS_+1];

// *************************************************************************************

    switch ( i_Extension )
    {
    case _CSV_:
        s_FileExtension = tr( ".csv" );
        break;
    default:
        s_FileExtension = tr( ".txt" );
        break;
    }

    s_FileExtension = tr( ".txt" ); // always txt

    if ( ( b_useFilenameInAsEventLabel == true ) && ( b_splitFile == false ) )
        s_baseNameEventLabel = s_baseNameFilenameIn;
    else
        s_baseNameEventLabel = createValidFilename( sl_Data.at( 1 ).section( "\t", 0, 0 ) );

    if ( b_makeFilenameUnique == true )
        s_FileExtension = QString( "_%1.txt" ).arg( QTime::currentTime().toString( "hhmmsszzz") );

    s_OutputFile = s_OutputPath + tr( "/" ) + s_baseNameEventLabel + s_FileExtension;

    if ( ( b_markSmallFile == true ) && ( sl_Data.count() == 2 ) )  // rename if surface data
        s_OutputFile = s_OutputPath + tr( "/z1_" ) + s_baseNameEventLabel + s_FileExtension;

    if ( ( b_markSmallFile == true ) && ( sl_Data.count() == 3 ) )  // rename if 2-line station
        s_OutputFile = s_OutputPath + tr( "/z2_" ) + s_baseNameEventLabel + s_FileExtension;

    if ( ( b_markSmallFile == true ) && ( sl_Data.count() == 4 ) )  // rename if 3-line station
        s_OutputFile = s_OutputPath + tr( "/z3_" ) + s_baseNameEventLabel + s_FileExtension;

// *************************************************************************************

    s_EventLabel.replace( "~", "/" );	// Event labels contains "/"

// *************************************************************************************

    b_hasEmptyColumn = findEmptyColumns( sl_Data, i_NumOfColumns, b_EmptyColumn );

// *************************************************************************************

    memset( d_Factor, 0, ( _MAX_NUM_OF_COLUMNS_+1 ) * sizeof( double ) );
    memset( d_RangeMin, 0, ( _MAX_NUM_OF_COLUMNS_+1 ) * sizeof( double ) );
    memset( d_RangeMax, 0, ( _MAX_NUM_OF_COLUMNS_+1 ) * sizeof( double ) );
    memset( i_Digits, 0, ( _MAX_NUM_OF_COLUMNS_+1 ) * sizeof( int ) );

// *************************************************************************************

    if ( sl_Data.count() < 2 )
        return( _NOERROR_ );

// *************************************************************************************

    if ( i_NumOfColumns > _MAX_NUM_OF_COLUMNS_ )
        return( -132 );

// *************************************************************************************

    QFile fout( s_OutputFile );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text ) == false )
        return( -122 );

// *************************************************************************************
// write data with data description

    if ( b_writeHeader == true )
    {
        if ( ( gs_Version.section( "\t", 2, 2 ) != "JSON" ) && ( b_JSON_Test == false ) )
        {
            b_hasManyEvents = true;  // hasManyEvents not supported in old version of metaheader => always true
            s_EventHeader   = "Event label";

            writeDataDescription( &fout, i_Codec, b_EmptyColumn, s_baseNameFilenameIn, s_EventLabel, s_MinorLabel, sl_DSParameter, sl_MFParameter, sl_DataSetComment,
                              sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset,
                              sl_SourceReference, sl_SourceDataset, s_Author, s_Source,
                              s_DatasetTitle, s_ExportFilename, s_Reference, s_Project, s_DataSetComment, s_FurtherDetailsReference, s_FurtherDetailsDataset,
                              s_OtherVersionReference, s_OtherVersionDataset, s_SourceReference, s_SourceDataset, s_PI, s_User, s_Parent, i_Status, i_Login,
                              b_useFilenameInAsEventLabel, i_MetadataFileMode, i_TopologicType, b_overwriteDataset );

            writeDataHeader( &fout, i_Codec, sl_Data, sl_MFParameter, i_MetadataFileMode, b_EmptyColumn, d_Factor, d_RangeMin, d_RangeMax, i_Digits, s_defaultValue, s_EventHeader );
        }
        else
        {
            b_hasManyEvents = b_hasManyEventsIn;

            if ( b_hasManyEventsIn == true )
                s_EventHeader = "500000";

            writeDataDescriptionJSON( &fout, i_Codec, b_EmptyColumn, s_baseNameFilenameIn, s_EventLabel, s_MinorLabel, sl_DSParameter, sl_MFParameter, sl_DataSetComment,
                              sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset,
                              sl_SourceReference, sl_SourceDataset, s_Author, s_Source,
                              s_DatasetTitle, s_ExportFilename, s_Reference, s_Project, s_DataSetComment, s_FurtherDetailsReference, s_FurtherDetailsDataset,
                              s_OtherVersionReference, s_OtherVersionDataset, s_SourceReference, s_SourceDataset, s_PI, s_User, s_Parent, i_Status, i_Login,
                              b_useFilenameInAsEventLabel, b_hasManyEvents, i_MetadataFileMode, i_TopologicType, b_overwriteDataset );

            writeDataHeader( &fout, i_Codec, sl_Data, sl_MFParameter, i_MetadataFileMode, b_EmptyColumn, d_Factor, d_RangeMin, d_RangeMax, i_Digits, s_defaultValue, s_EventHeader );
        }

        switch ( i_MetadataFileMode )
        {
          case _NOTUSED_:
            writeData( &fout, i_Codec, sl_Data, b_hasManyEvents, b_EmptyColumn, b_hasEmptyColumn, i_NumOfSavedDataLines, 1, i_NumOfFiles );
            break;

          default:
            writeData( &fout, i_Codec, sl_Data, b_hasManyEvents, b_EmptyColumn, d_Factor, d_RangeMin, d_RangeMax, i_Digits, s_defaultValue, i_NumOfSavedDataLines, i_OutOfRangeValue, i_NumOfFiles );
            break;
        }
    }
    else
    {
        writeData( &fout, i_Codec, sl_Data, true, b_EmptyColumn, b_hasEmptyColumn, i_NumOfSavedDataLines, 0, i_NumOfFiles );
    }

// *************************************************************************************

    fout.close();

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::writeData( QIODevice *outDevice, const int i_Codec, const QStringList& sl_Data,
                           const bool b_hasManyEvents, const bool b_EmptyColumn[], const bool b_hasEmptyColumn,
                           const int i_NumOfSavedDataLines, const int i_firstLine, const int i_NumOfFiles )
{
    QString s_Output        = "";

// *************************************************************************************

    QTextStream tout( outDevice );

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

// *************************************************************************************

    for ( int i=i_firstLine; i<sl_Data.count(); i++ )
    {
        if ( b_hasEmptyColumn == true )
            s_Output = buildOutputString( sl_Data.at( i ), b_EmptyColumn );
        else
            s_Output = buildOutputString( sl_Data.at( i ) );

        if ( s_Output.isEmpty() == false )
        {
            if ( b_hasManyEvents == true )
                tout << s_Output << eol;
            else
                tout << s_Output.section( "\t", 1, -1 ) << eol;
        }

        if ( incProgress( i_NumOfFiles, i+i_NumOfSavedDataLines ) == _APPBREAK_ )
            return( _APPBREAK_ );
    }

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::writeData( QIODevice *outDevice, const int i_Codec, const QStringList& sl_Data,
                           const bool b_hasManyEvents, const bool b_EmptyColumn[],
                           double d_Factor[], double  d_RangeMin[], double d_RangeMax[],
                           int i_Digits[], QString s_defaultValue[],
                           const int i_NumOfSavedDataLines, const int i_OutOfRangeValue,
                           const int i_NumOfFiles )
{
//  int i_OutOfRangeValue = DELETE;

    int     i_ColumnLongitude   = -999;

    QString	s_QF                = "";

    QString s_Data              = "";
    QString s_Parameter         = "";

    QStringList sl_Output;

// *************************************************************************************

    QTextStream tout( outDevice );

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

// *************************************************************************************

    int i_NumOfParameters = NumOfSections( buildHeaderOutputString( sl_Data.at( 0 ) ) );

    for ( int i=1; i<sl_Data.count(); i++ )
    {
        if ( ( sl_Data.at( 0 ).section( "\t", i, i ).toLower() == "longitude" )  || ( sl_Data.at( 0 ).section( "\t", i, i ) == "longitude [dec.deg.E]" ) || ( sl_Data.at( 0 ).section( "\t", i, i ) == "1601" ) )
        {
            i_ColumnLongitude = i;
            i                 = sl_Data.count();
        }
    }

    for ( int i=1; i<sl_Data.count(); i++ )
    {
        s_Data = buildOutputString( sl_Data.at( i ) );

        if ( s_Data.isEmpty() == false )
        {
            sl_Output.clear();

            if ( b_hasManyEvents == true )
                sl_Output.append( s_Data.section( "\t", 0, 0 ) );

            for ( int j=1; j<i_NumOfParameters; j++ )
            {
                if ( b_EmptyColumn[j] == false )
                {
                    s_Parameter = s_Data.section( "\t", j, j );

                    if ( s_Parameter.isEmpty() == true )
                        s_Parameter = s_defaultValue[j]; // write default value if empty

                    if ( j == i_ColumnLongitude )
                    {
                        double d_Longitude = s_Parameter.toDouble();

                        if ( d_Longitude > 180. )
                            s_Parameter = QString( "%1" ).arg( d_Longitude-360., 0, 'f', 6 );
                    }

                    if ( s_Parameter.contains( QRegExp( "[a-zA-Z ]" ) ) == false )
                    {
                        if ( s_Parameter.indexOf( QRegExp( "[<>?/*#]" ) ) == 0 )
                        {
                            s_QF        = s_Parameter.left( 1 );
                            s_Parameter = s_Parameter.mid( 1 );

                            if ( d_Factor[j] != 1. )
                            {
                                if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                    sl_Output.append( s_QF + QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                else
                                {
                                    switch ( i_OutOfRangeValue )
                                    {
                                    case _IGNORE_:
                                        sl_Output.append( s_QF + QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    case _REMOVE_:
                                        sl_Output.append( "" );
                                        break;
                                    case _BAD_:
                                        sl_Output.append( QString( "/%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    case _QUESTIONABLE_:
                                        sl_Output.append( QString( "?%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    default:
                                        sl_Output.append( s_QF + QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                {
                                    sl_Output.append( s_QF + s_Parameter );
                                }
                                else
                                {
                                    switch ( i_OutOfRangeValue )
                                    {
                                    case _IGNORE_:
                                        sl_Output.append( s_QF + s_Parameter );
                                        break;
                                    case _REMOVE_:
                                        sl_Output.append( "" );
                                        break;
                                    case _BAD_:
                                        sl_Output.append( "/" + s_Parameter );
                                        break;
                                    case _QUESTIONABLE_:
                                        sl_Output.append( "?" + s_Parameter );
                                        break;
                                    default:
                                        sl_Output.append( s_QF + s_Parameter );
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if ( d_Factor[j] != 1. )
                            {
                                if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                {
                                    sl_Output.append( QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                }
                                else
                                {
                                    switch ( i_OutOfRangeValue )
                                    {
                                    case _IGNORE_:
                                        sl_Output.append( QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    case _REMOVE_:
                                        sl_Output.append( "" );
                                        break;
                                    case _BAD_:
                                        sl_Output.append( QString( "/%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    case _QUESTIONABLE_:
                                        sl_Output.append( QString( "?%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    default:
                                        sl_Output.append( QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] ) );
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                    sl_Output.append( s_Parameter );
                                else
                                {
                                    switch ( i_OutOfRangeValue )
                                    {
                                    case _IGNORE_:
                                        sl_Output.append( s_Parameter );
                                        break;
                                    case _REMOVE_:
                                        sl_Output.append( "" );
                                        break;
                                    case _BAD_:
                                       sl_Output.append( "/" + s_Parameter );
                                        break;
                                    case _QUESTIONABLE_:
                                        sl_Output.append( "?" + s_Parameter );
                                        break;
                                    default:
                                        sl_Output.append( s_Parameter );
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( s_Parameter != "@is empty@" )
                            sl_Output.append( s_Parameter ); // contains characters
                        else
                            sl_Output.append( "" );
                    }
                }
            }

            tout << sl_Output.join( "\t" ) << eol;
        }

        if ( incProgress( i_NumOfFiles, i+i_NumOfSavedDataLines ) == _APPBREAK_ )
            return( _APPBREAK_ );
    }

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::writeDataDescription( QIODevice *outDevice, const int i_Codec, const bool b_EmptyColumn[],
                                      const QString& s_baseNameFilenameIn, const QString& s_EventLabel, const QString& s_MinorLabel,
                                      const QStringList& sl_DSParameter, const QStringList& sl_MFParameter,
                                      const QStringList& sl_DataSetComment,
                                      const QStringList& sl_FurtherDetailsReference, const QStringList &sl_FurtherDetailsDataset,
                                      const QStringList& sl_OtherVersionReference, const QStringList& sl_OtherVersionDataset,
                                      const QStringList& sl_SourceReference, const QStringList& sl_SourceDataset,
                                      const QString& s_Author, const QString& s_Source, const QString& s_DatasetTitle,
                                      const QString& s_ExportFilename, const QString& s_Reference,
                                      const QString& s_Project, const QString& s_DataSetComment,
                                      const QString& s_FurtherDetailsReference, const QString& s_FurtherDetailsDataset,
                                      const QString& s_OtherVersionReference, const QString& s_OtherVersionDataset,
                                      const QString& s_SourceReference, const QString& s_SourceDataset,
                                      const QString& s_PI, const QString& s_User, const QString& s_Parent,
                                      const int i_Status, const int i_Login, const bool b_useFilenameInAsEventLabel,
                                      const int i_MetadataFileMode, const int i_TopologicType, const bool b_overwriteDataset )
{
   QString s_Parameter                   = "";
   QString s_ParameterDS                 = "";
   QString s_ParameterMF                 = "";

   QString s_Title                       = "";

   QString s_tempAuthor                  = s_Author;
   QString s_tempSource                  = s_Source;
   QString s_tempReference               = s_Reference;
   QString s_tempPI                      = s_PI;
   QString s_tempUser                    = s_User;
   QString s_tempParent                  = s_Parent;
   QString s_tempExportFilename          = s_ExportFilename;
   QString s_tempDatasetTitle            = s_DatasetTitle;
   QString s_tempDatasetComment          = s_DataSetComment;
   QString s_tempProject                 = s_Project;
   QString s_tempFurtherDetailsReference = s_FurtherDetailsReference;
   QString s_tempFurtherDetailsDataset   = s_FurtherDetailsDataset;
   QString s_tempOtherVersionReference   = s_OtherVersionReference;
   QString s_tempOtherVersionDataset     = s_OtherVersionDataset;
   QString s_tempSourceReference         = s_SourceReference;
   QString s_tempSourceDataset           = s_SourceDataset;
   QString s_tempEventLabel              = s_EventLabel;

   QString sd_DataSetComment             = "";
   QString sd_FurtherDetails             = "";
   QString sd_OtherVersion               = "";
   QString sd_Source                     = "";

   bool	b_foundInDataSetCommentList      = false;
   bool	b_foundInFurtherDetailsList      = false;
   bool	b_foundInOtherVersionList        = false;
   bool	b_foundInSourceList              = false;

// *************************************************************************************

   QTextStream tout( outDevice );

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

// *************************************************************************************

   if ( b_useFilenameInAsEventLabel == true )
       s_tempEventLabel = s_baseNameFilenameIn;
   else
       s_tempEventLabel = s_EventLabel;

   if ( s_MinorLabel.isEmpty() == false )
       s_tempEventLabel.append( "-" + s_MinorLabel );

// *************************************************************************************
// start of metaheader

   tout << "/* DATA DESCRIPTION:" << eol;

// *************************************************************************************
// PI

    if ( s_tempPI == "999999" )
        s_tempPI = "@GP@" + s_tempEventLabel + "@";

// *************************************************************************************
// Parent

    if ( s_tempParent.isEmpty() == false )
    {
        if ( s_tempParent == "999999" )
            s_tempParent = "@Par@" + s_tempEventLabel + "@";

        s_tempParent.replace( " ", "" );
        s_tempParent.replace( ";", "," );

        tout << tr( "Parent:" ) << "\t" << s_tempParent.section( ",", 0, 0 ) << eol;
    }

// *************************************************************************************
// overwrite Dataset

    if ( b_overwriteDataset == true )
        tout << tr( "DataSet ID:" ) << "\t" << "@I@" << s_tempEventLabel << "@" << eol;

// *************************************************************************************
// Author

    if ( s_tempAuthor.isEmpty() == false )
    {
        if ( s_tempAuthor == "999999" )
            s_tempAuthor = "@A@" + s_tempEventLabel + "@";

        s_tempAuthor.replace( " ", "" );
        s_tempAuthor.replace( ";", "," );

        int i_NumOfAuthors = s_tempAuthor.count( "," ) + 1;

        tout << tr( "Author:" ) << "\t" << s_tempAuthor.section( ",", 0, 0 ) << eol;

        for ( int j=1; j<i_NumOfAuthors; j++ )
            tout << "\t" << s_tempAuthor.section( ",", j, j ) << eol;
    }

// *************************************************************************************
// Source

    if ( s_tempSource.isEmpty() == false )
    {
        if ( s_tempSource == "999999" )
            s_tempSource = "@S@" + s_tempEventLabel + "@";

        s_tempSource.replace( " ", "" );
        s_tempSource.replace( ";", "," );

        tout << tr( "Source:" ) << "\t" << s_tempSource.section( ",", 0, 0 ) << eol;
    }

// *************************************************************************************
// Dataset title

    if ( s_tempDatasetTitle.isEmpty() == false )
    {
        s_tempDatasetTitle.replace( "999999", "@D@" + s_tempEventLabel + "@" );

        if ( b_useFilenameInAsEventLabel == true )
            s_tempDatasetTitle.replace( "$E", s_baseNameFilenameIn );
        else
            s_tempDatasetTitle.replace( "$E", s_EventLabel );

        s_tempDatasetTitle.replace( "$@", s_MinorLabel );
        s_tempDatasetTitle.replace( "\n", "*" );
        s_tempDatasetTitle.replace( "-track", "" );
        s_tempDatasetTitle.replace( "Track.", "" );

        if ( s_tempDatasetTitle.length() <= 255 )
            tout << tr( "Title:" ) << "\t" << s_tempDatasetTitle << eol;
        else
        {
            int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Title comprises more that 255 characters.\nAbort?" ), QMessageBox::Yes, QMessageBox::No );

            if ( err == QMessageBox::No )
                tout << tr( "Title:" ) << "\t" << s_tempDatasetTitle.left( 252 ) << "..." << eol;
            else
                return( _ERROR_ );
        }
    }

// *************************************************************************************
// Reference

    if ( s_tempReference.isEmpty() == false )
    {
        s_Title = "Reference:";

        s_tempReference.replace( " ", "" );
        s_tempReference.replace( ";", "," );

        int i_NumOfReferences = s_tempReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempReference == "999999" )
                tout << s_Title << "\t" << "@R@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _RELATEDTO_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempReference.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _RELATEDTO_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Further details reference IDs

    if ( sl_FurtherDetailsReference.count() > 0 )
    {
        s_Title = "Reference:";

        for ( int i=0; i<sl_FurtherDetailsReference.count(); i++ )
        {
            sd_FurtherDetails = sl_FurtherDetailsReference.at( i );

            if ( sd_FurtherDetails.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_FurtherDetails.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;

                s_Title                     = "";
                b_foundInFurtherDetailsList = true;
            }
        }
    }

    if ( ( s_tempFurtherDetailsReference.isEmpty() == false ) && ( b_foundInFurtherDetailsList == false ) )
    {
        s_Title = "Reference:";

        s_tempFurtherDetailsReference.replace( " ", "" );
        s_tempFurtherDetailsReference.replace( ";", "," );

        int i_NumOfReferences = s_tempFurtherDetailsReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempFurtherDetailsReference == "999999" )
                tout << s_Title << "\t" << "@FR@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempFurtherDetailsReference.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Other version reference IDs

    if ( sl_OtherVersionReference.count() > 0 )
    {
        s_Title = "Reference:";

        for ( int i=0; i<sl_OtherVersionReference.count(); i++ )
        {
            sd_OtherVersion = sl_OtherVersionReference.at( i );

            if ( sd_OtherVersion.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_OtherVersion.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;

                s_Title                   = "";
                b_foundInOtherVersionList = true;
            }
        }
    }

    if ( ( s_tempOtherVersionReference.isEmpty() == false ) && ( b_foundInOtherVersionList == false ) )
    {
        s_Title = "Reference:";

        s_tempOtherVersionReference.replace( " ", "" );
        s_tempOtherVersionReference.replace( ";", "," );

        int i_NumOfReferences = s_tempOtherVersionReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempOtherVersionReference == "999999" )
                tout << s_Title << "\t" << "@OR@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempOtherVersionReference.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Source dataset reference IDs

    if ( sl_SourceReference.count() > 0 )
    {
        s_Title = "Reference:";

        for ( int i=0; i<sl_SourceReference.count(); i++ )
        {
            sd_Source = sl_SourceReference.at( i );

            if ( sd_Source.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_Source.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;

                s_Title             = "";
                b_foundInSourceList = true;
            }
        }
    }

    if ( ( s_tempSourceReference.isEmpty() == false ) && ( b_foundInSourceList == false ) )
    {
        s_Title = "Reference:";

        s_tempSourceReference.replace( " ", "" );
        s_tempSourceReference.replace( ";", "," );

        int i_NumOfReferences = s_tempSourceReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempSourceReference == "999999" )
                tout << s_Title << "\t" << "@SR@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempSourceReference.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Further details dataset IDs

    if ( sl_FurtherDetailsDataset.count() > 0 )
    {
        s_Title = "PANGAEA data reference:";

        for ( int i=0; i<sl_FurtherDetailsDataset.count(); i++ )
        {
            sd_FurtherDetails = sl_FurtherDetailsDataset.at( i );

            if ( sd_FurtherDetails.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_FurtherDetails.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;

                s_Title                     = "";
                b_foundInFurtherDetailsList = true;
            }
        }
    }

    if ( ( s_tempFurtherDetailsDataset.isEmpty() == false ) && ( b_foundInFurtherDetailsList == false ) )
    {
        s_Title = "PANGAEA data reference:";

        s_tempFurtherDetailsDataset.replace( " ", "" );
        s_tempFurtherDetailsDataset.replace( ";", "," );

        int i_NumOfReferences = s_tempFurtherDetailsDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempFurtherDetailsDataset == "999999" )
                tout << s_Title << "\t" << "@FD@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempFurtherDetailsDataset.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _FURTHERDETAILS_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Other version dataset IDs

    if ( sl_OtherVersionDataset.count() > 0 )
    {
        s_Title = "PANGAEA data reference:";

        for ( int i=0; i<sl_OtherVersionDataset.count(); i++ )
        {
            sd_OtherVersion = sl_OtherVersionDataset.at( i );

            if ( sd_OtherVersion.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_OtherVersion.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;

                s_Title                   = "";
                b_foundInOtherVersionList = true;
            }
        }
    }

    if ( ( s_tempOtherVersionDataset.isEmpty() == false ) && ( b_foundInOtherVersionList == false ) )
    {
        s_Title = "PANGAEA data reference:";

        s_tempOtherVersionDataset.replace( " ", "" );
        s_tempOtherVersionDataset.replace( ";", "," );

        int i_NumOfReferences = s_tempOtherVersionDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempOtherVersionDataset == "999999" )
                tout << s_Title << "\t" << "@OD@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempOtherVersionDataset.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _OTHERVERSION_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Source dataset dataset IDs

    if ( sl_SourceDataset.count() > 0 )
    {
        s_Title = "PANGAEA data reference:";

        for ( int i=0; i<sl_SourceDataset.count(); i++ )
        {
            sd_Source = sl_SourceDataset.at( i );

            if ( sd_Source.section( "\t", 0, 0 ) == s_EventLabel )
            {
                tout << s_Title << "\t" << sd_Source.section( "\t", 1, 1 ) << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;

                s_Title             = "";
                b_foundInSourceList = true;
            }
        }
    }

    if ( ( s_tempSourceDataset.isEmpty() == false ) && ( b_foundInSourceList == false ) )
    {
        s_Title = "PANGAEA data reference:";

        s_tempSourceDataset.replace( " ", "" );
        s_tempSourceDataset.replace( ";", "," );

        int i_NumOfReferences = s_tempSourceDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
        {
            if ( s_tempSourceDataset == "999999" )
                tout << s_Title << "\t" << "@SD@" << s_tempEventLabel << "@" << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;
            else
                tout << s_Title << "\t" << s_tempSourceDataset.section( ",", j, j ) << QString( " * RELATIONTYPE: %1" ).arg( _SOURCEDATASET_ ) << eol;

            s_Title = "";
        }
    }

// *************************************************************************************
// Export filename

    if ( s_tempExportFilename.isEmpty() == false )
    {
        s_tempExportFilename.replace( "999999", "@E@" + s_tempEventLabel + "@" );

        if ( b_useFilenameInAsEventLabel == true )
            s_tempExportFilename.replace( "$E", s_baseNameFilenameIn );
        else
            s_tempExportFilename.replace( "$E", s_EventLabel );

        s_tempExportFilename.replace( "$@", s_MinorLabel );
        s_tempExportFilename.replace( "-track", "" );
        s_tempExportFilename.replace( "Track.", "" );

        s_tempExportFilename = createValidFilename( s_tempExportFilename );

        if ( s_tempExportFilename.length() <= 80)
        {
            tout << tr( "Export Filename:" ) << "\t" << s_tempExportFilename << eol;
        }
        else
        {
            int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Export filename comprises more that 80 characters.\nAbort?" ), QMessageBox::Yes, QMessageBox::No );

            if ( err == QMessageBox::No )
                tout << tr( "Export Filename:" ) << "\t" << s_tempExportFilename.left( 77 ) << "..." << eol;
            else
                return( _ERROR_ );
        }
    }

// *************************************************************************************
// Event label

    if ( s_EventLabel.isEmpty() == false )
        tout << tr( "Event:" ) << "\t" << s_EventLabel << eol;

// *************************************************************************************
// PI

    if ( s_tempPI.isEmpty() == false )
        tout << tr( "PI:" ) << "\t" << s_tempPI << eol;
    else
        tout << tr( "PI:\t506" ) << eol;

// *************************************************************************************
// Parameters

    if ( i_MetadataFileMode != _NOTUSED_ ) // begin parameter from metadata file
    {
        int j    = 0;
        int i_MF = 0;

        tout << tr( "Parameter:" );

        switch ( i_MetadataFileMode )
        {
        case _BYNAME_:
            for ( int i_DS=1; i_DS<sl_DSParameter.count(); i_DS++ ) // i_DS = 0 => Event label
            {
                j++;

                s_ParameterDS	= sl_DSParameter.at( i_DS );
                s_Parameter		= "";
                i_MF			= 0;

                while ( ( i_MF<sl_MFParameter.count() ) && ( s_Parameter.isEmpty() == true ) )
                {
                    s_ParameterMF = sl_MFParameter.at( i_MF++ );

                    if ( s_ParameterDS.contains( QRegExp( "[@a-zA-Z]" ) ) == false )
                    {
                        if ( s_ParameterMF.section( "\t", 1, 1 ) == s_ParameterDS ) // s_ParameterDS is a number
                            s_Parameter = buildParameter( s_ParameterMF, s_tempEventLabel );
                    }
                    else
                    {
                        if ( s_ParameterMF.section( "\t", 0, 0 ) == s_ParameterDS ) // s_ParameterDS includes characters
                            s_Parameter = buildParameter( s_ParameterMF, s_tempEventLabel );
                    }
                }

                if ( s_Parameter.isEmpty() == true )
                {
                    tout << "\t" << s_ParameterDS << tr( " * not define in metadata file" ) << eol;
                }
                else
                {
                    s_Parameter.replace( "@$E@", "@" + s_tempEventLabel + "@" );

                    if ( b_EmptyColumn[j] == false )
                        tout << "\t" << s_Parameter << eol;
                }
            }
            break;

        case _AUTO_:
        case _BYPOSITION_:
            for ( int i_MF=0; i_MF<sl_MFParameter.count(); i_MF++ )
            {
                j++;

                s_ParameterMF	= sl_MFParameter.at( i_MF );
                s_Parameter		= buildParameter( s_ParameterMF, s_tempEventLabel );

                s_Parameter.replace( "@$E@", "@" + s_tempEventLabel + "@" );

                if ( b_EmptyColumn[j] == false )
                    tout << "\t" << s_Parameter << eol;
            }
            break;
        }
    } // end parameter from metadata file

// *************************************************************************************
// Dataset comment

    if ( sl_DataSetComment.count() > 0 )
    {
        QString DummyStr = s_EventLabel;

        if ( s_MinorLabel.isEmpty() == false )
            DummyStr.append( "@" + s_MinorLabel );

        for ( int i=0; i<sl_DataSetComment.count(); i++ )
        {
            sd_DataSetComment = sl_DataSetComment.at( i );
            sd_DataSetComment.replace( "\"", "" );

            if ( sd_DataSetComment.section( "\t", 0, 0 ) == DummyStr )
            {
                tout << tr( "DataSet Comment:" ) << "\t" << sd_DataSetComment.section( "\t", 1, 1 ) << eol;
                i = sl_DataSetComment.count();
                b_foundInDataSetCommentList = true;
            }
        }
    }

    if ( ( s_tempDatasetComment.isEmpty() == false ) && ( b_foundInDataSetCommentList == false ) )
    {
        if ( s_tempDatasetComment == "999999" )
            s_tempDatasetComment = "@C@" + s_tempEventLabel + "@";

        if ( b_useFilenameInAsEventLabel == true )
            s_tempDatasetComment.replace( "$E", s_baseNameFilenameIn );
        else
            s_tempDatasetComment.replace( "$E", s_EventLabel );

        s_tempDatasetComment.replace( "$@", s_MinorLabel );
        s_tempDatasetComment.replace( "\n", " " );

        if ( s_tempDatasetComment.length() <= 1000 )
            tout << tr( "DataSet Comment:" ) << "\t" << s_tempDatasetComment << eol;
        else
        {
            int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Comment comprises more that 1000 characters.\nAbort?" ),QMessageBox::Yes,QMessageBox::No);
            if ( err == QMessageBox::No )
                tout << tr( "DataSet Comment:" ) << "\t" << s_tempDatasetComment.left( 997 ) << "..." << eol;
            else
                return( _ERROR_ );
        }
    }

// *************************************************************************************
// Project

    if ( s_tempProject.isEmpty() == false )
    {
        if ( s_tempProject == "999999" )
            s_tempProject = "@Pro@" + s_tempEventLabel + "@";

        s_tempProject.replace( " ", "" );
        s_tempProject.replace( ";", "," );

        int i_NumOfProjects = s_tempProject.count( "," ) + 1;

        tout << tr( "Project:" ) << "\t" << s_tempProject.section( ",", 0, 0 ) << eol;

        for ( int j=1; j<i_NumOfProjects; j++ )
            tout << "\t" << s_tempProject.section( ",", j, j ) << eol;
    }

// *************************************************************************************
// Topologic type

    if ( i_TopologicType > 0 )
        tout << tr( "Topologic Type:" ) << "\t" << i_TopologicType << eol;

// *************************************************************************************
// Status

    tout << tr( "Status:" ) << "\t" << i_Status + 2 << eol;

// *************************************************************************************
// User

    if ( ( s_tempUser.isEmpty() == false ) && ( ( i_Login == _SIGNUPREQUIRED_ ) || ( i_Login == _ACCESSRIGHTSNEEDED_ ) ) )
    {
        if ( s_tempUser == "999999")
            s_tempUser = "@U@" + s_tempEventLabel + "@";

        s_tempUser.replace( " ", "" );
        s_tempUser.replace( ";", "," );

        int i_NumOfUsers = s_tempUser.count( "," ) + 1;

        tout << tr( "User:" ) << "\t" << s_tempUser.section( ",", 0, 0 ) << eol;

        for ( int j=1; j<i_NumOfUsers; j++ )
            tout << "\t" << s_tempUser.section( ",", j, j ) << eol;
    }

// *************************************************************************************
// Login

/*  2011-01-13 - Michael hat das Login erweitert.

    in dem Metaheader kann man nicht mehr "ON", "OFF" als Werte verwenden,
    sondern "unrestricted", "signup required", oder "access rights needed".
    Alternativ einfach die IDs benutzen: "1", "2", "3". Im Falle von Gerts
    Datensätzen jetzt also "signup required" oder "2" als Wert setzen.
*/

    if ( i_Login == _UNRESTRICTED_ )
        tout << tr( "Login:" ) << "\t1" << eol;

    if ( i_Login == _SIGNUPREQUIRED_ )
        tout << tr( "Login:" ) << "\t2" << eol;

    if ( i_Login == _ACCESSRIGHTSNEEDED_ )
        tout << tr( "Login:" ) << "\t3" << eol;

    if ( i_Login == _EVENT_ )
        tout << tr( "Login:" ) << "\t@L@" + s_tempEventLabel + "@" << eol;

    tout << "*/" << eol;   // end of metaheader

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2015-04-11 - new format of data description block

int MainWindow::writeDataDescriptionJSON( QIODevice *outDevice, const int i_Codec, const bool b_EmptyColumn[],
                                      const QString& s_baseNameFilenameIn, const QString& s_EventLabel, const QString& s_MinorLabel,
                                      const QStringList& sl_DSParameter, const QStringList& sl_MFParameter,
                                      const QStringList& sl_DataSetComment,
                                      const QStringList& sl_FurtherDetailsReference, const QStringList &sl_FurtherDetailsDataset,
                                      const QStringList& sl_OtherVersionReference, const QStringList& sl_OtherVersionDataset,
                                      const QStringList& sl_SourceReference, const QStringList& sl_SourceDataset,
                                      const QString& s_Author, const QString& s_Source, const QString& s_DatasetTitle,
                                      const QString& s_ExportFilename, const QString& s_Reference,
                                      const QString& s_Project, const QString& s_DataSetComment,
                                      const QString& s_FurtherDetailsReference, const QString& s_FurtherDetailsDataset,
                                      const QString& s_OtherVersionReference, const QString& s_OtherVersionDataset,
                                      const QString& s_SourceReference, const QString& s_SourceDataset,
                                      const QString& s_PI, const QString& s_User, const QString& s_Parent, const int i_Status,
                                      const int i_Login, const bool b_useFilenameInAsEventLabel, const bool b_hasManyEvents,
                                      const int i_MetadataFileMode, const int i_TopologicType, const bool b_overwriteDataset )
{
   int     j                             = 0;
   int     i_MF                          = 0;

   QString q                             = "\"";
   QString qs                            = "  \"";
   QString qe                            = "\": ";

   QString s_Parameter                   = "";
   QString s_ParameterDS                 = "";
   QString s_ParameterMF                 = "";

   QString s_tempAuthor                  = s_Author;
   QString s_tempSource                  = s_Source;
   QString s_tempReference               = s_Reference;
   QString s_tempPI                      = s_PI;
   QString s_tempUser                    = s_User;
   QString s_tempParent                  = s_Parent;
   QString s_tempExportFilename          = s_ExportFilename;
   QString s_tempDatasetTitle            = s_DatasetTitle;
   QString s_tempDatasetComment          = s_DataSetComment;
   QString s_tempProject                 = s_Project;
   QString s_tempFurtherDetailsReference = s_FurtherDetailsReference;
   QString s_tempFurtherDetailsDataset   = s_FurtherDetailsDataset;
   QString s_tempOtherVersionReference   = s_OtherVersionReference;
   QString s_tempOtherVersionDataset     = s_OtherVersionDataset;
   QString s_tempSourceReference         = s_SourceReference;
   QString s_tempSourceDataset           = s_SourceDataset;
   QString s_tempEventLabel              = s_EventLabel;

   QString sd_Login                      = "";
   QString sd_DatasetID                  = "";
   QString sd_DataSetComment             = "";
   QString sd_FurtherDetails             = "";
   QString sd_OtherVersion               = "";
   QString sd_Source                     = "";

   QStringList  sl_Reference;
   QStringList  sl_DataReference;
   QStringList  sl_Parameter;

   bool	b_foundInDataSetCommentList      = false;
   bool	b_foundInFurtherDetailsList      = false;
   bool	b_foundInOtherVersionList        = false;
   bool	b_foundInSourceList              = false;

// *************************************************************************************

   QTextStream tout( outDevice );

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

// *************************************************************************************

   if ( b_useFilenameInAsEventLabel == true )
       s_tempEventLabel = s_baseNameFilenameIn;
   else
       s_tempEventLabel = s_EventLabel;

   if ( s_MinorLabel.isEmpty() == false )
       s_tempEventLabel.append( "-" + s_MinorLabel );

   if ( s_tempPI == "999999" )
       s_tempPI = "@PI@" + s_tempEventLabel + "@";

   if ( s_tempPI.isEmpty() == true )
       s_tempPI = "506";

   if ( s_tempParent == "999999" )
       s_tempParent = "@Par@" + s_tempEventLabel + "@";

   if ( b_overwriteDataset == true )
       sd_DatasetID = "@I@" + s_tempEventLabel + "@";

   if ( s_tempAuthor == "999999" )
       s_tempAuthor = "@A@" + s_tempEventLabel + "@";

   if ( s_tempSource == "999999" )
       s_tempSource = "@S@" + s_tempEventLabel + "@";

   s_tempDatasetTitle.replace( "999999", "@D@" + s_tempEventLabel + "@" );

   if ( b_useFilenameInAsEventLabel == true )
       s_tempDatasetTitle.replace( "$E", s_baseNameFilenameIn );
   else
       s_tempDatasetTitle.replace( "$E", s_EventLabel );

   s_tempExportFilename.replace( "999999", "@E@" + s_tempEventLabel + "@" );

   if ( b_useFilenameInAsEventLabel == true )
       s_tempExportFilename.replace( "$E", s_baseNameFilenameIn );
   else
       s_tempExportFilename.replace( "$E", s_EventLabel );

   s_tempDatasetComment.replace( "999999", "@C@" + s_tempEventLabel + "@" );

   if ( b_useFilenameInAsEventLabel == true )
       s_tempDatasetComment.replace( "$E", s_baseNameFilenameIn );
   else
       s_tempDatasetComment.replace( "$E", s_EventLabel );

   if ( s_tempProject == "999999" )
       s_tempProject = "@Pro@" + s_tempEventLabel + "@";

   if ( s_tempUser == "999999")
       s_tempUser = "@U@" + s_tempEventLabel + "@";

   s_tempSource.replace( " ", "" );
   s_tempSource.replace( ";", "," );
   s_tempAuthor.replace( " ", "" );
   s_tempAuthor.replace( ";", "," );
   s_tempParent.replace( " ", "" );
   s_tempParent.replace( ";", "," );
   s_tempDatasetTitle.replace( "$@", s_MinorLabel );
   s_tempDatasetTitle.replace( "\n", "<*?*>" );
   s_tempDatasetTitle.replace( "-track", "" );
   s_tempDatasetTitle.replace( "Track.", "" );
   s_tempDatasetTitle.replace( "\"", "\\\"" );
   s_tempExportFilename.replace( "$@", s_MinorLabel );
   s_tempExportFilename.replace( "-track", "" );
   s_tempExportFilename.replace( "Track.", "" );
   s_tempDatasetComment.replace( "$@", s_MinorLabel );
   s_tempDatasetComment.replace( "\n", "<newline>" );
   s_tempDatasetComment.replace( "\"", "\\\"" );
   s_tempProject.replace( " ", "" );
   s_tempProject.replace( ";", "," );
   s_tempUser.replace( " ", "" );
   s_tempUser.replace( ";", "," );
   s_tempReference.replace( " ", "" );
   s_tempReference.replace( ";", "," );
   s_tempFurtherDetailsReference.replace( " ", "" );
   s_tempFurtherDetailsReference.replace( ";", "," );
   s_tempOtherVersionReference.replace( " ", "" );
   s_tempOtherVersionReference.replace( ";", "," );
   s_tempSourceReference.replace( " ", "" );
   s_tempSourceReference.replace( ";", "," );
   s_tempFurtherDetailsDataset.replace( " ", "" );
   s_tempFurtherDetailsDataset.replace( ";", "," );
   s_tempOtherVersionDataset.replace( " ", "" );
   s_tempOtherVersionDataset.replace( ";", "," );
   s_tempSourceDataset.replace( " ", "" );
   s_tempSourceDataset.replace( ";", "," );

   s_tempExportFilename = createValidFilename( s_tempExportFilename );

   if ( s_tempExportFilename.length() > 80 )
   {
       int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Export filename comprises more that 80 characters.\nAbort?" ), QMessageBox::Yes, QMessageBox::No );

       if ( err == QMessageBox::No )
           s_tempExportFilename = s_tempExportFilename.left( 77 ) + "...";
       else
           return( _ERROR_ );
   }

   if ( s_tempDatasetTitle.length() > 255 )
   {
       int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Title comprises more that 255 characters.\nAbort?" ), QMessageBox::Yes, QMessageBox::No );

       if ( err == QMessageBox::No )
           s_tempDatasetTitle = s_tempDatasetTitle.left( 252 ) + "...";
       else
           return( _ERROR_ );
   }

   if ( s_tempDatasetComment.length() > 1000 )
   {
       int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Comment comprises more that 1000 characters.\nAbort?" ),QMessageBox::Yes,QMessageBox::No);

       if ( err == QMessageBox::No )
           s_tempDatasetComment = s_tempDatasetComment.left( 997 ) + "...";
       else
           return( _ERROR_ );
   }

   switch( i_Login )
   {
   case _UNRESTRICTED_:
       sd_Login = "1";
       break;
   case _SIGNUPREQUIRED_:
       sd_Login = "2";
       break;
   case _ACCESSRIGHTSNEEDED_:
       sd_Login = "3";
       break;
   case _EVENT_:
       sd_Login = "@L@" + s_tempEventLabel + "@";
   }

// *************************************************************************************

//   tout << ParameterFirst( 1599, i_PIID, 43, "yyyy-MM-dd'T'HH:mm", "" );
//   tout << ParameterLast( 49377, i_PIID, i_MethodID, "###0" );


// *************************************************************************************
// start of metaheader

   tout << OpenDataDescriptionHeader();

// *************************************************************************************
// Parent

   tout << ParentID( s_tempParent );

// *************************************************************************************
// overwrite Dataset

   tout << DataSetID( sd_DatasetID );

// *************************************************************************************
// Author(s)

   tout << AuthorIDs( s_tempAuthor );

// *************************************************************************************
// Source

   tout << SourceID( s_tempSource );

// *************************************************************************************
// Dataset title

   tout << DatasetTitle( s_tempDatasetTitle );

// *************************************************************************************
// Reference

    if ( s_tempReference.isEmpty() == false )
    {
        int i_NumOfReferences = s_tempReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_Reference.append( Reference( s_tempReference.section( ",", j, j ), _RELATEDTO_, "R", s_tempEventLabel ) );
    }

// *************************************************************************************
// Further details reference IDs

    if ( sl_FurtherDetailsReference.count() > 0 )
    {
        for ( int i=0; i<sl_FurtherDetailsReference.count(); i++ )
        {
            sd_FurtherDetails = sl_FurtherDetailsReference.at( i );

            if ( sd_FurtherDetails.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_Reference.append( Reference( sd_FurtherDetails.section( ",", 1, 1 ), _FURTHERDETAILS_, "FR", s_tempEventLabel ) );

                b_foundInFurtherDetailsList = true;
            }
        }
    }

    if ( ( s_tempFurtherDetailsReference.isEmpty() == false ) && ( b_foundInFurtherDetailsList == false ) )
    {
        int i_NumOfReferences = s_tempFurtherDetailsReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_Reference.append( Reference( s_tempFurtherDetailsReference.section( ",", j, j ), _FURTHERDETAILS_, "FR", s_tempEventLabel ) );
    }

// *************************************************************************************
// Other version reference IDs

    if ( sl_OtherVersionReference.count() > 0 )
    {
        for ( int i=0; i<sl_OtherVersionReference.count(); i++ )
        {
            sd_OtherVersion = sl_OtherVersionReference.at( i );

            if ( sd_OtherVersion.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_Reference.append( Reference( sd_OtherVersion.section( ",", 1, 1 ), _OTHERVERSION_, "OR", s_tempEventLabel ) );

                b_foundInOtherVersionList = true;
            }
        }
    }

    if ( ( s_tempOtherVersionReference.isEmpty() == false ) && ( b_foundInOtherVersionList == false ) )
    {
        int i_NumOfReferences = s_tempOtherVersionReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_Reference.append( Reference( s_tempOtherVersionReference.section( ",", j, j ), _OTHERVERSION_, "OR", s_tempEventLabel ) );
    }

// *************************************************************************************
// Source dataset reference IDs

    if ( sl_SourceReference.count() > 0 )
    {
        for ( int i=0; i<sl_SourceReference.count(); i++ )
        {
            sd_Source = sl_SourceReference.at( i );

            if ( sd_Source.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_Reference.append( Reference( sd_Source.section( ",", 1, 1 ), _SOURCEDATASET_, "SR", s_tempEventLabel ) );

                b_foundInSourceList = true;
            }
        }
    }

    if ( ( s_tempSourceReference.isEmpty() == false ) && ( b_foundInSourceList == false ) )
    {
        int i_NumOfReferences = s_tempSourceReference.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_Reference.append( Reference( s_tempSourceReference.section( ",", j, j ), _SOURCEDATASET_, "SR", s_tempEventLabel ) );
    }

// *************************************************************************************
// write references block

    if ( sl_Reference.count() > 0 )
    {
        tout << qs << tr( "ReferenceIDs" ) << qe << "[ " << eol;

        for ( int i=0; i<sl_Reference.count()-1; i++ )
            tout << sl_Reference.at( i ) << "," << eol;

        tout << sl_Reference.at( sl_Reference.count()-1 ) << " ]," << eol;
    }

// *************************************************************************************
// Further details dataset IDs

    if ( sl_FurtherDetailsDataset.count() > 0 )
    {
        for ( int i=0; i<sl_FurtherDetailsDataset.count(); i++ )
        {
            sd_FurtherDetails = sl_FurtherDetailsDataset.at( i );

            if ( sd_FurtherDetails.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_DataReference.append( Reference( sd_FurtherDetails.section( ",", 1, 1 ), _FURTHERDETAILS_, "FD", s_tempEventLabel ) );

                b_foundInFurtherDetailsList = true;
            }
        }
    }

    if ( ( s_tempFurtherDetailsDataset.isEmpty() == false ) && ( b_foundInFurtherDetailsList == false ) )
    {
        int i_NumOfReferences = s_tempFurtherDetailsDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_DataReference.append( Reference( s_tempFurtherDetailsDataset.section( ",", j, j ), _FURTHERDETAILS_, "FD", s_tempEventLabel ) );
    }

// *************************************************************************************
// Other version dataset IDs

    if ( sl_OtherVersionDataset.count() > 0 )
    {
        for ( int i=0; i<sl_OtherVersionDataset.count(); i++ )
        {
            sd_OtherVersion = sl_OtherVersionDataset.at( i );

            if ( sd_OtherVersion.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_DataReference.append( Reference( sd_OtherVersion.section( ",", 1, 1 ), _OTHERVERSION_, "OD", s_tempEventLabel ) );

                b_foundInOtherVersionList = true;
            }
        }
    }

    if ( ( s_tempOtherVersionDataset.isEmpty() == false ) && ( b_foundInOtherVersionList == false ) )
    {
        int i_NumOfReferences = s_tempOtherVersionDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_DataReference.append( Reference( s_tempOtherVersionDataset.section( ",", j, j ), _OTHERVERSION_, "OD", s_tempEventLabel ) );
    }

// *************************************************************************************
// Source dataset dataset IDs

    if ( sl_SourceDataset.count() > 0 )
    {
        for ( int i=0; i<sl_SourceDataset.count(); i++ )
        {
            sd_Source = sl_SourceDataset.at( i );

            if ( sd_Source.section( "\t", 0, 0 ) == s_EventLabel )
            {
                sl_DataReference.append( Reference( sd_Source.section( ",", 1, 1 ), _SOURCEDATASET_, "SD", s_tempEventLabel ) );

                b_foundInSourceList = true;
            }
        }
    }

    if ( ( s_tempSourceDataset.isEmpty() == false ) && ( b_foundInSourceList == false ) )
    {
        int i_NumOfReferences = s_tempSourceDataset.count( "," ) + 1;

        for ( int j=0; j<i_NumOfReferences; j++ )
            sl_DataReference.append( Reference( s_tempSourceDataset.section( ",", j, j ), _SOURCEDATASET_, "SD", s_tempEventLabel ) );
    }

// *************************************************************************************
// write data references block

    if ( sl_DataReference.count() > 0 )
    {
        tout << qs << tr( "PANGAEADataReferenceIDs" ) << qe << "[ " << eol;

        for ( int i=0; i<sl_DataReference.count()-1; i++ )
            tout << sl_DataReference.at( i ) << "," << eol;

        tout << sl_DataReference.at( sl_DataReference.count()-1 ) << " ]," << eol;
    }

// *************************************************************************************
// Export filename

    tout << ExportFilename( s_tempExportFilename );

// *************************************************************************************
// Event label

    if ( ( s_EventLabel.isEmpty() == false ) && ( b_hasManyEvents == false ) )
        tout << EventLabel( s_EventLabel );

// *************************************************************************************
// Parameter

    switch ( i_MetadataFileMode )
    {
    case _BYNAME_:
        for ( int i_DS=1; i_DS<sl_DSParameter.count(); i_DS++ ) // i_DS = 0 => Event label
        {
            if ( b_EmptyColumn[++j] == false )
            {
                s_ParameterDS	= sl_DSParameter.at( i_DS );
                s_Parameter		= "";
                i_MF			= 0;

                while ( ( i_MF<sl_MFParameter.count() ) && ( s_Parameter.isEmpty() == true ) )
                {
                    s_ParameterMF = sl_MFParameter.at( i_MF++ );

                    if ( s_ParameterDS.contains( QRegExp( "[@a-zA-Z]" ) ) == false )
                    {
                        if ( s_ParameterMF.section( "\t", 1, 1 ) == s_ParameterDS ) // s_ParameterDS is a number
                            s_Parameter = buildParameterJSON( s_ParameterMF, s_tempEventLabel );
                    }
                    else
                    {
                        if ( s_ParameterMF.section( "\t", 0, 0 ) == s_ParameterDS ) // s_ParameterDS includes characters
                            s_Parameter = buildParameterJSON( s_ParameterMF, s_tempEventLabel );
                    }
                }

                if ( s_Parameter.isEmpty() == true )
                    sl_Parameter.append( buildParameterJSON( s_ParameterDS, "Parameter not found" ) );
                else
                    sl_Parameter.append( s_Parameter );
            }
        }
        break;

    case _AUTO_:
    case _BYPOSITION_:
        for ( int i_MF=0; i_MF<sl_MFParameter.count(); i_MF++ )
        {
            if ( b_EmptyColumn[++j] == false )
                sl_Parameter.append( buildParameterJSON( sl_MFParameter.at( i_MF ), s_tempEventLabel ) );
        }
        break;
    }

// *************************************************************************************
// write parameter block

    if ( sl_Parameter.count() > 0 )
    {
        tout << qs << tr( "ParameterIDs" ) << qe << "[" << eol;

        if ( b_hasManyEvents == true )
            tout << buildParameterJSON( s_tempPI, "has many events" ) + "," << eol;

        for ( int i=0; i<sl_Parameter.count()-1; i++ )
            tout << sl_Parameter.at( i ) << " }," << eol;

        tout << sl_Parameter.at( sl_Parameter.count()-1 ) << " } ]," << eol;
    }

// *************************************************************************************
// Dataset comment

    if ( sl_DataSetComment.count() > 0 )
    {
        QString DummyStr = s_EventLabel;

        if ( s_MinorLabel.isEmpty() == false )
            DummyStr.append( "@" + s_MinorLabel );

        for ( int i=0; i<sl_DataSetComment.count(); i++ )
        {
            sd_DataSetComment = sl_DataSetComment.at( i );

            if ( sd_DataSetComment.section( "\t", 0, 0 ) == DummyStr )
            {
                tout << DatasetComment( sd_DataSetComment.section( "\t", 1, 1 ) );

                i = sl_DataSetComment.count();
                b_foundInDataSetCommentList = true;
            }
        }
    }

    if ( ( s_tempDatasetComment.isEmpty() == false ) && ( b_foundInDataSetCommentList == false ) )
        tout << DatasetComment( s_tempDatasetComment );

// *************************************************************************************
// Project

    tout << ProjectIDs( s_tempProject );

// *************************************************************************************
// Topologic type

    if ( i_TopologicType > 0 )
        tout << TopologicTypeID( num2str( i_TopologicType ) );

// *************************************************************************************
// Status

    tout << StatusID( num2str( i_Status + 2 ) );

// *************************************************************************************
// User

    if ( ( s_tempUser.isEmpty() == false ) && ( ( i_Login == _SIGNUPREQUIRED_ ) || ( i_Login == _ACCESSRIGHTSNEEDED_ ) ) )
        tout << UserIDs( s_tempUser );

// *************************************************************************************
// Login

    tout << LoginID( sd_Login );

// *************************************************************************************
// end of metaheader

    tout << CloseDataDescriptionHeader();

    return( _NOERROR_ );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

int MainWindow::writeDataHeader( QIODevice *outDevice, const int i_Codec, const QStringList& sl_Data, const QStringList& sl_MFParameter,
                                 const int i_MetadataFileMode, const bool b_EmptyColumn[],
                                 double d_Factor[], double  d_RangeMin[], double d_RangeMax[],
                                 int i_Digits[], QString s_defaultValue[], const QString& s_EventHeader )
{
   #if defined(Q_OS_LINUX)
       const char eol = '\n';
   #endif

   #if defined(Q_OS_WIN)
       const char eol = '\n';
   #endif

   #if defined(Q_OS_MAC)
       const char eol = '\r';
   #endif

   int i_NumOfParameters    = 0;

   QString s_Header         = "";
   QString s_Parameter      = "";
   QString s_ParameterMF    = "";

   QStringList sl_Header;

// *************************************************************************************

   QTextStream tout( outDevice );

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

// *************************************************************************************

    s_Header          = buildHeaderOutputString( sl_Data.at( 0 ) );
    i_NumOfParameters = NumOfSections( s_Header );

// *************************************************************************************
// write header

    if ( s_EventHeader.isEmpty() == false )
        sl_Header.append( s_EventHeader );

    switch ( i_MetadataFileMode )
    {
    case _NOTUSED_:
        for ( int j=1; j<i_NumOfParameters; j++ )
        {
            if ( b_EmptyColumn[j] == false )
                sl_Header.append( s_Header.section( "\t", j, j ) );
        }
        break;

    case _BYNAME_:
        for ( int j=1; j<i_NumOfParameters; j++ )
        {
            if ( b_EmptyColumn[j] == false )
            {
                s_Parameter = s_Header.section( "\t", j, j );
                d_Factor[j] = 1;

                for ( int i_MF=0; i_MF<sl_MFParameter.count(); i_MF++ )
                {
                    s_ParameterMF = sl_MFParameter.at( i_MF );

                    if ( ( s_Parameter == s_ParameterMF.section( "\t", 0, 0 ) ) || ( s_Parameter == s_ParameterMF.section( "\t", 1, 1 ) ) )
                    {
                        if ( s_ParameterMF.section( "\t", 1, 1 ).isEmpty() == false )
                            s_Parameter = s_ParameterMF.section( "\t", 1, 1 );
                        else
                            s_Parameter = s_ParameterMF.section( "\t", 0, 0 );

                        i_Digits[j]			= s_ParameterMF.section( "\t", 5, 5 ).section( ".", 1, 1 ).length();
                        d_Factor[j]			= s_ParameterMF.section( "\t", 6, 6 ).toDouble();
                        s_defaultValue[j]	= s_ParameterMF.section( "\t", 7, 7 );
                        d_RangeMin[j]		= s_ParameterMF.section( "\t", 8, 8 ).toDouble();
                        d_RangeMax[j]		= s_ParameterMF.section( "\t", 9, 9 ).toDouble();

                        if ( d_Factor[j] == 0. )
                            d_Factor[j] = 1.;

                        if ( ( d_RangeMin[j] == 0. ) && ( d_RangeMax[j] == 0. ) )
                        {
                            d_RangeMin[j] = -10E30;
                            d_RangeMax[j] = 10E30;
                        }

                        i_MF = sl_MFParameter.count();
                    }
                }

                sl_Header.append( s_Parameter );
            }
        }
        break;

// ***********************************************************************************

    case _AUTO_:
    case _BYPOSITION_:
        for ( int j=1; j<i_NumOfParameters; j++ )
        {
            if ( b_EmptyColumn[j] == false )
            {
                s_Parameter		= s_Header.section( "\t", j, j );
                s_ParameterMF	= sl_MFParameter.at( j-1 );
                d_Factor[j]		= 1.;

                if ( s_ParameterMF.section( "\t", 1, 1 ).isEmpty() == false )
                    s_Parameter = s_ParameterMF.section( "\t", 1, 1 );
                else
                    s_Parameter = s_ParameterMF.section( "\t", 0, 0 );

                i_Digits[j]			= s_ParameterMF.section( "\t", 5, 5 ).section( ".", 1, 1 ).length();
                d_Factor[j]			= s_ParameterMF.section( "\t", 6, 6 ).toDouble();
                s_defaultValue[j]	= s_ParameterMF.section( "\t", 7, 7 );
                d_RangeMin[j]		= s_ParameterMF.section( "\t", 8, 8 ).toDouble();
                d_RangeMax[j]		= s_ParameterMF.section( "\t", 9, 9 ).toDouble();

                if ( d_Factor[j] == 0. )
                    d_Factor[j] = 1.;

                if ( ( d_RangeMin[j] == 0. ) && ( d_RangeMax[j] == 0. ) )
                {
                    d_RangeMin[j] = -10E30;
                    d_RangeMax[j] = 10E30;
                }

                sl_Header.append( s_Parameter );
            }
        }
        break;
    }

    tout << sl_Header.join( "\t" ) << eol;

    return( _NOERROR_ );
}
