/* 
 * Proview   $Id: rt_io_base.c,v 1.25 2008-09-23 07:28:40 claes Exp $
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

/* rt_io_base.c -- io base routines. */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
# include float
# include math
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <float.h>
# include <math.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_io_msg.h"
#include "co_cdh.h"
#include "rt_errh.h"
#include "rt_io_supervise.h"
#include "rt_io_base.h"

#define IO_CONVMASK_ALL         0xFFFF
#define IO_CLASSES_SIZE 	200

#define DI_AREA_MIN_SIZE	2048
#define DO_AREA_MIN_SIZE	2048
#define AI_AREA_MIN_SIZE	512
#define AO_AREA_MIN_SIZE	512
#define DV_AREA_MIN_SIZE	4096
#define AV_AREA_MIN_SIZE	512
#define CO_AREA_MIN_SIZE	64


pwr_tBoolean io_writeerr;
pwr_tBoolean io_readerr;
pwr_tBoolean io_fatal_error;

pwr_dImport pwr_BindIoUserClasses(User);
pwr_dImport pwr_BindIoClasses(Base);
typedef struct s_cardlist {
	pwr_tObjid		objid;
	pwr_tUInt32		maxnoofchannels;
	struct s_cardlist	*next;
} io_sCardList;

static pwr_tStatus io_trv_child(
  pwr_tObjid 	parent,
  int		deep,
  pwr_tStatus	(* func) (),
  void		*arg1,
  void		*arg2,
  int		arg3,
  int           arg4);

static pwr_tStatus io_replace_symbol( pwr_sAttrRef *chan, pwr_sAttrRef *sig)
{
  if ( chan->Objid.vid == cdh_cIoConnectVolume ) {
    // Replace with attrref in IoConnected
    pwr_tAName name;
    pwr_tStatus sts;
    pwr_sAttrRef connect;
    pwr_tCid cid;
    char *s;

    sts = gdh_AttrrefToName( sig, name, sizeof(name),
			   cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    while ( (s = strrchr( name, '.'))) {
      *s = 0;
      strcat( name, ".IoConnect");
      sts = gdh_GetObjectInfo( name, &connect, sizeof(connect));
      if ( ODD(sts)) 
	break;
      *s = 0;
    }
    if ( EVEN(sts)) return sts;

    if ( cdh_ObjidIsNull( connect.Objid)) {
      errh_Info("IO init: Object is not connected '%s'", name);
      return IO__NOTCONNECTED;
    }
    // Check class
    sts = gdh_GetAttrRefTid( &connect, &cid);
    if ( cid != chan->Objid.oix) {
      errh_Info("IO init:  class of connected object '%s'", name);
      return IO__CONNECTEDCLASS;
    }

    // Replace
    chan->Objid = connect.Objid;
    chan->Offset += connect.Offset;
    return IO__REPLACED;
  }
  return IO__SUCCESS;
}

int io_connect_status( pwr_sAttrRef *sig_aref, pwr_sAttrRef *chan_aref)
{
  pwr_sAttrRef status_aref;
  pwr_sAttrRef ioconnect_aref;
  static pwr_sAttrRef last_ioconnect = {{0,0},0,0,0,{0}};
  pwr_sAttrRef iostatus_aref;
  pwr_sAttrRef card_aref;
  pwr_tStatus sts;
  pwr_tStatus *status_p;
  pwr_tStatus *iostatus_p;
  pwr_tAName sname;
  char *s;
  int found;
  
  if ( chan_aref->Offset == 0 || sig_aref->Offset == 0)
    return 0;

  /* Find content of IoConnect attribute in the signal object */
  sts = gdh_AttrrefToName( sig_aref, sname, sizeof(sname),
			   cdh_mName_volumeStrict);
  if ( EVEN(sts)) return 0;

  found = 0;
  for (;;) {
    s = strrchr( sname, '.');
    if ( !s) 
      break;
    *s = 0;
    strcat( sname, ".IoConnect");
    sts = gdh_NameToAttrref( pwr_cNObjid, sname, &ioconnect_aref);
    if ( ODD(sts)) {
      if ( cdh_ArefIsEqual( &ioconnect_aref, &last_ioconnect))
	return 1;
	   
      found = 1;
      
      *s = 0;
      strcat( sname, ".IoStatus");
      sts = gdh_NameToAttrref( pwr_cNObjid, sname, &iostatus_aref);
      if (EVEN(sts)) return 0;

      break;
    }
    *s = 0;
  }

  if ( !found)
    return 0;

  /* Get the Status attribute in the connected module */
  sts = gdh_GetObjectInfoAttrref( &ioconnect_aref, &card_aref, sizeof(card_aref));
  if ( EVEN(sts)) return 0;
  
  sts = gdh_AttrrefToName( &card_aref, sname, sizeof(sname),
			   cdh_mName_volumeStrict);
  if ( EVEN(sts)) return 0;
  strcat( sname, ".Status");

  sts = gdh_NameToAttrref( pwr_cNObjid, sname, &status_aref);
  if ( EVEN(sts)) return 0;

  /* Store status pointer in IoStatus */
  sts = gdh_AttrRefToPointer( &iostatus_aref, (void **)&iostatus_p);
  if ( EVEN(sts)) return 0;

  sts = gdh_AttrRefToPointer( &status_aref, (void **)&status_p);
  if ( EVEN(sts)) return 0;

  gdh_StoreRtdbPointer( (unsigned long *)iostatus_p, status_p);

  last_ioconnect = ioconnect_aref;
  return 1;
}


/*----------------------------------------------------------------------------*\
  Initialization of ai signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_ai_signals( 
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_AiArea		*area_op;
  pwr_sClass_Ai			*sig_op;
  pwr_sClass_ChanAi		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-ai", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-ai_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanAi, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanAi, &sig_aref, &sig_aref);
  }
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanAit, &sig_aref);
  while (ODD(sts)) {
    pwr_sClass_ChanAit chan_opt;
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_opt);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanAit, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Ai, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  !(class == pwr_cClass_ChanAi || class == pwr_cClass_ChanAit)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Ai, &sig_aref, &sig_aref);
  }

  /* Store number of Ai in node object  */
  io_op->AiCount = sig_count;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of ao signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_ao_signals(
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_AoArea		*area_op;
  pwr_sClass_Ao			*sig_op;
  pwr_sClass_ChanAo		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-ao", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-ao_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanAo, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanAo, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Ao, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  (class != pwr_cClass_ChanAo)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Ao, &sig_aref, &sig_aref);
  }

  /* Store number of Ao in node object  */
  io_op->AoCount = sig_count;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of di signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_di_signals(
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_DiArea		*area_op;
  pwr_sClass_Di			*sig_op;
  pwr_sClass_ChanDi		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName		       	buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-di", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-di_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanDi, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanDi, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Di, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  (class != pwr_cClass_ChanDi)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Di, &sig_aref, &sig_aref);
  }

  /* Store number of Di in node object  */
  io_op->DiCount = sig_count;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of do signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_do_signals(
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_DoArea		*area_op;
  pwr_sClass_Do			*sig_op;
  pwr_sClass_ChanDo		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-do", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-do_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanDo, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanDo, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Do, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) ||
	  (class != pwr_cClass_ChanDo)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Do, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Po, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) ||
	  (class != pwr_cClass_ChanDo)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Po, &sig_aref, &sig_aref);
  }

  /* Store number of Do in node object  */
  io_op->DoCount = sig_count;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of co signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_co_signals( 
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_CoArea		*area_op;
  pwr_sClass_CaArea		*abs_area_op;
  pwr_sClass_Co			*sig_op;
  pwr_sClass_ChanCo		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-co", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  sts = gdh_NameToObjid( "pwrNode-active-io-ca", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &abs_area_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanCo, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanCo, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Co, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  (class != pwr_cClass_ChanCo)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->RawValue, &area_op->Value[sig_count]);
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->AbsValue, &abs_area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;
    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Co, &sig_aref, &sig_aref);
  }

  /* Store number of Co in node object  */
  io_op->CoCount = sig_count;

  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Initialization of av signals.
\*----------------------------------------------------------------------------*/

static pwr_tStatus
io_init_av_signals (
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tObjid			area_objid;
  pwr_sAttrRef			aref;
  pwr_sClass_AvArea		*area_op;
  pwr_sClass_Av			*o;
  pwr_tStatus			sts;
  pwr_tFloat32			*p;
  pwr_tInt32			av_count = 0;	
  pwr_sClass_InitArea		*iarea_op;

 
  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-av", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-av_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  p = area_op->Value; 

  /* Loop Av-object */

  sts = gdh_GetClassListAttrRef(pwr_cClass_Av, &aref);
  if (EVEN(sts) && sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) {
    errh_Error("Get class list of Av\n%m", sts);
  }

  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &aref, (void *)&o);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
			    
    if (sts != GDH__REMOTE) {
      gdh_StoreRtdbPointer( (unsigned long *)&o->ActualValue, p);
      o->ValueIndex = av_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[av_count], &o->InitialValue);
      av_count++;
      p++;
    }

    sts = gdh_GetNextAttrRef( pwr_cClass_Av, &aref, &aref);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
  }		

  if (sts == GDH__NO_TYPE || sts == GDH__NOSUCHOBJ || sts == GDH__BADOBJTYPE) 
    sts = IO__SUCCESS;

  io_op->AvCount = av_count;

  return sts;
}

/*----------------------------------------------------------------------------*\
  Initialization of dv signals.
\*----------------------------------------------------------------------------*/

static pwr_tStatus
io_init_dv_signals (
  pwr_sClass_IOHandler	    *io_op)
{
  pwr_tObjid		    area_objid;
  pwr_sAttrRef		    aref;
  pwr_sClass_DvArea	    *area_op;
  pwr_sClass_Dv		    *o;
  pwr_tStatus		    sts;
  pwr_tBoolean		    *p;
  pwr_tInt32		    dv_count=0;
  pwr_sClass_InitArea	    *iarea_op;
 
  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-dv", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-dv_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  p = area_op->Value; 

  /* Loop DV-object */

  sts = gdh_GetClassListAttrRef( pwr_cClass_Dv, &aref);
  if (EVEN(sts) && sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) {
    errh_Error("Get class list of Dv\n%m", sts);
  }

  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &aref, (void *)&o);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
			    
    if (sts != GDH__REMOTE)  {
      gdh_StoreRtdbPointer( (unsigned long *)&o->ActualValue, p);
      o->ValueIndex = dv_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[dv_count], &o->InitialValue);
      dv_count++;
      p++;
    }

    sts = gdh_GetNextAttrRef( pwr_cClass_Dv, &aref, &aref);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
  }		

  if (sts == GDH__NO_TYPE || sts == GDH__NOSUCHOBJ || sts == GDH__BADOBJTYPE) 
    sts = IO__SUCCESS;

  io_op->DvCount = dv_count;

  return sts;
}


/*----------------------------------------------------------------------------*\
  Initialization of Ii signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_ii_signals(
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_IiArea		*area_op;
  pwr_sClass_Ii			*sig_op;
  pwr_sClass_ChanIi		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  // Get pointer to area-object
  sts = gdh_NameToObjid( "pwrNode-active-io-ii", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-ii_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanIi, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanIi, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Ii, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  (class != pwr_cClass_ChanIi)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Ii, &sig_aref, &sig_aref);
  }

  /* Store number of Ii in node object  */
  io_op->IiCount = sig_count;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of io signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_io_signals(
  pwr_sClass_IOHandler		*io_op)
{
  pwr_tStatus			sts;
  pwr_tObjid			area_objid;
  pwr_sClass_IoArea		*area_op;
  pwr_sClass_Io			*sig_op;
  pwr_sClass_ChanIo		*chan_op;
  pwr_sAttrRef			sig_aref;
  pwr_tAName   			buf;
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_InitArea		*iarea_op;

  // Get pointer to area-object
  sts = gdh_NameToObjid( "pwrNode-active-io-io", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-io_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  // Check SigChanCon and put signal in channels SigChanCon

  // Zero sigchancon in channel to detect double connections
  sts = gdh_GetClassListAttrRef( pwr_cClass_ChanIo, &sig_aref);
  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &chan_op);
    if ( ODD(sts))
      chan_op->SigChanCon.Objid = pwr_cNObjid;
    sts = gdh_GetNextAttrRef( pwr_cClass_ChanIo, &sig_aref, &sig_aref);
  }

  sts = gdh_GetClassListAttrRef( pwr_cClass_Io, &sig_aref);
  while (ODD(sts)) {

    sts = gdh_AttrrefToName( &sig_aref, buf, sizeof(buf), cdh_mNName);
    if ( EVEN(sts)) return sts;

    sts = gdh_AttrRefToPointer( &sig_aref, (void *) &sig_op);
    if (EVEN(sts)) return sts;
    
    if ( cdh_ObjidIsNull( sig_op->SigChanCon.Objid)) {
      errh_Info("IO init: Signal is not connected '%s'", buf);
    }
    else if ( EVEN(io_replace_symbol( &sig_op->SigChanCon, &sig_aref))) {
    }
    else {
      sts = gdh_GetAttrRefTid( &sig_op->SigChanCon, &class);
      if (EVEN(sts) || 
	  (class != pwr_cClass_ChanIo)) {
	errh_Error("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Error("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else {
	  if ( cdh_ObjidIsNotNull( chan_op->SigChanCon.Objid)) {
	    pwr_tAName oldsig;
	    sts = gdh_AttrrefToName( &chan_op->SigChanCon, oldsig, sizeof(oldsig), cdh_mNName);
	    errh_Error( "IO init: Double signal connection '%s' and '%s'", buf, oldsig);
	  }
	  chan_op->SigChanCon = sig_aref;
	}
      }		
    }
    gdh_StoreRtdbPointer( (unsigned long *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

    io_connect_status( &sig_aref, &sig_op->SigChanCon);
    sig_count++;

    sts = gdh_GetNextAttrRef( pwr_cClass_Io, &sig_aref, &sig_aref);
  }

  /* Store number of Io in node object  */
  io_op->IoCount = sig_count;

  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Initialization of iv signals.
\*----------------------------------------------------------------------------*/

static pwr_tStatus
io_init_iv_signals (
  pwr_sClass_IOHandler	    *io_op)
{
  pwr_tObjid		    area_objid;
  pwr_sAttrRef		    aref;
  pwr_sClass_IvArea	    *area_op;
  pwr_sClass_Iv		    *o;
  pwr_tStatus		    sts;
  pwr_tInt32		    *p;
  pwr_tInt32		    iv_count=0;
  pwr_sClass_InitArea	    *iarea_op;
 
  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-iv", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &area_op);
  if (EVEN(sts)) return sts;

  /* Get pointer to area-object */
  sts = gdh_NameToObjid( "pwrNode-active-io-iv_init", &area_objid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( area_objid, (void *) &iarea_op);
  if (EVEN(sts)) return sts;

  p = area_op->Value; 

  /* Loop Iv-object */

  sts = gdh_GetClassListAttrRef( pwr_cClass_Iv, &aref);
  if (EVEN(sts) && sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) {
    errh_Error("Get class list of Iv\n%m", sts);
  }

  while (ODD(sts)) {
    sts = gdh_AttrRefToPointer( &aref, (void *)&o);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
			    
    if (sts != GDH__REMOTE)  {
      gdh_StoreRtdbPointer( (unsigned long *)&o->ActualValue, p);
      o->ValueIndex = iv_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (unsigned long *)&iarea_op->Value[iv_count], &o->InitialValue);
      iv_count++;
      p++;
    }

    sts = gdh_GetNextAttrRef( pwr_cClass_Iv, &aref, &aref);
    if (EVEN(sts) && sts != GDH__NO_TYPE) return sts;
  }		

  if (sts == GDH__NO_TYPE || sts == GDH__NOSUCHOBJ || sts == GDH__BADOBJTYPE) 
    sts = IO__SUCCESS;

  io_op->IvCount = iv_count;

  return sts;
}


/*----------------------------------------------------------------------------*\
  Initialize io handler object.
\*----------------------------------------------------------------------------*/

static pwr_tStatus io_init_iohandler_object (  
  void
) 
{
  pwr_tStatus		sts;
  pwr_tObjid		oid;
  pwr_sClass_IOHandler	*o;
  pwr_tString80		Name;

  /* Default values in seconds */
  pwr_tFloat32		CycleTimeBus = 0.2;
  pwr_tFloat32		CycleTimeSerial = 1.0;
  pwr_tFloat32		CycleTimeBusMin = 0.02;
  pwr_tFloat32		CycleTimeSerialMin = 0.5;

  /* Get IOHandler object */
  sts = gdh_GetClassList(pwr_cClass_IOHandler, &oid);
  if (EVEN(sts))
  {
    if (sts != GDH__NOSUCHOBJ && sts != GDH__NO_TYPE)
      return sts;
    else
    {
      /* Create Handler object */
      strcpy(Name,"pwrNode-IOHandler");
      sts = gdh_CreateObject(Name, pwr_cClass_IOHandler,
	sizeof(pwr_sClass_IOHandler), &oid, pwr_cNObjid, 0, pwr_cNObjid);
      if (EVEN(sts)) return sts;

      sts = gdh_ObjidToPointer( oid, (void *) &o);
      if (EVEN(sts)) return sts;

      o->CycleTimeBus = CycleTimeBus;
      o->CycleTimeSerial = CycleTimeSerial;
    }
  }
  else
  {
    sts = gdh_ObjidToPointer( oid, (void *) &o);
    if (EVEN(sts)) return sts;

    if ( o->CycleTimeBus < CycleTimeBusMin)
      o->CycleTimeBus = CycleTimeBusMin;
    if ( o->CycleTimeSerial < CycleTimeSerialMin)
      o->CycleTimeSerial = CycleTimeSerial;
  }  

  return IO__SUCCESS;

}


/*----------------------------------------------------------------------------*\
  Get io handler object.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_get_iohandler_object (  
  pwr_sClass_IOHandler	**o,
  pwr_tObjid		*roid
)
{
  pwr_tStatus		sts;
  pwr_tObjid		oid;

  /* Get IOHandler object */
  sts = gdh_GetClassList(pwr_cClass_IOHandler, &oid);
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( oid, (void *) o);
  if (EVEN(sts)) return sts;

  if (roid != NULL)
    *roid = oid;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialization of signals and channels.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_signals( void)
{
  pwr_tStatus	sts;
  pwr_sClass_IOHandler	*io_op;

  io_init_iohandler_object();
  sts = io_get_iohandler_object(&io_op, NULL);  
  if ( EVEN(sts)) return sts;

  sts = io_init_di_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_do_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_ai_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_ao_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_co_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_dv_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_av_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_ii_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_io_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_iv_signals( io_op);
  if ( EVEN(sts)) return sts;

  sts = io_init_iohandler_object();
  if ( EVEN(sts)) return sts;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Find the io methods for a class.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_FindMethods( 
  pwr_tClassId 	class,
  io_eType	type,
  pwr_tStatus 	(** Init) (),
  pwr_tStatus 	(** Close) (),
  pwr_tStatus 	(** Read) (),
  pwr_tStatus 	(** Write) (),
  pwr_tStatus 	(** Swap) ()
)
{
  int		found;
  pwr_tStatus 	sts;
  int		i, j, k;
  char 		*s;
  char		methodstr[80];
  char		classstr[80];
  pwr_tOName	methodobject;
  pwr_sMethod	info;
  pwr_tStatus 	(* method) ();
  int		next;
  pwr_tCid	cid;
  
  if ( Init)
    *Init = 0;
  if ( Close)
    *Close = 0;
  if ( Read)
    *Read = 0;
  if ( Write)
    *Write = 0;
  if ( Swap)
    *Swap = 0;

  for ( k = 0; k < 5; k++) {
    // Init, Close, Read, Write, Swap loop
    cid = class;
    next = 0;
    while ( 1) {
      // Super class loop, inherit methods from superclass

      sts = gdh_ObjidToName( cdh_ClassIdToObjid( cid), methodobject, sizeof(methodobject),
			   cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      /* Detect name of methods to look for */
      switch ( type) {
      case io_eType_Agent:
	strcat( methodobject, "-IoMethods-IoAgent");
	break;
      case io_eType_Rack:
	strcat( methodobject, "-IoMethods-IoRack");
	break;
      case io_eType_Card:
	strcat( methodobject, "-IoMethods-IoCard");
	break;
      default:
	return IO__NOMETHOD;
      }
      switch ( k) {
      case 0:
	strcat( methodobject, "Init");
	break;
      case 1:
	strcat( methodobject, "Close");
	break;
      case 2:
	strcat( methodobject, "Read");
	break;
      case 3:
	strcat( methodobject, "Write");
	break;
      case 4:
	strcat( methodobject, "Swap");
	break;
      }

      sts = gdh_GetObjectInfo( methodobject, &info, sizeof(info));
      if ( ODD(sts)) {
	strcpy( classstr, info.MethodName);
	if ( (s = strchr( classstr, '-'))) {
	  strcpy( methodstr, s+1);
	  *s = 0;
	}	 
	break;
      }

      sts = gdh_GetSuperClass( cid, &cid, pwr_cNObjid);
      if ( EVEN(sts)) {
	next = 1;
	break;
      }
    }
    if ( next)
      continue;

    method = 0;
    found = 0;
    /* Search i IoUser methods */
    for ( i = 0;; i++) {
      if (pwr_gUser_IoUserClassMethods[i].ClassName[0] == '\0') 
	break; 
      {
	if ( strcmp(pwr_gUser_IoUserClassMethods[i].ClassName, classstr) == 0) {
	  for (j = 0;; j++) {
	    found = 1;
	    if ((*pwr_gUser_IoUserClassMethods[i].Methods)[j].MethodName[0] == '\0')
	      break;
	    if ( strcmp( (*pwr_gUser_IoUserClassMethods[i].Methods)[j].MethodName,
			 methodstr) == 0)
	      method = (*pwr_gUser_IoUserClassMethods[i].Methods)[j].Method;
	  }
	}
	if ( found)
	  break;
      }
    }
    if (!found) {
      for ( i = 0;; i++) {
      if (pwr_gBase_IoClassMethods[i].ClassName[0] == '\0') 
	break;

      if ( strcmp(pwr_gBase_IoClassMethods[i].ClassName, classstr) == 0) {
	for (j = 0;; j++) {
	  found = 1;
	  if ((*pwr_gBase_IoClassMethods[i].Methods)[j].MethodName[0] == '\0')
	    break;
	  if ( strcmp( (*pwr_gBase_IoClassMethods[i].Methods)[j].MethodName,
		       methodstr) == 0)
	    method = (*pwr_gBase_IoClassMethods[i].Methods)[j].Method;
	}
      }
      if ( found)
	break;
      }
    }

    switch ( k) {
    case 0:
      if ( Init)
	*Init = method;
      break;
    case 1:
      if ( Close)
	*Close = method;
      break;
    case 2:
      if ( Read)
	*Read = method;
      break;
    case 3:
      if ( Write)
	*Write = method;
      break;
    case 4:
      if ( Swap)
	*Swap = method;
      break;
    }
  }
  if ( !found)
    return IO__NOMETHOD;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Insert a card object into the context list.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_init_card(
  pwr_tObjid	objid, 
  io_tCtx	ctx,
  io_sRack	*rp,
  int		agent_type,
  int           swap)
{
  pwr_tStatus 	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* CardInit) ();
  pwr_tStatus 	(* CardClose) ();
  pwr_tStatus 	(* CardRead) ();
  pwr_tStatus 	(* CardWrite) ();
  pwr_tStatus 	(* CardSwap) ();
  pwr_tAName   	cname;
  pwr_tAName   	attrname;
  pwr_tUInt32	process = 0;
  io_sCard	*cp;
  io_sCard	*clp;
  pwr_sAttrRef	attrref;
  pwr_tObjid	chan;
  pwr_tDlid	chandlid;
  pwr_tDlid	sigdlid;
  pwr_tUInt32	number;
  pwr_tUInt16	maxchan;
  pwr_sAttrRef	sigchancon;
  void		*chan_op;
  void		*sig_op;
  io_sChannel	*chanp;
  pwr_tClassId	sigclass;
  pwr_tFloat32	scantime;
  pwr_tObjid	thread;
  int		ok;
  int		child_found = 0;
  int		fix_channels = 1;
  int		chan_cnt = 0;
  int 		sig_found;
  int		idx;
  
  sts = gdh_GetObjectClass( objid, &class);
  if ( EVEN(sts)) return sts;

  if ( io_CheckClassIoType( io_eType_Card, class)) {
    sts = io_FindMethods( class, io_eType_Card, 
			  &CardInit, &CardClose, &CardRead, &CardWrite, &CardSwap);
    if ( ODD(sts)) {
      if ( CardInit != NULL || CardClose != NULL || CardRead != NULL || CardWrite != NULL || CardSwap != NULL) {
	/* This is a card object */
	/* Check if the rack should be handled by this process */      
      
	sts = gdh_ObjidToName( objid, cname, sizeof(cname),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;

	ok = 0;
	strcpy( attrname, cname);
	strcat( attrname, ".Process");
	sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
	if ( ((EVEN(sts) && ctx->Process == io_mProcess_User) ||
	      (ODD(sts) && ctx->Process == (int) process)) && !swap) {
	  if ( process == io_mProcess_Plc) {
	    /* Check thread also */
	    strcpy( attrname, cname);
	    strcat( attrname, ".ThreadObject");
	    sts = gdh_GetObjectInfo( attrname, &thread, sizeof(thread));
	    if ( ODD(sts) && cdh_ObjidIsEqual( thread, ctx->Thread))
	      ok = 1;
	  }
	  else
	    ok = 1;
	}
	else if (ODD(sts) && swap && CardSwap != NULL) {
	  ok = 1;
	}

	if ( ok) {
	  /* Treat this card in this process */
	  strcpy( attrname, cname);
	  strcat( attrname, ".MaxNoOfChannels");
	  sts = gdh_GetObjectInfo( attrname, &maxchan, sizeof(maxchan));
	  if ( EVEN(sts)) {
	    strcpy( attrname, cname);
	    strcat( attrname, ".MaxNoOfCounters");
	    sts = gdh_GetObjectInfo( attrname, &maxchan, sizeof(maxchan));
	    if ( EVEN(sts)) {
	      maxchan = IO_CHANLIST_SIZE;
	      fix_channels = 0;
	    }
	  }

	  cp = calloc( 1, sizeof(io_sCard));
	  cp->chanlist = (io_sChannel *) calloc( maxchan, sizeof(io_sChannel));
	  cp->ChanListSize = maxchan;
	  cp->Class = class;
	  cp->Local = NULL;
	  cp->Objid = objid;
	  strcpy( cp->Name, cname);
          if (!swap) {
	    if ( CardRead != NULL)
	      cp->Action |= io_mAction_Read;
	    if ( CardWrite != NULL)
	      cp->Action |= io_mAction_Write;
	    cp->Init = CardInit;
	    cp->Close = CardClose;
	    cp->Read = CardRead;
	    cp->Write = CardWrite;
	    cp->Swap = CardSwap;
	  } else {
	    if ( CardSwap != NULL)
	      cp->Action |= io_mAction_Swap;
	    cp->Init = NULL;
	    cp->Close = CardClose;
	    cp->Read = NULL;
	    cp->Write = NULL;
	    cp->Swap = CardSwap;
	  }
	  if ( agent_type == io_eType_Agent)
	    cp->AgentControlled = 1;
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = objid;
	  sts = gdh_DLRefObjectInfoAttrref( &attrref, (void *) &cp->op, &cp->Dlid);
	  if ( EVEN(sts)) return sts;
	  strcpy( attrname, cname);
	  strcat( attrname, ".ScanTime");
	  sts = gdh_GetObjectInfo( attrname, &scantime, sizeof(scantime));
	  if (ODD(sts)) 
	    cp->scan_interval = scantime / ctx->ScanTime + FLT_EPSILON;
	  else
	    cp->scan_interval = 1;
	  
	  /* Insert last in cardlist */
	  if ( rp->cardlist == NULL)
	    rp->cardlist = cp;
	  else {
	    for ( clp = rp->cardlist; clp->next != NULL; clp = clp->next) ;
	    clp->next = cp;
	  }
	  
	  /* Fill in the channel and signal lists */
	  
	  if (!swap) {

	    /* Find children */
	    sts = gdh_GetChild( objid, &chan);
	    while( ODD(sts)) {
	      memset( &attrref, 0, sizeof(attrref));
	      attrref.Objid = chan;
	      sts = gdh_DLRefObjectInfoAttrref( &attrref, (void *) &chan_op, &chandlid);
	      if ( EVEN(sts)) return sts;

	      sts = gdh_GetObjectClass( chan, &class);
	      if ( EVEN(sts)) return sts;

	      switch ( class) {
	      case pwr_cClass_ChanAi:
	        sigchancon = ((pwr_sClass_ChanAi *) chan_op)->SigChanCon;
	        number = ((pwr_sClass_ChanAi *) chan_op)->Number;
	        break;
	      case pwr_cClass_ChanAit:
	        sigchancon = ((pwr_sClass_ChanAit *) chan_op)->SigChanCon;
	        number = ((pwr_sClass_ChanAit *) chan_op)->Number;
	        break;
	      case pwr_cClass_ChanAo:
	        sigchancon = ((pwr_sClass_ChanAo *) chan_op)->SigChanCon;
	        number = ((pwr_sClass_ChanAo *) chan_op)->Number;
	        break;
	      case pwr_cClass_ChanDo:
	        sigchancon = ((pwr_sClass_ChanDo *) chan_op)->SigChanCon;
	        number = ((pwr_sClass_ChanDo *) chan_op)->Number;
	        break;
	      case pwr_cClass_ChanDi:
	        sigchancon = ((pwr_sClass_ChanDi *) chan_op)->SigChanCon;
	        number = ((pwr_sClass_ChanDi *) chan_op)->Number;
	        break;
	      case pwr_cClass_ChanIi:
		sigchancon = ((pwr_sClass_ChanIi *) chan_op)->SigChanCon;
		number = ((pwr_sClass_ChanIi *) chan_op)->Number;
		break;
	      case pwr_cClass_ChanIo:
		sigchancon = ((pwr_sClass_ChanIo *) chan_op)->SigChanCon;
		number = ((pwr_sClass_ChanIo *) chan_op)->Number;
		break;
	      case pwr_cClass_ChanCo:
		sigchancon = ((pwr_sClass_ChanCo *) chan_op)->SigChanCon;
		number = ((pwr_sClass_ChanCo *) chan_op)->Number;
		break;
	      default:
		sts = gdh_DLUnrefObjectInfo( chandlid);
		sts = gdh_GetNextSibling( chan, &chan);
		continue;
	      }

	      child_found = 1;
	      chan_cnt++;
	      if ( !fix_channels && chan_cnt > maxchan) {
		errh_Error( "IO init error: max number of channels exceeded %s, chan nr %d", 
			   cp->Name, chan_cnt);
		sts = gdh_DLUnrefObjectInfo( chandlid);
		break;
	      }

	      if ( fix_channels && (int) number > maxchan-1) {
		/* Number out of range */
		errh_Error( "IO init error: number out of range %s, chan nr %d", 
			    cp->Name, number);
		sts = gdh_DLUnrefObjectInfo( chandlid);
		sts = gdh_GetNextSibling( chan, &chan);
		continue;
	      }

	      /* Find signal */
	      sig_found = 1;
	      if ( cdh_ObjidIsNull( sigchancon.Objid)) {
		/* Not connected */
		sig_found = 0;
	      }

	      if ( sig_found) {
		sts = gdh_GetAttrRefTid( &sigchancon, &sigclass);
		if ( EVEN(sts))
		  sig_found = 0;
	      }

	      if ( sig_found) {
		sts = gdh_DLRefObjectInfoAttrref( &sigchancon, (void *) &sig_op, &sigdlid);
		if ( EVEN(sts))
		  sig_found = 0;
	      }

	      if ( fix_channels && !sig_found) {
		  sts = gdh_DLUnrefObjectInfo( chandlid);
		  sts = gdh_GetNextSibling( chan, &chan);
		  continue;
	      }

	      if ( fix_channels)
		idx = number;
	      else
		idx = chan_cnt - 1;

	      /* Insert */
	      chanp = &cp->chanlist[idx];
	      chanp->cop = chan_op;
	      chanp->ChanDlid = chandlid;
	      chanp->ChanAref = cdh_ObjidToAref(chan);
	      chanp->ChanClass = class;
	      if ( sig_found) {
		chanp->sop = sig_op;
		chanp->SigDlid = sigdlid;
		chanp->SigAref = sigchancon;
		chanp->SigClass = sigclass;
		switch( sigclass) {
		case pwr_cClass_Di:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		   (unsigned long) ((pwr_sClass_Di *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Do:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Do *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Po:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Po *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Ai:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Ai *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Ao:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Ao *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Ii:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Ii *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Io:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Io *)sig_op)->ActualValue);
		  break;
		case pwr_cClass_Co:
		  chanp->vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Co *)sig_op)->RawValue);
		  chanp->abs_vbp = gdh_TranslateRtdbPointer( 
		    (unsigned long) ((pwr_sClass_Co *)sig_op)->AbsValue);
		  break;
		default:
		  errh_Error( 
		    "IO init error: unknown signal class card  %, chan nr %d", 
		    cp->Name, number);
		  sts = gdh_DLUnrefObjectInfo( chandlid);
		  sts = gdh_DLUnrefObjectInfo( sigdlid);
		  memset( chanp, 0, sizeof(*chanp));
		}

		/* If the signal has a Sup-object as a child, this will be inserted
		   in the suplist */
		/* if ( process != io_mProcess_Plc) */
		io_ConnectToSupLst( ctx->SupCtx, sigclass, sigchancon.Objid, sig_op);
	      }
	      sts = gdh_GetNextSibling( chan, &chan);
	    }

	    /* Look for internal object attributes */
	    if ( !child_found) {
	      gdh_sAttrDef *bd;
	      int rows;
	      int csize;
	      int i, j;
	      int elem;

	      sts = gdh_GetObjectBodyDef( cp->Class, &bd, &rows, pwr_cNObjid);
	      if ( EVEN(sts)) return sts;

	      for ( i = 0; i < rows; i++) {
		switch ( bd[i].attr->Param.TypeRef) {
		case pwr_cClass_ChanAi:
		  csize = sizeof( pwr_sClass_ChanAi);
		  break;
		case pwr_cClass_ChanAit:
		  csize = sizeof( pwr_sClass_ChanAit);
		  break;
		case pwr_cClass_ChanAo:
		  csize = sizeof( pwr_sClass_ChanAo);
		  break;
		case pwr_cClass_ChanDi:
		  csize = sizeof( pwr_sClass_ChanDi);
		  break;
		case pwr_cClass_ChanDo:
		  csize = sizeof( pwr_sClass_ChanDo);
		  break;
		case pwr_cClass_ChanIi:
		  csize = sizeof( pwr_sClass_ChanIi);
		  break;
		case pwr_cClass_ChanIo:
		  csize = sizeof( pwr_sClass_ChanIo);
		  break;
		case pwr_cClass_ChanCo:
		  csize = sizeof( pwr_sClass_ChanCo);
		  break;
		default:
		  continue;
		}

		elem = 1;
		if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY)
		  elem = bd[i].attr->Param.Info.Elements;
		for ( j = 0; j < elem; j++) {
		  chan_op = ((char *)cp->op) + bd[i].attr->Param.Info.Offset + csize * j;

		  switch ( bd[i].attr->Param.TypeRef) {
		  case pwr_cClass_ChanAi:
		    sigchancon = ((pwr_sClass_ChanAi *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanAit:
		    sigchancon = ((pwr_sClass_ChanAit *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanAo:
		    sigchancon = ((pwr_sClass_ChanAo *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanDi:
		    sigchancon = ((pwr_sClass_ChanDi *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanDo:
		    sigchancon = ((pwr_sClass_ChanDo *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanIi:
		    sigchancon = ((pwr_sClass_ChanIi *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanIo:
		    sigchancon = ((pwr_sClass_ChanIo *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  case pwr_cClass_ChanCo:
		    sigchancon = ((pwr_sClass_ChanCo *) chan_op)->SigChanCon;
		    number = chan_cnt;
		    break;
		  default:
		    ;
		  }

		  chan_cnt++;

		  /* Find signal */
		  sig_found = 0;
		  if ( cdh_ObjidIsNotNull( sigchancon.Objid)) {
		    sts = gdh_GetAttrRefTid( &sigchancon, &sigclass);
		    if ( ODD(sts)) {
		      sts = gdh_DLRefObjectInfoAttrref( &sigchancon, (void *) &sig_op, &sigdlid);
		      if ( ODD(sts)) 
			sig_found = 1;
		    }
		  }
		  if ( !sig_found) {
		    sig_op = 0;
		    sigdlid = pwr_cNDlid;
		    sigclass = 0;
		  }
		  /* Insert */
		  if ( elem > 1)
		    sprintf( attrname, "%s.%s[%d]", cname, bd[i].attrName, j);
		  else
		    sprintf( attrname, "%s.%s", cname, bd[i].attrName);

		  chanp = &cp->chanlist[number];
		  chanp->cop = chan_op;
		  chanp->ChanDlid = pwr_cNDlid;
		  sts = gdh_NameToAttrref( pwr_cNObjid, attrname, &chanp->ChanAref);
		  if ( EVEN(sts)) return sts;
		  chanp->sop = sig_op;
		  chanp->SigDlid = sigdlid;
		  chanp->SigAref = sigchancon;
		  chanp->ChanClass = bd[i].attr->Param.TypeRef;
		  chanp->SigClass = sigclass;
		  if ( sig_found) {
		    switch( sigclass) {
		    case pwr_cClass_Di:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Di *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Do:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
	               (unsigned long) ((pwr_sClass_Do *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Po:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Po *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Ai:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Ai *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Ao:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Ao *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Ii:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Ii *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Io:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Io *)sig_op)->ActualValue);
		      break;
		    case pwr_cClass_Co:
		      chanp->vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Co *)sig_op)->RawValue);
		      chanp->abs_vbp = gdh_TranslateRtdbPointer( 
		       (unsigned long) ((pwr_sClass_Co *)sig_op)->AbsValue);
		      break;
		    default:
		      errh_Error( 
		       "IO init error: unknown signal class card  %, chan nr %d", 
		       cp->Name, number);
		      sts = gdh_DLUnrefObjectInfo( sigdlid);
		      sig_op = 0;
		      sigdlid = pwr_cNDlid;
		    }
		  }
		}
	      }
	      free( (char *)bd);
	    }
	  }
	}
      }
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Insert a rack object in the context list.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_init_rack(
  pwr_tObjid	objid,
  io_tCtx	ctx,
  io_sAgent	*ap,
  int		agent_type,
  int           swap)
{
  pwr_tStatus	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* RackInit) ();
  pwr_tStatus 	(* RackClose) ();
  pwr_tStatus 	(* RackRead) ();
  pwr_tStatus 	(* RackWrite) ();
  pwr_tStatus 	(* RackSwap) ();
  pwr_tAName   	rname;
  pwr_tAName   	attrname;
  pwr_tUInt32	process = 0;
  io_sRack	*rp;
  io_sRack	*rlp;
  pwr_sAttrRef	attrref;
  pwr_tFloat32	scantime;
  pwr_tObjid	thread;
  int		ok;

  sts = gdh_GetObjectClass( objid, &class);
  if ( EVEN(sts)) return sts;

  if ( agent_type == io_eType_Node) {
    /* If this is an agent object, ignore this object and the descendents */
    if ( io_CheckClassIoType( io_eType_Agent, class))
      return IO__TRV_NEXT;
  }

  if ( io_CheckClassIoType( io_eType_Rack, class)) {
    /* This is a rack object,  */

    sts = io_FindMethods( class, io_eType_Rack, &RackInit, &RackClose, &RackRead, &RackWrite, &RackSwap);

    /* Check if the rack should be handled by this process */      
      
    sts = gdh_ObjidToName( objid, rname, sizeof(rname),
			   cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    
    ok = 0;
    strcpy( attrname, rname);
    strcat( attrname, ".Process");

    sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
    if ( (EVEN(sts) ||
	  (ODD(sts) && ctx->Process & process)) && !swap) {
      if ( EVEN(sts))
	process = io_mProcess_All;

      if ( process & io_mProcess_Plc) {
	/* Check thread also */
	strcpy( attrname, rname);
	strcat( attrname, ".ThreadObject");
	sts = gdh_GetObjectInfo( attrname, &thread, sizeof(thread));
	if ( EVEN(sts))
	  ok = 1;
	else if ( ODD(sts) && cdh_ObjidIsEqual( thread, ctx->Thread))
	  ok = 1;
      }
      else
	ok = 1;
    }
    else if (ODD(sts) && swap && RackSwap != NULL) {
      ok = 1;
    }

    if ( ok) {

      /* Treat this rack in this process */
      rp = calloc( 1, sizeof(io_sRack));
      rp->Local = NULL;
      rp->Class = class;
      rp->Objid = objid;
      strcpy( rp->Name, rname);
      rp->Process = process;
      if (!swap) { 
	if ( RackRead != NULL)
	  rp->Action |= io_mAction_Read;
	if ( RackWrite != NULL)
	  rp->Action |= io_mAction_Write;
	rp->Init = RackInit;
	rp->Close = RackClose;
	rp->Read = RackRead;
	rp->Write = RackWrite;
	rp->Swap = RackSwap;
      } else {
	if ( RackSwap != NULL)
	  rp->Action |= io_mAction_Swap;
	rp->Init = NULL;
	rp->Close = RackClose;
	rp->Read = NULL;
	rp->Write = NULL;
	rp->Swap = RackSwap;
      }
      if ( agent_type == io_eType_Agent)
	rp->AgentControlled = 1;
      memset( &attrref, 0, sizeof(attrref));
      attrref.Objid = objid;
      sts = gdh_DLRefObjectInfoAttrref( &attrref, &rp->op, &rp->Dlid);
      if ( EVEN(sts)) return sts;
      strcpy( attrname, rname);
      strcat( attrname, ".ScanTime");
      sts = gdh_GetObjectInfo( attrname, &scantime, sizeof(scantime));
      if (ODD(sts)) 
	rp->scan_interval = scantime / ctx->ScanTime + FLT_EPSILON;
      else
	rp->scan_interval = 1;
      
      /* Insert last in racklist */
      if ( ap->racklist == NULL)
	ap->racklist = rp;
      else {
	for ( rlp = ap->racklist; rlp->next != NULL; rlp = rlp->next) ;
	rlp->next = rp;
      }
	  
      sts = io_trv_child( objid, 0, io_init_card, ctx, rp, agent_type, swap);
    }
    return IO__TRV_NEXT;
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Insert an agent object in the context list.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_init_agent(
  pwr_tObjid	objid,
  io_tCtx	ctx,
  void		*dummy,
  int		agent_type,
  int           swap)
{
  pwr_tStatus	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* AgentInit) ();
  pwr_tStatus 	(* AgentClose) ();
  pwr_tStatus 	(* AgentRead) ();
  pwr_tStatus 	(* AgentWrite) ();
  pwr_tStatus 	(* AgentSwap) ();
  pwr_tAName   	aname;
  pwr_tAName   	attrname;
  pwr_tUInt32	process = 0;
  io_sAgent	*ap;
  io_sAgent	*alp;
  pwr_sAttrRef	attrref;
  pwr_tFloat32	scantime;
  pwr_tObjid	thread;
  int		ok;

  sts = gdh_GetObjectClass( objid, &class);
  if ( EVEN(sts)) return sts;

  if ( io_CheckClassIoType( io_eType_Agent, class)) {
    sts = io_FindMethods( class, io_eType_Agent, &AgentInit, &AgentClose, &AgentRead, &AgentWrite, &AgentSwap);
    if ( ODD(sts)) {
      if ( AgentInit != NULL || AgentClose != NULL || AgentRead != NULL || AgentWrite != NULL || AgentSwap != NULL) {
	/* This is a agent object or the node object,  */
	/* Check if the agent should be handled by this process */ 
      
	sts = gdh_ObjidToName( objid, aname, sizeof(aname),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;

	ok = 0;
	strcpy( attrname, aname);
	strcat( attrname, ".Process");
	sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
	if (( EVEN(sts) ||
	     (ODD(sts) && ctx->Process & process)) && !swap) {
	  if ( EVEN(sts))
	    process = io_mProcess_All;
	  if ( ctx->Process == io_mProcess_Profibus)
	    ok = 1;
	  else if ( process & io_mProcess_Plc) {
	    /* Check thread also */
	    strcpy( attrname, aname);
	    strcat( attrname, ".ThreadObject");
	    sts = gdh_GetObjectInfo( attrname, &thread, sizeof(thread));
	    if ( EVEN(sts))
	      ok = 1;
	    else if ( ODD(sts) && cdh_ObjidIsEqual( thread, ctx->Thread))
	      ok = 1;
	  }
	  else
	    ok = 1;
	}
	else if (ODD(sts) && swap && AgentSwap != NULL) {
	  /* IoComm-process should always handle the Swap-method */
	  ok = 1;
	}

	if ( ok) {
	       
	  /* Treat this agent in this process */
	  ap = calloc( 1, sizeof(io_sAgent));
	  ap->Class = class;
	  ap->Local = NULL;
	  ap->Objid = objid;
	  strcpy( ap->Name, aname);
	  ap->Process = process;
          if (!swap) {
	    if ( AgentRead != NULL)
	      ap->Action |= io_mAction_Read;
	    if ( AgentWrite != NULL)
	      ap->Action |= io_mAction_Write;
	    ap->Init = AgentInit;
	    ap->Close = AgentClose;
	    ap->Read = AgentRead;
	    ap->Write = AgentWrite;
	    ap->Swap = AgentSwap;
	  } else {
	    if ( AgentSwap != NULL)
	      ap->Action |= io_mAction_Swap;
	    ap->Init = NULL;
	    ap->Close = AgentClose;
	    ap->Read = NULL;
	    ap->Write = NULL;
	    ap->Swap = AgentSwap;
	  }
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = objid;
	  sts = gdh_DLRefObjectInfoAttrref( &attrref, &ap->op, &ap->Dlid);
	  if ( EVEN(sts)) return sts;
	  strcpy( attrname, aname);
	  strcat( attrname, ".ScanTime");
	  sts = gdh_GetObjectInfo( attrname, &scantime, sizeof(scantime));
	  if (ODD(sts)) 
	    ap->scan_interval = scantime / ctx->ScanTime + FLT_EPSILON;
	  else
	    ap->scan_interval = 1;
 
	  /* Insert last in agentlist */
	  if ( ctx->agentlist == NULL)
	    ctx->agentlist = ap;
	  else {
	    for ( alp = ctx->agentlist; alp->next != NULL; alp = alp->next) ;
	    alp->next = ap;
	  }
	  
	  sts = io_trv_child( objid, 0, io_init_rack, ctx, ap, agent_type, swap);
	}
      }
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Traverse all the children or all the descendants to an object.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_trv_child(
  pwr_tObjid 	parent,
  int		deep,
  pwr_tStatus	(* func) (),
  void		*arg1,
  void		*arg2,
  int		arg3,
  int           arg4)
{
  pwr_tObjid	child;
  pwr_tStatus 	sts;
  
  sts = gdh_GetChild( parent, &child);
  while ( ODD(sts))
  {
    sts = (func) ( child, arg1, arg2, arg3, arg4);
    if ( EVEN(sts)) return sts;

    if ( deep && sts != IO__TRV_NEXT)
    {
      sts = io_trv_child( child, deep, func, arg1, arg2, arg3, arg4);
      if ( EVEN(sts)) return sts;
    }
    sts = gdh_GetNextSibling( child, &child);
  }
  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Move di data word to valuebase.
\*----------------------------------------------------------------------------*/
void io_DiUnpackWord( 
  io_sCard	*cp,
  pwr_tUInt16	data,
  pwr_tUInt16	mask,
  int		index)
{
  io_sChannel 	*chanp;

  if ( index == 0)
    chanp = &cp->chanlist[0];
  else
    chanp = &cp->chanlist[16];

  if ( mask == IO_CONVMASK_ALL)
  {
    /* No conversion test */
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 64) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 128) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 256) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 512) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1024) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2048) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4096) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8192) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16384) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32768) != 0);
    chanp++;
  }
  else
  {
    if ( chanp->cop && chanp->sop && mask & 1)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 2)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 4)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 8)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 16)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 32)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 64)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 64) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 128)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 128) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 256)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 256) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 512)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 512) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 1024)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1024) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 2048)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2048) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 4096)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4096) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 8192)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8192) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 16384)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16384) != 0);
    chanp++;
    if ( chanp->cop && chanp->sop && mask & 32768)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32768) != 0);
    chanp++;
  }
}

/*----------------------------------------------------------------------------*\
  Move do from valuebase to data word.
\*----------------------------------------------------------------------------*/
void io_DoPackWord( 
  io_sCard	*cp,
  pwr_tUInt16	*data,
  int		index)
{
  io_sChannel 	*chanp;

  if ( index == 0)
    chanp = &cp->chanlist[0];
  else
    chanp = &cp->chanlist[16];

  *data = 0;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 32;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 64;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 128;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 256;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 512;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1024;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2048;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4096;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8192;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16384;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 32768;
  chanp++;
}

/*----------------------------------------------------------------------------*\
  Initialize io racks and cards.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init ( 
  io_mProcess	process,
  pwr_tObjid	thread,
  io_tCtx 	*ctx,
  int		relativ_vector,
  float		scan_time)
{
  pwr_tObjid 	node;
  pwr_sNode	*node_op;
  pwr_sClass_IOHandler	*io_op;
  pwr_tStatus	sts;
  io_sAgent	*ap;
  io_sRack	*rp;
  io_sCard	*cp;
  int		rw_action_found;

  if ( scan_time <= 0)
    return IO__TIMEINVALID;

  sts = io_get_iohandler_object(&io_op, NULL);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetNodeObject( 0, &node);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( node, (void *) &node_op);
  if ( EVEN(sts)) return sts;

  *ctx = calloc( 1, sizeof(**ctx));
  (*ctx)->Process = process;
  (*ctx)->RelativVector = relativ_vector;
  (*ctx)->Node = node_op;
  (*ctx)->IOHandler = io_op;
  (*ctx)->ScanTime = scan_time;
  (*ctx)->Thread = thread;

  /* Init of timerlist for Sup-objects */
  /* if ( process != io_mProcess_Plc) */
  io_InitSupLst( &(*ctx)->SupCtx, scan_time);

  if ( (*ctx)->Node->EmergBreakTrue || !(*ctx)->IOHandler->IOReadWriteFlag)
    return IO__IS_STOPPED;

  /* Traverse all objects in the NodeHierarchy, find methods and build
     the io context tree for the local racks and cards. */
  sts = io_init_agent( node, *ctx, NULL, io_eType_Node, 0);
  if ( EVEN(sts)) return sts;

  /* Build the io context tree for the remote racks and cards. */
  sts = io_trv_child( node, 1, io_init_agent, *ctx, NULL, io_eType_Agent, 0);

/*
  sts = io_init_signals();
  if ( EVEN(sts)) return sts;
*/

  /* Call the init methods for racks and cards */
  rw_action_found = 0;
  for ( ap = (*ctx)->agentlist; ap != NULL; ap = ap->next)
  {
    if ( ap->Process & process)
    {
      if ( ap->Init != NULL)
      {
        sts = (ap->Init) ( *ctx, ap);
        if ( EVEN(sts)) return sts;
        if (ap->Action & io_mAction_Read || ap->Action & io_mAction_Write)
          rw_action_found = 1;
      }
      for ( rp = ap->racklist; rp != NULL; rp = rp->next)
      {
        if ( rp->Process & process)
        {
          if ( rp->Init != NULL)
          {
            sts = (rp->Init) ( *ctx, ap, rp);
            if ( EVEN(sts)) return sts;

            if ( rp->Action & io_mAction_Read || rp->Action & io_mAction_Write)
              rw_action_found = 1;
          }
          for ( cp = rp->cardlist; cp != NULL; cp = cp->next)
          {
            if ( cp->Init != NULL)
            {
              sts = (cp->Init) ( *ctx, ap, rp, cp);
              if ( EVEN(sts)) return sts;

              if ( cp->Action & io_mAction_Read || cp->Action & io_mAction_Write)
                rw_action_found = 1;
	    }
          }
        }
      }
    }
  }
  if ( !rw_action_found)
  {
    errh_Info("IO init: no read or write actions found for this process");
    return IO__NO_RWACTION;
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Initialize io racks and cards.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_init_swap ( 
  io_mProcess	process,
  pwr_tObjid	thread,
  io_tCtx 	*ctx,
  int		relativ_vector,
  float		scan_time)
{
  pwr_tObjid 	node;
  pwr_sNode	*node_op;
  pwr_sClass_IOHandler	*io_op;
  pwr_tStatus	sts;
  io_sAgent	*ap;

  if ( scan_time <= 0)
    return IO__TIMEINVALID;

  sts = io_get_iohandler_object(&io_op, NULL);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetNodeObject( 0, &node);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( node, (void *) &node_op);
  if ( EVEN(sts)) return sts;

  *ctx = calloc( 1, sizeof(**ctx));
  (*ctx)->Process = process;
  (*ctx)->RelativVector = relativ_vector;
  (*ctx)->Node = node_op;
  (*ctx)->IOHandler = io_op;
  (*ctx)->ScanTime = scan_time;
  (*ctx)->Thread = thread;

  if ( (*ctx)->Node->EmergBreakTrue || !(*ctx)->IOHandler->IOReadWriteFlag)
    return IO__IS_STOPPED;

  /* Traverse all objects in the NodeHierarchy, find methods and build
     the io context tree for the local racks and cards. */
  sts = io_init_agent( node, *ctx, NULL, io_eType_Node, 1);
  if ( EVEN(sts)) return sts;

  /* Build the io context tree for the remote racks and cards. */
  sts = io_trv_child( node, 1, io_init_agent, *ctx, NULL, io_eType_Agent, 1);

  /* Call the swap method for agents */

  for ( ap = (*ctx)->agentlist; ap != NULL; ap = ap->next)
  {
    if (ap->Action & io_mAction_Swap)
    {
      sts = (ap->Swap) ( *ctx, ap);
      if ( EVEN(sts)) return sts;
    }
  }

  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Read io racks and cards.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_read(
  io_tCtx 	ctx)
{
  pwr_tStatus	sts;
  io_sAgent	*ap;
  io_sRack	*rp;
  io_sCard	*cp;

  if ( ctx->Node->EmergBreakTrue || !ctx->IOHandler->IOReadWriteFlag)
    return IO__IS_STOPPED;

  /* Call the read methods for agents, racks and cards */
  for ( ap = ctx->agentlist; ap != NULL; ap = ap->next)
  {
    if ( ap->Process & ctx->Process &&
         ap->Action & io_mAction_Read)
    {
      if ( ap->scan_interval_cnt <= 1)
      {
        sts = (ap->Read) ( ctx, ap);
        if ( EVEN(sts)) return sts;
        ap->scan_interval_cnt = ap->scan_interval;
      }
      else
        ap->scan_interval_cnt--;
    }

    for ( rp = ap->racklist; rp != NULL; rp = rp->next)
    {
      if ( rp->Process & ctx->Process &&
           rp->Action & io_mAction_Read)
      {
        if ( rp->scan_interval_cnt <= 1)
        {
          sts = (rp->Read) ( ctx, ap, rp);
          if ( EVEN(sts)) return sts;
          rp->scan_interval_cnt = rp->scan_interval;
        }
        else
          rp->scan_interval_cnt--;
      }

      for ( cp = rp->cardlist; cp != NULL; cp = cp->next)
      {
        if ( cp->Action & io_mAction_Read)
        {
          if ( cp->scan_interval_cnt <= 1)
          {
            sts = (cp->Read) ( ctx, ap, rp, cp);
            if ( EVEN(sts)) return sts;
            cp->scan_interval_cnt = cp->scan_interval;
          }
          else
            cp->scan_interval_cnt--;
        }
      }
    }
  }

  /* Scan the Sup lists */
  /* if ( ctx->Process != io_mProcess_Plc) */
  sts = io_ScanSupLst( ctx->SupCtx);

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Write io racks and cards.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_write(
  io_tCtx 	ctx)
{
  pwr_tStatus	sts;
  io_sAgent	*ap;
  io_sRack	*rp;
  io_sCard	*cp;

  if ( ctx->Node->EmergBreakTrue || !ctx->IOHandler->IOReadWriteFlag)
    return IO__IS_STOPPED;

  /* Call the write methods for agents, racks and cards, in reverse order */
  for ( ap = ctx->agentlist; ap != NULL; ap = ap->next)
  {
    for ( rp = ap->racklist; rp != NULL; rp = rp->next)
    {
      for ( cp = rp->cardlist; cp != NULL; cp = cp->next)
      {
        if ( cp->Action & io_mAction_Write)
        {
          if ( cp->scan_interval_cnt <= 1)
          {
            sts = (cp->Write) ( ctx, ap, rp, cp);
            if ( EVEN(sts)) return sts;
            cp->scan_interval_cnt = cp->scan_interval;
          }
          else
            cp->scan_interval_cnt--;
        }
      }
      if ( rp->Process & ctx->Process &&
           rp->Action & io_mAction_Write)
      {
        if ( rp->scan_interval_cnt <= 1)
        {
          sts = (rp->Write) ( ctx, ap, rp);
          if ( EVEN(sts)) return sts;
          rp->scan_interval_cnt = rp->scan_interval;
        }
        else
          rp->scan_interval_cnt--;
      }

    }
    if ( ap->Process & ctx->Process &&
         ap->Action & io_mAction_Write)
    {
      if ( ap->scan_interval_cnt <= 1)
      {
        sts = (ap->Write) ( ctx, ap);
        if ( EVEN(sts)) return sts;
        ap->scan_interval_cnt = ap->scan_interval;
      }
      else
        ap->scan_interval_cnt--;
    }

  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Swap io racks and cards.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_swap(
  io_tCtx 	ctx)
{
  pwr_tStatus	sts;
  io_sAgent	*ap;
  io_sRack	*rp;
  io_sCard	*cp;

  if ( ctx->Node->EmergBreakTrue || !ctx->IOHandler->IOReadWriteFlag)
    return IO__IS_STOPPED;

  /* Call the read methods for agents, racks and cards */
  for ( ap = ctx->agentlist; ap != NULL; ap = ap->next)
  {
    if (ap->Action & io_mAction_Swap)
    {
      if ( ap->scan_interval_cnt <= 1)
      {
        sts = (ap->Swap) ( ctx, ap);
        if ( EVEN(sts)) return sts;
        ap->scan_interval_cnt = ap->scan_interval;
      }
      else
        ap->scan_interval_cnt--;
    }

    for ( rp = ap->racklist; rp != NULL; rp = rp->next)
    {
      if (rp->Action & io_mAction_Swap)
      {
        if ( rp->scan_interval_cnt <= 1)
        {
          sts = (rp->Swap) ( ctx, ap, rp);
          if ( EVEN(sts)) return sts;
          rp->scan_interval_cnt = rp->scan_interval;
        }
        else
          rp->scan_interval_cnt--;
      }

      for ( cp = rp->cardlist; cp != NULL; cp = cp->next)
      {
        if (cp->Action & io_mAction_Swap)
        {
          if ( cp->scan_interval_cnt <= 1)
          {
            sts = (cp->Swap) ( ctx, ap, rp, cp);
            if ( EVEN(sts)) return sts;
            cp->scan_interval_cnt = cp->scan_interval;
          }
          else
            cp->scan_interval_cnt--;
	}
      }
    }
  }

  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Close Io.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_close(
  io_tCtx 	ctx)
{
  int		i;
  pwr_tStatus	sts;
  io_sAgent	*ap;
  io_sRack	*rp;
  io_sCard	*cp;
  io_sAgent	*ap_next;
  io_sRack	*rp_next;
  io_sCard	*cp_next;

  /* Call the write methods for agents, racks and cards, in reverse order */
  for ( ap = ctx->agentlist; ap != NULL; ap = ap->next)
  {
    for ( rp = ap->racklist; rp != NULL; rp = rp->next)
    {
      for ( cp = rp->cardlist; cp != NULL; cp = cp->next)
      {
        if ( cp->Close)
        {
          sts = (cp->Close) ( ctx, ap, rp, cp);
          if ( EVEN(sts)) return sts;
        }
      }
      if ( rp->Process & ctx->Process &&
           rp->Close)
      {
        sts = (rp->Close) ( ctx, ap, rp);
        if ( EVEN(sts)) return sts;
      }

    }
    if ( ap->Process & ctx->Process &&
         ap->Close)
    {
      sts = (ap->Close) ( ctx, ap);
      if ( EVEN(sts)) return sts;
    }

  }

  /* if ( ctx->Process != io_mProcess_Plc) */
  io_ClearSupLst( ctx->SupCtx);

  /* Free ctx */
  for ( ap = ctx->agentlist; ap != NULL; ap = ap_next)
  {
    for ( rp = ap->racklist; rp != NULL; rp = rp_next)
    {
      for ( cp = rp->cardlist; cp != NULL; cp = cp_next)
      {
        if ( cp->chanlist)
	{
	  for ( i = 0; i < cp->ChanListSize; i++)
	  {
	    if ( cp->chanlist[i].cop)
	    {
	      if ( cdh_RefIdIsNotNull( cp->chanlist[i].ChanDlid))
		gdh_DLUnrefObjectInfo( cp->chanlist[i].ChanDlid);
	      gdh_DLUnrefObjectInfo( cp->chanlist[i].SigDlid);
	    }
	  }
          free( (char *) cp->chanlist);
        }
	gdh_DLUnrefObjectInfo( cp->Dlid);
        cp_next = cp->next;
        free( (char *) cp);
      }
      gdh_DLUnrefObjectInfo( rp->Dlid);
      rp_next = rp->next;
      free( (char *) rp);
    }
    gdh_DLUnrefObjectInfo( ap->Dlid);
    ap_next = ap->next;
    free( (char *) ap);
  }
  free( ctx);

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Calculate polynomial coefficients for ai conversion.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_AiRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAi	*cop;
  pwr_tAName   		buf;
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for RawValue to SignalValue conversion */
    if ( cop->RawValRangeHigh != cop->RawValRangeLow)
    {
      cop->SigValPolyCoef1 =
	(cop->ChannelSigValRangeHigh - cop->ChannelSigValRangeLow) /
	(cop->RawValRangeHigh - cop->RawValRangeLow);
      cop->SigValPolyCoef0 = cop->ChannelSigValRangeHigh - 
	cop->RawValRangeHigh * cop->SigValPolyCoef1;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid RawValueRange in Ai channel %s", buf);
      return IO__CHANRANGE;
    }

    /* Coef for RawValue to ActualValue conversion */
    if ( chanp->ChanClass != pwr_cClass_ChanAit && cop->SensorPolyType == 1)
    {
      if ( cop->SensorSigValRangeHigh != cop->SensorSigValRangeLow)
      {
        PolyCoef1 = (cop->ActValRangeHigh - cop->ActValRangeLow)/
		(cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow);
        PolyCoef0 = cop->ActValRangeHigh - cop->SensorSigValRangeHigh *
		PolyCoef1;
        cop->SensorPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
        cop->SensorPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
      }
      else
      {
        sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;
 	errh_Error( "Invalid SensorSigValueRange in Ai channel %s", buf);
        return IO__CHANRANGE;
      }
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Calculate polynomial coefficients for ao conversion.
\*----------------------------------------------------------------------------*/

pwr_tStatus io_AoRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAo	*cop;
  pwr_tAName   		buf;
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for ActualValue to RawValue conversion */
    if ( cop->ActValRangeHigh != cop->ActValRangeLow)
    {
      cop->SigValPolyCoef1 = (cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow)/
		(cop->ActValRangeHigh - cop->ActValRangeLow);
      cop->SigValPolyCoef0 = cop->SensorSigValRangeHigh - cop->ActValRangeHigh *
		cop->SigValPolyCoef1;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid ActValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
    /* Coef for ActualValue to SignalValue conversion */
    if ( cop->ChannelSigValRangeHigh != 0)
    {
      PolyCoef1 =
	(cop->RawValRangeHigh - cop->RawValRangeLow) /
	(cop->ChannelSigValRangeHigh - cop->ChannelSigValRangeLow);
      PolyCoef0 = cop->RawValRangeHigh - cop->ChannelSigValRangeHigh *
		PolyCoef1;
      cop->OutPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
      cop->OutPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid ChannelSigValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Convert ai from rawvalue to actualvalue.
\*----------------------------------------------------------------------------*/

void io_ConvertAi (
  pwr_sClass_ChanAi  *cop,
  pwr_tInt16	      rawvalue,
  pwr_tFloat32	      *actvalue_p
) 
{
  pwr_tFloat32		sigvalue;
  pwr_tFloat32		actvalue;
  pwr_tFloat32		*polycoef_p;
  int			i;

  switch ( cop->SensorPolyType)
  {
    case 0:
      *actvalue_p = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * rawvalue;
      break;
    case 1:
      *actvalue_p = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * rawvalue;
      break;
    case 2:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * rawvalue;
      polycoef_p = &cop->SensorPolyCoef2;
      actvalue = 0;
      for ( i = 0; i < 3; i++)
      {
        actvalue = sigvalue * actvalue + *polycoef_p;
        polycoef_p--;
      }
      *actvalue_p = actvalue;
      break;
    case 3:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * rawvalue;
      actvalue = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        *actvalue_p = cop->SensorPolyCoef2 * sqrt( actvalue);
      else
        *actvalue_p = 0;
      break;      
    case 4:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * rawvalue;
      actvalue = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        *actvalue_p = cop->SensorPolyCoef2 * sqrt( actvalue);
      else
        *actvalue_p = -cop->SensorPolyCoef2 * sqrt( -actvalue);
      break;      
  }
}


/*----------------------------------------------------------------------------*\
  Convert ait from rawvalue to actualvalue.
\*----------------------------------------------------------------------------*/

void io_ConvertAit (
  pwr_sClass_ChanAit  *cop,
  pwr_tInt16	      rawvalue,
  pwr_tFloat32	      *actvalue_p
) 
{
  pwr_tFloat32	Slope;
  pwr_tFloat32  Intercept;
  pwr_tFloat32  TransTabValue;
  pwr_tUInt16	First = 0;
  pwr_tUInt16	Middle;
  pwr_tUInt16	Last;
  pwr_tFloat32  sigvalue;

  sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * rawvalue;

  Last = cop->NoOfCoordinates;

  while ( (Last - First) > 1) {
    Middle = (First+Last) / 2;
    TransTabValue = cop->InValue[Middle];
    if (sigvalue < TransTabValue)
      Last = Middle;
    else
      First = Middle;
  }
  
  Intercept = cop->Intercept[First];
  Slope = cop->Slope[First];
  *actvalue_p = Intercept + sigvalue * Slope; 
}

/*----------------------------------------------------------------------------*\
  Print io methods
\*----------------------------------------------------------------------------*/
void io_methods_print()
{
  int i, j;

  printf( "Base Methods\n");
  for ( i = 0;; i++) {
    if (pwr_gBase_IoClassMethods[i].ClassName[0] == 0)
      break;
    printf( "  %3d %-20s ", i, pwr_gBase_IoClassMethods[i].ClassName);
    for ( j = 0;; j++) {
      if ((*pwr_gBase_IoClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      printf( "%s ", (*pwr_gBase_IoClassMethods[i].Methods)[j].MethodName);
    }
    printf( "\n");
  }
  printf( "\nUser Methods\n");
  for ( i = 0;; i++) {
    if (pwr_gUser_IoUserClassMethods[i].ClassName[0] == 0)
      break;
    printf( "  %3d %-20s ", i, pwr_gUser_IoUserClassMethods[i].ClassName);
    for ( j = 0;; j++) {
      if ((*pwr_gUser_IoUserClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      printf( "%s ", (*pwr_gUser_IoUserClassMethods[i].Methods)[j].MethodName);
    }
    printf( "\n");
  }
}
