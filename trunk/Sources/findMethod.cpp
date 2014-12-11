/* findMethod.cpp		 	 */
/* 2012-07-03                */
/* Dr. Rainer Sieger         */

#include "Application.h"

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// 2012-07-03

QString MainWindow::findMethod( const QString& s_Comment )
{
    QString s_Method = "43"; // not given

    if ( ( s_Comment.startsWith( "(CTD - " ) == true ) || ( s_Comment == "CTD" )  || ( s_Comment == "(CTD)" ) )
        s_Method = "199";

    if ( s_Comment == "(CTD, SEA-BIRD)" )
        s_Method = "49";

    if ( ( s_Comment == "(CTD, Neil Brown, Mark III B)" ) || ( s_Comment == "(CTD, Neil Brown, Mark III B - PSU)" ) )
        s_Method = "44";

    if ( s_Comment == "(CTD, pre-1978 standard (Fofonoff & Millard, 1983, ...)" )
        s_Method = "5145";

    if ( s_Comment == "(CTD, Bisset-Berman)" )
        s_Method = "5148";

    if ( s_Comment == "(CTD, Guildline)" )
        s_Method = "5149";

    if ( s_Comment == "(CTD, SEA-BIRD SBE 9)" )
        s_Method = "4820";

    if ( s_Comment == "(CTD, SEA-BIRD SBE 911plus)" )
        s_Method = "716";

    if ( ( s_Comment == "(ODP sample designation)" ) || ( s_Comment == "ODP sample designation" ) || ( s_Comment == "ODP sample label" ) )
        s_Method = "5059";

    if ( s_Comment == "(calculated)" )
        s_Method = "50";

    if ( s_Comment == "(Calculated, see reference(s))" )
        s_Method = "135";

    if ( s_Comment == "(Reversing thermometer - ITS-90)" )
        s_Method = "4656";

    if ( s_Comment == "(Reversing thermometer)" )
        s_Method = "4656";

    if ( ( s_Comment == "(Salinometer, inductive - PSU)" ) || ( s_Comment == "(Salinometer, inductive - PSS-78)" ) )
        s_Method = "471";

    if ( ( s_Comment == "(computed using UNESCO funct...)" ) || ( s_Comment == "(Potential temperature (computed using UNESCO funct...)" ) )
        s_Method = "50";

    if ( ( s_Comment == "(Computed by UNESCO SVAN function)" ) || ( s_Comment == "(Sigma theta (Computed by UNESCO SVAN function))" ) )
        s_Method = "50";

    if ( s_Comment == "(Oxygen probe, Beckmann Instruments)" )
        s_Method = "4809";

    if ( s_Comment == "(Fluorometer, in-situ)" )
        s_Method = "4810";

    if ( s_Comment == "(Transmissometer - 661 nm (red light))" )
        s_Method = "4679";

    if ( s_Comment == "(Oxygen, Benson & Krause)" )
        s_Method = "371";

    if ( s_Comment.startsWith( "(CTD with attached" ) == true )
        s_Method = "706";

    if ( s_Comment == "(JGOFS methods)" )
        s_Method = "784";

    if ( s_Comment == "(JGOFS AWI methods)" )
        s_Method = "4612";

    return( s_Method );
}


