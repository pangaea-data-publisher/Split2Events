/* BSRN_ToolboxTools.cpp          */
/* 2007-11-07                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"

const QString PrefDate = "BSRN Toolbox, 2008-05-20";

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt die PANGAEA ID des Stationsleiters.
*
*   @param s_StationScientist Name des Stationsleiters
*   @param Staff_ptr Pointer auf Array aller Personen
*
*	@return ID der Person. -999 wird zurueckgegeben wenn keine ID gefunden wurde
*/

int BSRN_Toolbox::findPiId( const QString& s_StationScientist, structStaff *Staff_ptr )
{
	int i = 0;

	for ( i=1; i<=MAX_NUM_OF_STAFF; i++ )
	{
		if ( Staff_ptr[i].StationScientist == s_StationScientist )
			return( Staff_ptr[i].StaffID );
	}

	return ( -999 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt die PANGAEA ID der Methode.
*
*   @param i_StationNumber Nummer der Station
*   @param i_WRMCnumber WRMC Code des Geraetes = PANGAEA Methode
*   @param Method_ptr Pointer auf Array aller Methoden
*
*	@return ID der Methode. -999 wird zurueckgegeben wenn keine ID gefunden wurde
*/

int BSRN_Toolbox::findMethodID( const int i_StationNumber, const int i_WRMCnumber, structMethod *Method_ptr )
{
	int i = 0;

	for ( i=1; i<=MAX_NUM_OF_METHOD; i++ )
	{
		if ( ( Method_ptr[i].StationID == i_StationNumber ) && ( Method_ptr[i].WRMCnumber == i_WRMCnumber ) )
			return( Method_ptr[i].MethodID );
	}

	return( -999 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt die PANGAEA ID der Methode.
*
*   @param s_InstrumentIdentificationNumber Identifikationsnummer des Geraetes = PANGAEA Methode
*   @param Method_ptr Pointer auf Array aller Methoden
*
*	@return ID der Methode. -999 wird zurueckgegeben wenn keine ID gefunden wurde
*/

int BSRN_Toolbox::findMethodID( const QString& s_InstrumentIdentificationNumber, structMethod *Method_ptr )
{
	int	 i = 0;

	for ( i=1; i<=MAX_NUM_OF_METHOD; i++ )
	{
		if ( Method_ptr[i].SerialNumber == s_InstrumentIdentificationNumber ) 
			return( Method_ptr[i].MethodID );
	}

	return( -999 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt den Kurznamen der Station.
*
*   @param i_StationNumber Nummer der Station
*   @param Station_ptr Pointer auf Array aller Stationsnamen
*
*	@return Kurzname der Station. "unknown" wird zurueckgegeben wenn keine Station gefunden wurde
*/

QString BSRN_Toolbox::findEventLabel( const int i_StationNumber, structStation *Station_ptr )
{
	int i = 0;

	for ( i=1; i<=MAX_NUM_OF_STATION; i++ )
	{
		if ( Station_ptr[i].StationID == i_StationNumber )
			return( Station_ptr[i].EventLabel );
	}

	return ( "unknown" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt den Namen der Station.
*
*   @param i_StationNumber Nummer der Station
*   @param Station_ptr Pointer auf Array aller Stationsnamen
*
*	@return Name der Station. "unknown" wird zurueckgegeben wenn keine Station gefunden wurde
*/

QString BSRN_Toolbox::findStationName( const int i_StationNumber, structStation *Station_ptr )
{
	int		i				= 0;
	QString	s_StationName	= "";

	for ( i=1; i<=MAX_NUM_OF_STATION; i++ )
	{
		if ( Station_ptr[i].StationID == i_StationNumber )
		{
			s_StationName = Station_ptr[i].StationName;
			s_StationName.replace( "Ny-&Aring%lesund", "Ny-Ålesund" );
			s_StationName.replace( "S&atilde%o", "São" );
			return( s_StationName );
		}
	}

	return ( "unknown" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermittelt das Heimatinstitut der Station.
*
*   @param i_StationNumber Nummer der Station
*   @param Station_ptr Pointer auf Array aller Institute
*
*	@return ID des Instituts. -999 wird zurueckgegeben wenn kein Institut gefunden wurde
*/

int BSRN_Toolbox::findInstituteID( const int i_StationNumber, structStation *Station_ptr )
{
	int i = 0;

	for ( i=1; i<=MAX_NUM_OF_STATION; i++ )
	{
		if ( Station_ptr[i].StationID == i_StationNumber )
			return( Station_ptr[i].InstituteID );
	}

	return ( -999 );
}

// ***********************************************************************************************************************

/*! @brief Liest alle IDs aus einer Datei
*
*   @return Fehlercode, 0 = kein Fehler, -1 = keine Einstellungsdatei gefunden
*/

int BSRN_Toolbox::readIDs()
{
	int		i					= 0;
	int		err					= 0;

	QString InputStr			= "";
	QString PreferenceFilename	= "";
	QString IDsFilename			= "";
	QString CurrentDate			= "";

// **********************************************************************************************
    
	PreferenceFilename = getPreferenceFilename();

	if ( PreferenceFilename == "ERROR" )
		return( -1 );

// ***********************************************************************************************************************

	QFileInfo fi( PreferenceFilename );

 	IDsFilename = fi.absolutePath() + "/" + "BSRN_IDs.txt";

	QFile finIDs( IDsFilename );

	if ( finIDs.exists() == FALSE )
		err = writeDefaultIDs( IDsFilename );

	if ( finIDs.open( QIODevice::ReadOnly ) == FALSE )
	    return( -30 );

	QTextStream tinIDs( &finIDs ); // tin.setEncoding( QTextStream::Latin1 );

	InputStr = tinIDs.readLine();
	InputStr = tinIDs.readLine();

	CurrentDate = InputStr.section( "\t", 0, 0 ).section( ", ", 1, 1 );

	if ( ( InputStr.startsWith( "BSRN Toolbox" ) == FALSE ) || ( CurrentDate < PrefDate.section( ", ", 1, 1 ) ) )
	{
		finIDs.close();
	
		err = writeDefaultIDs( IDsFilename );
		err = readIDs();

		QMessageBox::warning( this, getApplicationName(), tr( "Cannot download BSRN IDs file.\nThe default file ID will be used." ) );

		return( NOERROR );
	}

	InputStr = tinIDs.readLine();

	while ( tinIDs.atEnd() == FALSE )
	{
		if ( InputStr.startsWith( "[Station]" ) == TRUE )
		{
			InputStr = tinIDs.readLine(); i = 0;

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				// 1 \t ASP \t Alice Springs \t 2894

				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					Station_ptr[i].StationID	= InputStr.section( "\t", 0, 0 ).toInt();
					Station_ptr[i].EventLabel	= InputStr.section( "\t", 1, 1 );
					Station_ptr[i].StationName	= InputStr.section( "\t", 2, 2 );
					Station_ptr[i].InstituteID	= InputStr.section( "\t", 3, 3 ).toInt();
				}
			}
		}

		if ( InputStr.startsWith( "[Staff]" ) == TRUE )
		{
			InputStr = tinIDs.readLine(); i = 0;

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				// Ain Kallis \t 31684
				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					Staff_ptr[i].StationScientist	= InputStr.section( "\t", 0, 0 );
					Staff_ptr[i].StaffID			= InputStr.section( "\t", 1, 1 ).toInt();
				}
			}
		}

		if ( InputStr.startsWith( "[Expanded]" ) == TRUE )
		{
			InputStr = tinIDs.readLine(); i = 0;

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					Method_ptr[i].StationID		= -999;
					Method_ptr[i].SerialNumber	= InputStr.section( "\t", 0, 0 ).simplified();
					Method_ptr[i].WRMCnumber	= -999;
					Method_ptr[i].MethodID		= InputStr.section( "\t", 1, 1 ).toInt();
				}
			}
		}

		if ( InputStr.startsWith( "[Ozonesonde]" ) == TRUE )
		{
			InputStr = tinIDs.readLine(); // Header

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				// Beck Instruments, 91 \t 111111

				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					Method_ptr[i].StationID		= -999;
					Method_ptr[i].SerialNumber	= InputStr.section( "\t", 0, 0 ).simplified();
					Method_ptr[i].WRMCnumber	= -999;
					Method_ptr[i].MethodID		= InputStr.section( "\t", 1, 1 ).toInt();
				}
			}
		}

		if ( InputStr.startsWith( "[Radiosonde]" ) == TRUE )
		{
			InputStr = tinIDs.readLine();  // Header

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				// Vaisala DigiCora, RS80 \t 5435

				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					Method_ptr[i].StationID		= -999;
					Method_ptr[i].SerialNumber	= InputStr.section( "\t", 0, 0 ).simplified();
					Method_ptr[i].WRMCnumber	= -999;
					Method_ptr[i].MethodID		= InputStr.section( "\t", 1, 1 ).toInt();
				}
			}
		}

		if ( InputStr.startsWith( "[Methods]" ) == TRUE )
		{
			InputStr = tinIDs.readLine(); // Header

			while ( ( tinIDs.atEnd() == FALSE ) && ( InputStr.startsWith( "[" ) == FALSE ) )
			{
				// 1, SN 28987F3, WRMC No. 1004 \t 5435
				InputStr = tinIDs.readLine();

				if ( InputStr.startsWith( "[" ) == FALSE )
				{
					i++;

					InputStr.replace( "\t", "," );
					InputStr.replace( " WRMC No. ", "" );

					Method_ptr[i].StationID		= InputStr.section( ",", 0, 0 ).toInt();
					Method_ptr[i].SerialNumber	= InputStr.section( ",", 1, 1 ).simplified();
					Method_ptr[i].WRMCnumber	= InputStr.section( ",", 2, 2 ).toInt();
					Method_ptr[i].MethodID		= InputStr.section( ",", 3, 3 ).toInt();
				}
			}
		}

		if ( ( tinIDs.atEnd() == FALSE ) && ( i > 0 ) ) 
			InputStr = tinIDs.readLine();
	}

	finIDs.close();

	return( NOERROR );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erzeugt die ID Datenbank
*
*   @param Filename Dateiname der Datenbank
*
*   @return Fehlercode
*/

int BSRN_Toolbox::writeDefaultIDs( const QString& Filename )
{
	QFile fout( QDir::convertSeparators( Filename ) );

	if ( fout.open( QIODevice::WriteOnly | QIODevice::Text ) == FALSE )
		return( -20 );

//-----------------------------------------------------------------------------------------------------------------------

	QTextStream tout( &fout ); // tout.setEncoding( QTextStream::Latin1 );

	tout << "[Version]" << endl;
	tout << PrefDate << endl;

	tout << "[Station]" << endl;
	tout << "BSRN Station ID\tPANGAEA Event label\tFull name\tPANGAEA Institute ID" << endl;
	tout << "1\tASP\tAlice Springs\t2894" << endl;
	tout << "22\tBAR\tBarrow\t1809" << endl;
	tout << "24\tBER\tBermuda\t1809" << endl;
	tout << "28\tBIL\tBillings\t2876" << endl;
	tout << "32\tBON\tBondville\t2887" << endl;
	tout << "34\tBOS\tBoulder, SURFRAD\t2887" << endl;
	tout << "23\tBOU\tBoulder\t2887" << endl;
	tout << "53\tCAB\tCabauw\t1057" << endl;
	tout << "50\tCAM\tCamborne\t2886" << endl;
	tout << "10\tCAR\tCarpentras\t2879" << endl;
	tout << "39\tCLH\tChesapeake Light\t2891" << endl;
	tout << "40\tDAA\tDe Aar\t2897" << endl;
	tout << "2\tDAR\tDarwin\t2888" << endl;
	tout << "35\tDRA\tDesert Rock\t2887" << endl;
	tout << "27\tE13\tS. Great Plains\t2876" << endl;
	tout << "3\tFLO\tFlorianopolis\t2889" << endl;
	tout << "31\tFPE\tFort Peck\t2887" << endl;
	tout << "33\tGCR\tGoodwin Creek\t2887" << endl;
	tout << "13\tGVN\tNeumayer Station\t32" << endl;
	tout << "38\tILO\tIlorin\t2880" << endl;
	tout << "25\tKWA\tKwajalein\t1809" << endl;
	tout << "60\tLAU\tLauder\t2894" << endl;
	tout << "51\tLER\tLerwick\t2886" << endl;
	tout << "12\tLIN\tLindenberg\t2885" << endl;
	tout << "29\tMAN\tMomote\t2888" << endl;
	tout << "30\tNAU\tNauru Island\t2888" << endl;
	tout << "11\tNYA\tNy-&Aring%lesund\t33" << endl;
	tout << "63\tPAL\tPalaiseau Cedex\t2884" << endl;
	tout << "21\tPAY\tPayerne\t2878" << endl;
	tout << "36\tPSU\tRock Springs\t2887" << endl;
	tout << "5\tREG\tRegina\t2895" << endl;
	tout << "43\tSBO\tSede Boqer\t2881" << endl;
	tout << "70\tSMS\tS&atilde%o Martinho da Serra\t2901" << endl;
	tout << "41\tSOV\tSolar Village\t2882" << endl;
	tout << "26\tSPO\tSouth Pole\t1809" << endl;
	tout << "17\tSYO\tSyowa\t1673" << endl;
	tout << "42\tTAM\tTamanrasset\t2893" << endl;
	tout << "16\tTAT\tTateno\t2877" << endl;
	tout << "9\tTOR\tToravere\t2892" << endl;
	tout << "44\tXIA\tXianghe\t2875" << endl;
	tout << "[Staff]" << endl;
	tout << "Station Scientist\tPANGAEA Staff ID\t\t" << endl;
	tout << "Ain Kallis\t31684\t\t" << endl;
	tout << "B Forgan\t31644\t\t" << endl;
	tout << "David Halliwell\t31673\t\t" << endl;
	tout << "DR CHARLES LONG\t31662\t\t" << endl;
	tout << "DR DOUGLAS L. SISTERSON\t31648\t\t" << endl;
	tout << "DR JOHN DELUISI\t31650" << endl;
	tout << "Dr. A. Heimo\t31856" << endl;
	tout << "Dr. Andreas Herber\t30608" << endl;
	tout << "Dr. Gert Koenig-Langlo\t10396" << endl;
	tout << "Dr. Enio Bueno Pereira\t31919" << endl;
	tout << "Dr. Marion Maturilli\t31988" << endl;
	tout << "Dr. L. Vuilleumier\t31671" << endl;
	tout << "Dr. Naif Al-Abbadi\t31676" << endl;
	tout << "Dr. T. YAMANOUCHI\t31678" << endl;
	tout << "Dr. W. Knap\t31652" << endl;
	tout << "Ellsworth Dutton\t31646" << endl;
	tout << "Gregory L. Schuster\t31658" << endl;
	tout << "H. Naganuma\t31857" << endl;
	tout << "H. Shimura\t31858" << endl;
	tout << "K. BEHRENS\t31667" << endl;
	tout << "Martial Haeffelin\t31670" << endl;
	tout << "Mohamed Mimouni\t31680" << endl;
	tout << "Mr John OLIVIERI\t31656" << endl;
	tout << "Mr. Danie Esterhuyse\t31660" << endl;
	tout << "N. Ohkawara\t31682" << endl;
	tout << "P.Fishwick\t31654" << endl;
	tout << "Prof. Abraham Zangvil\t31674" << endl;
	tout << "Prof. Sergio Colle\t31663" << endl;
	tout << "T. O. Aro\t31665" << endl;
	tout << "T. Ueno\t31859" << endl;
	tout << "T.O. Aro\t31665" << endl;
	tout << "William L. Smith Jr.\t31860" << endl;
	tout << "Xiangao Xia\t31686" << endl;
	tout << "Y. Hirose\t31861" << endl;
	tout << "[Expanded]" << endl;
	tout << "Method text" << endl;
	tout << "Cloud base height is measured using the cloud ceilometer LD-WHX 137.1819.00/002\t6122" << endl;
	tout << "Cloud base height is measured using the laser ceilometer LD-40 137.2005.00\t6123" << endl;
	tout << "[Ozonesonde]" << endl;
	tout << "Manufacturer, Identification\tPANGAEA Method ID" << endl;
	tout << "Vaisala DigiCora, 01004\t6093" << endl;
	tout << "Vaisala DigiCora, 89002\t6093" << endl;
	tout << "Beck Instruments, 61\t6121" << endl;
	tout << "Beck Instruments, 80\t6119" << endl;
	tout << "Beck Instruments, 91\t6120" << endl;
	tout << "Dobson, 00101\t6117" << endl;
	tout << "Dobson, 101\t6117" << endl;
	tout << "Dobson, 11\t6118" << endl;
	tout << "Dobson, 001\t6117" << endl;
	tout << "Dobson, 1\t6117" << endl;
	tout << "[Radiosonde]" << endl;
	tout << "Manufacturer, Identification\tPANGAEA Method ID" << endl;
	tout << "Unknown manufacturer\t5041" << endl;
	tout << "METEOLABOR AG, SRSCH\t6087" << endl;
	tout << "Meisei, RS280\t6088" << endl;
	tout << "Meisei, RS291\t6089" << endl;
	tout << "VIZ, MARK2\t6090" << endl;
	tout << "VIZ/Sippican, 72387\t6091" << endl;
	tout << "Vaisala, RS90 \t6092" << endl;
	tout << "Vaisala DigiCora, RS92 \t6221" << endl;
	tout << "Vaisala DigiCora, 01004\t6093" << endl;
	tout << "Vaisala DigiCora, 89002\t6093" << endl;
	tout << "Vaisala DigiCora, RS80 \t6094" << endl;
	tout << "Vaisala Model RS80-15GH, XXXXX\t6095" << endl;
	tout << "Vaisala Model RS80-15LH, XXXXX\t6096" << endl;
	tout << "Vaisala RS80-57H, 72469\t6097" << endl;
	tout << "Vaisala RS80-57H, 74560\t6097" << endl;
	tout << "Vaisala----------------------, RS80L\t6098" << endl;
	tout << "Vaisala----------------------, RS92G\t6099" << endl;
	tout << "[Methods]" << endl;
	tout << "Station ID, Serial No., WRMC No.\tPANGAEA Method ID" << endl;
	tout << "1, SN 28987F3, WRMC No. 1004\t5435" << endl;
	tout << "1, SN 29075F3, WRMC No. 1005\t5436" << endl;
	tout << "1, SN 29076F3, WRMC No. 1006\t5437" << endl;
	tout << "1, SN 924021, WRMC No. 1002\t5432" << endl;
	tout << "1, SN 924048, WRMC No. 1001\t5433" << endl;
	tout << "1, SN 940042, WRMC No. 1003\t5434" << endl;
	tout << "2, SN 31293F3, WRMC No. 27154\t5534" << endl;
	tout << "2, SN 33386, WRMC No. 27756\t6030" << endl;
	tout << "2, SN 31300F3, WRMC No. 27391\t5777" << endl;
	tout << "2, SN 29914F3, WRMC No. 27025\t5480" << endl;
	tout << "2, SN 30084F3, WRMC No. 27328\t5748" << endl;
	tout << "2, SN 30168F3a, WRMC No. 27335\t5750" << endl;
	tout << "2, SN 31283F3, WRMC No. 27145\t5528" << endl;
	tout << "2, SN 31284F3, WRMC No. 27146\t5529" << endl;
	tout << "2, SN 31285F3, WRMC No. 27147\t5530" << endl;
	tout << "2, SN 31291F3, WRMC No. 27153\t5533" << endl;
	tout << "2, SN 31307F3, WRMC No. 27399\t5781" << endl;
	tout << "2, SN 31345E6, WRMC No. 27599\t5903" << endl;
	tout << "2, SN 31350E6a, WRMC No. 27604\t5908" << endl;
	tout << "2, SN 31389F3, WRMC No. 27406\t5783" << endl;
	tout << "2, SN 31390F3, WRMC No. 27407\t5784" << endl;
	tout << "2, SN 31875E6, WRMC No. 27620\t5918" << endl;
	tout << "2, SN 33057F3, WRMC No. 27425\t5817" << endl;
	tout << "2, SN 33058F3, WRMC No. 27426\t5818" << endl;
	tout << "2, SN 33061F3, WRMC No. 27429\t5820" << endl;
	tout << "2, SN 33386, WRMC No. 27756\t6030" << endl;
	tout << "2, SN 33387, WRMC No. 27757\t6031" << endl;
	tout << "2, SN 33551E6, WRMC No. 27622\t5921" << endl;
	tout << "2, SN 33574, WRMC No. 27225\t6033" << endl;
	tout << "2, SN 33703F3, WRMC No. 27186\t5546" << endl;
	tout << "2, SN 33704F3, WRMC No. 27187\t5547" << endl;
	tout << "3, SN 27909F3, WRMC No. 3005\t5698" << endl;
	tout << "3, SN 28024E6, WRMC No. 3004\t5862" << endl;
	tout << "3, SN 892724, WRMC No. 3003\t5578" << endl;
	tout << "3, SN 902820, WRMC No. 3001\t5579" << endl;
	tout << "5, SN 28054, WRMC No. 5001\t5863" << endl;
	tout << "5, SN 29002, WRMC No. 5002\t5870" << endl;
	tout << "5, SN 29105, WRMC No. 5104\t5724" << endl;
	tout << "5, SN 29106, WRMC No. 5102\t5725" << endl;
	tout << "5, SN 29313, WRMC No. 5103\t5730" << endl;
	tout << "5, SN 29315, WRMC No. 5101\t5731" << endl;
	tout << "5, SN 32616, WRMC No. 5106\t5810" << endl;
	tout << "5, SN 32617, WRMC No. 5105\t5811" << endl;
	tout << "5, SN 910012, WRMC No. 5203\t5610" << endl;
	tout << "5, SN 910018, WRMC No. 5204\t5611" << endl;
	tout << "5, SN 920063, WRMC No. 5202\t5615" << endl;
	tout << "5, SN 920065, WRMC No. 5201\t5616" << endl;
	tout << "5, SN 940069, WRMC No. 5003\t5934" << endl;
	tout << "5, SN 950249, WRMC No. 5209\t5625" << endl;
	tout << "5, SN 950255, WRMC No. 5207\t5626" << endl;
	tout << "5, SN 980470, WRMC No. 5205\t5644" << endl;
	tout << "5, SN 980481, WRMC No. 5208\t5645" << endl;
	tout << "5, SN 980482, WRMC No. 5206\t5646" << endl;
	tout << "9, SN 010868, WRMC No. 9021\t5606" << endl;
	tout << "9, SN 010870, WRMC No. 9027\t5607" << endl;
	tout << "9, SN 012042, WRMC No. 9019\t5971" << endl;
	tout << "9, SN 017441, WRMC No. 9020\t5566" << endl;
	tout << "9, SN 041338, WRMC No. 9036\t5608" << endl;
	tout << "9, SN 1, WRMC No. 9011\t6069" << endl;
	tout << "9, SN 1085, WRMC No. 9023\t5658" << endl;
	tout << "9, SN 1125, WRMC No. 9015\t5659" << endl;
	tout << "9, SN 1274, WRMC No. 9014\t5660" << endl;
	tout << "9, SN 16418, WRMC No. 9002\t5849" << endl;
	tout << "9, SN 2553, WRMC No. 9024\t5661" << endl;
	tout << "9, SN 26802F3, WRMC No. 9037\t5687" << endl;
	tout << "9, SN 2772, WRMC No. 9025\t5662" << endl;
	tout << "9, SN 28808F3, WRMC No. 9026\t5712" << endl;
	tout << "9, SN 295, WRMC No. 9007\t5663" << endl;
	tout << "9, SN 331, WRMC No. 9004\t5441" << endl;
	tout << "9, SN 554, WRMC No. 9008\t5664" << endl;
	tout << "9, SN 610, WRMC No. 9012\t5665" << endl;
	tout << "9, SN 661, WRMC No. 9013\t5666" << endl;
	tout << "9, SN 829, WRMC No. 9010\t5667" << endl;
	tout << "9, SN 856, WRMC No. 9009\t5668" << endl;
	tout << "9, SN 903301, WRMC No. 9005\t5587" << endl;
	tout << "9, SN 972, WRMC No. 9022\t5669" << endl;
	tout << "9, SN A-00353, WRMC No. 9016\t5972" << endl;
	tout << "10, SN 18060E6, WRMC No. 10001\t5858" << endl;
	tout << "10, SN 27594f3, WRMC No. 10005\t5694" << endl;
	tout << "10, SN 840687, WRMC No. 10002\t5572" << endl;
	tout << "10, SN 903272, WRMC No. 10008\t5586" << endl;
	tout << "10, SN 924458, WRMC No. 10006\t5598" << endl;
	tout << "10, SN 924459, WRMC No. 10007\t5599" << endl;
	tout << "10, SN 9474E6, WRMC No. 10015\t6192" << endl;
	tout << "11, SN 28058 E6, WRMC No. 13017\t5865" << endl;
	tout << "11, SN 28692 E6, WRMC No. 11005\t5867" << endl;
	tout << "11, SN 28858 F3, WRMC No. 11002\t5714" << endl;
	tout << "11, SN 28859 F3, WRMC No. 11003\t5715" << endl;
	tout << "11, SN 28895 F3, WRMC No. 11004\t5716" << endl;
	tout << "11, SN 28897 F3, WRMC No. 11001\t5717" << endl;
	tout << "11, SN 913704, WRMC No. 11010\t5589" << endl;
	tout << "11, SN 913707, WRMC No. 11014\t5590" << endl;
	tout << "11, SN 913708, WRMC No. 11011\t5591" << endl;
	tout << "11, SN 913709, WRMC No. 11015\t5592" << endl;
	tout << "11, SN 913711, WRMC No. 11009\t5593" << endl;
	tout << "11, SN 913712, WRMC No. 11016\t5594" << endl;
	tout << "12, SN 010824, WRMC No. 12009\t5602" << endl;
	tout << "12, SN 010826, WRMC No. 12010\t5603" << endl;
	tout << "12, SN 27701F3, WRMC No. 12005\t5697" << endl;
	tout << "12, SN 29344F3, WRMC No. 12003\t5734" << endl;
	tout << "12, SN 29536E6, WRMC No. 12004\t5873" << endl;
	tout << "12, SN 30474F3, WRMC No. 12006\t5755" << endl;
	tout << "12, SN 30477F3, WRMC No. 12008\t5756" << endl;
	tout << "12, SN 32802F3, WRMC No. 12011\t5813" << endl;
	tout << "12, SN 930095, WRMC No. 12001\t5617" << endl;
	tout << "12, SN 930096, WRMC No. 12002\t5618" << endl;
	tout << "12, SN 960129, WRMC No. 12007\t5939" << endl;
	tout << "12, SN 990220, WRMC No. 12012\t5949" << endl;
	tout << "13, SN 27600 F3, WRMC No. 13005\t5695" << endl;
	tout << "13, SN 27603 F3, WRMC No. 13006\t5696" << endl;
	tout << "13, SN 28057 E6, WRMC No. 13018\t5864" << endl;
	tout << "13, SN 28058 E6, WRMC No. 13017\t5865" << endl;
	tout << "13, SN 28150 F3, WRMC No. 13002\t5701" << endl;
	tout << "13, SN 28151 F3, WRMC No. 13003\t5702" << endl;
	tout << "13, SN 28152 F3, WRMC No. 13004\t5703" << endl;
	tout << "13, SN 28345 F3, WRMC No. 13001\t5704" << endl;
	tout << "13, SN 28694 E6, WRMC No. 11006\t5868" << endl;
	tout << "13, SN 29034 E6, WRMC No. 13021\t5872" << endl;
	tout << "13, SN 29327 F3, WRMC No. 13028\t5732" << endl;
	tout << "13, SN 29328 F3, WRMC No. 13026\t5733" << endl;
	tout << "13, SN 810127, WRMC No. 13023\t5567" << endl;
	tout << "13, SN 810144, WRMC No. 13022\t5568" << endl;
	tout << "13, SN 810150, WRMC No. 13030\t5569" << endl;
	tout << "13, SN 820258, WRMC No. 13029\t5570" << endl;
	tout << "13, SN 820259, WRMC No. 13029\t5571" << endl;
	tout << "13, SN 871804, WRMC No. 13024\t5575" << endl;
	tout << "13, SN 892440, WRMC No. 13027\t5576" << endl;
	tout << "13, SN 892441, WRMC No. 13025\t5577" << endl;
	tout << "13, SN 903243, WRMC No. 13012\t5580" << endl;
	tout << "13, SN 903244, WRMC No. 13013\t5581" << endl;
	tout << "13, SN 903245, WRMC No. 13014\t5582" << endl;
	tout << "13, SN 903246, WRMC No. 13007\t5583" << endl;
	tout << "13, SN 903247, WRMC No. 13008\t5584" << endl;
	tout << "13, SN 903248, WRMC No. 13009\t5585" << endl;
	tout << "16, SN 010048, WRMC No. 16023\t5650" << endl;
	tout << "16, SN 010567, WRMC No. 16025\t5827" << endl;
	tout << "16, SN 13539E6, WRMC No. 16006\t5837" << endl;
	tout << "16, SN 26662E6, WRMC No. 16007\t5861" << endl;
	tout << "16, SN 28849F3, WRMC No. 16008\t5713" << endl;
	tout << "16, SN 29460F3, WRMC No. 16009\t5735" << endl;
	tout << "16, SN 30700F3, WRMC No. 16010\t5766" << endl;
	tout << "16, SN 31239F3, WRMC No. 16016\t5774" << endl;
	tout << "16, SN 31714F3, WRMC No. 16021\t5793" << endl;
	tout << "16, SN 32205F3, WRMC No. 16022\t5803" << endl;
	tout << "16, SN 861161, WRMC No. 16004\t5573" << endl;
	tout << "16, SN 861164, WRMC No. 16005\t5574" << endl;
	tout << "16, SN 87057, WRMC No. 16020\t5950" << endl;
	tout << "16, SN 87058, WRMC No. 16018\t5951" << endl;
	tout << "16, SN 950093, WRMC No. 16011\t5936" << endl;
	tout << "16, SN 950236, WRMC No. 16017\t5624" << endl;
	tout << "16, SN 960330, WRMC No. 16013\t5627" << endl;
	tout << "16, SN 960331, WRMC No. 16014\t5628" << endl;
	tout << "16, SN 960332, WRMC No. 16015\t5629" << endl;
	tout << "16, SN 970366, WRMC No. 16024\t5637" << endl;
	tout << "16, SN 970423, WRMC No. 16019\t5639" << endl;
	tout << "16, SN F87021, WRMC No. 16001\t5997" << endl;
	tout << "16, SN F87022, WRMC No. 16002\t5998" << endl;
	tout << "16, SN F87023, WRMC No. 16003\t5999" << endl;
	tout << "16, SN F95127, WRMC No. 16012\t6000" << endl;
	tout << "16, SN 030641, WRMC No. 16032\t6193" << endl;
	tout << "16, SN 010582, WRMC No. 16026\t6194" << endl;
	tout << "17, SN 000738, WRMC No. 17029\t5601" << endl;
	tout << "17, SN 010565, WRMC No. 17031\t5826" << endl;
	tout << "17, SN 26403F3, WRMC No. 17012\t5686" << endl;
	tout << "17, SN 30431F3, WRMC No. 17013\t5754" << endl;
	tout << "17, SN 30788F3, WRMC No. 17014\t5768" << endl;
	tout << "17, SN 32032F3, WRMC No. 17023\t5797" << endl;
	tout << "17, SN 86023, WRMC No. 17004\t5993" << endl;
	tout << "17, SN 86024, WRMC No. 17005\t5994" << endl;
	tout << "17, SN 8631, WRMC No. 17002\t5989" << endl;
	tout << "17, SN 87001, WRMC No. 17008\t5990" << endl;
	tout << "17, SN 91022, WRMC No. 17006\t5995" << endl;
	tout << "17, SN 92009, WRMC No. 17009\t5991" << endl;
	tout << "17, SN 93005, WRMC No. 17010\t5992" << endl;
	tout << "17, SN 95046, WRMC No. 17007\t5996" << endl;
	tout << "17, SN 970397, WRMC No. 17022\t5638" << endl;
	tout << "17, SN 980520, WRMC No. 17025\t5647" << endl;
	tout << "17, SN 990001, WRMC No. 17028\t5830" << endl;
	tout << "17, SN 990198, WRMC No. 17027\t5948" << endl;
	tout << "17, SN 990574, WRMC No. 17026\t5648" << endl;
	tout << "21, SN 030077, WRMC No. 21051\t5653" << endl;
	tout << "21, SN 1495, WRMC No. 21038\t5964" << endl;
	tout << "21, SN 2842, WRMC No. 21031\t5965" << endl;
	tout << "21, SN 2843, WRMC No. 21032\t5966" << endl;
	tout << "21, SN 2844, WRMC No. 21033\t5967" << endl;
	tout << "21, SN 2849, WRMC No. 21037\t5968" << endl;
	tout << "21, SN 2849, WRMC No. 21050\t5960" << endl;
	tout << "21, SN 28676E6, WRMC No. 21003\t5866" << endl;
	tout << "21, SN 2873, WRMC No. 21036\t5969" << endl;
	tout << "21, SN 2874, WRMC No. 21048\t5961" << endl;
	tout << "21, SN 28806F3, WRMC No. 21047\t5709" << endl;
	tout << "21, SN 28807F3, WRMC No. 21008\t5710" << endl;
	tout << "21, SN 28808F3, WRMC No. 21009\t5711" << endl;
	tout << "21, SN 29586F3, WRMC No. 21019\t5736" << endl;
	tout << "21, SN 29587F3, WRMC No. 21022\t5737" << endl;
	tout << "21, SN 29588F3, WRMC No. 21021\t5738" << endl;
	tout << "21, SN 30333F3, WRMC No. 21010\t5751" << endl;
	tout << "21, SN 30962F3, WRMC No. 21011\t5773" << endl;
	tout << "21, SN 31961F3, WRMC No. 21045\t5794" << endl;
	tout << "21, SN 31965F3, WRMC No. 21044\t5795" << endl;
	tout << "21, SN 31966F3, WRMC No. 21046\t5796" << endl;
	tout << "21, SN 3548, WRMC No. 21030\t5953" << endl;
	tout << "21, SN 3550, WRMC No. 21034\t5970" << endl;
	tout << "21, SN 3550, WRMC No. 21049\t5962" << endl;
	tout << "21, SN 3551, WRMC No. 21052\t5963" << endl;
	tout << "21, SN 3549, WRMC No. 21053\t6125" << endl;
	tout << "21, SN 8124, WRMC No. 21012\t5442" << endl;
	tout << "21, SN 920039, WRMC No. 21005\t5612" << endl;
	tout << "21, SN 920041, WRMC No. 21004\t5613" << endl;
	tout << "21, SN 920042, WRMC No. 21006\t5614" << endl;
	tout << "21, SN 930040, WRMC No. 21035\t5931" << endl;
	tout << "21, SN 930108, WRMC No. 21020\t5619" << endl;
	tout << "21, SN 930109, WRMC No. 21024\t5620" << endl;
	tout << "21, SN 930110, WRMC No. 21023\t5621" << endl;
	tout << "21, SN 940215, WRMC No. 21043\t5622" << endl;
	tout << "21, SN 970426, WRMC No. 21042\t5640" << endl;
	tout << "21, SN 970428, WRMC No. 21041\t5641" << endl;
	tout << "22, SN 11079, WRMC No. 22010\t6001" << endl;
	tout << "22, SN 12263, WRMC No. 22002\t5446" << endl;
	tout << "22, SN 12266, WRMC No. 22007\t5448" << endl;
	tout << "22, SN 12618, WRMC No. 22005\t5454" << endl;
	tout << "22, SN 13156, WRMC No. 22001\t5836" << endl;
	tout << "22, SN 23216, WRMC No. 26003\t5677" << endl;
	tout << "22, SN 24328, WRMC No. 22004\t5679" << endl;
	tout << "22, SN 25985, WRMC No. 23006\t5683" << endl;
	tout << "22, SN 27406, WRMC No. 25003\t5689" << endl;
	tout << "22, SN 27407, WRMC No. 22003\t5690" << endl;
	tout << "22, SN 27454, WRMC No. 22008\t5692" << endl;
	tout << "22, SN 27455, WRMC No. 23002\t5693" << endl;
	tout << "22, SN 28139, WRMC No. 26007\t5700" << endl;
	tout << "22, SN 28595, WRMC No. 26006\t5707" << endl;
	tout << "22, SN 30556, WRMC No. 23021\t5762" << endl;
	tout << "22, SN 32870, WRMC No. 22009\t6011" << endl;
	tout << "22, SN 16319, WRMC No. 22011\t6191" << endl;
	tout << "22, SN 8024D1, WRMC No. 26021\t5549" << endl;
	tout << "22, SN 8030D1, WRMC No. 26015\t5552" << endl;
	tout << "22, SN 8050D1, WRMC No. 24007\t5565" << endl;
	tout << "23, SN 10153, WRMC No. 23030\t5444" << endl;
	tout << "23, SN 11049, WRMC No. 26013\t5445" << endl;
	tout << "23, SN 11755, WRMC No. 23023\t5833" << endl;
	tout << "23, SN 12265, WRMC No. 24004\t5447" << endl;
	tout << "23, SN 12272, WRMC No. 26005\t5451" << endl;
	tout << "23, SN 12287, WRMC No. 23015\t5452" << endl;
	tout << "23, SN 12560, WRMC No. 23005\t5453" << endl;
	tout << "23, SN 12619, WRMC No. 26012\t5455" << endl;
	tout << "23, SN 14857, WRMC No. 23024\t5839" << endl;
	tout << "23, SN 14860, WRMC No. 23019\t5456" << endl;
	tout << "23, SN 14861, WRMC No. 23026\t5457" << endl;
	tout << "23, SN 14888, WRMC No. 23011\t5459" << endl;
	tout << "23, SN 15131, WRMC No. 23001\t5461" << endl;
	tout << "23, SN 15197, WRMC No. 23017\t5462" << endl;
	tout << "23, SN 16521, WRMC No. 23003\t5850" << endl;
	tout << "23, SN 23215, WRMC No. 23014\t5676" << endl;
	tout << "23, SN 23396, WRMC No. 23029\t5860" << endl;
	tout << "23, SN 24328, WRMC No. 22004\t5679" << endl;
	tout << "23, SN 24333, WRMC No. 26004\t5680" << endl;
	tout << "23, SN 25985, WRMC No. 23006\t5683" << endl;
	tout << "23, SN 27173, WRMC No. 23012\t5688" << endl;
	tout << "23, SN 27406, WRMC No. 25003\t5689" << endl;
	tout << "23, SN 27407, WRMC No. 22003\t5690" << endl;
	tout << "23, SN 27415, WRMC No. 23007\t5691" << endl;
	tout << "23, SN 27454, WRMC No. 22008\t5692" << endl;
	tout << "23, SN 27455, WRMC No. 23002\t5693" << endl;
	tout << "23, SN 28138, WRMC No. 25005\t5699" << endl;
	tout << "23, SN 28945, WRMC No. 24005\t5718" << endl;
	tout << "23, SN 29949, WRMC No. 23018\t5886" << endl;
	tout << "23, SN 30108, WRMC No. 23020\t5484" << endl;
	tout << "23, SN 30556, WRMC No. 23021\t5762" << endl;
	tout << "23, SN 32598, WRMC No. 23027\t5808" << endl;
	tout << "23, SN 33161, WRMC No. 23028\t5821" << endl;
	tout << "23, SN 5496, WRMC No. 23010\t5924" << endl;
	tout << "23, SN 8021D1, WRMC No. 23025\t5548" << endl;
	tout << "23, SN 8026D1, WRMC No. 23009\t5550" << endl;
	tout << "23, SN 8030D1, WRMC No. 26015\t5552" << endl;
	tout << "23, SN 8040D1, WRMC No. 23008\t5557" << endl;
	tout << "23, SN 8045D1, WRMC No. 23022\t5561" << endl;
	tout << "23, SN 8046D1, WRMC No. 24002\t5562" << endl;
	tout << "23, SN 8048D1, WRMC No. 23016\t5563" << endl;
	tout << "23, SN 8049D1, WRMC No. 26016\t5564" << endl;
	tout << "23, SN 8050D1, WRMC No. 24007\t5565" << endl;
	tout << "24, SN 12265, WRMC No. 24004\t5447" << endl;
	tout << "24, SN 13911, WRMC No. 24009\t5838" << endl;
	tout << "24, SN 14857, WRMC No. 23024\t5839" << endl;
	tout << "24, SN 16522, WRMC No. 24010\t5851" << endl;
	tout << "24, SN 16524, WRMC No. 25001\t5852" << endl;
	tout << "24, SN 16526, WRMC No. 26010\t5854" << endl;
	tout << "24, SN 23216, WRMC No. 26003\t5677" << endl;
	tout << "24, SN 25838, WRMC No. 24001\t5682" << endl;
	tout << "24, SN 25985, WRMC No. 23006\t5683" << endl;
	tout << "24, SN 28595, WRMC No. 26006\t5707" << endl;
	tout << "24, SN 28945, WRMC No. 24005\t5718" << endl;
	tout << "24, SN 33329, WRMC No. 24012\t5822" << endl;
	tout << "24, SN 33367, WRMC No. 24011\t5987" << endl;
	tout << "24, SN 33368, WRMC No. 24014\t5988" << endl;
	tout << "24, SN 8026D1, WRMC No. 23009\t5550" << endl;
	tout << "24, SN 8028D1, WRMC No. 24006\t5551" << endl;
	tout << "24, SN 8033D1, WRMC No. 24013\t5553" << endl;
	tout << "24, SN 8037D1, WRMC No. 26017\t5555" << endl;
	tout << "24, SN 8040D1, WRMC No. 23008\t5557" << endl;
	tout << "24, SN 8041D1, WRMC No. 24003\t5558" << endl;
	tout << "24, SN 8042D1, WRMC No. 26014\t5559" << endl;
	tout << "24, SN 8043D1, WRMC No. 24008\t5560" << endl;
	tout << "24, SN 8046D1, WRMC No. 24002\t5562" << endl;
	tout << "24, SN 8050D1, WRMC No. 24007\t5565" << endl;
	tout << "25, SN 11914, WRMC No. 25008\t5834" << endl;
	tout << "25, SN 11948, WRMC No. 25006\t5835" << endl;
	tout << "25, SN 12560, WRMC No. 23005\t5453" << endl;
	tout << "25, SN 15348, WRMC No. 25004\t5463" << endl;
	tout << "25, SN 15952, WRMC No. 25010\t5464" << endl;
	tout << "25, SN 15953, WRMC No. 25014\t5465" << endl;
	tout << "25, SN 16139, WRMC No. 25011\t5843" << endl;
	tout << "25, SN 16521, WRMC No. 23003\t5850" << endl;
	tout << "25, SN 16524, WRMC No. 25001\t5852" << endl;
	tout << "25, SN 21408, WRMC No. 25009\t5471" << endl;
	tout << "25, SN 21409, WRMC No. 25007\t5472" << endl;
	tout << "25, SN 27406, WRMC No. 25003\t5689" << endl;
	tout << "25, SN 27415, WRMC No. 23007\t5691" << endl;
	tout << "25, SN 28138, WRMC No. 25005\t5699" << endl;
	tout << "25, SN 30039, WRMC No. 25012\t5930" << endl;
	tout << "25, SN 30108, WRMC No. 23020\t5484" << endl;
	tout << "25, SN 30555, WRMC No. 25022\t5761" << endl;
	tout << "25, SN 32592, WRMC No. 25015\t5805" << endl;
	tout << "25, SN 32600, WRMC No. 25021\t5809" << endl;
	tout << "25, SN 33816, WRMC No. 25024\t5922" << endl;
	tout << "25, SN 5496, WRMC No. 23010\t5924" << endl;
	tout << "25, SN 8035D1, WRMC No. 25023\t5554" << endl;
	tout << "25, SN 8037D1, WRMC No. 26017\t5555" << endl;
	tout << "25, SN 960112, WRMC No. 25016\t5937" << endl;
	tout << "25, SN 960113, WRMC No. 25013\t5938" << endl;
	tout << "26, SN 11049, WRMC No. 26013\t5445" << endl;
	tout << "26, SN 12269, WRMC No. 26002\t5449" << endl;
	tout << "26, SN 12270, WRMC No. 26008\t5450" << endl;
	tout << "26, SN 12619, WRMC No. 26012\t5455" << endl;
	tout << "26, SN 16526, WRMC No. 26010\t5854" << endl;
	tout << "26, SN 16529, WRMC No. 26001\t5855" << endl;
	tout << "26, SN 23215, WRMC No. 23014\t5676" << endl;
	tout << "26, SN 23216, WRMC No. 26003\t5677" << endl;
	tout << "26, SN 23896, WRMC No. 26019\t5678" << endl;
	tout << "26, SN 25510, WRMC No. 26018\t5476" << endl;
	tout << "26, SN 25985, WRMC No. 23006\t5683" << endl;
	tout << "26, SN 27407, WRMC No. 22003\t5690" << endl;
	tout << "26, SN 28138, WRMC No. 25005\t5699" << endl;
	tout << "26, SN 28139, WRMC No. 26007\t5700" << endl;
	tout << "26, SN 28595, WRMC No. 26006\t5707" << endl;
	tout << "26, SN 30556, WRMC No. 23021\t5762" << endl;
	tout << "26, SN 32600, WRMC No. 25021\t5809" << endl;
	tout << "26, SN 8024D1, WRMC No. 26021\t5549" << endl;
	tout << "26, SN 8037D1, WRMC No. 26017\t5555" << endl;
	tout << "26, SN 8039D1, WRMC No. 26020\t5556" << endl;
	tout << "26, SN 8041D1, WRMC No. 24003\t5558" << endl;
	tout << "26, SN 8042D1, WRMC No. 26014\t5559" << endl;
	tout << "26, SN 8043D1, WRMC No. 24008\t5560" << endl;
	tout << "26, SN 8045D1, WRMC No. 23022\t5561" << endl;
	tout << "26, SN 8046D1, WRMC No. 24002\t5562" << endl;
	tout << "27, SN 29001E6, WRMC No. 27551\t5869" << endl;
	tout << "27, SN 29554E6, WRMC No. 27558\t5874" << endl;
	tout << "27, SN 29608F3, WRMC No. 27011\t5477" << endl;
	tout << "27, SN 29741E6, WRMC No. 27566\t5876" << endl;
	tout << "27, SN 29852E6, WRMC No. 27573\t5879" << endl;
	tout << "27, SN 29936E6, WRMC No. 27580\t5883" << endl;
	tout << "27, SN 30009F3, WRMC No. 27319\t5740" << endl;
	tout << "27, SN 30358F3, WRMC No. 27430\t5753" << endl;
	tout << "27, SN 30585E6, WRMC No. 27586\t5894" << endl;
	tout << "27, SN 30668F3, WRMC No. 27049\t5500" << endl;
	tout << "27, SN 30671F3, WRMC No. 27051\t5502" << endl;
	tout << "27, SN 30682F3, WRMC No. 27347\t5763" << endl;
	tout << "27, SN 30683F3, WRMC No. 27348\t5764" << endl;
	tout << "27, SN 30709F3, WRMC No. 27056\t5503" << endl;
	tout << "27, SN 30720E6, WRMC No. 27590\t5896" << endl;
	tout << "27, SN 30723E6, WRMC No. 27593\t5898" << endl;
	tout << "27, SN 30776F3, WRMC No. 27058\t5504" << endl;
	tout << "27, SN 30784F3, WRMC No. 27433\t5767" << endl;
	tout << "27, SN 30799F3, WRMC No. 27065\t5505" << endl;
	tout << "27, SN 30802F3, WRMC No. 27066\t5506" << endl;
	tout << "27, SN 30828F3, WRMC No. 27431\t5769" << endl;
	tout << "27, SN 30891F3, WRMC No. 27082\t5509" << endl;
	tout << "27, SN 30897F3, WRMC No. 27085\t5510" << endl;
	tout << "27, SN 30899F3, WRMC No. 27086\t5511" << endl;
	tout << "27, SN 30901F3, WRMC No. 27088\t5512" << endl;
	tout << "27, SN 30940F3, WRMC No. 27096\t5513" << endl;
	tout << "27, SN 30943F3, WRMC No. 27098\t5514" << endl;
	tout << "27, SN 30944F3, WRMC No. 27099\t5515" << endl;
	tout << "27, SN 30947F3, WRMC No. 27102\t5516" << endl;
	tout << "27, SN 30951F3, WRMC No. 27106\t5517" << endl;
	tout << "27, SN 30954F3, WRMC No. 27109\t5519" << endl;
	tout << "27, SN 30958F3, WRMC No. 27113\t5520" << endl;
	tout << "27, SN 31096F3, WRMC No. 27115\t5521" << endl;
	tout << "27, SN 31625F3, WRMC No. 27158\t5538" << endl;
	tout << "27, SN 31630F3, WRMC No. 27163\t5540" << endl;
	tout << "27, SN 31747E6, WRMC No. 27612\t5913" << endl;
	tout << "27, SN 32972, WRMC No. 27705\t6012" << endl;
	tout << "27, SN 33236, WRMC No. 27710\t6014" << endl;
	tout << "27, SN 33238, WRMC No. 27712\t6015" << endl;
	tout << "27, SN 33241, WRMC No. 27715\t6016" << endl;
	tout << "27, SN 33242, WRMC No. 27716\t6017" << endl;
	tout << "27, SN 33247, WRMC No. 27719\t6020" << endl;
	tout << "27, SN 33275, WRMC No. 27739\t6025" << endl;
	tout << "28, SN 14862F3, WRMC No. 27000\t5458" << endl;
	tout << "28, SN 14894F3, WRMC No. 27185\t5460" << endl;
	tout << "28, SN 16332E6a, WRMC No. 27619\t5847" << endl;
	tout << "28, SN 21096, WRMC No. 27849\t6002" << endl;
	tout << "28, SN 28630F3, WRMC No. 27300\t5708" << endl;
	tout << "28, SN 29011E6, WRMC No. 27555\t5871" << endl;
	tout << "28, SN 29554E6, WRMC No. 27558\t5874" << endl;
	tout << "28, SN 29578E6, WRMC No. 27562\t5875" << endl;
	tout << "28, SN 29608F3, WRMC No. 27011\t5477" << endl;
	tout << "28, SN 29617F3, WRMC No. 27018\t5478" << endl;
	tout << "28, SN 29743E6, WRMC No. 27568\t5877" << endl;
	tout << "28, SN 29850E6, WRMC No. 27571\t5878" << endl;
	tout << "28, SN 29935E6, WRMC No. 27579\t5882" << endl;
	tout << "28, SN 30357F3, WRMC No. 27432\t5752" << endl;
	tout << "28, SN 30615F3, WRMC No. 27031\t5496" << endl;
	tout << "28, SN 30617F3, WRMC No. 27032\t5497" << endl;
	tout << "28, SN 30664F3, WRMC No. 27045\t5498" << endl;
	tout << "28, SN 30665F3, WRMC No. 27046\t5499" << endl;
	tout << "28, SN 30668F3, WRMC No. 27049\t5500" << endl;
	tout << "28, SN 30670F3, WRMC No. 27050\t5501" << endl;
	tout << "28, SN 30687F3, WRMC No. 27352\t5765" << endl;
	tout << "28, SN 30709F3, WRMC No. 27056\t5503" << endl;
	tout << "28, SN 30718E6, WRMC No. 27588\t5895" << endl;
	tout << "28, SN 30720E6, WRMC No. 27590\t5896" << endl;
	tout << "28, SN 30721E6, WRMC No. 27591\t5897" << endl;
	tout << "28, SN 30824F3, WRMC No. 27076\t5507" << endl;
	tout << "28, SN 30826F3, WRMC No. 27078\t5508" << endl;
	tout << "28, SN 30828F3, WRMC No. 27431\t5769" << endl;
	tout << "28, SN 30953F3, WRMC No. 27108\t5518" << endl;
	tout << "28, SN 31096F3, WRMC No. 27115\t5521" << endl;
	tout << "28, SN 31343E6, WRMC No. 27597\t5902" << endl;
	tout << "28, SN 31387E6, WRMC No. 27609\t5912" << endl;
	tout << "28, SN 31626F3, WRMC No. 27159\t5539" << endl;
	tout << "28, SN 31633F3, WRMC No. 27166\t5541" << endl;
	tout << "28, SN 31636F3, WRMC No. 27169\t5542" << endl;
	tout << "28, SN 31747E6, WRMC No. 27612\t5913" << endl;
	tout << "28, SN 32972, WRMC No. 27705\t6012" << endl;
	tout << "28, SN 33247, WRMC No. 27719\t6020" << endl;
	tout << "28, SN 33267, WRMC No. 27732\t6022" << endl;
	tout << "28, SN 33273, WRMC No. 27737\t6024" << endl;
	tout << "28, SN 33278, WRMC No. 27741\t6026" << endl;
	tout << "28, SN 33283, WRMC No. 27743\t6027" << endl;
	tout << "29, SN 1899, WRMC No. 27852\t5954" << endl;
	tout << "29, SN 1900, WRMC No. 27853\t5955" << endl;
	tout << "29, SN 2866, WRMC No. 27855\t5956" << endl;
	tout << "29, SN 29868E6, WRMC No. 27576\t5880" << endl;
	tout << "29, SN 29869E6, WRMC No. 27577\t5881" << endl;
	tout << "29, SN 29913F3, WRMC No. 27024\t5479" << endl;
	tout << "29, SN 29914F3, WRMC No. 27025\t5480" << endl;
	tout << "29, SN 29915F3, WRMC No. 27026\t5481" << endl;
	tout << "29, SN 29916F3, WRMC No. 27027\t5482" << endl;
	tout << "29, SN 29925F3, WRMC No. 27316\t5739" << endl;
	tout << "29, SN 29937E6, WRMC No. 27581\t5884" << endl;
	tout << "29, SN 30012F3, WRMC No. 27322\t5741" << endl;
	tout << "29, SN 30056F3, WRMC No. 27326\t5747" << endl;
	tout << "29, SN 31274F3, WRMC No. 27136\t5523" << endl;
	tout << "29, SN 31275F3, WRMC No. 27137\t5524" << endl;
	tout << "29, SN 31277F3, WRMC No. 27139\t5526" << endl;
	tout << "29, SN 31279F3, WRMC No. 27141\t5527" << endl;
	tout << "29, SN 31290F3, WRMC No. 27152\t5532" << endl;
	tout << "29, SN 31291F3, WRMC No. 27153\t5533" << endl;
	tout << "29, SN 31293F3, WRMC No. 27154\t5534" << endl;
	tout << "29, SN 31300F3, WRMC No. 27391\t5777" << endl;
	tout << "29, SN 31301F3, WRMC No. 27392\t5778" << endl;
	tout << "29, SN 31306F3, WRMC No. 27398\t5780" << endl;
	tout << "29, SN 31312F3, WRMC No. 27405\t5782" << endl;
	tout << "29, SN 31343E6, WRMC No. 27597\t5902" << endl;
	tout << "29, SN 31346E6, WRMC No. 27600\t5904" << endl;
	tout << "29, SN 31347E6, WRMC No. 27601\t5905" << endl;
	tout << "29, SN 31349E6, WRMC No. 27603\t5907" << endl;
	tout << "29, SN 31361E6, WRMC No. 27607\t5911" << endl;
	tout << "29, SN 31391F3, WRMC No. 27408\t5785" << endl;
	tout << "29, SN 32977, WRMC No. 27708\t6013" << endl;
	tout << "29, SN 33058F3, WRMC No. 27426\t5818" << endl;
	tout << "29, SN 33243, WRMC No. 27717\t6018" << endl;
	tout << "29, SN 33251, WRMC No. 27720\t6021" << endl;
	tout << "29, SN 33271, WRMC No. 27736\t6023" << endl;
	tout << "30, SN 1900, WRMC No. 27853\t5955" << endl;
	tout << "30, SN 2870, WRMC No. 27859\t5957" << endl;
	tout << "30, SN 29915F3, WRMC No. 27026\t5481" << endl;
	tout << "30, SN 29937E6, WRMC No. 27581\t5884" << endl;
	tout << "30, SN 30006F3, WRMC No. 27029\t5483" << endl;
	tout << "30, SN 30012F3, WRMC No. 27322\t5741" << endl;
	tout << "30, SN 30167F3, WRMC No. 27334\t5749" << endl;
	tout << "30, SN 31274F3, WRMC No. 27136\t5523" << endl;
	tout << "30, SN 31275F3, WRMC No. 27137\t5524" << endl;
	tout << "30, SN 31276F3, WRMC No. 27138\t5525" << endl;
	tout << "30, SN 31277F3, WRMC No. 27139\t5526" << endl;
	tout << "30, SN 31283F3, WRMC No. 27145\t5528" << endl;
	tout << "30, SN 31287F3, WRMC No. 27149\t5531" << endl;
	tout << "30, SN 31290F3, WRMC No. 27152\t5532" << endl;
	tout << "30, SN 31291F3, WRMC No. 27153\t5533" << endl;
	tout << "30, SN 31295F3, WRMC No. 27156\t5535" << endl;
	tout << "30, SN 31301F3, WRMC No. 27392\t5778" << endl;
	tout << "30, SN 31305F3, WRMC No. 27397\t5779" << endl;
	tout << "30, SN 31307F3, WRMC No. 27399\t5781" << endl;
	tout << "30, SN 31346E6, WRMC No. 27600\t5904" << endl;
	tout << "30, SN 31348E6, WRMC No. 27602\t5906" << endl;
	tout << "30, SN 31349E6, WRMC No. 27603\t5907" << endl;
	tout << "30, SN 31350E6a, WRMC No. 27604\t5908" << endl;
	tout << "30, SN 31351E6, WRMC No. 27605\t5909" << endl;
	tout << "30, SN 31352E6, WRMC No. 27606\t5910" << endl;
	tout << "30, SN 31389F3, WRMC No. 27406\t5783" << endl;
	tout << "30, SN 33060F3, WRMC No. 27428\t5819" << endl;
	tout << "30, SN 33251, WRMC No. 27720\t6021" << endl;
	tout << "30, SN 33271, WRMC No. 27736\t6023" << endl;
	tout << "30, SN 33386, WRMC No. 27756\t6030" << endl;
	tout << "30, SN 33575, WRMC No. 27758\t6034" << endl;
	tout << "30, SN 3873, WRMC No. 27861\t5958" << endl;
	tout << "30, SN 4433, WRMC No. 27862\t5959" << endl;
	tout << "31, SN 15543E6, WRMC No. 31251\t5840" << endl;
	tout << "31, SN 16333E6, WRMC No. 31258\t5848" << endl;
	tout << "31, SN 21381F3, WRMC No. 31153\t5675" << endl;
	tout << "31, SN 29256F3, WRMC No. 31163\t5727" << endl;
	tout << "31, SN 29258F3, WRMC No. 31165\t5729" << endl;
	tout << "31, SN 30015F3, WRMC No. 31166\t5742" << endl;
	tout << "31, SN 30016F3, WRMC No. 31167\t5743" << endl;
	tout << "31, SN 30395E6, WRMC No. 31265\t5891" << endl;
	tout << "31, SN 30399E6, WRMC No. 31267\t5893" << endl;
	tout << "31, SN 30551F3, WRMC No. 31171\t5757" << endl;
	tout << "31, SN 30552F3, WRMC No. 31172\t5758" << endl;
	tout << "31, SN 30553F3, WRMC No. 31173\t5759" << endl;
	tout << "31, SN 30726E6, WRMC No. 31268\t5899" << endl;
	tout << "31, SN 30727E6, WRMC No. 31269\t5900" << endl;
	tout << "31, SN 31608F3, WRMC No. 31182\t5790" << endl;
	tout << "31, SN 31609F3, WRMC No. 31183\t5791" << endl;
	tout << "31, SN 32865, WRMC No. 31305\t6008" << endl;
	tout << "31, SN 32909F3, WRMC No. 31185\t5814" << endl;
	tout << "31, SN 32910F3, WRMC No. 31186\t5815" << endl;
	tout << "31, SN 32911F3, WRMC No. 31187\t5816" << endl;
	tout << "31, SN 33381, WRMC No. 31312\t6029" << endl;
	tout << "31, SN 33489F3, WRMC No. 31188\t5823" << endl;
	tout << "31, SN 33490F3, WRMC No. 31189\t5824" << endl;
	tout << "31, SN 73-19, WRMC No. 31101\t6037" << endl;
	tout << "31, SN 73-23, WRMC No. 31102\t6038" << endl;
	tout << "31, SN 73-25, WRMC No. 31103\t6039" << endl;
	tout << "31, SN 73-38, WRMC No. 31106\t6042" << endl;
	tout << "31, SN 73-44, WRMC No. 31108\t6045" << endl;
	tout << "31, SN 73-54, WRMC No. 31110\t6049" << endl;
	tout << "31, SN 73-55, WRMC No. 31111\t6050" << endl;
	tout << "31, SN 73-59, WRMC No. 31112\t6051" << endl;
	tout << "31, SN 73-60, WRMC No. 31113\t6052" << endl;
	tout << "31, SN 73-80, WRMC No. 31118\t6057" << endl;
	tout << "31, SN 73-82, WRMC No. 31119\t6058" << endl;
	tout << "31, SN 73-83, WRMC No. 31120\t6059" << endl;
	tout << "31, SN 73-84, WRMC No. 31121\t6060" << endl;
	tout << "31, SN 73-85, WRMC No. 31122\t6061" << endl;
	tout << "31, SN 73-87, WRMC No. 31123\t6062" << endl;
	tout << "31, SN 73-88, WRMC No. 31124\t6063" << endl;
	tout << "31, SN 73-89, WRMC No. 31125\t6064" << endl;
	tout << "31, SN 73-93, WRMC No. 31145\t6065" << endl;
	tout << "31, SN 73-98, WRMC No. 31128\t6067" << endl;
	tout << "31, SN 73-99, WRMC No. 31129\t6068" << endl;
	tout << "31, SN 930701, WRMC No. 31427\t5975" << endl;
	tout << "31, SN 930805, WRMC No. 31428\t5976" << endl;
	tout << "31, SN 930812, WRMC No. 31430\t5978" << endl;
	tout << "31, SN 941212, WRMC No. 31437\t5981" << endl;
	tout << "31, SN 950207, WRMC No. 31439\t5982" << endl;
	tout << "31, SN 970812, WRMC No. 31442\t5983" << endl;
	tout << "31, SN 970818, WRMC No. 31443\t5984" << endl;
	tout << "31, SN 970819, WRMC No. 31444\t5985" << endl;
	tout << "32, SN 15545E6, WRMC No. 31253\t5842" << endl;
	tout << "32, SN 16230E6, WRMC No. 31254\t5844" << endl;
	tout << "32, SN 16332E6, WRMC No. 31257\t5846" << endl;
	tout << "32, SN 16333E6, WRMC No. 31258\t5848" << endl;
	tout << "32, SN 16658E6, WRMC No. 31261\t5856" << endl;
	tout << "32, SN 16665E6, WRMC No. 31262\t5857" << endl;
	tout << "32, SN 29255F3, WRMC No. 31162\t5726" << endl;
	tout << "32, SN 29256F3, WRMC No. 31163\t5727" << endl;
	tout << "32, SN 29257F3, WRMC No. 31164\t5728" << endl;
	tout << "32, SN 29258F3, WRMC No. 31165\t5729" << endl;
	tout << "32, SN 30017F3, WRMC No. 31168\t5744" << endl;
	tout << "32, SN 30018F3, WRMC No. 31169\t5745" << endl;
	tout << "32, SN 30019F3, WRMC No. 31170\t5746" << endl;
	tout << "32, SN 30395E6, WRMC No. 31265\t5891" << endl;
	tout << "32, SN 30398E6, WRMC No. 31266\t5892" << endl;
	tout << "32, SN 30551F3, WRMC No. 31171\t5757" << endl;
	tout << "32, SN 30553F3, WRMC No. 31173\t5759" << endl;
	tout << "32, SN 30554F3, WRMC No. 31174\t5760" << endl;
	tout << "32, SN 30918F3, WRMC No. 31179\t5772" << endl;
	tout << "32, SN 31270F3, WRMC No. 31181\t5776" << endl;
	tout << "32, SN 32861, WRMC No. 31302\t6005" << endl;
	tout << "32, SN 33565, WRMC No. 31329\t6032" << endl;
	tout << "32, SN 73-18, WRMC No. 31100\t6036" << endl;
	tout << "32, SN 73-19, WRMC No. 31101\t6037" << endl;
	tout << "32, SN 73-23, WRMC No. 31102\t6038" << endl;
	tout << "32, SN 73-31, WRMC No. 31104\t6040" << endl;
	tout << "32, SN 73-37, WRMC No. 31105\t6041" << endl;
	tout << "32, SN 73-38, WRMC No. 31106\t6042" << endl;
	tout << "32, SN 73-46, WRMC No. 31136\t6046" << endl;
	tout << "32, SN 73-48, WRMC No. 31138\t6047" << endl;
	tout << "32, SN 73-50, WRMC No. 31109\t6048" << endl;
	tout << "32, SN 73-54, WRMC No. 31110\t6049" << endl;
	tout << "32, SN 73-59, WRMC No. 31112\t6051" << endl;
	tout << "32, SN 73-83, WRMC No. 31120\t6059" << endl;
	tout << "32, SN 73-84, WRMC No. 31121\t6060" << endl;
	tout << "32, SN 73-87, WRMC No. 31123\t6062" << endl;
	tout << "32, SN 73-89, WRMC No. 31125\t6064" << endl;
	tout << "32, SN 73-98, WRMC No. 31128\t6067" << endl;
	tout << "32, SN 73-99, WRMC No. 31129\t6068" << endl;
	tout << "32, SN 930502, WRMC No. 31426\t5974" << endl;
	tout << "32, SN 930701, WRMC No. 31427\t5975" << endl;
	tout << "32, SN 930806, WRMC No. 31429\t5977" << endl;
	tout << "32, SN 930820, WRMC No. 31431\t5979" << endl;
	tout << "32, SN 940301, WRMC No. 31432\t5980" << endl;
	tout << "32, SN 950207, WRMC No. 31439\t5982" << endl;
	tout << "32, SN 970812, WRMC No. 31442\t5983" << endl;
	tout << "32, SN 970818, WRMC No. 31443\t5984" << endl;
	tout << "32, SN 970819, WRMC No. 31444\t5985" << endl;
	tout << "32, SN 990509, WRMC No. 31445\t5986" << endl;
	tout << "33, SN 15543E6, WRMC No. 31251\t5840" << endl;
	tout << "33, SN 16331E6, WRMC No. 31256\t5845" << endl;
	tout << "33, SN 16332E6, WRMC No. 31257\t5846" << endl;
	tout << "33, SN 21342F3, WRMC No. 31151\t5673" << endl;
	tout << "33, SN 21343F3, WRMC No. 31152\t5674" << endl;
	tout << "33, SN 28495F3, WRMC No. 31157\t5705" << endl;
	tout << "33, SN 28496F3, WRMC No. 31158\t5706" << endl;
	tout << "33, SN 29256F3, WRMC No. 31163\t5727" << endl;
	tout << "33, SN 30015F3, WRMC No. 31166\t5742" << endl;
	tout << "33, SN 30016F3, WRMC No. 31167\t5743" << endl;
	tout << "33, SN 30398E6, WRMC No. 31266\t5892" << endl;
	tout << "33, SN 30399E6, WRMC No. 31267\t5893" << endl;
	tout << "33, SN 30551F3, WRMC No. 31171\t5757" << endl;
	tout << "33, SN 30552F3, WRMC No. 31172\t5758" << endl;
	tout << "33, SN 30726E6, WRMC No. 31268\t5899" << endl;
	tout << "33, SN 30915F3, WRMC No. 31177\t5770" << endl;
	tout << "33, SN 30917F3, WRMC No. 31178\t5771" << endl;
	tout << "33, SN 31269F3, WRMC No. 31180\t5775" << endl;
	tout << "33, SN 31270F3, WRMC No. 31181\t5776" << endl;
	tout << "33, SN 31852E6, WRMC No. 31270\t5915" << endl;
	tout << "33, SN 31881E6, WRMC No. 31273\t5919" << endl;
	tout << "33, SN 32860, WRMC No. 31301\t6004" << endl;
	tout << "33, SN 32863, WRMC No. 31303\t6006" << endl;
	tout << "33, SN 32867, WRMC No. 31307\t6010" << endl;
	tout << "33, SN 32909F3, WRMC No. 31185\t5814" << endl;
	tout << "33, SN 32911F3, WRMC No. 31187\t5816" << endl;
	tout << "33, SN 73-18, WRMC No. 31100\t6036" << endl;
	tout << "33, SN 73-23, WRMC No. 31102\t6038" << endl;
	tout << "33, SN 73-25, WRMC No. 31103\t6039" << endl;
	tout << "33, SN 73-41, WRMC No. 31107\t6043" << endl;
	tout << "33, SN 73-44, WRMC No. 31108\t6045" << endl;
	tout << "33, SN 73-50, WRMC No. 31109\t6048" << endl;
	tout << "33, SN 73-54, WRMC No. 31110\t6049" << endl;
	tout << "33, SN 73-55, WRMC No. 31111\t6050" << endl;
	tout << "33, SN 73-66, WRMC No. 31116\t6055" << endl;
	tout << "33, SN 73-70, WRMC No. 31117\t6056" << endl;
	tout << "33, SN 73-80, WRMC No. 31118\t6057" << endl;
	tout << "33, SN 73-82, WRMC No. 31119\t6058" << endl;
	tout << "33, SN 73-88, WRMC No. 31124\t6063" << endl;
	tout << "33, SN 73-89, WRMC No. 31125\t6064" << endl;
	tout << "33, SN 73-99, WRMC No. 31129\t6068" << endl;
	tout << "33, SN 930502, WRMC No. 31426\t5974" << endl;
	tout << "33, SN 930701, WRMC No. 31427\t5975" << endl;
	tout << "33, SN 930812, WRMC No. 31430\t5978" << endl;
	tout << "33, SN 930820, WRMC No. 31431\t5979" << endl;
	tout << "33, SN 940301, WRMC No. 31432\t5980" << endl;
	tout << "33, SN 941212, WRMC No. 31437\t5981" << endl;
	tout << "33, SN 950207, WRMC No. 31439\t5982" << endl;
	tout << "34, SN 16665E6, WRMC No. 31262\t5857" << endl;
	tout << "34, SN 19536E6, WRMC No. 31299\t5859" << endl;
	tout << "34, SN 28495F3, WRMC No. 31157\t5705" << endl;
	tout << "34, SN 28496F3, WRMC No. 31158\t5706" << endl;
	tout << "34, SN 29255F3, WRMC No. 31162\t5726" << endl;
	tout << "34, SN 30015F3, WRMC No. 31166\t5742" << endl;
	tout << "34, SN 30016F3, WRMC No. 31167\t5743" << endl;
	tout << "34, SN 30017F3, WRMC No. 31168\t5744" << endl;
	tout << "34, SN 30019F3, WRMC No. 31170\t5746" << endl;
	tout << "34, SN 30398E6, WRMC No. 31266\t5892" << endl;
	tout << "34, SN 30399E6, WRMC No. 31267\t5893" << endl;
	tout << "34, SN 30553F3, WRMC No. 31173\t5759" << endl;
	tout << "34, SN 30554F3, WRMC No. 31174\t5760" << endl;
	tout << "34, SN 30727E6, WRMC No. 31269\t5900" << endl;
	tout << "34, SN 30918F3, WRMC No. 31179\t5772" << endl;
	tout << "34, SN 31269F3, WRMC No. 31180\t5775" << endl;
	tout << "34, SN 31608F3, WRMC No. 31182\t5790" << endl;
	tout << "34, SN 31609F3, WRMC No. 31183\t5791" << endl;
	tout << "34, SN 31853E6, WRMC No. 31271\t5916" << endl;
	tout << "34, SN 32864, WRMC No. 31304\t6007" << endl;
	tout << "34, SN 33380, WRMC No. 31311\t6028" << endl;
	tout << "34, SN 73-102, WRMC No. 31148\t6035" << endl;
	tout << "34, SN 73-23, WRMC No. 31102\t6038" << endl;
	tout << "34, SN 73-25, WRMC No. 31103\t6039" << endl;
	tout << "34, SN 73-31, WRMC No. 31104\t6040" << endl;
	tout << "34, SN 73-37, WRMC No. 31105\t6041" << endl;
	tout << "34, SN 73-41, WRMC No. 31107\t6043" << endl;
	tout << "34, SN 73-55, WRMC No. 31111\t6050" << endl;
	tout << "34, SN 73-60, WRMC No. 31113\t6052" << endl;
	tout << "34, SN 73-63, WRMC No. 31114\t6053" << endl;
	tout << "34, SN 73-65, WRMC No. 31115\t6054" << endl;
	tout << "34, SN 73-70, WRMC No. 31117\t6056" << endl;
	tout << "34, SN 73-82, WRMC No. 31119\t6058" << endl;
	tout << "34, SN 73-83, WRMC No. 31120\t6059" << endl;
	tout << "34, SN 73-84, WRMC No. 31121\t6060" << endl;
	tout << "34, SN 73-88, WRMC No. 31124\t6063" << endl;
	tout << "34, SN 73-89, WRMC No. 31125\t6064" << endl;
	tout << "34, SN 73-96, WRMC No. 31146\t6066" << endl;
	tout << "34, SN 930502, WRMC No. 31426\t5974" << endl;
	tout << "34, SN 930812, WRMC No. 31430\t5978" << endl;
	tout << "34, SN 930820, WRMC No. 31431\t5979" << endl;
	tout << "34, SN 940301, WRMC No. 31432\t5980" << endl;
	tout << "34, SN 950207, WRMC No. 31439\t5982" << endl;
	tout << "34, SN 970812, WRMC No. 31442\t5983" << endl;
	tout << "35, SN 15544E6, WRMC No. 31252\t5841" << endl;
	tout << "35, SN 16331E6, WRMC No. 31256\t5845" << endl;
	tout << "35, SN 16658E6, WRMC No. 31261\t5856" << endl;
	tout << "35, SN 18873F3, WRMC No. 31004\t5467" << endl;
	tout << "35, SN 18874F3, WRMC No. 31005\t5468" << endl;
	tout << "35, SN 21235F3, WRMC No. 31006\t5470" << endl;
	tout << "35, SN 30015F3, WRMC No. 31166\t5742" << endl;
	tout << "35, SN 30016F3, WRMC No. 31167\t5743" << endl;
	tout << "35, SN 30018F3, WRMC No. 31169\t5745" << endl;
	tout << "35, SN 30019F3, WRMC No. 31170\t5746" << endl;
	tout << "35, SN 30551F3, WRMC No. 31171\t5757" << endl;
	tout << "35, SN 30553F3, WRMC No. 31173\t5759" << endl;
	tout << "35, SN 30554F3, WRMC No. 31174\t5760" << endl;
	tout << "35, SN 30727E6, WRMC No. 31269\t5900" << endl;
	tout << "35, SN 31269F3, WRMC No. 31180\t5775" << endl;
	tout << "35, SN 31270F3, WRMC No. 31181\t5776" << endl;
	tout << "35, SN 31552F3, WRMC No. 31016\t5536" << endl;
	tout << "35, SN 31553F3, WRMC No. 31017\t5537" << endl;
	tout << "35, SN 31608F3, WRMC No. 31182\t5790" << endl;
	tout << "35, SN 31609F3, WRMC No. 31183\t5791" << endl;
	tout << "35, SN 31853E6, WRMC No. 31271\t5916" << endl;
	tout << "35, SN 31854E6, WRMC No. 31272\t5917" << endl;
	tout << "35, SN 32859, WRMC No. 31300\t6003" << endl;
	tout << "35, SN 32864, WRMC No. 31304\t6007" << endl;
	tout << "35, SN 32866, WRMC No. 31306\t6009" << endl;
	tout << "35, SN 73-31, WRMC No. 31104\t6040" << endl;
	tout << "35, SN 73-41, WRMC No. 31107\t6043" << endl;
	tout << "35, SN 73-43, WRMC No. 31135\t6044" << endl;
	tout << "35, SN 73-55, WRMC No. 31111\t6050" << endl;
	tout << "35, SN 73-60, WRMC No. 31113\t6052" << endl;
	tout << "35, SN 73-63, WRMC No. 31114\t6053" << endl;
	tout << "35, SN 73-65, WRMC No. 31115\t6054" << endl;
	tout << "35, SN 73-66, WRMC No. 31116\t6055" << endl;
	tout << "35, SN 73-85, WRMC No. 31122\t6061" << endl;
	tout << "35, SN 930820, WRMC No. 31431\t5979" << endl;
	tout << "35, SN 950207, WRMC No. 31439\t5982" << endl;
	tout << "35, SN 970812, WRMC No. 31442\t5983" << endl;
	tout << "35, SN 970818, WRMC No. 31443\t5984" << endl;
	tout << "35, SN 970819, WRMC No. 31444\t5985" << endl;
	tout << "36, SN 010927, WRMC No. 31446\t5973" << endl;
	tout << "36, SN 15545E6, WRMC No. 31253\t5842" << endl;
	tout << "36, SN 16665E6, WRMC No. 31262\t5857" << endl;
	tout << "36, SN 21381F3, WRMC No. 31153\t5675" << endl;
	tout << "36, SN 23088F3, WRMC No. 31008\t5473" << endl;
	tout << "36, SN 23090F3, WRMC No. 31009\t5474" << endl;
	tout << "36, SN 23982F3, WRMC No. 31010\t5475" << endl;
	tout << "36, SN 25576F3, WRMC No. 31154\t5681" << endl;
	tout << "36, SN 30015F3, WRMC No. 31166\t5742" << endl;
	tout << "36, SN 30016F3, WRMC No. 31167\t5743" << endl;
	tout << "36, SN 30018F3, WRMC No. 31169\t5745" << endl;
	tout << "36, SN 30019F3, WRMC No. 31170\t5746" << endl;
	tout << "36, SN 30398E6, WRMC No. 31266\t5892" << endl;
	tout << "36, SN 30551F3, WRMC No. 31171\t5757" << endl;
	tout << "36, SN 30552F3, WRMC No. 31172\t5758" << endl;
	tout << "36, SN 30727E6, WRMC No. 31269\t5900" << endl;
	tout << "36, SN 30915F3, WRMC No. 31177\t5770" << endl;
	tout << "36, SN 30917F3, WRMC No. 31178\t5771" << endl;
	tout << "36, SN 31269F3, WRMC No. 31180\t5775" << endl;
	tout << "36, SN 31270F3, WRMC No. 31181\t5776" << endl;
	tout << "36, SN 31610F3, WRMC No. 31184\t5792" << endl;
	tout << "36, SN 31882E6, WRMC No. 31274\t5920" << endl;
	tout << "36, SN 32860, WRMC No. 31301\t6004" << endl;
	tout << "36, SN 32863, WRMC No. 31303\t6006" << endl;
	tout << "36, SN 32911F3, WRMC No. 31187\t5816" << endl;
	tout << "36, SN 33246, WRMC No. 31309\t6019" << endl;
	tout << "36, SN 73-18, WRMC No. 31100\t6036" << endl;
	tout << "36, SN 73-25, WRMC No. 31103\t6039" << endl;
	tout << "36, SN 73-31, WRMC No. 31104\t6040" << endl;
	tout << "36, SN 73-38, WRMC No. 31106\t6042" << endl;
	tout << "36, SN 73-44, WRMC No. 31108\t6045" << endl;
	tout << "36, SN 73-63, WRMC No. 31114\t6053" << endl;
	tout << "36, SN 73-65, WRMC No. 31115\t6054" << endl;
	tout << "36, SN 73-70, WRMC No. 31117\t6056" << endl;
	tout << "36, SN 73-80, WRMC No. 31118\t6057" << endl;
	tout << "36, SN 73-84, WRMC No. 31121\t6060" << endl;
	tout << "36, SN 73-93, WRMC No. 31145\t6065" << endl;
	tout << "36, SN 73-98, WRMC No. 31128\t6067" << endl;
	tout << "36, SN 930806, WRMC No. 31429\t5977" << endl;
	tout << "36, SN 940301, WRMC No. 31432\t5980" << endl;
	tout << "36, SN 970812, WRMC No. 31442\t5983" << endl;
	tout << "36, SN 970819, WRMC No. 31444\t5985" << endl;
	tout << "38, SN 16525E6, WRMC No. 38004\t5853" << endl;
	tout << "38, SN 17675F3, WRMC No. 38001\t5466" << endl;
	tout << "38, SN 19227F3, WRMC No. 38003\t5469" << endl;
	tout << "38, SN 20468F3, WRMC No. 38002\t5672" << endl;
	tout << "38, SN N4330020, WRMC No. 38009\t5952" << endl;
	tout << "38, SN T2747-181, WRMC No. 38010\t6070" << endl;
	tout << "39, SN 000025, WRMC No. 39035\t5654" << endl;
	tout << "39, SN 000507, WRMC No. 39032\t5655" << endl;
	tout << "39, SN 010254, WRMC No. 39038\t5925" << endl;
	tout << "39, SN 26168F3, WRMC No. 39003\t5684" << endl;
	tout << "39, SN 26181F3, WRMC No. 39030\t5685" << endl;
	tout << "39, SN 31605F3, WRMC No. 39027\t5789" << endl;
	tout << "39, SN 960132, WRMC No. 39016\t5940" << endl;
	tout << "39, SN 960133, WRMC No. 39017\t5941" << endl;
	tout << "39, SN 990004, WRMC No. 39014\t5656" << endl;
	tout << "39, SN 990005, WRMC No. 39015\t5657" << endl;
	tout << "40, SN 010264, WRMC No. 40010\t5926" << endl;
	tout << "40, SN 010858, WRMC No. 40012\t5604" << endl;
	tout << "40, SN 010859, WRMC No. 40013\t5605" << endl;
	tout << "40, SN 101267, WRMC No. 40011\t5929" << endl;
	tout << "40, SN 32200F3, WRMC No. 40005\t5800" << endl;
	tout << "40, SN 32201F3, WRMC No. 40006\t5801" << endl;
	tout << "40, SN 32202F3, WRMC No. 40007\t5802" << endl;
	tout << "40, SN 970156, WRMC No. 40001\t5946" << endl;
	tout << "40, SN 970157, WRMC No. 40002\t5947" << endl;
	tout << "40, SN 970442, WRMC No. 40003\t5642" << endl;
	tout << "40, SN 970443, WRMC No. 40004\t5643" << endl;
	tout << "41, SN 30179E6, WRMC No. 41001\t5887" << endl;
	tout << "41, SN 30182E6, WRMC No. 41020\t5888" << endl;
	tout << "41, SN 30185E6, WRMC No. 41002\t5889" << endl;
	tout << "41, SN 30194E6, WRMC No. 41011\t5890" << endl;
	tout << "41, SN 30201F3, WRMC No. 41019\t5486" << endl;
	tout << "41, SN 30207F3, WRMC No. 41013\t5487" << endl;
	tout << "41, SN 30210F3, WRMC No. 41003\t5488" << endl;
	tout << "41, SN 30211F3, WRMC No. 41004\t5489" << endl;
	tout << "41, SN 30212F3, WRMC No. 41005\t5490" << endl;
	tout << "41, SN 30215F3, WRMC No. 41014\t5491" << endl;
	tout << "41, SN 30216F3, WRMC No. 41015\t5492" << endl;
	tout << "41, SN 30218F3, WRMC No. 41018\t5493" << endl;
	tout << "41, SN 30221F3, WRMC No. 41006\t5494" << endl;
	tout << "41, SN 30248F3, WRMC No. 41007\t5495" << endl;
	tout << "41, SN 31107, WRMC No. 41010\t5440" << endl;
	tout << "41, SN 31134E6, WRMC No. 41012\t5901" << endl;
	tout << "41, SN 31223F3, WRMC No. 41016\t5522" << endl;
	tout << "41, SN 32084F3, WRMC No. 41008\t5798" << endl;
	tout << "41, SN 32085F3, WRMC No. 41009\t5799" << endl;
	tout << "41, SN 32690F3, WRMC No. 41017\t5812" << endl;
	tout << "42, SN 29948 E6, WRMC No. 42002\t5885" << endl;
	tout << "42, SN 30123 F3, WRMC No. 42001\t5485" << endl;
	tout << "42, SN 32591 F3, WRMC No. 42005\t5804" << endl;
	tout << "42, SN 32593 F3, WRMC No. 42004\t5806" << endl;
	tout << "42, SN 32597 F3, WRMC No. 42006\t5807" << endl;
	tout << "42, SN 32608 F3, WRMC No. 42003\t5545" << endl;
	tout << "43, SN 15380F3, WRMC No. 43004\t5670" << endl;
	tout << "43, SN 18775F3, WRMC No. 43004\t5671" << endl;
	tout << "43, SN 31752E6, WRMC No. 43001\t5914" << endl;
	tout << "43, SN 32455F3, WRMC No. 43003\t5543" << endl;
	tout << "43, SN 32456F3, WRMC No. 43002\t5544" << endl;
	tout << "44, SN 33872, WRMC No. 42002\t5923" << endl;
	tout << "44, SN 33896, WRMC No. 42003\t5443" << endl;
	tout << "44, SN 33972, WRMC No. 42004\t5825" << endl;
	tout << "44, SN 41251, WRMC No. 42001\t5609" << endl;
	tout << "50, SN 31521F3, WRMC No. 50004\t5786" << endl;
	tout << "50, SN 950079, WRMC No. 50002\t5935" << endl;
	tout << "50, SN 970135, WRMC No. 50006\t5943" << endl;
	tout << "50, SN 970338, WRMC No. 50005\t5630" << endl;
	tout << "50, SN 970341, WRMC No. 50001\t5633" << endl;
	tout << "50, SN 970343, WRMC No. 50007\t5635" << endl;
	tout << "50, SN 970344, WRMC No. 50003\t5636" << endl;
	tout << "51, SN 18342E6, WRMC No. 51010\t5832" << endl;
	tout << "51, SN 31522, WRMC No. 51011\t5787" << endl;
	tout << "51, SN 31568F3, WRMC No. 51004\t5788" << endl;
	tout << "51, SN 940221, WRMC No. 51003\t5623" << endl;
	tout << "51, SN 970134, WRMC No. 51012\t5942" << endl;
	tout << "51, SN 970136, WRMC No. 51006\t5944" << endl;
	tout << "51, SN 970137, WRMC No. 51002\t5945" << endl;
	tout << "51, SN 970338, WRMC No. 50005\t5630" << endl;
	tout << "51, SN 970339, WRMC No. 51001\t5631" << endl;
	tout << "51, SN 970340, WRMC No. 51005\t5632" << endl;
	tout << "51, SN 970342, WRMC No. 51007\t5634" << endl;
	tout << "51, SN 970343, WRMC No. 50007\t5635" << endl;
	tout << "53, SN 010032, WRMC No. 53002\t5649" << endl;
	tout << "53, SN 020053, WRMC No. 53001\t5651" << endl;
	tout << "53, SN 020283, WRMC No. 53003\t5927" << endl;
	tout << "53, SN 020611, WRMC No. 53004\t5828" << endl;
	tout << "60, SN 29070F3, WRMC No. 1004\t5720" << endl;
	tout << "60, SN 29081F3, WRMC No. 1007\t5723" << endl;
	tout << "60, SN 5007, WRMC No. 1006\t5831" << endl;
	tout << "60, SN 913437, WRMC No. 1002\t5588" << endl;
	tout << "60, SN 924043, WRMC No. 1001\t5596" << endl;
	tout << "60, SN 924653, WRMC No. 1005\t5600" << endl;
	tout << "60, SN 940043, WRMC No. 1003\t5933" << endl;
	tout << "63, SN 020058, WRMC No. 63002\t5652" << endl;
	tout << "63, SN 020318, WRMC No. 63001\t5928" << endl;
	tout << "63, SN 020622, WRMC No. 63003\t5829" << endl;
	tout << "70, SN 021049, WRMC No. 70001\t6178" << endl;
	tout << "70, SN 33545E6, WRMC No. 70002\t6181" << endl;
	tout << "70, SN 020068, WRMC No. 70003\t6179" << endl;
	tout << "70, SN 33492F3, WRMC No. 70004\t6180" << endl;
	tout << "71, SN 33491F3, WRMC No. 71004\t6210" << endl;
	tout << "71, SN 021046, WRMC No. 71001\t6211" << endl;
	tout << "71, SN 020071, WRMC No. 71003\t6212" << endl;
	tout << "72, SN 33544E6, WRMC No. 72002\t6218" << endl;
	tout << "72, SN 33493F3, WRMC No. 72004\t6220" << endl;
	tout << "72, SN 021051, WRMC No. 72001\t6217" << endl;
	tout << "72, SN 020070, WRMC No. 72003\t6219" << endl;
	tout << "73, SN 31960E6, WRMC No. 73002\t6214" << endl;
	tout << "73, SN 33494F3, WRMC No. 73004\t6216" << endl;
	tout << "73, SN 021047, WRMC No. 73001\t6213" << endl;
	tout << "73, SN 020069, WRMC No. 73003\t6215" << endl;

	fout.close();

	return( NOERROR );
}
