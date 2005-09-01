/* 
 * Proview   $Id: ini_localboot.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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

/* ini_localboot.c 
   This module returns the location of the boot files.  */

#ifdef OS_ELN
#include $kerneldef
#include $kernelmsg
#include $vaxelnc
#include $ni_utility
#endif

#include stdio
#include string
#include "pwr.h"

/************************************************************************
*
* Name:	ini_LocalBoot()
* 
* Type:	int	return true or false whether we booted locally
*	
* 
* Type		Parameter	IOGF	Description
* 
* Description:
*
*************************************************************************/

int ini_LocalBoot ()

{
#ifdef OS_ELN

    globalref unsigned short ker$gq_host_address [3];
    return ((ker$gq_host_address[2]>>10) & 63) == 0;

#endif

#ifdef OS_VMS
    return 1;   

#endif

} 

/************************************************************************
*
* Name:	ini_GetNodeInfo(HW,DECnet,name,DECnethost,hostname,diskname)
* 
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* char *	HW		 O	Ethernetadressen för noden
* char *	DECnet		 O	DECnetadressen för noden
* char *	name		 O	Namnet på noden
* char *	hostDECnet	 O	DECnetadressen för bootnoden
* char *	hostname	 O	Namnet på bootnoden
* char *	diskname	 O	Namnet på bootdisken 
*
* Description:	Returnerar namn, adresser och bootdisk för systemet
*************************************************************************/
int ini_GetNodeInfo(
  char *hw,
  char *DECnet,
  char *name,
  char *DECnethost,
  char *hostname,
  char *diskname
)
{
#ifdef OS_ELN
	typedef struct {
		short	len;
		char	name[6];
		} nodedesc;

	globalref unsigned short ker$gq_host_address [3];
	globalref nodedesc ker$gt_host_name;
	globalref nodedesc ker$gt_node_name;
	globalref struct scr *ker$ga_device_list;
	static unsigned int status;
	static unsigned int count;
	static	        int len;
	static	        char fname[100];
	static struct   eln$ni_configuration  config;
	static struct   eln$ni_attributes    *attrib;
        struct scr *scrdescp;
        unsigned int tmp;	
		 int no_host=0;
		 int disk_found=0;	
		 int i;	
		 FILE *fp;

	/* Skriv nodnamn om pekare finns */
	if (name) {
	  memcpy(name,    &ker$gt_node_name.name,ker$gt_node_name.len);
	  name[ker$gt_node_name.len] = '\0';
	}
	
	/* Skriv hostnamn om pekare finns */
	if (hostname) {
	  memcpy(hostname,&ker$gt_host_name.name,ker$gt_host_name.len);
	  hostname[ker$gt_host_name.len] = '\0';
	}
	
	/* Läs konfigureringen om pekare finns */
	if (DECnet) {
	  eln$ni_get_configuration ( &status, &count, &config);
	  if ( !(status & 1) ) return 0;

	  /* Använd den för att hitta attribut hos 1:a device */
	  eln$ni_get_attributes ( &status, 
				   config.clist.list[0].control_port,
				  &attrib);
	  if ( !(status & 1) ) return 0;


	  /* DECnetadressen */
	  len = sprintf(DECnet,"%d.%d",
			  (short)attrib->physical_address.address[5]/4,
			  (short)attrib->physical_address.address[4]);

	}

	/* Läs decnet host om pekare finns */
	if (DECnethost) 
	  sprintf (DECnethost, "%d.%d",  ((ker$gq_host_address[2]>>10) & 63),
					  (ker$gq_host_address[2] & 1023) );

	/* Disknamnet letas upp om man inte har någon host och diskname finns */
	if (diskname) {

	  no_host = ini_LocalBoot();
#ifdef DEBUG
	  no_host = 1;
#endif
	  if ( no_host ) {

	    /* Leta upp device-listan och datas{tt pekaren 'scrdescp' */
	    tmp = &ker$ga_device_list;
	    (char *)scrdescp = (char *)ker$ga_device_list + (int)tmp;

	    /* Loopa runt alla device */
	    do {

	      /* Kolla om DUA */
	      if (!strncmp(scrdescp->scr$t_name,"DUA",3)) {
		
	         /* Kolla efter SYSBOOT.EXE p} DUA0 - DUA9. Den disk 	  */
		 /* som svarar anses vara bootdisken, eftersom det b}de */
		 /* finns en SYSBOOT.EXE OCH disken redan {r mountad.	  */

	         for (i=0; i<9 && !disk_found; i++) {
	           sprintf(diskname,"DUA%d:",i);
	           sprintf(fname,"%s[SYS0.SYSEXE]SYSBOOT.EXE",diskname);
	           fp = fopen(fname,"r");
	           if (fp) {
	             fclose(fp);
	             disk_found = 1;
	           } /* if fp... */
		 } /* for ... */
	       } /* if DUA... */

	      /* Ta nästa */
	      (char *)scrdescp += (int)scrdescp->scr$l_next;
	    } /* do ... */
	    while (scrdescp && !disk_found);
	  } /* if no_host... */
	  else
	    sprintf (diskname, "");
	} /* if diskname... */
	
	/* Hårdvaruadressen om pekare finns */
	if (hw)
	  sprintf(hw,"%02X-%02X-%02X-%02X-%02X-%02X",
			  (long)attrib->hardware_address.address[0],
			  (long)attrib->hardware_address.address[1],
			  (long)attrib->hardware_address.address[2],
			  (long)attrib->hardware_address.address[3],
			  (long)attrib->hardware_address.address[4],
			  (long)attrib->hardware_address.address[5]);
#else
#endif
	return 1;
}

/************************************************************************
*
* Name:	ini_BootFilesLocation()
* 
* Type:	char *	returns a string describing the location of the bootfiles
*	
* 
* Type		Parameter	IOGF	Description
* 
* Description:
* 	This routine returns the destination av the bootfiles.
*	If a remote boot it returns "host::pwrp_load:"
*	and if local boot it returns "disk:[sys0.sysexe]"
*
*************************************************************************/

char *ini_BootFilesLocation ()

{
#ifdef OS_ELN
    char		hw	 [20];
    char		hostname [8];
    char		hostspec [8];
    char		nodename [8];
    char		nodespec [8];
    char		diskname [8];
    pwr_tUInt32		area;
    pwr_tUInt32		node;
    static char 	buffer	[256];

    buffer[0] = '\0';
 
    /* Check out the node */
    ini_GetNodeInfo(hw, nodespec, nodename, hostspec, hostname, diskname);
   
    /* If local boot - take our disk */
    if ( strlen(diskname) ) 
       sprintf(buffer,"%s[SYS0.SYSEXE]",diskname);
    else
       sprintf(buffer,"%s::PWRP_LOAD:",hostspec);

#endif

#ifdef OS_VMS
  static char buffer[256];

  sprintf(buffer,"pwrp_load:");

#endif

  return buffer;

} 
