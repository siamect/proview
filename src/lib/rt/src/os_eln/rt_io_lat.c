/* 
 * Proview   $Id: rt_io_lat.c,v 1.2 2005-09-01 14:57:56 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* rt_io_lat.c 
   This file contains routines to assist in createing and
   communicating with LAT circuits.  */

#include $vaxelnc
#include $dda_utility
#include $lat_utility
#include $elnmsg
#include descrip
#include lib$routines

#include "pwr.h"
#include "rt_io_msg.h"

#include "rt_io_lat.h"

#define	_MAX_MAP_TRIES	10
#define _WAIT_TIME  "0 00:00:2"

static $DESCRIPTOR(LATCtrlName,"$LAT_CONTROL");

/************************************************************************
*
* Name:	io_lat_set_characteristics(DDAPort,Escape,Echo,PassAll,EightBit,	
*				   Scope,TermSync);
*
* Type: pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* PORT		*DDAPort	I	The PORT identifying the connection
* pwr_tBoolean	Escape		I	Escape recognition on/off
* pwr_tBoolean	Echo		I	Echo on/off
* pwr_tBoolean	PassAll		I	Read Pass all (disregard ctrl chars)
* pwr_tBoolean	Eightbit	I	Pass the eighth bit 
* pwr_tBoolean	Scope		I	Scope/Hardcopy term
* pwr_tBoolean	TermSync	I	Terminal uses XON-XOFF
*
* Description: This routine sets the charteristics that are modifiable on
*	       a LAT circuit
*	
*************************************************************************/

pwr_tInt32 io_lat_set_characteristics(PORT	    *DDAPort,
				 pwr_tBoolean    Escape,
				 pwr_tBoolean    Echo,
				 pwr_tBoolean    PassAll,
				 pwr_tBoolean    EightBit,
				 pwr_tBoolean    Scope,
				 pwr_tBoolean    TermSync)


{
    DDA$_TERMINAL_CHAR_PTR	CharPtr;
    int				Sts;

    eln$tty_get_characteristics(DDAPort,	
				&CharPtr,
				&Sts);	/* Get the characteristics */

    if ((Sts & 1) != 1)			/* Check if it's ok */
	return Sts;			/* No */

    CharPtr->escape	= Escape;
    CharPtr->echo	= Echo;
    CharPtr->passall	= PassAll;
    CharPtr->eight_bit	= EightBit;
    CharPtr->scope	= Scope;
    CharPtr->tty_sync	= TermSync;


    eln$tty_set_characteristics(DDAPort,
				CharPtr,
				&Sts);

    return Sts;

} /* END io_lat_set_characteristics */

/************************************************************************
*
* Name: io_lat_create_and_map_port(DDAPort,
*				   PortName,
*				   RemoteServName,
*				   RemotePortName)
*
* Type:	pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* PORT		*DDAPort	I	Pointer to a port
* char		*PortName	I	Name of local port (i.e. LTA0)
* char		*RemoteServName	I	Name of server node (i.e. DSV001)
* char		*RemotePortName	I	Name of the port on the server node
*					(i.e. PORT_6)
*
* Description:	This routine does the following:
*		1. creates a local lat port
*		2. connects a circuit to the remote LAT port
*
*************************************************************************/

pwr_tInt32	io_lat_create_and_map_port(PORT	    *DDAPort,
				   char	    *PortName,
				   char	    *RemoteServName,
				   char	    *RemotePortName)




{
static $DESCRIPTOR(WaitTimeStr,"0 00:00:2");
       VARYING_STRING_CONSTANT(NullName," ");

    char	TempStr[80];

    struct  dsc$descriptor_s TempDesc = {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,TempStr};
    PORT	LATPort;
    LAT$STRING	LATPortName;
    LAT$STRING	ServerName;
    LAT$STRING	ServPortName;
    pwr_tBoolean	PortMapped;
    int		NoOfTries;
    int		Sts;
    int		RejectReason;
    LARGE_INTEGER WaitTime;
    LAT$LINK_NAME_LIST LinkNames;

    /* Create a port to communicate with the local lat control port */

    ker$create_port(&Sts,
		    &LATPort,
		    NULL); 

    if ((Sts & 1) != 1)
	return Sts;

    ker$connect_circuit(&Sts,
			&LATPort,
			NULL,
			&LATCtrlName,
			NULL,
			NULL,
			NULL);

    if ((Sts & 1) != 1)
	{
	ker$delete(NULL,&LATPort);
	return Sts;
	}

    /* Now we create a local LAT port */

    CSTRING_TO_VARYING(PortName,LATPortName);

    eln$lat_create_port(&LATPort,
			LAT$APPLICATION,
			&LATPortName,
			&Sts);

    if (((Sts & 1) != 1) && (Sts != ELN$_PORTEXISTS))
	{
	ker$delete(NULL,&LATPort);
	return Sts;
	}

    /* Now we try to start the LAT protocoll on this node. If it's already
       running we will get an error return that we just ignore */

    eln$lat_start_node(&LATPort,
		       0,
		       LinkNames,	
		       &Sts);

    if (((Sts & 1) != 1) && (Sts != ELN$_LATACTIVE))
	{
	ker$delete(NULL,&LATPort);
	return Sts;
	}

    /* Connect the DDA port to the name XXXX$ACCESS */

    strcpy(TempStr,PortName);
    strcat(TempStr,"$ACCESS");
    TempDesc.dsc$w_length = strlen(TempStr);

    ker$connect_circuit(&Sts,
			DDAPort,
			NULL,
			&TempDesc,
			NULL,
			NULL,
			NULL);

    if ((Sts & 1) != 1)
	{
	ker$delete(NULL,&LATPort);
	return Sts;
	}

    /* Now it time to map the local port to the remote server connection 
       This is done in a way that allows a number of retries if the port
       isn't availible */

    CSTRING_TO_VARYING(RemoteServName,ServerName);
    CSTRING_TO_VARYING(RemotePortName,ServPortName);
    WaitTime = eln$time_value(&WaitTimeStr);
    
    for (NoOfTries = 0, PortMapped = FALSE; NoOfTries < _MAX_MAP_TRIES; NoOfTries++)
	{
	eln$lat_map_port(DDAPort,
			 LAT$M_SET_QUEUED_STATUS|LAT$M_SET_NODE|LAT$M_SET_PORT,
			 FALSE,
			 &ServerName,
			 &ServPortName,
			 &NullName,
			 &Sts);
        if ((Sts & 1) == 1)
	    {
	    PortMapped = TRUE;
	    break;
	    }
	ker$wait_any(NULL,
		     NULL,
		     &WaitTime);
	}
    if (!PortMapped)
	{
	ker$delete(NULL,&LATPort);
	return Sts;
	}

    /* Connect the lat connection. Watch it, don't forget this, otherwise it
       may be possible comunicate, but not possible to set characteristics */

    eln$lat_connect_port(DDAPort,
			 &RejectReason,
			 &Sts);

    if (EVEN(Sts))
      errh_CErrLog(IO__LATCONERR,
                   errh_ErrArgAF(PortName),
                   errh_ErrArgAF(RemoteServName),
                   errh_ErrArgAF(RemotePortName),
                   errh_ErrArgL(RejectReason), NULL);

    ker$delete(NULL,&LATPort);
    return Sts;

} /* END io_lat_create_and_map_port */

/************************************************************************
*
* Name:	io_lat_crc16(Buffer,Count)	
*
* Type:	pwr_tInt16	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
* char		Buffer[]	I	Data buffer
* int		Count		I	Number of characters to checksum over
*
* Description:	This routine calculates a CRC-16 checksum thru buffer
*
*************************************************************************/

pwr_tUInt16 io_lat_crc16(char	    Buffer[],
		    int		    Count)

{
    static pwr_tBoolean  TableLoaded = FALSE;
    static int		 CrcTable[16];
    static unsigned int  Polynominal = 0xA001; /* This is the CRC 16 coefficients */
    static int		 InitialCrc = 0;

    pwr_tUInt32   CrcVal;
    struct  dsc$descriptor_s BufferDesc = {0,DSC$K_DTYPE_T,DSC$K_CLASS_S,NULL};

    if (!TableLoaded)
	{
	lib$crc_table(&Polynominal,CrcTable);
	TableLoaded = TRUE;
	}

    BufferDesc.dsc$w_length = Count;
    BufferDesc.dsc$a_pointer = Buffer;
 
    CrcVal = lib$crc(CrcTable,
		     &InitialCrc,
		     &BufferDesc);

    return (CrcVal & 0xffff);
} /* END io_lat_crc16 */

/************************************************************************
*
* Name: io_lat_disc_and_delete_port(PORT *DDAPort,
*				    char *PortName)		
*
* Type:	pwr_tInt32	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* PORT		DDAPort		I	The DDA port to the lat circuit
* char		*PortName	I	Name of the local lat port to remove
* Description:	
*************************************************************************/

pwr_tInt32 io_lat_disc_and_delete_port(PORT	*DDAPort,
				  char	*PortName)

{
    pwr_tInt32	Sts;
    PORT	LATCtrlPort;
    LAT$STRING	LATPortName;

    ker$create_port(&Sts,
		    &LATCtrlPort,
		    NULL);
    if (EVEN(Sts))
	return Sts;

    eln$lat_disconnect_port(DDAPort,
			    &Sts);

    if (ODD(Sts))
	{
	ker$connect_circuit(&Sts,
			    &LATCtrlPort,
			    NULL,
			    &LATCtrlName,
			    NULL,
			    NULL,
			    NULL);
	if (ODD(Sts))
	    {	
	    CSTRING_TO_VARYING(PortName,LATPortName);
	    eln$lat_delete_port(&LATCtrlPort,
				&LATPortName,
				&Sts);
	    }
	}
    
    ker$delete(&Sts,
	       &LATCtrlPort);


    return Sts;

} /* END io_lat_disconnect_and_delete_port */

