/* wb_wsx.h -- wb syntax control

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

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

