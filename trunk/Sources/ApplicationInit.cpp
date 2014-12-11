/* ApplicationMainWindow.cpp  */
/* 2009-10-28                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Initalisierung des CentralWidget.
*
*   Am Beginn der Methode wird der Cursor auf den WaitCursor des Betriebssystems
*   gesetzt. Dann folgt die Intialisierung einiger globaler Variablen (ActionNumer, sl_FilenameIn,
*   sl_Message). Model und ListView werden erzeugt und das CentralWidget gesetzt. Nach dem Laden
*   der Programmeinstellungen werden die Menues erzeugt. Die Fensterposition und -groesse wird
*   gesetzt. Abschliessend wird das Fenster angezeigt und der Cursor zuruek auf den NormalCursor gesetzt.
*/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
//  QObject::connect( this, SIGNAL( finishedBuildFilelist( bool ) ), this, SLOT( doIt( bool ) ) );

// **********************************************************************************************

    setWaitCursor();

// **********************************************************************************************
// init

    gi_ActionNumber         = 0;

    gs_Version              = "unknown";

    gi_NumOfParametersInPDB = 0;
    gi_NumOfParameterFiles  = -999;

    gp_Parameter            = new structParameter[_MAX_NUM_OF_PARAMETERS_+1];

    gsl_FilenameList.clear();
    gsl_Message.clear();

// **********************************************************************************************

    Model		= new QStringListModel( this );
    ListView	= new QListView;

    ListView->setModel( Model );
    ListView->setEditTriggers( QAbstractItemView::NoEditTriggers );

    setCentralWidget( ListView );

    setAcceptDrops( true );

// **********************************************************************************************

    loadPreferences();

    if ( gi_NumOfProgramStarts++ < 1 )
        savePreferences();

    gs_Version = getVersion();

// **********************************************************************************************

    createActions();
    createMenus();
    createStatusBar();

// **********************************************************************************************

    if ( gb_mergeNewParameters == false )
      mergeParameterDBAction->setVisible( false );

    this->resize( sizeWindow );
    this->move( posWindow );
    this->show();

// **********************************************************************************************

    gsl_FilenameList = expandCommandline();

// **********************************************************************************************

    if ( ( gb_refreshOnStart == true ) || ( gb_firstProgramStart == true ) )
        doCreateParameterDB();

// **********************************************************************************************

    setWTitle( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );
    setStatusBar( tr( "Ready" ), 2 );
    setNormalCursor();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

MainWindow::~MainWindow()
{
}
