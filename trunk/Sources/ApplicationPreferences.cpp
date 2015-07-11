/****************************************************************
**
** Preferences
** 2008-03-21, Dr. Rainer Sieger
** last change: 2008-04-27
**
****************************************************************/

#include "Application.h"

/*! @brief Sichern der Programmeinstellungen */

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-07-03

void MainWindow::savePreferences()
{
    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );
    settings.setValue( "NumOfProgramStarts", gi_NumOfProgramStarts );
    settings.setValue( "FirstProgramStart", false );
    settings.setValue( "WindowX", x() );
    settings.setValue( "WindowY", y() );
    settings.setValue( "WindowHeight", height() );
    settings.setValue( "WindowWidth", width() );

    settings.setValue( "DialogX", posDialog.x() );
    settings.setValue( "DialogY", posDialog.y() );
    settings.setValue( "DialogHeight", sizeDialog.height() );
    settings.setValue( "DialogWidth", sizeDialog.width() );

    settings.setValue( "SettingDialogX", posSettingDialog.x() );
    settings.setValue( "SettingDialogY", posSettingDialog.y() );
    settings.setValue( "SettingDialogHeight", sizeSettingDialog.height() );
    settings.setValue( "SettingDialogWidth", sizeSettingDialog.width() );

    settings.setValue( "Path", gs_Path );
    settings.setValue( "FilenameFormat", gs_FilenameFormat );

    // Parameter
    settings.setValue( "PI", gs_PI );
    settings.setValue( "Author", gs_Author );
    settings.setValue( "Reference", gs_Reference );
    settings.setValue( "Source", gs_Source );
    settings.setValue( "Project", gs_Project );
    settings.setValue( "DatasetTitle", gs_DatasetTitle );
    settings.setValue( "ExportFilename", gs_ExportFilename );

    settings.setValue( "DatasetComment", gs_DataSetComment );
    settings.setValue( "FurtherDetailsReference", gs_FurtherDetailsReference );
    settings.setValue( "FurtherDetailsDataset", gs_FurtherDetailsDataset );
    settings.setValue( "OtherVersionReference", gs_OtherVersionReference );
    settings.setValue( "OtherVersionDataset", gs_OtherVersionDataset );
    settings.setValue( "SourceReference", gs_SourceReference );
    settings.setValue( "SourceDataset", gs_SourceDataset );
    settings.setValue( "Parent", gs_Parent );
    settings.setValue( "User", gs_User );

    settings.setValue( "TopologicType", gi_TopologicType );
    settings.setValue( "Status", gi_Status );
    settings.setValue( "MetadataFileMode", gi_MetadataFileMode );
    settings.setValue( "OutOfRangeValue", gi_OutOfRangeValue );
    settings.setValue( "Login", gi_Login );
    settings.setValue( "WriteHeader", gb_writeHeader );
    settings.setValue( "SplitFile", gb_splitFile );
    settings.setValue( "MarkSmallFile", gb_markSmallFile );
    settings.setValue( "MakeFilenameUnique", gb_makeFilenameUnique );
    settings.setValue( "OverwriteDataset", gb_overwriteDataset );
    settings.setValue( "UseFilenameInAsEventLabel", gb_useFilenameInAsEventLabel );
    settings.setValue( "WriteParameterImportFile", gb_writeParameterImportFile );

    settings.setValue( "NumberOfParameterFiles", gi_NumOfParameterFiles );
    settings.setValue( "ShowParameterImportFileCreatedMessage", gb_showParameterImportFileCreatedMessage );
    settings.setValue( "MergeNewParameters", gb_mergeNewParameters );
    settings.setValue( "RefreshOnStart", gb_refreshOnStart );
    settings.setValue( "CreateAdditionalMetadataOptions", gb_createAdditionMetadataOptions );
    settings.setValue( "Codec", gi_Codec );
    settings.setValue( "Extension", gi_Extension );
    settings.setValue( "FilenameOfPDB", gs_FilenamePDB );

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2013-07-18

/*! @brief Laden der Programmeinstellungen */

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::loadPreferences()
{
    #if defined(Q_OS_LINUX)
        gi_Codec = _UTF8_; // UTF-8
        gi_EOL   = _WIN_;
    #endif

    #if defined(Q_OS_MAC)
        gi_Codec = _APPLEROMAN_; // Mac Roman
        gi_EOL   = _UNIX_;
    #endif

    #if defined(Q_OS_WIN)
        gi_Codec = _LATIN1_; // Latin-1 = ISO 8859-1
        gi_EOL   = _WIN_;
    #endif

    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    posWindow.setX( settings.value( "WindowX", 100 ).toInt() );
    posWindow.setY( settings.value( "WindowY", 100 ).toInt() );
    sizeWindow.setHeight( settings.value( "WindowHeight", 250 ).toInt() );
    sizeWindow.setWidth( settings.value( "WindowWidth", 600 ).toInt() );

    posDialog.setX( settings.value( "DialogX", 110 ).toInt() );
    posDialog.setY( settings.value( "DialogY", 110 ).toInt() );
    sizeDialog.setHeight( settings.value( "DialogHeight", 100 ).toInt() );
    sizeDialog.setWidth( settings.value( "DialogWidth", 100 ).toInt() );

    posSettingDialog.setX( settings.value( "SettingDialogX", 110 ).toInt() );
    posSettingDialog.setY( settings.value( "SettingDialogY", 110 ).toInt() );
    sizeSettingDialog.setHeight( settings.value( "SettingDialogHeight", 100 ).toInt() );
    sizeSettingDialog.setWidth( settings.value( "SettingDialogWidth", 100 ).toInt() );

    gs_Path                                     = settings.value( "Path", getDocumentDir() ).toString();
    gs_FilenameFormat                           = settings.value( "FilenameFormat", "zz%a_%N.txt" ).toString();
    gi_NumOfProgramStarts                       = settings.value( "NumOfProgramStarts", 0 ).toInt();
    gb_firstProgramStart                        = settings.value( "FirstProgramStart", true ).toBool();

    gs_PI                                       = settings.value( "PI", "" ).toString();
    gs_Author                                   = settings.value( "Author", "" ).toString();
    gs_Reference                                = settings.value( "Reference", "" ).toString();
    gs_Source                                   = settings.value( "Source", "" ).toString();
    gs_Project                                  = settings.value( "Project", "" ).toString();
    gs_DatasetTitle                             = settings.value( "DatasetTitle", "" ).toString();
    gs_ExportFilename                           = settings.value( "ExportFilename", "" ).toString();

    gs_DataSetComment                           = settings.value( "DatasetComment", "" ).toString();
    gs_FurtherDetailsReference                  = settings.value( "FurtherDetailsReference", "" ).toString();
    gs_FurtherDetailsDataset                    = settings.value( "FurtherDetailsDataset", "" ).toString();
    gs_OtherVersionReference                    = settings.value( "OtherVersionReference", "" ).toString();
    gs_OtherVersionDataset                      = settings.value( "OtherVersionDataset", "" ).toString();
    gs_SourceReference                          = settings.value( "SourceReference", "" ).toString();
    gs_SourceDataset                            = settings.value( "SourceDataset", "" ).toString();
    gs_Parent                                   = settings.value( "Parent", "" ).toString();
    gs_User                                     = settings.value( "User", "" ).toString();

    gi_TopologicType                            = settings.value( "TopologicType", _NOTSPECIFIED_ ).toInt();
    gi_Status                                   = settings.value( "Status", 2 ).toInt();
    gi_MetadataFileMode                         = settings.value( "MetadataFileMode", _BYNAME_ ).toInt();
    gi_OutOfRangeValue                          = settings.value( "OutOfRangeValue", _IGNORE_ ).toInt();
    gi_Login                                    = settings.value( "Login", _UNRESTRICTED_ ).toInt();
    gb_writeHeader                              = settings.value( "WriteHeader", true ).toBool();
    gb_splitFile                                = settings.value( "SplitFile", true ).toBool();
    gb_markSmallFile                            = settings.value( "MarkSmallFile", false ).toBool();
    gb_makeFilenameUnique                       = settings.value( "MakeFilenameUnique", false ).toBool();
    gb_overwriteDataset                         = settings.value( "OverwriteDataset", false ).toBool();
    gb_useFilenameInAsEventLabel                = settings.value( "UseFilenameInAsEventLabel", false ).toBool();
    gb_writeParameterImportFile                 = settings.value( "WriteParameterImportFile", false ).toBool();

    gi_NumOfParameterFiles                      = settings.value( "NumberOfParameterFiles", 0 ).toInt();
    gb_showParameterImportFileCreatedMessage    = settings.value( "ShowParameterImportFileCreatedMessage", true ).toBool();
    gb_mergeNewParameters                       = settings.value( "MergeNewParameters", false ).toBool();
    gb_refreshOnStart                           = settings.value( "RefreshOnStart", false ).toBool();
    gb_createAdditionMetadataOptions            = settings.value( "CreateAdditionalMetadataOptions", false ).toBool();
    gi_Codec                                    = settings.value( "Codec", gi_Codec ).toInt();
    gi_Extension                                = settings.value( "Extension", _TXT_ ).toInt();
    gs_FilenamePDB                              = settings.value( "FilenameOfPDB", getDataLocation() + QLatin1String( "/ParameterDB.pdb" ) ).toString();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Datei, die die Anzahl der DDIs enthaelt.
*   @return Name der Datei. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getParameterDDIFilename()
{
    QString s_ParameterDDIFilename  = "_ERROR_";

    #if defined(Q_OS_LINUX)
        s_ParameterDDIFilename = getDataLocation() + "/" + QCoreApplication::applicationName() + QLatin1String( "_ddi.txt" );
    #endif

    #if defined(Q_OS_MAC)
        s_ParameterDDIFilename = getDataLocation() + "/" + QCoreApplication::applicationName() + QLatin1String( "_ddi.txt" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
            s_ParameterDDIFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( "_ddi.txt" );
        else
            s_ParameterDDIFilename = getDataLocation() + "/" + QCoreApplication::applicationName() + QLatin1String( "_ddi.txt" );
    #endif

    return( s_ParameterDDIFilename );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Programmeinstellungen.
*   @return Name der Datei fuer die Programmeinstellungen. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getPreferenceFilename()
{
    QString s_PrefFilename = "";

    #if defined(Q_OS_LINUX)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/.config/" ) + QCoreApplication::organizationName() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".conf" );
    #endif

    #if defined(Q_OS_MAC)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/Library/Preferences/" ) + QCoreApplication::organizationDomain().section( ".", 1, 1 ) + "." + QCoreApplication::organizationDomain().section( ".", 0, 0 ) + "." + QCoreApplication::applicationName() + QLatin1String( ".plist" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
        {
            s_PrefFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
        else
        {
            QSettings cfg( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() );
            s_PrefFilename = QFileInfo( cfg.fileName() ).absolutePath() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
    #endif

    return( s_PrefFilename );
}
