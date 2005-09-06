/* 
 * Proview   $Id: wb_wsx.h,v 1.3 2005-09-06 10:43:32 claes Exp $
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
 **/

/* wb_wsx.h -- wb syntax control
   Wb syntax control.
   This module control the syntax for objects.  */

typedef enum {
  wsx_mCardOption_None 		= 0,
  wsx_mCardOption_DevName 	= 1 << 0
} wsx_mCardOption;


pwr_tStatus wsx_error_msg( 
    ldh_tSesContext	sesctx,
    pwr_tStatus 	sts,
    pwr_tObjid		objid,
    int			*errorcount,
    int			*warningcount
);

pwr_tStatus wsx_CheckCard( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount,
	wsx_mCardOption options
);

pwr_tStatus wsx_CheckCoCard( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount
);

pwr_tStatus wsx_CheckSigChanCon( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount	
);

pwr_tStatus wsx_CheckVolume( 
	ldh_tSesContext	sesctx,
	pwr_tObjid	objid,
	int		*errorcount,
	int		*warningcount	
);

