/****************************************************************
**
** Class deleteEmptyColumns
** 01.06.2003, Dr. Rainer Sieger
** last change: 09.08.2005
**
****************************************************************/

#include <QtGui>

#include "Application.h"
#include "Globals.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 31.07.2004

int MainWindow::deleteEmptyColumns( const QString& FilenameOut, const QString& s_baseNameIn, int i_EmptyParameter[],
                                      const int i_NumOfParameters, const int i_NumOfLines,
                                      QStringList sl_DSParameter, QStringList sl_MFParameter,
                                      QStringList sl_DataSetComment, QStringList sl_URLDataDetails, QStringList sl_URLOtherVersion,
                                      QString& s_EventLabel, QString& s_MinorLabel,
                                      QString& s_Author, QString& s_Source, QString& s_DatasetTitle,
                                      QString& s_ExportFilename, QString& s_Reference,
                                      QString& s_Project, QString& s_DataSetComment,
                                      QString& s_URLDataDetails, QString& s_URLOtherVersion, QString& s_PI,
                                      QString& s_User, const int i_Status, const bool b_Login,
                                      const bool b_writeHeader, const bool b_useFilenameInAsEventLabel,
                                      const int i_MetadataFileMode, const int i_TopologicType, const bool b_overwriteDataset )
{
    #if defined(Q_WS_X11)
        const char eol = '\n';
    #endif

    #if defined(Q_WS_WIN)
        const char eol = '\n';
    #endif

    #if defined(Q_WS_MACX)
        const char eol = '\r';
    #endif

    int		i							= 0;
    int		j							= 0;

    int		i_deletedParameters			= 0;

    QString	InputStr					= "";
    QString	DummyStr					= "";
    QString	FilenameTemp				= "";

    QString s_fullNameOut				= "";
    QString s_baseNameOut				= "";
    QString	s_Parameter					= "";
    QString	s_ParameterDS				= "";
    QString	s_ParameterMF				= "";
    QString	s_QF						= "";

    QString s_tempAuthor				= s_Author;
    QString s_tempSource				= s_Source;
    QString s_tempReference				= s_Reference;
    QString s_tempPI					= s_PI;
    QString s_tempUser					= s_User;
    QString s_tempExportFilename		= s_ExportFilename;
    QString s_tempDatasetTitle			= s_DatasetTitle;
    QString s_tempDatasetComment		= s_DataSetComment;
    QString s_tempProject				= s_Project;
    QString s_tempURLDataDetails		= s_URLDataDetails;
    QString s_tempURLOtherVersion		= s_URLOtherVersion;

    QString sd_DataSetComment			= "";
    QString sd_URLDataDetails			= "";
    QString sd_URLOtherVersion			= "";

    bool	b_foundInDataSetCommentList	= FALSE;
    bool	b_foundInURLDataDetailsList	= FALSE;

    double	d_Factor[MAX_NUM_OF_COLUMNS+1];
    double	d_RangeMin[MAX_NUM_OF_COLUMNS+1];
    double	d_RangeMax[MAX_NUM_OF_COLUMNS+1];

    int		i_Digits[MAX_NUM_OF_COLUMNS+1];

    QString	s_defaultValue[MAX_NUM_OF_COLUMNS+1];

// *************************************************************************************

    memset( d_Factor, 0, ( MAX_NUM_OF_COLUMNS+1 ) * sizeof( double ) );
    memset( d_RangeMin, 0, ( MAX_NUM_OF_COLUMNS+1 ) * sizeof( double ) );
    memset( d_RangeMax, 0, ( MAX_NUM_OF_COLUMNS+1 ) * sizeof( double ) );
    memset( i_Digits, 0, ( MAX_NUM_OF_COLUMNS+1 ) * sizeof( int ) );

// *************************************************************************************

    QFileInfo fi( FilenameOut );

    FilenameTemp = fi.absolutePath() + "/" + "tempFile.txt";

    QFile fin( FilenameTemp );
    if ( fin.open( QIODevice::ReadOnly | QIODevice::Text ) == FALSE )
        return( -110 );

    QFile fout( FilenameOut );
    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text ) == FALSE )
    {
        fin.close();
        return( -120 );
    }

// *************************************************************************************

    s_fullNameOut = s_EventLabel;			// base name of the output file

    if ( s_MinorLabel.isEmpty() == FALSE )
        s_fullNameOut.append( "@" + s_MinorLabel );

// *************************************************************************************

    s_baseNameOut = s_EventLabel;			// base name of the output file
    s_baseNameOut.replace( "~", "/" );		// Event labels contains "/"

// *************************************************************************************

    QTextStream tin( &fin ); // tin.setEncoding( QTextStream::Latin1 );
    QTextStream tout( &fout ); // tout.setEncoding( QTextStream::Latin1 );

// *************************************************************************************

    for ( j=1; j<=i_NumOfParameters; j++ )
    {
        if ( i_EmptyParameter[j] < i_NumOfLines )
            i_EmptyParameter[j] = 0;
        else
            i_deletedParameters++;
    }

// *************************************************************************************
// With header

    if ( b_writeHeader == TRUE )
    {
        tout << "/* DATA DESCRIPTION:" << eol;

// *************************************************************************************
// PI

        if ( s_tempPI == "999999" )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempPI = "@GP@" + s_baseNameIn + "@";
            else
                s_tempPI = "@GP@" + s_fullNameOut + "@";
        }

// *************************************************************************************
// Author

        if ( s_tempAuthor.isEmpty() == FALSE )
        {
            s_tempAuthor.replace( " ", "" );
            s_tempAuthor.replace( ";", "," );

            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempAuthor.replace( "999999", "@A@" + s_baseNameIn + "@" );
            else
                s_tempAuthor.replace( "999999", "@A@" + s_fullNameOut + "@" );

            int i_NumOfAuthors = s_tempAuthor.count( "," ) + 1;
            tout << tr( "Author:" ) << "\t" << s_tempAuthor.section( ",", 0, 0 ) << eol;
            for ( j=1; j<i_NumOfAuthors; j++ )
                tout << "\t" << s_tempAuthor.section( ",", j, j ) << eol;
        }

// *************************************************************************************
// Source

        if ( s_tempSource.isEmpty() == FALSE )
        {
            s_tempSource.replace( " ", "" );
            s_tempSource.replace( ";", "," );

            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempSource.replace( "999999", "@S@" + s_baseNameIn + "@" );
            else
                s_tempSource.replace( "999999", "@S@" + s_fullNameOut + "@" );

            tout << tr( "Source:" ) << "\t" << s_tempSource.section( ",", 0, 0 ) << eol;
        }

// *************************************************************************************
// Dataset title

        if ( s_tempDatasetTitle.isEmpty() == FALSE )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
            {
                s_tempDatasetTitle.replace( "999999", "@D@" + s_baseNameIn + "@" );
                s_tempDatasetTitle.replace( "$E", s_baseNameIn );
            }
            else
            {
                s_tempDatasetTitle.replace( "999999", "@D@" + s_fullNameOut + "@" );
                s_tempDatasetTitle.replace( "$E", s_baseNameOut );
            }

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
                {
                    fin.close();
                    fout.close();
                    return( -1 );
                }
            }
        }

// *************************************************************************************
// Reference

        if ( s_tempReference.isEmpty() == FALSE )
        {
            int j = 0;

            s_tempReference.replace( " ", "" );
            s_tempReference.replace( ";", "," );

            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempReference.replace( "999999", "@R@" + s_baseNameIn + "@" );
            else
                s_tempReference.replace( "999999", "@R@" + s_fullNameOut + "@" );

            int i_NumOfReferences = s_tempReference.count( "," ) + 1;
            tout << tr( "Reference:" ) << "\t" << s_tempReference.section( ",", 0, 0 ) << eol;
            for ( j=1; j<i_NumOfReferences; j++ )
                tout << "\t" << s_tempReference.section( ",", j, j ) << eol;
        }

// *************************************************************************************
// Export filename

        if ( s_tempExportFilename.isEmpty() == FALSE )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
            {
                s_tempExportFilename.replace( "999999", "@E@" + s_baseNameIn + "@" );
                s_tempExportFilename.replace( "$E", s_baseNameIn );
            }
            else
            {
                s_tempExportFilename.replace( "999999", "@E@" + s_fullNameOut + "@" );
                s_tempExportFilename.replace( "$E", s_baseNameOut );
            }

            s_tempExportFilename.replace( "$@", s_MinorLabel );
            s_tempExportFilename.replace( "-track", "" );
            s_tempExportFilename.replace( "Track.", "" );

            s_tempExportFilename = createValidFilename( s_tempExportFilename );

            if ( s_tempExportFilename.length() <= 80)
                tout << tr( "Export Filename:" ) << "\t" << s_tempExportFilename << eol;
            else
            {
                int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "Export filename comprises more that 80 characters.\nAbort?" ), QMessageBox::Yes, QMessageBox::No );
                if ( err == QMessageBox::No )
                    tout << tr( "Export Filename:" ) << "\t" << s_tempExportFilename.left( 77 ) << "..." << eol;
                else
                {
                    fin.close();
                    fout.close();
                    return( -1 );
                }
            }
        }

// *************************************************************************************
// Event label

        if ( s_EventLabel.isEmpty() == FALSE )
            tout << tr( "Event:" ) << "\t" << s_EventLabel << eol;

// *************************************************************************************
// PI

        if ( s_tempPI.isEmpty() == FALSE )
            tout << tr( "PI:" ) << "\t" << s_tempPI << eol;
        else
            tout << tr( "PI:\t506" ) << eol;

// *************************************************************************************
// Parameters

        if ( i_MetadataFileMode != NOTUSED ) // begin parameter from metadata file
        {
            int j    = 0;
            int i_DS = 0;
            int i_MF = 0;

            tout << tr( "Parameter:" );

            switch ( i_MetadataFileMode )
            {
            case BYNAME:
                for ( i_DS=0; i_DS<sl_DSParameter.count(); i_DS++ )
                {
                    j++;

                    s_ParameterDS	= sl_DSParameter.at( i_DS );
                    s_Parameter		= "";
                    i_MF			= 0;

                    while ( ( i_MF<sl_MFParameter.count() ) && ( s_Parameter.isEmpty() == TRUE ) )
                    {
                        s_ParameterMF = sl_MFParameter.at( i_MF++ );

                        if ( s_ParameterDS.contains( QRegExp( "[@a-zA-Z]" ) ) == FALSE )
                        {
                            if ( s_ParameterMF.section( "\t", 1, 1 ) == s_ParameterDS ) // s_ParameterDS is a number
                                s_Parameter = buildParameter( s_PI, s_ParameterMF, s_fullNameOut );
                        }
                        else
                        {
                            if ( s_ParameterMF.section( "\t", 0, 0 ) == s_ParameterDS ) // s_ParameterDS includes characters
                                s_Parameter = buildParameter( s_PI, s_ParameterMF, s_fullNameOut );
                        }
                    }

                    if ( s_Parameter.isEmpty() == TRUE )
                    {
                        tout << "\t" << s_ParameterDS << tr( " * not define in metadata file" ) << eol;
                    }
                    else
                    {
                        if ( i_EmptyParameter[j] == 0 )
                            tout << "\t" << s_Parameter << eol;
                    }
                }
                break;

            case AUTO:
            case BYPOSITION:
                for ( i_MF=0; i_MF<sl_MFParameter.count(); i_MF++ )
                {
                    j++;

                    s_ParameterMF	= sl_MFParameter.at( i_MF );
                    s_Parameter		= buildParameter( s_PI, s_ParameterMF, s_fullNameOut );

                    if ( b_useFilenameInAsEventLabel == TRUE )
                        s_Parameter.replace( "@$E@", "@" + s_baseNameIn + "@" );
                    else
                        s_Parameter.replace( "@$E@", "@" + s_fullNameOut + "@" );

                    if ( i_EmptyParameter[j] == 0 )
                        tout << "\t" << s_Parameter << eol;
                }
                break;
            }
        } // end parameter from metadata file

// *************************************************************************************
// Dataset comment

        if ( sl_DataSetComment.count() > 0 )
        {
            DummyStr = s_EventLabel;

            if ( s_MinorLabel.isEmpty() == FALSE )
                DummyStr.append( "@"+s_MinorLabel );

            for ( i=0; i<sl_DataSetComment.count(); i++ )
            {
                sd_DataSetComment = sl_DataSetComment.at( i );
                sd_DataSetComment.replace( "\"", "" );

                if ( sd_DataSetComment.section( "\t", 0, 0 ) == DummyStr )
                {
                    tout << tr( "DataSet Comment:" ) << "\t" << sd_DataSetComment.section( "\t", 1, 1 ) << eol;
                    i = sl_DataSetComment.count();
                    b_foundInDataSetCommentList = TRUE;
                }
            }
        }

        if ( ( s_tempDatasetComment.isEmpty() == FALSE ) && ( b_foundInDataSetCommentList == FALSE ) )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
            {
                s_tempDatasetComment.replace( "999999", "@C@" + s_baseNameIn + "@" );
                s_tempDatasetComment.replace( "$E", s_baseNameIn );
            }
            else
            {
                s_tempDatasetComment.replace( "999999", "@C@" + s_baseNameOut + "@" );
                s_tempDatasetComment.replace( "$E", s_baseNameOut );
            }

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
                {
                    fin.close();
                    fout.close();
                    return( -1 );
                }
            }
        }

// *************************************************************************************
// Project

        if ( s_tempProject.isEmpty() == FALSE )
        {
            int j = 0;

            s_tempProject.replace( " ", "" );
            s_tempProject.replace( ";", "," );

            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempProject.replace( "999999", "@Pro@" + s_baseNameIn + "@" );
            else
                s_tempProject.replace( "999999", "@Pro@" + s_fullNameOut + "@" );

            int i_NumOfProjects = s_tempProject.count( "," ) + 1;
            tout << tr( "Project:" ) << "\t" << s_tempProject.section( ",", 0, 0 ) << eol;
            for ( j=1; j<i_NumOfProjects; j++ )
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

        if ( ( b_Login == ON ) && ( s_tempUser.isEmpty() == FALSE ) )
        {
            int j = 0;

            s_tempUser.replace( " ", "" );
            s_tempUser.replace( ";", "," );

            if ( b_useFilenameInAsEventLabel == TRUE )
                s_tempUser.replace( "999999", "@U@" + s_baseNameIn + "@" );
            else
                s_tempUser.replace( "999999", "@U@" + s_fullNameOut + "@" );

            int i_NumOfUsers = s_tempUser.count( "," ) + 1;

            if ( i_NumOfUsers == 1 )
            {
                if ( s_tempUser.section( ",", 0, 0 ) != s_tempPI )
                    tout << tr( "User:" ) << "\t" << s_tempUser.section( ",", 0, 0 ) << eol;
            }
            else
            {
                tout << tr( "User:" );
                for ( j=0; j<i_NumOfUsers; j++ )
                {
                    if ( s_tempUser.section( ",", j, j ) != s_tempPI )
                        tout << "\t" << s_tempUser.section( ",", j, j ) << eol;
                }
            }
        }

// *************************************************************************************
// Login

        if ( b_Login == ON )
            tout << tr( "Login:" ) << "\tON" << eol;
        else
            tout << tr( "Login:" ) << "\tOFF" << eol;

// *************************************************************************************
// overwrite Dataset

        if ( b_overwriteDataset == TRUE )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
                tout << tr( "DataSet ID:" ) << "\t" << "@I@" + s_baseNameIn + "@" << eol;
            else
                tout << tr( "DataSet ID:" ) << "\t" << "@I@" + s_fullNameOut + "@" << eol;
        }

// *************************************************************************************
// Further details URL

        if ( sl_URLDataDetails.count() > 0 )
        {
            for ( i=0; i<sl_URLDataDetails.count(); i++ )
            {
                sd_URLDataDetails = sl_URLDataDetails.at( i );

                if ( sd_URLDataDetails.section( "\t", 0, 0 ) == s_EventLabel )
                {
                    tout << tr( "URL Data details:" ) << "\t" << sd_URLDataDetails.section( "\t", 1, 1 ) << eol;
                    i = sl_URLDataDetails.count();
                    b_foundInURLDataDetailsList = TRUE;
                }
            }
        }

        if ( ( s_tempURLDataDetails.isEmpty() == FALSE ) && ( b_foundInURLDataDetailsList == FALSE ) )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
            {
                s_tempURLDataDetails.replace( "999999", "@F@" + s_baseNameIn + "@" );
                s_tempURLDataDetails.replace( "$E", s_baseNameIn );
            }
            else
            {
                s_tempURLDataDetails.replace( "999999", "@F@" + s_fullNameOut + "@" );
                s_tempURLDataDetails.replace( "$E", s_fullNameOut );
            }

            s_tempURLDataDetails.replace( "$@", s_MinorLabel );

            if ( s_tempURLDataDetails.length() <= 1000 )
                tout << tr( "URL Data details:" ) << "\t" << s_tempURLDataDetails << eol;
            else
            {
                int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "URL comprises more that 1000 characters.\nAbort?" ),QMessageBox::Yes,QMessageBox::No);
                if ( err == QMessageBox::No )
                    tout << tr( "URL Data details:" ) << "\t" << s_tempURLDataDetails.left( 997 ) << "..." << eol;
                else
                {
                    fin.close();
                    fout.close();
                    return( -1 );
                }
            }
        }

// *************************************************************************************
// Other version URL

        if ( sl_URLOtherVersion.count() > 0 )
        {
            for ( i=0; i<sl_URLOtherVersion.count(); i++ )
            {
                sd_URLOtherVersion = sl_URLOtherVersion.at( i );

                if ( sd_URLOtherVersion.section( "\t", 0, 0 ) == s_EventLabel )
                {
                    tout << tr( "URL other version:" ) << "\t" << sd_URLOtherVersion.section( "\t", 1, 1 ) << eol;
                    i = sl_URLOtherVersion.count();
                    b_foundInURLDataDetailsList = TRUE;
                }
            }
        }

        if ( ( s_tempURLOtherVersion.isEmpty() == FALSE ) && ( b_foundInURLDataDetailsList == FALSE ) )
        {
            if ( b_useFilenameInAsEventLabel == TRUE )
            {
                s_tempURLOtherVersion.replace( "999999", "@O@" + s_baseNameIn + "@" );
                s_tempURLOtherVersion.replace( "$E", s_baseNameIn );
            }
            else
            {
                s_tempURLOtherVersion.replace( "999999", "@O@" + s_fullNameOut + "@" );
                s_tempURLOtherVersion.replace( "$E", s_fullNameOut );
            }

            s_tempURLOtherVersion.replace( "$@", s_MinorLabel );

            if ( s_tempURLOtherVersion.length() <= 1000 )
                tout << tr( "URL other version:" ) << "\t" << s_tempURLOtherVersion << eol;
            else
            {
                int err = QMessageBox::warning( this, tr( "Split2Events" ), tr( "URL comprises more that 1000 characters.\nAbort?" ),QMessageBox::Yes,QMessageBox::No);
                if ( err == QMessageBox::No )
                    tout << tr( "URL other version:" ) << "\t" << s_tempURLOtherVersion.left( 997 ) << "..." << eol;
                else
                {
                    fin.close();
                    fout.close();
                    return( -1 );
                }
            }
        }

// *************************************************************************************

        tout << "*/" << eol;  // end of header

// *************************************************************************************

        if ( i_MetadataFileMode != NOTUSED ) // begin data table
        {
            InputStr = tin.readLine();
            InputStr.replace( "\"", "" );

            tout << tr( "Event label" );

            int j    = 0;
            int i_MF = 0;

            switch ( i_MetadataFileMode )
            {
            case BYNAME:
                for ( j=1; j<=i_NumOfParameters; j++ )
                {
                    if ( i_EmptyParameter[j] == 0 )
                    {
                        s_Parameter = InputStr.section( "\t", j, j );
                        d_Factor[j] = 1;

                        for ( i_MF=0; i_MF<sl_MFParameter.count(); i_MF++ )
                        {
                            s_ParameterMF = sl_MFParameter.at( i_MF );

                            if ( ( s_Parameter == s_ParameterMF.section( "\t", 0, 0 ) ) || ( s_Parameter == s_ParameterMF.section( "\t", 1, 1 ) ) )
                            {
                                if ( s_ParameterMF.section( "\t", 1, 1 ).isEmpty() == FALSE )
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
                        tout << "\t" << s_Parameter;
                    }
                }
                tout << eol;
                break;

            case AUTO:
            case BYPOSITION:
                for ( j=1; j<=i_NumOfParameters; j++ )
                {
                    if ( i_EmptyParameter[j] == 0 )
                    {
                        s_Parameter		= InputStr.section( "\t", j, j );
                        s_ParameterMF	= sl_MFParameter.at( j-1 );
                        d_Factor[j]		= 1.;

                        if ( s_ParameterMF.section( "\t", 1, 1 ).isEmpty() == FALSE )
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

                        tout << "\t" << s_Parameter;
                    }
                }
                tout << eol;
                break;
            }

// *************************************************************************************

            while ( tin.atEnd() == FALSE )
            {
                InputStr = tin.readLine();
                InputStr.replace( "\"", "" );

                qApp->processEvents();

                if ( InputStr.isEmpty() == FALSE )
                {
                    tout << InputStr.section( "\t", 0, 0 );

                    int j = 0;
                    for ( j=1; j<=i_NumOfParameters; j++ )
                    {
                        if ( i_EmptyParameter[j] == 0 )
                        {
                            s_Parameter = InputStr.section( "\t", j, j );

                            if ( s_Parameter.isEmpty() == FALSE )
                            {
                                if ( s_Parameter.contains( QRegExp( "[a-zA-Z ]" ) ) == FALSE )
                                {
                                    if ( s_Parameter.indexOf( QRegExp( "[<>?/*#]" ) ) == 0 )
                                    {
                                        s_QF = s_Parameter.left( 1 );
                                        s_Parameter.replace( s_QF, "" );

                                        if ( d_Factor[j] != 1. )
                                        {
                                            if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                                tout << "\t" << s_QF << QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] );
                                            else
                                                tout << "\t/" << QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] );
                                        }
                                        else
                                        {
                                            if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                                tout << "\t" << s_QF << s_Parameter;
                                            else
                                                tout << "\t/" << s_Parameter;
                                        }
                                    }
                                    else
                                    {
                                        if ( d_Factor[j] != 1. )
                                        {
                                            if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                                tout << "\t" << QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] );
                                            else
                                                tout << "\t/" << QString( "%1" ).arg( s_Parameter.toDouble()*d_Factor[j], 0, 'f', i_Digits[j] );
                                        }
                                        else
                                        {
                                            if ( ( s_Parameter.toDouble() >= d_RangeMin[j] ) && ( s_Parameter.toDouble() <= d_RangeMax[j] ) )
                                                tout << "\t" << s_Parameter;
                                            else
                                                tout << "\t/" << s_Parameter;
                                        }
                                    }
                                }
                                else
                                {
                                    tout << "\t" << s_Parameter; // contains characters
                                }
                            }
                            else
                            {
                                tout << "\t" << s_defaultValue[j]; // wirte default value if empty
                            }
                        }
                    }
                    tout << eol;
                }
            }
        }  // end with metadata file
        else
        { // begin without metadata file
            InputStr = tin.readLine();
            InputStr.replace( "\"", "" );

            tout << tr( "Event label" );

            int j = 0;
            for ( j=1; j<= i_NumOfParameters; j++ )
                if ( i_EmptyParameter[j] == 0 )
                    tout << "\t" << InputStr.section( "\t", j, j );

            tout << eol;

            while ( tin.atEnd() == FALSE )
            {
                InputStr = tin.readLine();
                InputStr.replace( "\"", "" );

                if ( InputStr.isEmpty() == FALSE )
                {
                    tout << InputStr.section( "\t", 0, 0 );

                    for ( j=1; j<=i_NumOfParameters; j++ )
                        if ( i_EmptyParameter[j] == 0 )
                            tout << "\t" << InputStr.section( "\t", j, j );

                    tout << eol;
                }
            }
        }
    } // end with header
    else
    {	// Without header
        InputStr = tin.readLine();
        InputStr.replace( "\"", "" );

        tout << tr( "Event label" );

        int j = 0;
        for ( j=1; j<= i_NumOfParameters; j++ )
            if ( i_EmptyParameter[j] == 0 )
                tout << "\t" << InputStr.section( "\t", j, j );

        tout << eol;

        while ( tin.atEnd() == FALSE )
        {
            InputStr = tin.readLine();
            InputStr.replace( "\"", "" );

            if ( InputStr.isEmpty() == FALSE )
            {
                tout << InputStr.section( "\t", 0, 0 );

                for ( j=1; j<=i_NumOfParameters; j++ )
                    if ( i_EmptyParameter[j] == 0 )
                        tout << "\t" << InputStr.section( "\t", j, j );

                tout << eol;
            }
        }
    }

// *************************************************************************************

    fin.close();
    fout.close();

    fin.remove(); // temp file

    if ( ( gb_markSmallFile == TRUE ) && ( i_NumOfLines == 1 ) )  // rename if surface data
    {
        QDir file( FilenameOut );
        file.rename( FilenameOut, fi.absolutePath() + "/" + "z1_" + fi.baseName() + ".txt" );
    }

    if ( ( gb_markSmallFile == TRUE ) && ( i_NumOfLines == 2 ) )  // rename if 2-line station
    {
        QDir file( FilenameOut );
        file.rename( FilenameOut, fi.absolutePath() + "/" + "z2_" + fi.baseName() + ".txt" );
    }

    if ( ( gb_markSmallFile == TRUE ) && ( i_NumOfLines == 3 ) )  // rename if 3-line station
    {
        QDir file( FilenameOut );
        file.rename( FilenameOut, fi.absolutePath() + "/" + "z3_" + fi.baseName() + ".txt" );
    }

    return( i_deletedParameters );
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

