#ifndef _GLOBALS

    #define _GLOBALS

// Application constants
   #if defined(Q_OS_LINUX)
       const char eol = '\n';
   #endif

   #if defined(Q_OS_WIN)
       const char eol = '\n';
   #endif

   #if defined(Q_OS_MAC)
       const char eol = '\r';
   #endif

    const QString _PROGRAMDOI_           = "doi:10.1594/PANGAEA.835398";
    const QString _PROGRAMREADME_        = "http://wiki.pangaea.de/wiki/Split2Events";

    const QString _VERSION_              = "12.6";	// Version number, 2015-06-25
    const QString _PROVERSION_           = "Split2Event - Project - 2013-03-23";

    const int	_NOERROR_                = 0;
    const int	_ERROR_                  = 1;
    const int	_APPBREAK_               = 2;

    const int	_ZIP_                    = 1;
    const int	_GZIP_                   = 2;

// EOL
    const int   _UNIX_                   = 0;    // LF
    const int   _MACOS_                  = 1;    // CR
    const int   _WIN_                    = 2;    // CR+LF

// Encoding
    const int   _SYSTEM_                 = -1;   // System
    const int   _UTF8_                   = 0;    // UTF-8
    const int   _APPLEROMAN_             = 1;    // Apple Roman
    const int   _LATIN1_                 = 2;    // Latin-1 = ISO 8859-1

// Extension
    const int   _TXT_                    = 0;
    const int   _CSV_                    = 1;

// Constants
    const int	_UNRESTRICTED_           = 1; // unrestricted
    const int	_SIGNUPREQUIRED_         = 2; // signup required
    const int	_ACCESSRIGHTSNEEDED_     = 3; // access rights needed
    const int   _EVENT_                  = 4;

    const int	_IGNORE_                 = 0;
    const int	_REMOVE_                 = 1;
    const int	_BAD_                    = 2;
    const int	_QUESTIONABLE_           = 3;

    const int	_NOTVALIDATED_           = 0;
    const int	_VALIDATED_              = 1;
    const int	_PUBLISHED_              = 2;

    const int	_NOTSPECIFIED_           = 1;
    const int	_POINT_                  = 2;
    const int	_VERTICALPROFILE_        = 3;
    const int	_HORIZONTALPROFILE_      = 4;
    const int	_PROFILESERIES_          = 5;
    const int	_IRREGULARSURFACE_       = 6;
    const int	_GRIDDEDSURFACE_         = 7;
    const int	_TIMESERIES_             = 8;
    const int	_TIMESLICE_              = 9;
    const int	_GRIDDEDTIMESLICE_       = 10;
    const int	_UNCODEDTABLE_           = 11;

    const int	_NOTUSED_                = 0;
    const int	_AUTO_                   = 1;
    const int	_BYPOSITION_             = 2;
    const int	_BYNAME_                 = 3;

    const int	_MAX_NUM_OF_COLUMNS_     = 4000;	 // maximum number of columns to handle
    const int	_MAX_NUM_OF_PARAMETERS_  = 200000;   // maximum number of parameters to handle, changed 2011-11-10

    const int	_CREATEMETADATATEMPLATE_ = 99;

// Relation Types
    const int   _REPLACEDBY_             = 10;       // Replaced by
    const int   _SUPPLEMENTTO_           = 11;       // Supplement to
    const int   _RELATEDTO_              = 12;       // Related to = IsCitedBy
    const int   _OTHERVERSION_           = 13;       // Other version
    const int   _NEWVERSION_             = 14;       // New version
    const int   _ORIGINALVERSION_        = 15;       // Original version
    const int   _SOURCEDATASET_          = 16;       // Source data set
    const int   _FURTHERDETAILS_         = 17;       // Further details

#endif
