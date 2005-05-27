/* rt_io_base.c -- io base routines.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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

pwr_dImport pwr_BindIoMethods(Node);
pwr_dImport pwr_BindIoMethods(Rack_SSAB);
pwr_dImport pwr_BindIoMethods(Di_DIX2);
pwr_dImport pwr_BindIoMethods(Do_HVDO32);
pwr_dImport pwr_BindIoMethods(Ao_HVAO4);
pwr_dImport pwr_BindIoMethods(Ao_AO8uP);  // Share methods with HVAO4
pwr_dImport pwr_BindIoMethods(Ai_HVAI32);
pwr_dImport pwr_BindIoMethods(Ai_AI32uP);
pwr_dImport pwr_BindIoMethods(Co_PI24BO);
pwr_dImport pwr_BindIoMethods(Co_CO4uP);  // Share methods with PI24BO
pwr_dImport pwr_BindIoMethods(Pb_Profiboard);
pwr_dImport pwr_BindIoMethods(Pb_DP_Slave);
pwr_dImport pwr_BindIoMethods(Pb_Module);
pwr_dImport pwr_BindIoMethods(Pb_Di);
pwr_dImport pwr_BindIoMethods(Pb_Do);
pwr_dImport pwr_BindIoMethods(Pb_Ai);
pwr_dImport pwr_BindIoMethods(Pb_Ao);
pwr_dImport pwr_BindIoMethods(Pb_Ii);
pwr_dImport pwr_BindIoMethods(Pb_Io);

pwr_BindIoClasses(Base) = {
  pwr_BindIoClass(Node),
  pwr_BindIoClass(Rack_SSAB),
  pwr_BindIoClass(Di_DIX2),
  pwr_BindIoClass(Do_HVDO32),
  pwr_BindIoClass(Ao_HVAO4),
  pwr_BindIoClass(Ao_AO8uP),
  pwr_BindIoClass(Ai_HVAI32),
  pwr_BindIoClass(Ai_AI32uP),
  pwr_BindIoClass(Co_PI24BO),
  pwr_BindIoClass(Co_CO4uP),
  pwr_BindIoClass(Pb_Profiboard),
  pwr_BindIoClass(Pb_DP_Slave),
  pwr_BindIoClass(Pb_Module),
  pwr_BindIoClass(Pb_Di),
  pwr_BindIoClass(Pb_Do),
  pwr_BindIoClass(Pb_Ai),
  pwr_BindIoClass(Pb_Ao),
  pwr_BindIoClass(Pb_Ii),
  pwr_BindIoClass(Pb_Io),
  pwr_NullClass
};

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
  int		arg3);

static pwr_tStatus io_replace_symbol( pwr_sAttrRef *chan, pwr_sAttrRef *sig)
{
  if ( chan->Objid.vid == cdh_cIoConnectVolume ) {
    // Replace with attrref in IoConnected
    char name[120];
    pwr_tStatus sts;
    pwr_sAttrRef connect;
    pwr_tCid cid;

    sts = gdh_ObjidToName( sig->Objid, name, sizeof(name),
			   cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    strcat( name, ".IoConnect");
    sts = gdh_GetObjectInfo( name, &connect, sizeof(connect));
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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;
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
  char				buf[140];
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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->RawValue, &area_op->Value[sig_count]);
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->AbsValue, &abs_area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;
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
  pwr_sClass_IvArea		*iarea_op;

 
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
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&o->ActualValue, p);
      o->ValueIndex = av_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[av_count], &o->InitialValue);
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
  pwr_sClass_IvArea	    *iarea_op;
 
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
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&o->ActualValue, p);
      o->ValueIndex = dv_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[dv_count], &o->InitialValue);
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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
  char				buf[140];
  pwr_tUInt32			sig_count = 0;
  pwr_tClassId			class;
  pwr_sClass_IvArea		*iarea_op;

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
	errh_Info("IO init: Signal SigChanCon error '%s'", buf);
      }
      else {
	sts = gdh_AttrRefToPointer( &sig_op->SigChanCon, (void *) &chan_op);
	if (EVEN(sts)) {
	  errh_Info("IO init: Signal SigChanCon error '%s'", buf);
	}	
	else
	  chan_op->SigChanCon = sig_aref;
      }		
    }
    gdh_StoreRtdbPointer( (pwr_tUInt32 *) &sig_op->ActualValue, &area_op->Value[sig_count]);
    sig_op->ValueIndex = sig_count;

    gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[sig_count], &sig_op->InitialValue);

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
  pwr_sClass_IvArea	    *iarea_op;
 
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
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&o->ActualValue, p);
      o->ValueIndex = iv_count;
#if 0
      *p = o->InitialValue;
#endif
      gdh_StoreRtdbPointer( (pwr_tUInt32 *)&iarea_op->Value[iv_count], &o->InitialValue);
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
  pwr_tStatus 	(** Write) ()
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

  for ( k = 0; k < 4; k++) {
    // Init, Close, Read, Write loop
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

      sts = gdh_GetSuperClass( cid, &cid);
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
    }
  }
  if ( !found)
    return IO__NOMETHOD;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Find classes belonging to an specific IO-type.
\*----------------------------------------------------------------------------*/
pwr_tStatus io_GetIoTypeClasses( 
  io_eType	type,
  pwr_tClassId 	**classes,
  int		*size
)
{
  pwr_tStatus 	sts;
  pwr_sClassDef cdef;
  pwr_tOid	oid;
  int		add_class;
  pwr_sAttrRef  aref;

  *size = 0;
  *classes = calloc( IO_CLASSES_SIZE, sizeof(pwr_tCid));

  for ( sts = gdh_GetClassList( pwr_eClass_ClassDef, &oid);
	ODD(sts);
	sts = gdh_GetNextObject( oid, &oid)) {
    
    aref = cdh_ObjidToAref( oid);
    sts = gdh_GetObjectInfoAttrref( &aref, &cdef, sizeof(cdef));
    if ( EVEN(sts)) return sts;

    add_class = 0;
    switch ( type) {
    case io_eType_Agent:
      if ( cdef.Flags.b.IOAgent)
	add_class = 1;
      break;
    case io_eType_Rack:
      if ( cdef.Flags.b.IORack)
	add_class = 1;
      break;
    case io_eType_Card:
      if ( cdef.Flags.b.IOCard)
	add_class = 1;
      break;
    default:
      return IO__NOMETHOD;
    }

    if ( add_class) {
      if ( *size >= IO_CLASSES_SIZE)
	return IO__CLASSEXCEED;

      (*classes)[ *size] = cdh_ClassObjidToId( oid);
      (*size)++;
    }
  }
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Check if class is an IO-type.
\*----------------------------------------------------------------------------*/
int io_CheckClassIoType( 
  io_eType	type,
  pwr_tCid 	cid
)
{
  pwr_tStatus 	sts;
  pwr_sClassDef cdef;
  pwr_sAttrRef aref;

  aref = cdh_ObjidToAref( cdh_ClassIdToObjid( cid));
  sts = gdh_GetObjectInfoAttrref( &aref, &cdef, sizeof(cdef));
  if ( EVEN(sts)) return 0;

  switch ( type) {
  case io_eType_Agent:
    if ( cdef.Flags.b.IOAgent)
      return 1;
    break;
  case io_eType_Rack:
    if ( cdef.Flags.b.IORack)
      return 1;
    break;
  case io_eType_Card:
    if ( cdef.Flags.b.IOCard)
      return 1;
    break;
  default: ;
  }
  return 0;
}


/*----------------------------------------------------------------------------*\
  Insert a card object into the context list.
\*----------------------------------------------------------------------------*/
static pwr_tStatus io_init_card(
  pwr_tObjid	objid, 
  io_tCtx	ctx,
  io_sRack	*rp,
  int		agent_type)
{
  pwr_tStatus 	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* CardInit) ();
  pwr_tStatus 	(* CardClose) ();
  pwr_tStatus 	(* CardRead) ();
  pwr_tStatus 	(* CardWrite) ();
  char		cname[140];
  char		attrname[140];
  pwr_tUInt32	process = 0;
  io_sCard	*cp;
  io_sCard	*clp;
  pwr_sAttrRef	attrref;
  pwr_tObjid	chan;
  pwr_tDlid	chandlid;
  pwr_tDlid	sigdlid;
  pwr_tUInt32	number;
  pwr_tUInt16	maxchan;
  pwr_tObjid	sigchancon;
  void		*chan_op;
  void		*sig_op;
  io_sChannel	*chanp;
  pwr_tClassId	sigclass;
  pwr_tFloat32	scantime;
  pwr_tObjid	thread;
  int		ok;

  sts = gdh_GetObjectClass( objid, &class);
  if ( EVEN(sts)) return sts;

  if ( io_CheckClassIoType( io_eType_Card, class)) {
    sts = io_FindMethods( class, io_eType_Card, 
			  &CardInit, &CardClose, &CardRead, &CardWrite);
    if ( ODD(sts)) {
      if ( CardInit != NULL || CardClose != NULL || CardRead != NULL || CardWrite != NULL) {
	/* This is a card object */
	/* Check if the rack should be handled by this process */      
      
	sts = gdh_ObjidToName( objid, cname, sizeof(cname),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;

	ok = 0;
	strcpy( attrname, cname);
	strcat( attrname, ".Process");
	sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
	if ( (EVEN(sts) && ctx->Process == io_mProcess_User) ||
	     (ODD(sts) && ctx->Process == (int) process)) {
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

	if ( ok) {
	  /* Tread this card in this process */
	  strcpy( attrname, cname);
	  strcat( attrname, ".MaxNoOfChannels");
	  sts = gdh_GetObjectInfo( attrname, &maxchan, sizeof(maxchan));
	  if ( EVEN(sts)) {
	    strcpy( attrname, cname);
	    strcat( attrname, ".MaxNoOfCounters");
	    sts = gdh_GetObjectInfo( attrname, &maxchan, sizeof(maxchan));
	    if ( EVEN(sts))
	      maxchan = IO_CHANLIST_SIZE;
	  }

	  cp = calloc( 1, sizeof(io_sCard));
	  cp->chanlist = (io_sChannel *) calloc( maxchan, sizeof(io_sChannel));
	  cp->ChanListSize = maxchan;
	  cp->Class = class;
	  cp->Local = NULL;
	  cp->Objid = objid;
	  strcpy( cp->Name, cname);
	  if ( CardRead != NULL)
	    cp->Action |= io_mAction_Read;
	  if ( CardWrite != NULL)
	    cp->Action |= io_mAction_Write;
	  cp->Init = CardInit;
	  cp->Close = CardClose;
	  cp->Read = CardRead;
	  cp->Write = CardWrite;
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
	      sigchancon = ((pwr_sClass_ChanAi *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanAi *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanAit:
	      sigchancon = ((pwr_sClass_ChanAit *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanAit *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanAo:
	      sigchancon = ((pwr_sClass_ChanAo *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanAo *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanDo:
	      sigchancon = ((pwr_sClass_ChanDo *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanDo *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanDi:
	      sigchancon = ((pwr_sClass_ChanDi *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanDi *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanIi:
	      sigchancon = ((pwr_sClass_ChanIi *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanIi *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanIo:
	      sigchancon = ((pwr_sClass_ChanIo *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanIo *) chan_op)->Number;
	      break;
	    case pwr_cClass_ChanCo:
	      sigchancon = ((pwr_sClass_ChanCo *) chan_op)->SigChanCon.Objid;
	      number = ((pwr_sClass_ChanCo *) chan_op)->Number;
	      break;
	    default:
	      sts = gdh_DLUnrefObjectInfo( chandlid);
	      sts = gdh_GetNextSibling( chan, &chan);
	      continue;
	    }
	    
	    if ( (int) number > maxchan-1) {
	      /* Number out of range */
	      errh_Error( 
			 "IO init error: number out of range %s, chan nr %d", 
			 cp->Name, number);
	      sts = gdh_DLUnrefObjectInfo( chandlid);
	      sts = gdh_GetNextSibling( chan, &chan);
	      continue;
	    }
	    /* Find signal */
	    if ( cdh_ObjidIsNull( sigchancon)) {
	      /* Not connected */
	      sts = gdh_DLUnrefObjectInfo( chandlid);
	      sts = gdh_GetNextSibling( chan, &chan);
	      continue;
	    }
	    
	    sts = gdh_GetObjectClass( sigchancon, &sigclass);
	    if ( EVEN(sts)) {
	      sts = gdh_DLUnrefObjectInfo( chandlid);
	      sts = gdh_GetNextSibling( chan, &chan);
	      continue;
	    }

	    memset( &attrref, 0, sizeof(attrref));
	    attrref.Objid = sigchancon;
	    sts = gdh_DLRefObjectInfoAttrref( &attrref, (void *) &sig_op, &sigdlid);
	    if ( EVEN(sts)) {
	      sts = gdh_DLUnrefObjectInfo( chandlid);
	      sts = gdh_GetNextSibling( chan, &chan);
	      continue;
	    }
	  
	    /* Insert */
	    chanp = &cp->chanlist[number];
	    chanp->cop = chan_op;
	    chanp->ChanDlid = chandlid;
	    chanp->ChanObjid = chan;
	    chanp->sop = sig_op;
	    chanp->SigDlid = sigdlid;
	    chanp->SigObjid = sigchancon;
	    chanp->ChanClass = class;
	    chanp->SigClass = sigclass;
	    switch( sigclass) {
	    case pwr_cClass_Di:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Di *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Do:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Do *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Po:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Po *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Ai:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Ai *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Ao:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Ao *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Ii:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Ii *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Io:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Io *)sig_op)->ActualValue);
	      break;
	    case pwr_cClass_Co:
	      chanp->vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Co *)sig_op)->RawValue);
	      chanp->abs_vbp = gdh_TranslateRtdbPointer( 
		(pwr_tUInt32) ((pwr_sClass_Co *)sig_op)->AbsValue);
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
	    io_ConnectToSupLst( ctx->SupCtx, sigclass, sigchancon, sig_op);

	    sts = gdh_GetNextSibling( chan, &chan);
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
  int		agent_type)
{
  pwr_tStatus	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* RackInit) ();
  pwr_tStatus 	(* RackClose) ();
  pwr_tStatus 	(* RackRead) ();
  pwr_tStatus 	(* RackWrite) ();
  char		rname[140];
  char		attrname[140];
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
    sts = io_FindMethods( class, io_eType_Rack, &RackInit, &RackClose, &RackRead, &RackWrite);
    if ( ODD(sts)) {
      if ( RackInit != NULL || RackClose != NULL || RackRead != NULL || RackWrite != NULL) {
	/* This is a rack object,  */
	/* Check if the rack should be handled by this process */      
      
	sts = gdh_ObjidToName( objid, rname, sizeof(rname),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;

	ok = 0;
	strcpy( attrname, rname);
	strcat( attrname, ".Process");

	sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
	if ( EVEN(sts) ||
	     (ODD(sts) && ctx->Process & process)) {
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

	if ( ok) {

	  /* Treat this rack in this process */
	  rp = calloc( 1, sizeof(io_sRack));
	  rp->Local = NULL;
	  rp->Class = class;
	  rp->Objid = objid;
	  strcpy( rp->Name, rname);
	  rp->Process = process;
	  if ( RackRead != NULL)
	    rp->Action |= io_mAction_Read;
	  if ( RackWrite != NULL)
	    rp->Action |= io_mAction_Write;
	  rp->Init = RackInit;
	  rp->Close = RackClose;
	  rp->Read = RackRead;
	  rp->Write = RackWrite;
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
	  
	  sts = io_trv_child( objid, 0, io_init_card, ctx, rp, agent_type);
	}
	return IO__TRV_NEXT;
      }
    }
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
  int		agent_type)
{
  pwr_tStatus	sts;
  pwr_tClassId	class;  
  pwr_tStatus 	(* AgentInit) ();
  pwr_tStatus 	(* AgentClose) ();
  pwr_tStatus 	(* AgentRead) ();
  pwr_tStatus 	(* AgentWrite) ();
  char		aname[140];
  char		attrname[140];
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
    sts = io_FindMethods( class, io_eType_Agent, &AgentInit, &AgentClose, &AgentRead, &AgentWrite);
    if ( ODD(sts)) {
      if ( AgentInit != NULL || AgentClose != NULL || AgentRead != NULL || AgentWrite != NULL) {
	/* This is a agent object or the node object,  */
	/* Check if the agent should be handled by this process */ 
      
	sts = gdh_ObjidToName( objid, aname, sizeof(aname),
			       cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;

	ok = 0;
	strcpy( attrname, aname);
	strcat( attrname, ".Process");
	sts = gdh_GetObjectInfo( attrname, &process, sizeof(process));
	if ( EVEN(sts) ||
	     (ODD(sts) && ctx->Process & process)) {
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

	if ( ok) {
	       
	  /* Treat this agent in this process */
	  ap = calloc( 1, sizeof(io_sAgent));
	  ap->Class = class;
	  ap->Local = NULL;
	  ap->Objid = objid;
	  strcpy( ap->Name, aname);
	  ap->Process = process;
	  if ( AgentRead != NULL)
	    ap->Action |= io_mAction_Read;
	  if ( AgentWrite != NULL)
	    ap->Action |= io_mAction_Write;
	  ap->Init = AgentInit;
	  ap->Close = AgentClose;
	  ap->Read = AgentRead;
	  ap->Write = AgentWrite;
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
	  
	  sts = io_trv_child( objid, 0, io_init_rack, ctx, ap, agent_type);
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
  int		arg3)
{
  pwr_tObjid	child;
  pwr_tStatus 	sts;
  
  sts = gdh_GetChild( parent, &child);
  while ( ODD(sts))
  {
    sts = (func) ( child, arg1, arg2, arg3);
    if ( EVEN(sts)) return sts;

    if ( deep && sts != IO__TRV_NEXT)
    {
      sts = io_trv_child( child, deep, func, arg1, arg2, arg3);
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
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 64) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 128) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 256) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 512) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1024) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2048) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4096) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8192) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16384) != 0);
    chanp++;
    if ( chanp->cop)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32768) != 0);
    chanp++;
  }
  else
  {
    if ( chanp->cop && mask & 1)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1) != 0);
    chanp++;
    if ( chanp->cop && mask & 2)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2) != 0);
    chanp++;
    if ( chanp->cop && mask & 4)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4) != 0);
    chanp++;
    if ( chanp->cop && mask & 8)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8) != 0);
    chanp++;
    if ( chanp->cop && mask & 16)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16) != 0);
    chanp++;
    if ( chanp->cop && mask & 32)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 32) != 0);
    chanp++;
    if ( chanp->cop && mask & 64)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 64) != 0);
    chanp++;
    if ( chanp->cop && mask & 128)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 128) != 0);
    chanp++;
    if ( chanp->cop && mask & 256)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 256) != 0);
    chanp++;
    if ( chanp->cop && mask & 512)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 512) != 0);
    chanp++;
    if ( chanp->cop && mask & 1024)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 1024) != 0);
    chanp++;
    if ( chanp->cop && mask & 2048)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 2048) != 0);
    chanp++;
    if ( chanp->cop && mask & 4096)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 4096) != 0);
    chanp++;
    if ( chanp->cop && mask & 8192)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 8192) != 0);
    chanp++;
    if ( chanp->cop && mask & 16384)
      * (pwr_tUInt16 *) (chanp->vbp) = ((data & 16384) != 0);
    chanp++;
    if ( chanp->cop && mask & 32768)
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
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 32;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 64;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 128;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 256;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 512;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1024;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2048;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4096;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8192;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16384;
  chanp++;
  if ( chanp->cop && * (pwr_tUInt16 *) (chanp->vbp))
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
  sts = io_init_agent( node, *ctx, NULL, io_eType_Node);
  if ( EVEN(sts)) return sts;

  /* Build the io context tree for the remote racks and cards. */
  sts = io_trv_child( node, 1, io_init_agent, *ctx, NULL, io_eType_Agent);

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
  char			buf[120];
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
      sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
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
        sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
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
  char			buf[120];
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
      sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
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
      sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
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

