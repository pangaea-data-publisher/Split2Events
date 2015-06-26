/* Application.h              */
/* 2013-05-15                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QTextEdit>
#include <QTextCodec>
#include <QTextStream>
#include <QStringListModel>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QTimer>
#include <QMimeData>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QStatusBar>
#include <QSettings>

#include "Globals.h"
#include "Webfile.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QLabel;
class QProgressDialog;
class QProgressBar;
class QTimer;
class QDateTime;
class QUrl;
class QTextEdit;
class QStringListModel;
class QListView;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "http://www.pangaea.de" )

    struct structPFormat
    {
        int integer;
        int digits;
        int lineInt;
        int lineDigi;
        float max;
        float min;
    };

    struct structParameter
    {
        QString	ParameterID;			// Parameter ID
        QString ParameterName;			// Parameter name
        QString ParameterNameL;         // Parameter name to lower
        QString ParameterAbbreviationL;	// Parameter abbreviation to lower
    };

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    int         gi_EOL;                 //!< End-of-Line Zeichen fuer die Outputdateien
    int         gi_Codec;               //!< Zeichenkodierung
    int         gi_Extension;           //!< Dateierweiterung fuer Ausgabedateien
    int         gi_ActionNumber;        //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.
    int         gi_NumOfParameterFiles; //!< Anzahl der Parameterdateien.
    int         gi_NumOfProgramStarts;  //!< Anzahl der Programmstarts

    QStringList gsl_FilenameList;       //!< Liste aller InputDateinamen incl. Pfad.
    QStringList gsl_FilenameOut;        //!< Liste aller OutputDateinamen incl. Pfad.
    QStringList gsl_Message;            //!< Liste von Nachrichten. Diese werden im Hauptfenster ausgegeben.

    QString     gs_FilenameFormat;      //!< Bestimmt in die Form des automatisch erzeugten neuen Dateinamens. \%a = aktuelle Actionnumber, \%N = Dateiname.
    QString     gs_Path;                //!< Der zuletzt verwendete Pfad.
    QString     gs_FilenamePDB;         //!< Name der Parameterdatenbank
    QString     gs_Version;             //!< Aktuelle Version des Programms.

    QString     gs_PI;
    QString     gs_Author;
    QString     gs_Source;
    QString     gs_Reference;
    QString     gs_Project;
    QString     gs_DatasetTitle;
    QString     gs_ExportFilename;
    QString     gs_DataSetComment;
    QString     gs_FurtherDetailsReference;
    QString     gs_FurtherDetailsDataset;
    QString     gs_OtherVersionReference;
    QString     gs_OtherVersionDataset;
    QString     gs_SourceReference;
    QString     gs_SourceDataset;
    QString     gs_User;
    QString     gs_Parent;

    int         gi_NumOfParametersInPDB;
    int         gi_MetadataFileMode;
    int         gi_TopologicType;
    int         gi_Status;
    int         gi_OutOfRangeValue;
    int         gi_Login;

    bool		gb_writeHeader;
    bool		gb_useParameterDatabase;
    bool		gb_splitFile;
    bool		gb_markSmallFile;
    bool		gb_makeFilenameUnique;
    bool		gb_overwriteDataset;
    bool		gb_useFilenameInAsEventLabel;
    bool		gb_writeParameterImportFile;
    bool		gb_createMetadataTemplate;
    bool        gb_mergeNewParameters;
    bool        gb_refreshOnStart;
    bool        gb_showParameterImportFileCreatedMessage;
    bool        gb_firstProgramStart;
    bool        gb_createAdditionMetadataOptions;

    bool isEmptyColumn( const QStringList& Input, const int ColumnNo );
    bool isNewEvent( const QStringList& Input, const int i );
    bool findEmptyColumns( const QStringList& Data, const int NumOfColumns, bool EmptyColumn[] );

    int resetDataList( const QStringList& Input, const int HeaderLine, const int DataLine, QStringList& Data );
    int isEmptyDataItem( const QString& Output, const int i, const int emptyDataItem );
    int readParameterDB( const QString& FilenamePDB, structParameter *Parameter );
    int createImportParameterFile( const QString& FilenameParameterImport );
    int readDataDescription( const QStringList& Input, QStringList& DataDescription, int& HeaderLine, int& DataLine );
    int readMetadataFile( const QString& FilenameMetadata, QStringList& Parameter, QStringList& DataSetComment, QStringList& FurtherDetailsReferenceList, QStringList& FurtherDetailsDatasetList, QStringList& OtherVersionReferenceList, QStringList& OtherVersionDatasetList );
    int readDataHeaderLine( const QString& Header, QStringList& ListParameter );
    int checkEventEntry( const QString &EventEntry );
    int getNumParameterQuerys();

    int findFormat( const QString& FilenameIn, const QStringList& Input, const int NumOfParameters, structPFormat Format[], const int NumOfFiles );

    QString findMethod( const QString& Comment );
    QString findParameterByName( const structParameter ParameterList[], const QString& Parameter );
    QString findParameterByID( const structParameter ParameterList[], const QString& Parameter );

    QString buildParameter( const QString& ParameterMF, const QString& EventLabel );
    QString buildParameterJSON( const QString& ParameterMF, const QString& EventLabel );

    QString createDir( const QString& Filename, const int NumOfFiles, const bool emtpyDir = true );
    QString buildOutputString( const QString& Data, const bool EmptyColumn[] );
    QString buildOutputString( const QString& Data );
    QString buildHeaderOutputString( const QString& Data );
    QString buildDataLine( const QStringList& Input, const int i );
    QString trimParameter( const QString& Parameter );
    QString createValidFilename( const QString& String );
    QString getMetadataFilename( const QString& firstFilenameIn, const int Extension, const int NumOfFiles );
    QString getParameterDDIFilename();
    QStringList addColumns( const QStringList& Input, const int NumOfColumns );

    int createMetadataTemplate(const QString& FilenameIn, const QString& FilenameMetadata, const QString& FilenameParameterImport,
                               const int Codec, const structParameter ParameterList[], const QString& PI,
                               const int MetadataFileMode, const bool createParameterImportFile, const bool createAdditionMetadataOptions,
                               const int NumOfFiles );

    int writeDataDescription( QIODevice *outDevice, const int Codec, const bool EmptyColumn[],
                              const QString& baseName, const QString& EventLabel, const QString& MinorLabel,
                              const QStringList& DSParameter, const QStringList& MFParameter,
                              const QStringList& DataSetCommentList,
                              const QStringList& FurtherDetailsReferenceList, const QStringList& FurtherDetailsDatasetList,
                              const QStringList& OtherVersionReferenceList, const QStringList& OtherVersionDatasetList,
                              const QStringList& SourceReferenceList, const QStringList& SourceDatasetList,
                              const QString& Author, const QString& Source, const QString& DatasetTitle,
                              const QString& ExportFilename, const QString& Reference,
                              const QString& Project, const QString& DataSetComment,
                              const QString& FurtherDetailsReference, const QString& FurtherDetailsDataset,
                              const QString& OtherVersionReference, const QString& OtherVersionDataset,
                              const QString& SourceReference, const QString& SourceDataset,
                              const QString& PI, const QString& User, const QString& Parent,
                              const int Status, const int Login, const bool useFilenameInAsEventLabel,
                              const int MetadataFileMode, const int TopologicType, const bool overwriteDataset );

    int writeDataDescriptionJSON( QIODevice *outDevice, const int Codec, const bool EmptyColumn[],
                              const QString& baseName, const QString& EventLabel, const QString& MinorLabel,
                              const QStringList& DSParameter, const QStringList& MFParameter,
                              const QStringList& DataSetCommentList,
                              const QStringList& FurtherDetailsReferenceList, const QStringList& FurtherDetailsDatasetList,
                              const QStringList& OtherVersionReferenceList, const QStringList& OtherVersionDatasetList,
                              const QStringList& SourceReferenceList, const QStringList& SourceDatasetList,
                              const QString& Author, const QString& Source, const QString& DatasetTitle,
                              const QString& ExportFilename, const QString& Reference,
                              const QString& Project, const QString& DataSetComment,
                              const QString& FurtherDetailsReference, const QString& FurtherDetailsDataset,
                              const QString& OtherVersionReference, const QString& OtherVersionDataset,
                              const QString& SourceReference, const QString& SourceDataset,
                              const QString& PI, const QString& User, const QString& Parent,
                              const int Status, const int Login, const bool useFilenameInAsEventLabel, const bool hasManyEvents,
                              const int MetadataFileMode, const int TopologicType, const bool overwriteDataset );

    int writeDataHeader( QIODevice *outDevice, const int Codec, const QStringList& Data, const QStringList& MFParameter,
                         const int MetadataFileMode, const bool EmptyColumn[], double Factor[], double  RangeMin[],
                         double RangeMax[], int Digits[], QString defaultValue[], const QString& EventHeader );

    int writeData( QIODevice *outDevice, const int Codec, const QStringList& Data,
                   const bool hasManyEvents, const bool EmptyColumn[], const bool hasEmptyColumn,
                   const int NumOfSavedDataLines, const int firstLine, const int NumOfFiles );

    int writeData( QIODevice *outDevice, const int Codec, const QStringList& Data,
                   const bool hasManyEvents, const bool EmptyColumn[],
                   double Factor[], double RangeMin[], double RangeMax[], int Digits[], QString defaultValue[],
                   const int NumOfSavedDataLines, const int OutOfRangeValue, const int NumOfFiles );

    int writeDataImportFile( const QString& FilenameIn, const QString& OutputPath, const int Codec, const int Extension, const QStringList& Data,
                             const QStringList& DSParameter, const QStringList& MFParameter,
                             const QStringList& DataSetCommentList,
                             const QStringList& FurtherDetailsReferenceList, const QStringList& FurtherDetailsDatasetList,
                             const QStringList& OtherVersionReferenceList, const QStringList& OtherVersionDatasetList,
                             const QStringList& SourceReferenceList, const QStringList& SourceDatasetList,
                             const QString& Author, const QString& Source, const QString& DatasetTitle,
                             const QString& ExportFilename, const QString& Reference,
                             const QString& Project, const QString& DataSetComment,
                             const QString& FurtherDetailsReference, const QString& FurtherDetailsDataset,
                             const QString& OtherVersionReference, const QString& OtherVersionDataset,
                             const QString& SourceReference, const QString& SourceDataset,
                             const QString& PI, const QString& User, const QString& Parent, const int Status, const int Login,
                             const bool writeHeader, const bool splitFile, const bool useFilenameInAsEventLabel, const bool makeFilenameUnique,
                             const bool hasManyEvents, const int MetadataFileMode, const int TopologicType, const bool overwriteDataset,
                             const bool markSmallFile, const int NumOfSavedDataLines, const int OutOfRangeValue, const int NumOfFiles );

    int Split2Events( const QString& FilenameIn, const int Codec, const int Extension,
                      const QStringList& ParameterList, const QStringList& DataSetCommentList,
                      const QStringList& FurtherDetailsReferenceList, const QStringList& FurtherDetailsDatasetList,
                      const QStringList& OtherVersionReferenceList, const QStringList& OtherVersionDatasetList,
                      const QStringList& SourceReferenceList, const QStringList& SourceDatasetList,
                      const QString& Author, const QString& Source, const QString& DatasetTitle, const QString& ExportFilename,
                      const QString& Reference, const QString& Project, const QString& DataSetComment,
                      const QString& FurtherDetailsReference, const QString& FurtherDetailsDataset,
                      const QString& OtherVersionReference, const QString& OtherVersionDataset,
                      const QString& SourceReference, const QString& SourceDataset,
                      const QString& PI, const QString& User, const QString& s_Parent, const int Status, const int Login,
                      const bool writeHeader, const bool splitFile, const int OutOfRangeValue,
                      const bool useFilenameInAsEventLabel, const bool makeFilenameUnique,
                      const int MetadataFileMode, const int TopologicType, const bool overwriteDataset,
                      const bool markSmallFile, const int FileNo, const int NumOfFiles );

    int concatenateFiles( const QString& FilenameOut, const QStringList FilenameIn, const QString& ProgressMessage, const int SkipNFirstLines, const bool deleteOriginalFiles );

    QString OpenDataDescriptionHeader();
    QString CloseDataDescriptionHeader();
    QString ParentID( const QString& ParentID = "" );
    QString DataSetID( const QString& DataSetID = "" );
    QString AuthorIDs(const QString& AuthorIDs = "" );
    QString SourceID( const QString& SourceID = "" );
    QString DatasetTitle( const QString& DatasetTitel = "" );
    QString ReferenceID( const QString &ReferenceID, const int RelationTypeID, const QString &ReferenceType, const QString& EventLabel = "" );
    QString ExportFilename( const QString& ExportFilename = "" );
    QString EventLabel( const QString& EventLabel = "" );
    QString Parameter( const int ParameterID, const int PIID = 506, const int MethodID = 43, const QString& Format = "", const QString& Comment = "" );
    QString ParameterFirst( const int ParameterID, const int PIID = 506, const int MethodID = 43, const QString& Format = "", const QString& Comment = "" );
    QString ParameterLast( const int ParameterID, const int PIID = 506, const int MethodID = 43, const QString& Format = "", const QString& Comment = "" );
    QString DatasetComment( const QString& DatasetComment = "" );
    QString ProjectIDs( const QString& ProjectID = "" );
    QString TopologicTypeID( const int TopologicTypeID = 0 );
    QString StatusID( const int StatusID = 1 );
    QString UserIDs( const QString& UserIDs = "" );
    QString LoginID( const QString& LoginID = "" );

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
    void closeEvent( QCloseEvent *event );
    void onError( const int err );

signals:
    void finishedBuildFilelist( bool );

private slots:
    int  chooseFile();
    int  chooseFiles();
    int  chooseFolder( const QString& Folder = "" );
    int  saveFile();
    int  saveFileAs();
    void hideWindow();
    void newWindow();
    void displayHelp();
    void about();
    void clearStatusMessage();

    void doCreateMetadataTemplate();
    void doCreateParameterDB();
    void doMergeParameterDB();
    void doSettingsDialog();
    void doMetadataTemplateOptionsDialog();
    void doSplit2EventsDialog();
    void doSplit2Events();

private:
    QStringList expandCommandline();
    QStringList renameFiles( const QStringList Filename, const QString &searchStr, const QString &replaceStr );
    bool buildFilename( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QString &Filename, QString &FilenameIn, QString &FilenameOut );
    bool existsFirstFile( const int ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList );
    bool isEmptyLine( const QString &String );
    int NumOfSections( const QString &String );
    int buildFileList( QStringList &FilenameList, const QString &Filename, const bool extractZipFile = true );
    int calcFileSizeClass( const QString &FilenameIn, const int NumOfFiles, const int lowerLimit = 100, const int upperLimit = 1000 );
    int copyStringList( const QStringList InStringList, QStringList &OutStringList, const QString &s_Pattern = "*" );
    int emptyDir( const QString &Dir );
    int extractZipFile( const QString &Filename, const bool createNewDir = false, const bool delZipFile = false );
    int incFileProgress( const int NumOfFiles, const int FileNumber );
    int incProgress( const int NumOfFiles, const int Step );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const int NumOfFiles = 0, const qint64 Bytes = 0 );
    int removeFile( const QString &Filename );
    int downloadFile( const QString &Url, const QString &absoluteFilePath );
    unsigned int incProgress( const int NumOfFiles, const unsigned int filesize, const unsigned int length, const QString &InputStr );
    void appendItem( QStringList &List, const QString &Item, const QString &SS = "", const QString &RS = "" );
    void clearFilenameList( int &ActionNumber, QStringList &FilenameList );
    void clearList( QStringList& List );
    void clearMessage();
    void createActions();
    void createMenus();
    void createStatusBar();
    void endTool( const int err, const int stopProgress, int &ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList, const QString &doneMessage = "Done", const QString &canceledMessage = "Converter was canceled", const bool clearList = false, const bool incActionNumber = true );
    void initFileProgress( const int NumOfFiles, const QString &FilenameIn, const QString &MessageText );
    void initProgress( const int NumOfFiles, const QString &Filename, const QString &MessageText, const int totalNumberOfSteps );
    void initProgress( const int totalNumberOfSteps );
    void listDir( QStringList &Filename, const QString &Directory, const bool extractZipFile = true );
    void loadPreferences();
    void resetFileProgress( const int NumOfFiles );
    void resetProgress( const int NumOfFiles = 0 );
    void savePreferences();
    void setNormalCursor();
    void setStatusBar( const QString &Message = "", const int seconds = 0 );
    void setStatusBarFileInProgress( const QString &Filename, const QString &Message = "", const int i_seconds = 0 );
    void setWTitle( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &sl_FilenameList );
    void setWaitCursor();
    void showFilenameList( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void showList( const QStringList List );
    void showMessage( const QString &Message, QStringList &MessageList );
    void wait( const int msec );

    QString getApplicationName( const bool replaceUnderline = false );
    QString getDataLocation();
    QString getDocumentDir();
    QString getPreferenceFilename();
    QString getVersion();
    QString setEOLChar( const int EOL );
    QString setExtension( const int Extension );

    structParameter *gp_Parameter;

    QMenu	*fileMenu;
    QMenu	*toolsMenu;
    QMenu	*helpMenu;

    QAction *newWindowAction;
    QAction *openFileAction;
    QAction *openFolderAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *hideWindowAction;
    QAction *exitAction;
    QAction *helpAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QAction *Split2EventsAction;
    QAction *Split2EventsWithoutDialogAction;
    QAction *createMetadataTemplateAction;
    QAction *createMetadataTemplateWithoutDialogAction;
    QAction *createParameterDBAction;
    QAction *mergeParameterDBAction;
    QAction *setSettingsAction;

    QProgressDialog *ProgressDialog;
    QProgressDialog *FileProgressDialog;
    QProgressBar    *ProgressBar;
    QLabel          *StatusMessage;

    QStringListModel	*Model;
    QListView			*ListView;
    QTextEdit			*textViewer;

    QPoint	posWindow;			//!< Position des Programmfensters.
    QSize	sizeWindow;			//!< Groesse des Programmfensters.

    QPoint	posDialog;			//!< Position des Programmfensters.
    QSize	sizeDialog;			//!< Groesse des Programmfensters.
};

#endif
