/* 
 * Proview   $Id: rt_alimserver.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef	rt_alimserver_h
#define rt_alimserver_h

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"

#define _RS_MH_ALIMS_CLASS   202
#define _RS_MH_ALIMS_PAMSADR 124
#define _RS_MH_ALIMS_REQTYP  1
#define _RS_MH_ALIMS_ANSTYP  2
#define _RS_MH_ALIMS_BUFSIZ  8192

/* Buffer used to request supervisor data from the server */

typedef struct {pwr_sAttrRef	Attribute;
		pwr_tBoolean	AllAttr; /* All attributes for this object ? */
		} rs_mh_sAlimRequest;

/* Primary data for an Asup object, more (Text data ) must be requested using
   gdh_GetObjectInfo */

typedef struct {pwr_tUInt32	EvType;
		pwr_tUInt32	EvPrio;
		pwr_tFloat32	Limit;
		pwr_tFloat32	Hyst;
		pwr_tBoolean	High;
		pwr_tBoolean	MsgOn;
		} rs_mh_sASupAlim;

/* Primary data for a Dsup object, more (Text data ) must be requested using
   gdh_GetObjectInfo */

typedef	struct {pwr_tUInt32	EvType;
		pwr_tUInt32	EvPrio;
		pwr_tBoolean	CtrlPos;
		} rs_mh_sDSupAlim;

/* Union containing the two sup types */

typedef union  {rs_mh_sASupAlim	A;
		rs_mh_sDSupAlim D;
		} rs_mh_uSupAlim;


/* This structure contains the objid of the supervisor object and the associated 
   data */

typedef struct {pwr_tObjid	SupObjid;
		rs_mh_uSupAlim	SupData;
		} rs_mh_sSupObjEntry;

/* This structure contains descriptions of the supervised object attribute */

typedef struct {pwr_sAttrRef	    SupAttr;
		pwr_tInt32	    NoOfSupObjs;
		pwr_tUInt32	    SupClass;
		rs_mh_sSupObjEntry  SupList[1];
		} rs_mh_sAttrSupList;

/* This is the buffer transmitted from the alarm limit server to the client */

typedef struct {pwr_tUInt32	    NoOfSupAttr;	/* Number of supervised attibutes*/
		rs_mh_sAttrSupList  AttrSupList[1];
		} rs_mh_sSupDataBuf;


/***************** G L O B A L    V A R I A B L E S *********************/
/*****************  L O C A L      V A R I A B L E S ********************/

#endif
