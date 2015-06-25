/****************************************************************
**
** Class Split2EventsTools
** 2003-06-01, Dr. Rainer Sieger
** last change: 2009-11-04
**
****************************************************************/

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-09

QString MainWindow::buildParameter( const QString& s_ParameterMF, const QString& s_EventLabel )
{
    QString s_Parameter	= "";

    if ( s_Parameter.length() > 0 )
        s_Parameter = s_ParameterMF.section( "\t", 0, 0 );
    else
        s_Parameter = s_ParameterMF.section( "\t", 1, 1 );

    if ( s_ParameterMF.section( "\t", 2, 2 ).isEmpty() == false )
    {
        s_Parameter += " * PI: ";

        if (  s_ParameterMF.section( "\t", 2, 2 ) == "999999" )
            s_Parameter += "@PP@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@";
        else
            s_Parameter += s_ParameterMF.section( "\t", 2, 2 );
    }

    if ( s_ParameterMF.section( "\t", 3, 3 ).isEmpty() == false )
    {
        s_Parameter += " * METHOD: ";

        if (  s_ParameterMF.section( "\t", 3, 3 ) == "999999" )
            s_Parameter += "@PM@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@";
        else
            s_Parameter += s_ParameterMF.section( "\t", 3, 3 );
    }

    if ( s_ParameterMF.section( "\t", 4, 4 ).isEmpty() == false )
    {
        s_Parameter += " * COMMENT: ";

        if (  s_ParameterMF.section( "\t", 4, 4 ) == "999999" )
            s_Parameter += "@PC@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@";
        else
            s_Parameter += s_ParameterMF.section( "\t", 4, 4 );
    }

    if ( s_ParameterMF.section( "\t", 5, 5 ).isEmpty() == false )
        s_Parameter += " * FORMAT: " + s_ParameterMF.section( "\t", 5, 5 );

    s_Parameter.replace( "@$E@", "@" + s_EventLabel + "@" );

    return( s_Parameter );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2015-04-12

QString MainWindow::buildParameterJSON( const QString& s_ParameterMF, const QString& s_EventLabel )
{
    QString q           = "\"";
    QString qe          = "\": ";

    QString s_Parameter	= "";

// *************************************************************************************

    if ( s_Parameter.length() > 0 )
        s_Parameter	= "    { " + q + "ID" + q + ": " + s_ParameterMF.section( "\t", 0, 0 );
    else
        s_Parameter = "    { " + q + "ID" + q + ": " + s_ParameterMF.section( "\t", 1, 1 );

    if ( s_ParameterMF.section( "\t", 2, 2 ).isEmpty() == false )
    {
        if (  s_ParameterMF.section( "\t", 2, 2 ) == "999999" )
            s_Parameter.append( ", " + q + "PI_ID" + qe + "@PP@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@" );
        else
            s_Parameter.append( ", " + q + "PI_ID" + qe + s_ParameterMF.section( "\t", 2, 2 ) );
    }

    if ( s_ParameterMF.section( "\t", 3, 3 ).isEmpty() == false )
    {
        if (  s_ParameterMF.section( "\t", 3, 3 ) == "999999" )
            s_Parameter.append( ", " + q + "MethodID" + qe + "@PM@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@" );
        else
            s_Parameter.append( ", " + q + "MethodID" + qe + s_ParameterMF.section( "\t", 3, 3 ) );
    }

    if ( s_ParameterMF.section( "\t", 5, 5 ).isEmpty() == false )
        s_Parameter.append( ", " + q + "Format" + qe + q + s_ParameterMF.section( "\t", 5, 5 ) + q );

    if ( s_ParameterMF.section( "\t", 4, 4 ).isEmpty() == false )
    {
        if (  s_ParameterMF.section( "\t", 4, 4 ) == "999999" )
            s_Parameter.append( ", " + q + "Comment" + qe + q + "@PC@" + s_EventLabel + "@" + s_ParameterMF.section( "\t", 1, 1 ) + "@" + q );
        else
            s_Parameter.append( ", " + q + "Comment" + qe + q + s_ParameterMF.section( "\t", 4, 4 ) + q );
    }

    s_Parameter.replace( "@$E@", "@" + s_EventLabel + "@" );

    return( s_Parameter );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-06

bool MainWindow::isNewEvent( const QStringList& sl_Input, const int i )
{
    if ( i <= 1 )
        return( false );

    if ( ( sl_Input.at( i-1 ).section( "\t", 0, 0 ) != sl_Input.at( i ).section( "\t", 0, 0 ) ) && ( sl_Input.at( i ).section( "\t", 0, 0 ).isEmpty() == false ) )
        return( true );

    return( false );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-05

int MainWindow::resetDataList( const QStringList& sl_Input, const int i_HeaderLine, const int i_FirstDataLine, QStringList& sl_Data )
{
    int         i_NumOfColumns      = 0;

    QString     s_HeaderLine        = "";
    QString     s_FirstDataLine     = "";

    QStringList sl_ListParameter;

// *************************************************************************************

    sl_Data.clear();

    i_NumOfColumns   = readDataHeaderLine( sl_Input.at( i_HeaderLine ), sl_ListParameter );

    if ( i_NumOfColumns <= 1 )
         return( -30 );

    s_HeaderLine = sl_ListParameter.at( 0 );

    for ( int j=1; j<i_NumOfColumns; j++ )
        s_HeaderLine.append( "\t" + sl_ListParameter.at( j ) );

    s_FirstDataLine = sl_Input.at( i_FirstDataLine ).section( "\t", 0, 0 ).trimmed();

    for ( int j=1; j<i_NumOfColumns; j++ )
        s_FirstDataLine.append( "\t" + sl_Input.at( i_FirstDataLine ).section( "\t", j, j ).trimmed() );

    sl_Data.append( s_HeaderLine );
    sl_Data.append( s_FirstDataLine );

    return( i_NumOfColumns );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2011-12-07

QString MainWindow::buildDataLine( const QStringList& sl_Input, const int i )
{
    int     i_NumOfColumns = NumOfSections( sl_Input.at( i ) );

    QString s_DataLine     = "";

// *************************************************************************************

    if ( sl_Input.at( i-1 ).section( "\t", 0, 0 ).trimmed() != sl_Input.at( i ).section( "\t", 0, 0 ).trimmed() )
        s_DataLine.append( sl_Input.at( i ).section( "\t", 0, 0 ).trimmed() );

    for ( int j=1; j<i_NumOfColumns; j++ )
        s_DataLine.append( "\t"+ sl_Input.at( i ).section( "\t", j, j ).trimmed() );

    return( s_DataLine );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2011-12-07

QString MainWindow::buildOutputString( const QString& s_Data )
{
    int     i_NumOfColumns  = NumOfSections( s_Data );

    QString s_Output        = "";

    QString test = "";

// *************************************************************************************

    for ( int j=2; j<i_NumOfColumns; j++ )
        s_Output.append( "\t" + s_Data.section( "\t", j, j ) );

    test = s_Output;

// *************************************************************************************

    if ( s_Output.length() > s_Output.count( "\t" ) )
    {
        s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 ) + s_Output;
    }
    else
    {
        if ( ( s_Data.count( "\t" ) <= 2 ) &&
             ( ( s_Data.section( "\t", 1, 1 ) == "15651" ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "Uniform resource locator" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "Universal resource locator" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "URL" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "http://" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "hdl:" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "doi:" ) ) == true ) ) )
            s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 );
        else
            s_Output = ""; // no data found
    }

    s_Output.replace( "\"", "" );

    return( s_Output );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-05

QString MainWindow::buildOutputString( const QString& s_Data, const bool b_EmptyColumn[] )
{
    int     i_NumOfColumns  = NumOfSections( s_Data );

    QString s_Output        = "";

// *************************************************************************************

    for ( int j=2; j<i_NumOfColumns; j++ )
    {
        if ( b_EmptyColumn[j] == false )
            s_Output.append( "\t" + s_Data.section( "\t", j, j ) );
    }

// *************************************************************************************

    if ( s_Output.length() > s_Output.count( "\t" ) )
    {
        s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 ) + s_Output;
    }
    else
    {
        if ( ( s_Data.count( "\t" ) <= 2 ) &&
             ( ( s_Data.section( "\t", 1, 1 ) == "15651" ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "Uniform resource locator" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "Universal resource locator" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "URL" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "http://" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "hdl:" ) ) == true ) ||
               ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "doi:" ) ) == true ) ) )
            s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 );
        else
            s_Output = ""; // no data found
    }

    s_Output.replace( "\"", "" );

    return( s_Output );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-05

QString MainWindow::buildHeaderOutputString( const QString& s_Data )
{
    QString s_Output = s_Data.section( "\t", 2 );

// *************************************************************************************

    s_Output.replace( "] (", "]@(" );
    s_Output.replace( "Salinity (", "Salinity []@(" );

    if ( s_Output.length() > s_Output.count( "\t" ) )
    {
        s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 ) + "\t" + s_Output;
    }
    else
    {
        if ( ( s_Data.count( "\t" ) <= 2 ) && ( ( s_Data.section( "\t", 1, 1 ) == "15651" ) || ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "Universal resource locator" ) ) == true ) || ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "URL" ) ) == true ) || ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "http://" ) ) == true ) || ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "hdl:" ) ) == true ) || ( s_Data.section( "\t", 1, 1 ).startsWith( tr( "doi:" ) ) == true ) ) )
            s_Output = s_Data.section( "\t", 0, 0 ).section( "@", 0, 0 ) + "\t" + s_Data.section( "\t", 1, 1 );
        else
            s_Output = ""; // no data found
    }

    s_Output.replace( "\"", "" );

    return( s_Output );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-05

bool MainWindow::findEmptyColumns( const QStringList& sl_Data, const int i_NumOfColumns, bool b_EmptyColumn[] )
{
    for ( int j=0; j<i_NumOfColumns; j++ )
        b_EmptyColumn[j] = isEmptyColumn( sl_Data, j );

    for ( int j=0; j<i_NumOfColumns; j++ )
    {
        if ( b_EmptyColumn[j] == true )
            return( true );
    }

    return( false );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-04

bool MainWindow::isEmptyColumn( const QStringList& sl_Input, const int i_ColumnNo )
{
    for ( int i=1; i<sl_Input.count(); i++ )
        if ( ( sl_Input.at( i ).section( "\t", i_ColumnNo, i_ColumnNo ).trimmed().isEmpty() == false ) && ( sl_Input.at( i ).section( "\t", i_ColumnNo, i_ColumnNo ) != "@is empty@" ) )
            return( false );

    return( true );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-05

int MainWindow::isEmptyDataItem( const QString& s_Output, const int i, const int i_emptyDataItem )
{
    if ( i_emptyDataItem > 0 )
        return( i_emptyDataItem );

    if ( s_Output.section( "\t", 1, 1 ).isEmpty() == true )
        return( i );

    return( 0 );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2009-11-04

QStringList MainWindow::addColumns( const QStringList& sl_Input, const int i_NumOfColumns )
{
    QString         s_Line = "";

    QStringList     sl_Output;

// *************************************************************************************

    for ( int i=0; i<sl_Input.count(); i++ )
    {
        s_Line = sl_Input.at( i );

        while ( NumOfSections( s_Line ) < i_NumOfColumns )
            s_Line.append( "\t" );

        sl_Output.append( s_Line );
    }

    return( sl_Output );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 27.4.2004

QString MainWindow::createValidFilename( const QString& InputStr )
{
    QString Filename = InputStr;

    Filename.replace( ":" , "~" );
    Filename.replace( "´" , "" );
    Filename.replace( "`" , "" );
    Filename.replace( "'" , "" );
    Filename.replace( "*" , "" );
    Filename.replace( "#" , "" );
    Filename.replace( " ", "_" );
    Filename.replace( ",", "_" );
    Filename.replace( "/" , "_" );
    Filename.replace( "\\" , "_" );
    Filename.replace( "ä" , "ae" );
    Filename.replace( "ü" , "ue" );
    Filename.replace( "ö" , "oe" );
    Filename.replace( "Ä" , "Ae" );
    Filename.replace( "Ü" , "Ue" );
    Filename.replace( "Ö" , "Oe" );
    Filename.replace( "ß" , "ss" );

    return( Filename );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

QString MainWindow::createDir( const QString& s_FilenameIn, const int NumOfFiles, const bool b_emptyDir )
{
    QFileInfo fi( s_FilenameIn );

    QString s_Dir = fi.absolutePath();

// *************************************************************************************

    if ( s_Dir.endsWith( "/" ) == false )
        s_Dir.append( tr( "/" ) );

    if ( NumOfFiles == 1 )
        s_Dir.append( fi.baseName() );
    else
        s_Dir.append( tr( "splitted" ) );

    if ( s_Dir.toLower().endsWith( "_imp" ) == false )
        s_Dir.append( tr( "_imp" ) );

    QFileInfo fd( s_Dir );

    if ( fd.isFile() == true )
        s_Dir.append( tr( "_folder" ) );

    if ( fd.exists() == false )
        QDir().mkdir( s_Dir );

    if ( b_emptyDir == true )
        emptyDir( s_Dir );

    return( s_Dir );
}

// ******************************************************************************
// ******************************************************************************
// ******************************************************************************

/*! @brief Dateinamen in einer Liste von Namen ersetzen.
*
*   In einer Liste von Dateinamen wird ein Search and Replace ausgefuehrt.
*
*   @param sl_FilenameIn Liste der Dateinamen in der gesucht und ersetzt werden soll.
*   @param s_searchStr String nach dem gesucht wird,
*   @param s_replaceStr String der den Gesuchten ersetzt.
*
*   @retval Liste der veraenderten Dateinamen.
*/

QStringList MainWindow::renameFiles( QStringList sl_FilenameIn, const QString& s_searchStr, const QString& s_replaceStr )
{
    int			i			= 0;

    QString		s_Filename	= "";

    QStringList	sl_FilenameOut;

    for ( i=0; i<sl_FilenameIn.count(); i++ )
    {
        s_Filename = sl_FilenameIn.at( i );
        s_Filename.replace( s_searchStr, s_replaceStr );
        sl_FilenameOut.append( s_Filename );
    }

    return( sl_FilenameOut );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::OpenDataDescriptionHeader()
{
    QString s_OutputStr = "";

    s_OutputStr.append( tr( "// METAHEADER - Data import at " ) );
    s_OutputStr.append( QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm" ) );
    s_OutputStr.append( "\n" );
    s_OutputStr.append( "{" );
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::CloseDataDescriptionHeader()
{
    QString s_OutputStr = "";

    s_OutputStr.append( "}" );
    s_OutputStr.append( "\n" );
    s_OutputStr.append( "//" );
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ParentID( const QString& s_ParentID )
{
    QString s_OutputStr = "";

    if ( s_ParentID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ParentID\": " );
        s_OutputStr.append( s_ParentID );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DataSetID( const QString& s_DatasetID )
{
    QString s_OutputStr = "";

    if ( s_DatasetID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"DataSetID\": " );
        s_OutputStr.append( s_DatasetID );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::AuthorIDs( const QString& s_AuthorIDs )
{
    QString s_OutputStr = "";

    if ( s_AuthorIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"AuthorIDs\": " );
        s_OutputStr.append( "[ "+ s_AuthorIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::SourceID( const QString& s_SourceID )
{
    QString s_OutputStr = "";

    if ( s_SourceID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"SourceID\": " );
        s_OutputStr.append( s_SourceID );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DatasetTitle( const QString& s_DatasetTitle )
{
    QString s_OutputStr = "";

    if ( s_DatasetTitle.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"Title\": \"" );
        s_OutputStr.append( s_DatasetTitle );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ExportFilename( const QString& s_ExportFilename )
{
    QString s_OutputStr = "";

    if ( s_ExportFilename.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ExportFilename\": \"" );
        s_OutputStr.append( s_ExportFilename );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::EventLabel( const QString& s_EventLabel )
{
    QString s_OutputStr = "";

    if ( s_EventLabel.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"EventLabel\": \"" );
        s_OutputStr.append( s_EventLabel );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::Parameter( const int i_ParameterID, const int i_PIID, const int i_MethodID, const QString& s_Format, const QString& s_Comment )
{
    QString s_OutputStr = "    ";

    s_OutputStr.append( "{ \"ID\": " + QString( "%1" ).arg( i_ParameterID ) + ", " );
    s_OutputStr.append( "\"PI_ID\": "  + QString( "%1" ).arg( i_PIID ) + ", " );
    s_OutputStr.append( "\"MethodID\": "  + QString( "%1" ).arg( i_MethodID ) );

    if ( s_Format.isEmpty() == false )
        s_OutputStr.append( ", \"Format\": \""  + s_Format + "\"" );

    if ( s_Comment.isEmpty() == false )
        s_OutputStr.append( ", \"Comment\": \""  + s_Comment + "\"" );

    s_OutputStr.append( " },");
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ParameterFirst( const int i_ParameterID, const int i_PIID, const int i_MethodID, const QString& s_Format, const QString& s_Comment )
{
    QString s_OutputStr = "  ";

    s_OutputStr.append( "\"ParameterIDs\": [" );
    s_OutputStr.append( "\n" );
    s_OutputStr.append( "    { \"ID\": " + QString( "%1" ).arg( i_ParameterID ) + ", " );
    s_OutputStr.append( "\"PI_ID\": "  + QString( "%1" ).arg( i_PIID ) + ", " );
    s_OutputStr.append( "\"MethodID\": "  + QString( "%1" ).arg( i_MethodID ) );

    if ( s_Format.isEmpty() == false )
        s_OutputStr.append( ", \"Format\": \""  + s_Format + "\"" );

    if ( s_Comment.isEmpty() == false )
        s_OutputStr.append( ", \"Comment\": \""  + s_Comment + "\"" );

    s_OutputStr.append( " },");
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ParameterLast( const int i_ParameterID, const int i_PIID, const int i_MethodID, const QString& s_Format, const QString& s_Comment )
{
    QString s_OutputStr = "    ";

    s_OutputStr.append( "{ \"ID\": " + QString( "%1" ).arg( i_ParameterID ) + ", " );
    s_OutputStr.append( "\"PI_ID\": "  + QString( "%1" ).arg( i_PIID ) + ", " );
    s_OutputStr.append( "\"MethodID\": "  + QString( "%1" ).arg( i_MethodID ) );

    if ( s_Format.isEmpty() == false )
        s_OutputStr.append( ", \"Format\": \""  + s_Format + "\"" );

    if ( s_Comment.isEmpty() == false )
        s_OutputStr.append( ", \"Comment\": \""  + s_Comment + "\"" );

    s_OutputStr.append( " } ],");
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DatasetComment( const QString& s_DatasetComment )
{
    QString s_OutputStr = "";

    if ( s_DatasetComment.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"DataSetComment\": \"" );
        s_OutputStr.append( s_DatasetComment );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ProjectIDs( const QString& s_ProjectIDs )
{
    QString s_OutputStr = "";

    if ( s_ProjectIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ProjectIDs\": " );
        s_OutputStr.append( "[ "+ s_ProjectIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::TopologicTypeID( const int i_TopologicTypeID )
{
    QString s_OutputStr = "  ";

    s_OutputStr.append( "\"TopologicTypeID\": " );
    s_OutputStr.append( QString( "%1," ).arg( i_TopologicTypeID ) );
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::StatusID( const int i_StatusID )
{
    QString s_OutputStr = "  ";

    s_OutputStr.append( "\"StatusID\": " );
    s_OutputStr.append( QString( "%1," ).arg( i_StatusID ) );
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::UserIDs( const QString& s_UserIDs )
{
    QString s_OutputStr = "";

    if ( s_UserIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"UserIDs\": " );
        s_OutputStr.append( "[ "+ s_UserIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( "\n" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::LoginID( const QString& s_LoginID )
{
    QString s_OutputStr = "  ";

    s_OutputStr.append( "\"LoginID\": " );
    s_OutputStr.append( s_LoginID );
    s_OutputStr.append( "\n" );

    return( s_OutputStr );
}

