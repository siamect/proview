#ifndef rt_io_lat_h
#define rt_io_lat_h

/* rt_io_lat.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "pwr.h"

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

extern pwr_tInt32 io_lat_set_characteristics(PORT	   *DDAPort,
					pwr_tBoolean    Escape,
					pwr_tBoolean    Echo,
					pwr_tBoolean    PassAll,
					pwr_tBoolean    EightBit,
					pwr_tBoolean    Scope,
					pwr_tBoolean    TermSync);

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

extern pwr_tInt32 io_lat_create_and_map_port(PORT	    *DDAPort,
					char	    *PortName,
					char	    *RemoteServName,
					char	    *RemotePortName);



/************************************************************************
*
* Name: io_lat_dis_and_delete_port(PORT *DDAPort,
*				   char *PortName)		
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
				  char	*PortName);

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

extern pwr_tUInt16 io_lat_crc16(char    Buffer[],
			   int     Count);
#endif
