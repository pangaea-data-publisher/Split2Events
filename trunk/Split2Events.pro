# Split2Events - a software tool to split one file with data from several events into several files, one for each event
# Rainer Sieger
# Alfred Wegener Institute, Bremerhaven, Germany
# last change: 2014-10-04

macx {
    # creating cache file
    cache()

    # Set SDK
    QMAKE_MAC_SDK = macosx10.13 # Xcode 9 - 2017-09-20

    # Only Intel binaries are accepted so force this
    CONFIG += x86_64

    # Minimum OS X version for submission is 10.7
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    # Icon
    ICON = ./Resources/icon/Application.icns

    # Replace default Info.plist
    QMAKE_INFO_PLIST = ./Resources/Info.plist

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32 {
    # Icon
    RC_FILE = ./Resources/Application.rc
}

linux-g++ {
}

TARGET = Split2Events
TEMPLATE = app

QT += widgets network

INCLUDEPATH += ./Headers

HEADERS = ./Headers/Application.h \
    ./Headers/Globals.h \
    ./Forms/AboutDialog/AboutDialog.h \
    ./Forms/SettingsDialog/SettingsDialog.h \
    ./Forms/MetadataTemplateOptionsDialog/MetadataTemplateOptionsDialog.h \
    ./Forms/Split2EventsDialog/Split2EventsDialog.h

SOURCES = ./Sources/ApplicationMainWindow.cpp \
    ./Sources/ApplicationErrors.cpp \
    ./Sources/ApplicationPreferences.cpp \
    ./Sources/ApplicationCreateMenu.cpp \
    ./Sources/ApplicationInit.cpp \
    ./Sources/createParameterDB.cpp \
    ./Sources/concatenateFiles.cpp \
    ./Sources/createMetadataTemplate.cpp \
    ./Sources/createImportParameterFile.cpp \
    ./Sources/doSplit2Events.cpp \
    ./Sources/Split2Events.cpp \
    ./Sources/readMetadataFile.cpp \
    ./Sources/readParameterDB.cpp \
    ./Sources/readParameters.cpp \
    ./Sources/findFormat.cpp \
    ./Sources/findMethod.cpp \
    ./Sources/Split2EventsTools.cpp \
    ./Forms/AboutDialog/AboutDialog.cpp \
    ./Forms/MetadataTemplateOptionsDialog/MetadataTemplateOptionsDialog.cpp \
    ./Forms/SettingsDialog/SettingsDialog.cpp \
    ./Forms/Split2EventsDialog/Split2EventsDialog.cpp

FORMS = ./Forms/AboutDialog/aboutdialog.ui \
    ./Forms/SettingsDialog/settingsdialog.ui \
    ./Forms/MetadataTemplateOptionsDialog/metadatatemplateoptionsdialog.ui \
    ./Forms/Split2EventsDialog/split2eventsdialog.ui
