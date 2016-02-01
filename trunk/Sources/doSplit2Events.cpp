/****************************************************************
**
** Split2Events
** 2003-06-01, Dr. Rainer Sieger
** last change: 2013-06-29
**
****************************************************************/

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2013-06-29

void MainWindow::doSplit2Events()
{
    int             i                           = 0;

    int             err                         = _NOERROR_;

    int             i_stopProgress              = 0;

    QString         s_FilenameMetadata          = "";

    QStringList     sl_ParameterList;
    QStringList     sl_DataSetComment;
    QStringList     sl_FurtherDetailsReference;
    QStringList     sl_FurtherDetailsDataset;
    QStringList     sl_OtherVersionReference;
    QStringList     sl_OtherVersionDataset;
    QStringList     sl_SourceReference;
    QStringList     sl_SourceDataset;

// *************************************************************************************

    if ( gsl_FilenameList.isEmpty() == true )
        chooseFiles();

    if ( gsl_FilenameList.isEmpty() == true )
        return;

    if ( gb_overwriteDataset == true )
    {
        if ( QMessageBox::warning( this, getApplicationName( true ), "The \"overwrite existing datasets\" option has been set!", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok ) == QMessageBox::Cancel )
            return;
    }

// *************************************************************************************

    gsl_FilenameList = renameFiles( gsl_FilenameList, "_metadata", "" );

// *************************************************************************************

    if ( ( ( gi_MetadataFileMode == _BYNAME_ ) || ( gi_MetadataFileMode == _BYPOSITION_ ) ) && ( gb_writeHeader == true ) )
    {
        s_FilenameMetadata = getMetadataFilename( gsl_FilenameList.at( 0 ), gi_Extension, gsl_FilenameList.count() );

        err = readMetadataFile( s_FilenameMetadata, sl_ParameterList, sl_DataSetComment, sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset );
    }

// *************************************************************************************

    if ( err == _NOERROR_ )
    {
        savePreferences();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Splitting files..." ) );

        while ( ( i<gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
        {
            QFileInfo fi( gsl_FilenameList.at( i ) );

            if ( ( fi.exists() == true ) && ( fi.isFile() == true ) )
            {
                if ( ( gi_MetadataFileMode == _AUTO_ ) && ( gb_writeHeader == true ) )
                {
                    doCreateMetadataTemplate();

                    switch ( gi_Extension )
                    {
                    case _CSV_:
                        s_FilenameMetadata	= fi.absolutePath() + "/" + fi.baseName() + tr( "_metadata.csv" );
                        break;
                    default:
                        s_FilenameMetadata	= fi.absolutePath() + "/" + fi.baseName() + tr( "_metadata.txt" );
                        break;
                    }

                    err = readMetadataFile( s_FilenameMetadata, sl_ParameterList, sl_DataSetComment, sl_FurtherDetailsReference, sl_FurtherDetailsDataset, sl_OtherVersionReference, sl_OtherVersionDataset );
                }

                if ( err == _NOERROR_ )
                    err = Split2Events( fi.absoluteFilePath(), gi_Codec, gi_Extension, sl_ParameterList, sl_DataSetComment, sl_FurtherDetailsReference, sl_FurtherDetailsDataset,
                                        sl_OtherVersionReference, sl_OtherVersionDataset, sl_SourceReference, sl_SourceDataset, gs_Author, gs_Source, gs_DatasetTitle,
                                        gs_ExportFilename, gs_Reference, gs_Project, gs_DataSetComment, gs_FurtherDetailsReference, gs_FurtherDetailsDataset,
                                        gs_OtherVersionReference, gs_OtherVersionDataset, gs_SourceReference, gs_SourceDataset, gs_PI, gs_User, gs_Parent, gs_Issue,
                                        gi_Status, gi_Login, gb_writeHeader, gb_splitFile, gi_OutOfRangeValue, gb_useFilenameInAsEventLabel, gb_makeFilenameUnique,
                                        gi_MetadataFileMode, gi_TopologicType, gb_overwriteDataset, gb_markSmallFile, i, gsl_FilenameList.count() );

                if ( ( gi_MetadataFileMode == _AUTO_ ) && ( gb_writeHeader == true ) )
                    QFile::remove( s_FilenameMetadata );
            }

            i_stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }

// *************************************************************************************

    if ( i_stopProgress == _APPBREAK_ )
        err = _APPBREAK_;

    if ( ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
        setStatusBar( tr( "Done" ), 2 );
    else
        setStatusBar( tr( "Splitting files was canceled" ), 2 );

    onError( err );
}
