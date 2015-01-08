// ***********************************************************************************************
// *                                                                                             *
// * createMenu.cpp - creates application menus                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2008-11-29                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erstellen der Menue-Aktionen. */

void MainWindow::createActions()
{
// File menu
    newWindowAction = new QAction(tr("&New window"), this);
    newWindowAction->setShortcut(tr("Ctrl+N"));
    connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(chooseFiles()));

    openFolderAction = new QAction(tr("Select &Folder..."), this);
    openFolderAction->setShortcut(tr("Ctrl+F"));
    connect(openFolderAction, SIGNAL(triggered()), this, SLOT(chooseFolder()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    hideWindowAction = new QAction(tr("&Close window"), this);
    hideWindowAction->setShortcut(tr("Ctrl+W"));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hideWindow()));

    setSettingsAction = new QAction(tr("General options..."), this);
    connect(setSettingsAction, SIGNAL(triggered()), this, SLOT(doSettingsDialog()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Tools menu
    Split2EventsAction = new QAction(tr("&Split to events..."), this);
    Split2EventsAction->setShortcut(tr("F5"));
    connect(Split2EventsAction, SIGNAL(triggered()), this, SLOT(doSplit2EventsDialog()));

    Split2EventsWithoutDialogAction = new QAction(tr("&Split to events"), this);
    Split2EventsWithoutDialogAction->setShortcut(tr("F2"));
    connect(Split2EventsWithoutDialogAction, SIGNAL(triggered()), this, SLOT(doSplit2Events()));

    createMetadataTemplateAction = new QAction(tr("Create metadata &template..."), this);
    createMetadataTemplateAction->setShortcut(tr("F4"));
    connect(createMetadataTemplateAction, SIGNAL(triggered()), this, SLOT(doMetadataTemplateOptionsDialog()));

    createMetadataTemplateWithoutDialogAction = new QAction(tr("&Create metadata template"), this);
    createMetadataTemplateWithoutDialogAction->setShortcut(tr("F3"));
    connect(createMetadataTemplateWithoutDialogAction, SIGNAL(triggered()), this, SLOT(doCreateMetadataTemplate()));

    createParameterDBAction = new QAction(tr("Refresh &parameter database"), this);
    connect(createParameterDBAction, SIGNAL(triggered()), this, SLOT(doCreateParameterDB()));

    mergeParameterDBAction = new QAction(tr("&Merge new parameters to parameter database"), this);
    connect(mergeParameterDBAction, SIGNAL(triggered()), this, SLOT(doMergeParameterDB()));

    // Help menu
    aboutAction = new QAction(tr("&About ") + getApplicationName( true ), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    helpAction = new QAction(getApplicationName( true ) + tr(" &Help"), this);
    helpAction->setShortcut(tr("F1"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

#if defined(Q_OS_WIN)
    newWindowAction->setStatusTip(tr("Create a new file"));
    openFileAction->setStatusTip(tr("Choose an existing file"));
    openFolderAction->setStatusTip(tr("Choose an existing folder"));
    saveAction->setStatusTip(tr("Save the document to disk"));
    saveAsAction->setStatusTip(tr("Save the document under a new name"));
    setSettingsAction->setStatusTip(tr("Configuration of program"));
    exitAction->setStatusTip(tr("Exit the application"));
    Split2EventsAction->setStatusTip(tr("Split file(s) to events"));
    Split2EventsWithoutDialogAction->setStatusTip(tr("Split file(s) to events"));
    createMetadataTemplateAction->setStatusTip(tr("Create metadata template"));
    createMetadataTemplateWithoutDialogAction->setStatusTip(tr("Create metadata template without dialog"));
    createParameterDBAction->setStatusTip(tr("Download a new parameter database from www.pangaea.de"));
    mergeParameterDBAction->setStatusTip(tr("Merge new parameters to parameter database"));
    aboutAction->setStatusTip(tr("Show the application's About box"));
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    helpAction->setStatusTip(tr("Show the application's Help"));
#endif
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Verbindet Menues mit Aktionen. */

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( openFileAction );
    fileMenu->addAction( openFolderAction );
    fileMenu->addSeparator();

#if defined(Q_OS_LINUX)
    ;
#endif

#if defined(Q_OS_MAC)
    fileMenu->addAction( newWindowAction );
    newWindowAction->setEnabled( false );
    fileMenu->addAction( hideWindowAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addAction( hideWindowAction );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( setSettingsAction );

#if defined(Q_OS_LINUX)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

// **********************************************************************************************

    toolsMenu = menuBar()->addMenu( tr( "&Tools" ) );
    toolsMenu->addAction( Split2EventsAction );
    toolsMenu->addAction( Split2EventsWithoutDialogAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( createMetadataTemplateAction );
    toolsMenu->addAction( createMetadataTemplateWithoutDialogAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( createParameterDBAction );
    toolsMenu->addAction( mergeParameterDBAction );

// **********************************************************************************************

    helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
    helpMenu->addSeparator();
    helpMenu->addAction( helpAction );
}
