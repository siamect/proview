/* 
 * Proview   $Id: wb_upgrade.c,v 1.1 2006-01-12 13:48:59 claes Exp $
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

/* wb_drive_convert.c -- converts plc objects
   Program to convert plc objects from v2.0 to v2.1.

   The program should be executed after loading objects in the
   workbench of v2.1.

   Converts the grapic mask for objects with new attributes.

   In this version the following classes are changed:
      BaseClasses-Drive  */

#include <stdlib.h>
#include "pwr.h"
#include "pwr_class.h"
#include "wb_ldh.h"
#include "wb_foe.h"
#include "wb_trv.h"
#include "wb_foe_macros.h"
#include "wb_foe_msg.h"
#include "pwr_baseclasses.h"

#define CHECK_STS		\
	if ( EVEN(sts))		\
	{			\
	  error_msg( sts);	\
	  exit(sts);		\
	}

static int	set_object_mask(
		pwr_tObjid	Objdid,
		ldh_tSesContext	ldhses,
		unsigned long	dum0,
		unsigned long	dum1,
		unsigned long	dum2,
		unsigned long	dum3);
static int	set_mask(
		ldh_tSesContext	ldhses,
		pwr_tClassId	classid);


static int doc_count = 0;

static int	set_object_mask(
		pwr_tObjid	Objdid,
		ldh_tSesContext	ldhses,
		unsigned long	dum0,
		unsigned long	dum1,
		unsigned long	dum2,
		unsigned long	dum3)
{
	pwr_sPlcNode		*nodebuffer;
	int			sts, size;
	pwr_eClass		class;

	sts = ldh_GetObjectBuffer( ldhses, Objdid, "DevBody", 
			"PlcNode", &class, (char **)&nodebuffer, &size);
	if( EVEN(sts)) return sts;

	sts = ldh_SetObjectPar( ldhses,
		Objdid, "DevBody", "DocumentOrientation", 
		(char *)&nodebuffer->mask[1], 4);
	if( EVEN(sts)) return sts;

	sts = ldh_SetObjectPar( ldhses,
		Objdid, "DevBody", "DocumentSize", 
		(char *)&nodebuffer->mask[0], 4);
	if( EVEN(sts)) return sts;

	XtFree( (char *)nodebuffer);

	doc_count++;
	return FOE__SUCCESS;
}

static int	set_mask(
		ldh_tSesContext	ldhses,
		pwr_tClassId	classid)
{
	int			sts;
	pwr_tClassId		class[2] = {0,0};

	doc_count = 0;
	class[0] = classid;

	/* Get all objects */
	sts = trv_get_objects_hier_class_name( ldhses, pwr_cNObjid, class, 0, 
		&set_object_mask, ldhses, 0, 0, 0, 0);
	if ( EVEN(sts)) return sts;

	printf( "-- %d Document objects converted\n", doc_count);
	return FOE__SUCCESS;
}

int main()
{
	ldh_tWBContext	wbctx;
	ldh_tSesContext	ldhses;
	int		sts;
	pwr_tVolumeId	volid;
	ldh_tVolContext	volctx;
	char		name[80];
	pwr_tClassId	class;
	int		size;
	
	/* Open session */
	sts = ldh_OpenWB( &wbctx, 0, 0);
	CHECK_STS;

	sts = ldh_GetVolumeList( wbctx, &volid);
    	while ( ODD(sts) )
    	{
  	  sts = ldh_GetVolumeClass( wbctx, volid, &class);
  	  if (EVEN(sts)) return sts;
  	
	  sts = ldh_VolumeIdToName( wbctx, volid, name, sizeof(name), 
		&size);
	  CHECK_STS;
	  printf("-- Processing volume %s\n", name);

   	  if ( class == pwr_eClass_RootVolume)
	  {
	    sts = ldh_AttachVolume( wbctx, volid, &volctx);
	    CHECK_STS;

	    sts = ldh_OpenSession( &ldhses, volctx, 
		ldh_eAccess_ReadWrite, ldh_eUtility_Pwr);
	    CHECK_STS;

	    sts = set_mask( ldhses, pwr_cClass_Document);
	    CHECK_STS;
	    sts = set_mask( ldhses, pwr_cClass_DocUser1);
	    CHECK_STS;
	    sts = set_mask( ldhses, pwr_cClass_DocUser2);
	    CHECK_STS;

	    sts = ldh_SaveSession( ldhses);
	    CHECK_STS;

	    sts = ldh_CloseSession( ldhses);
	    CHECK_STS;

	    sts = ldh_DetachVolume( wbctx, volctx);
	    CHECK_STS;
	  }
          sts = ldh_GetNextVolume( wbctx, volid, &volid);
        }
	return 0;
}

