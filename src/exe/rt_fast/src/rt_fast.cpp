
#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "rt_fast.h"
#include "co_error.h"
extern "C" {
#include "rt_gdh.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_ini_event.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"
#include "rt_qcom_msg.h"
}

void fastobject::open( double base_scantime)
{
  pwr_tStatus sts;
  pwr_tUInt32 size, offs, elem;
  pwr_tTypeId type_id;
  char name[120];

  // Link to object
  sts = gdh_DLRefObjectInfoAttrref( &aref, (void **)&p, &p_dlid); 
  if ( EVEN(sts)) throw co_error(sts);

  // Link to trigg object
  if ( cdh_ObjidIsNotNull( p->TriggObject.Objid)) {
    sts = gdh_DLRefObjectInfoAttrref( &p->TriggObject, (void **)&trigg, &trigg_dlid); 
    if ( EVEN(sts)) {
      if ( p->Function & fast_mFunction_ManTrigg || 
	   p->Function & fast_mFunction_LevelTrigg)
	trigg = 0;
      else
	throw co_error(sts);
    }
  }

  // Link to attributes
  for ( int i = 0; i < FAST_CURVES; i++) {
    if ( cdh_ObjidIsNotNull( p->Attribute[i].Objid)) {
      sts = gdh_DLRefObjectInfoAttrref( &p->Attribute[i], (void **)&attributes[i], &attributes_dlid[i]);
      if ( EVEN(sts)) throw co_error(sts);

      // Get attribute type
      sts = gdh_AttrrefToName( &p->Attribute[i], name, sizeof( name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) throw co_error(sts);
      
      sts = gdh_GetAttributeCharacteristics( name, &p->AttributeType[i],
					     &attributes_size[i], &offs, &elem);
      if ( EVEN(sts)) throw co_error(sts);

      p->CurveValid[i] = true;
    }
  }

  // Link to time buffer
  if ( cdh_ObjidIsNotNull( p->TimeBuffer.Objid)) {
    sts = gdh_DLRefObjectInfoAttrref( &p->TimeBuffer, (void **)&time_buffer, &time_buffer_dlid); 
    if ( EVEN(sts)) throw co_error(sts);

    // Get buffer size
    sts = gdh_AttrrefToName( &p->TimeBuffer, name, sizeof( name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) throw co_error(sts);

    sts = gdh_GetAttributeCharacteristics( name, &type_id, &size, &offs, &elem);
    if ( EVEN(sts)) throw co_error(sts);

    if ( size < p->NoOfPoints * sizeof(pwr_tFloat32))
      p->NoOfPoints = size / sizeof(pwr_tFloat32);
  }

  // Link to attribute buffers
  for ( int i = 0; i < FAST_CURVES; i++) {
    if ( cdh_ObjidIsNotNull( p->Buffers[i].Objid) && p->CurveValid[i]) {
      p->CurveValid[i] = false;
      sts = gdh_DLRefObjectInfoAttrref( &p->Buffers[i], (void **)&buffers[i], &buffers_dlid[i]); 
      if ( EVEN(sts)) throw co_error(sts);

      p->CurveValid[i] = true;

      // Get buffer size
      sts = gdh_AttrrefToName( &p->Buffers[i], name, sizeof( name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) throw co_error(sts);

      sts = gdh_GetAttributeCharacteristics( name, &type_id, &size, &offs, &elem);
      if ( EVEN(sts)) throw co_error(sts);

      if ( size < p->NoOfPoints * attributes_size[i])
	p->NoOfPoints = size / attributes_size[i];
    }
  }

  p->TriggMan = 0;
  p->Active = 0;
  p->Prepare = 0;
  p->TriggIndex = 0;
  if ( trigg)
    *trigg = 0;

  if ( p->ScanTime)
    scan_div = int( p->ScanTime / base_scantime + 0.5);
  else
    scan_div = 1;
  scan_base = base_scantime;
}

void fastobject::close()
{
  if ( trigg)
    gdh_DLUnrefObjectInfo( trigg_dlid);

  for ( int i = 0; i < FAST_CURVES; i++) {
    if ( p->CurveValid[i]) {
      gdh_DLUnrefObjectInfo( attributes_dlid[i]);
    }
  }
  gdh_DLUnrefObjectInfo( time_buffer_dlid);
  for ( int i = 0; i < FAST_CURVES; i++) {
    if ( p->CurveValid[i]) {
      gdh_DLUnrefObjectInfo( buffers_dlid[i]);
    }
  }
  for ( int i = 0; i < FAST_CURVES; i++)
    p->CurveValid[i] = 0;
  gdh_DLUnrefObjectInfo( p_dlid);
}

void fastobject::scan()
{
  int first = 0;

  if ( p->Active) {
    scan_cnt++;
    if ( scan_cnt >= scan_div)
      scan_cnt = 0;
    else
      return;
  }

  if ( p->Function & fast_mFunction_BeforeTrigg &&
       p->Function & fast_mFunction_AlwaysPrepared &&
       !p->New && !p->Prepare) {
    p->Prepare = 1;
    p->Active = 1;
    p->TriggIndex = fast_cNoTrigg;
  }

  if ( !p->Active || 
       (p->Function & fast_mFunction_BeforeTrigg && p->Prepare &&
	p->TriggIndex == fast_cNoTrigg)) {
    // Check trigg conditions

    if ( p->Function & fast_mFunction_BeforeTrigg && 
	 (p->Prepare && !old_prepare)) {
      // Start logging at index 0
      current_index = 0;
      p->Active = 1;
      p->FirstIndex = 0;
      p->LastIndex = 0;
      p->New = 0;
      p->TriggIndex = fast_cNoTrigg;
      clock_gettime( CLOCK_REALTIME, &prepare_time);
      first = 1;
      scan_cnt = 0;
    }
    if ( (p->Function & fast_mFunction_ManTrigg && p->TriggMan) ||
	 (trigg && *trigg && !old_trigg)) {
      if ( !( p->Function & fast_mFunction_BeforeTrigg)) {
	current_index = 0;
	p->Active = 1;
	p->FirstIndex = 0;
	p->LastIndex = 0;
	p->New = 0;
	clock_gettime( CLOCK_REALTIME, &prepare_time);
	scan_cnt = 0;
      }
      else {
	// Calculate stop index
	int points_before = p->NoOfPointsBeforeTrigg;
	if ( !points_before)
	  points_before++;

	p->TriggIndex = current_index;
	if ( (int)p->TriggIndex - points_before > 0)
	  stop_index = p->TriggIndex - points_before;
	else
	  stop_index = p->TriggIndex + p->NoOfPoints - points_before;
      }
      clock_gettime( CLOCK_REALTIME, &p->TriggTime);
      p->TriggMan = 0;
    }
    if ( p->Function & fast_mFunction_LevelTrigg &&
	 p->CurveValid[0] &&
	 p->AttributeType[0] == pwr_eType_Float32 &&
	 *(pwr_tFloat32 *)attributes[0] >= p->TriggLevel &&
	 old_level < p->TriggLevel) {
      if ( !( p->Function & fast_mFunction_BeforeTrigg)) {
	current_index = 0;
	p->Active = 1;
	p->FirstIndex = 0;
	p->LastIndex = 0;
	p->New = 0;
	clock_gettime( CLOCK_REALTIME, &prepare_time);
	scan_cnt = 0;
      }
      else {
	// Calculate stop index
	int points_before = p->NoOfPointsBeforeTrigg;
	if ( !points_before)
	  points_before++;

	p->TriggIndex = current_index;
	if ( (int)p->TriggIndex - points_before > 0)
	  stop_index = p->TriggIndex - points_before;
	else
	  stop_index = p->TriggIndex + p->NoOfPoints - points_before;
      }
      clock_gettime( CLOCK_REALTIME, &p->TriggTime);
    }
  }

  if ( p->Function & fast_mFunction_LevelTrigg &&
       p->CurveValid[0] &&
       p->AttributeType[0] == pwr_eType_Float32)
    old_level = *(pwr_tFloat32 *)attributes[0];

  if ( trigg)
    old_trigg = *trigg;
  old_prepare = p->Prepare;

  if ( p->New) {
    if ( new_cnt == int(2.0 / scan_base)) {
      new_cnt = 0;
      p->New = 0;
    }
    else
      new_cnt++;
  }

  if ( p->Active) {
    pwr_tTime now;
    pwr_tDeltaTime diff;
    pwr_tFloat32 fdiff;
    

    // Store time
    clock_gettime( CLOCK_REALTIME, &now);
    time_Adiff( &diff, &now, &prepare_time);
    time_DToFloat( &fdiff, &diff);
    ((pwr_tFloat32 *)time_buffer)[current_index] = fdiff;

    for ( int i = 0; i < FAST_CURVES; i++) {
      if ( p->CurveValid[i]) {
	switch ( p->AttributeType[i]) {
	case pwr_eType_Float32:
	  ((pwr_tFloat32 *)buffers[i])[current_index] = *(pwr_tFloat32 *)attributes[i];
	  break;
	case pwr_eType_Float64:
	  ((pwr_tFloat64 *)buffers[i])[current_index] = *(pwr_tFloat64 *)attributes[i];
	  break;
	case pwr_eType_UInt32:
	case pwr_eType_Int32:
	  ((pwr_tInt32 *)buffers[i])[current_index] = *(pwr_tInt32 *)attributes[i];
	  break;
	case pwr_eType_UInt16:
	case pwr_eType_Int16:
	  ((pwr_tInt16 *)buffers[i])[current_index] = *(pwr_tInt16 *)attributes[i];
	  break;
	case pwr_eType_UInt8:
	case pwr_eType_Int8:
	  ((pwr_tInt8 *)buffers[i])[current_index] = *(pwr_tInt8 *)attributes[i];
	  break;
	default:
	  ;
	}
      }
    }
    p->LastIndex = current_index;
    current_index++;

    if ( !( p->Function & fast_mFunction_BeforeTrigg)) {
      if ( current_index >= (int) p->NoOfPoints) {
	p->Active = 0;
	p->Prepare = 0;
	p->New = 1;
	new_cnt = 0;
      }
    }
    else {
      if ( p->TriggIndex == fast_cNoTrigg) {
	// No trigg yet
	if ( !first && current_index - 1 == (int) p->FirstIndex) {
	  p->FirstIndex = current_index;
	  if ( p->FirstIndex >= p->NoOfPoints)
	    p->FirstIndex = 0;
	}
	if ( current_index >= (int) p->NoOfPoints)
	  current_index = 0;

	if ( current_index >= (int) p->NoOfPoints)
	  current_index = 0;
      }
      else {
	// Check if it is time to stop
	if ( current_index - 1 == stop_index) {
	  p->Active = 0;
	  p->Prepare = 0;
	  p->New = 1;
	  new_cnt = 0;
	  p->LastIndex = current_index - 1;
	  if ( current_index - 1 == (int) p->FirstIndex) {
	    p->FirstIndex = current_index;
	    if ( p->FirstIndex >= p->NoOfPoints)
	      p->FirstIndex = 0;
	  }
	}
	else {
	  if ( current_index - 1 == (int) p->FirstIndex) {
	    p->FirstIndex = current_index;
	    if ( p->FirstIndex >= p->NoOfPoints)
	      p->FirstIndex = 0;
	  }
	  if ( current_index >= (int) p->NoOfPoints)
	    current_index = 0;
	}
      }
    }
  }
}


void rt_fast::open()
{
  pwr_tStatus sts;
  pwr_sAttrRef aref;
  pwr_tObjid oid;
  pwr_sClass_DsFastConf *conf_p;

  // Open server configuration object DsFastConf
  sts = gdh_GetClassList( pwr_cClass_DsFastConf, &oid);
  if ( ODD(sts)) {
    sts = gdh_ObjidToPointer( oid, (void **)&conf_p);
    if ( EVEN(sts)) throw co_error( sts);

    scan_time = 1.0 / conf_p->BaseFrequency;
  }
  else {
    scan_time = 0.1;
    errh_Info( "No fast configuration, using base frequency 10 Hz");
    oid = pwr_cNObjid;
  }

  aproc_RegisterObject( oid);

  // Open FastCurve object
  for ( sts = gdh_GetClassListAttrRef( pwr_cClass_DsFastCurve, &aref);
	ODD(sts);
	sts = gdh_GetNextAttrRef( pwr_cClass_DsFastCurve, &aref, &aref)) {
    fastobject *o = new fastobject( &aref);
    objects.push_back( o);
    try {
      o->open( scan_time);
      fast_cnt++;
    }    
    catch ( co_error& e) {
      delete o;
      objects.pop_back();
      errh_Error( "DsFastCurve configuration error: &s", (char *)e.what().c_str());
    }
  }
}

void rt_fast::close()
{
  for ( int i = objects.size() - 1; i >= 0; i--) {
    objects[i]->close();
    delete objects[i];
    objects.pop_back();
  }
  fast_cnt = 0;
}

void rt_fast::scan()
{
  aproc_TimeStamp();

  for ( int i = 0; i < (int) objects.size(); i++)
    objects[i]->scan();
}

void init( qcom_sQid *qid)
{
  qcom_sQid qini;
  qcom_sQattr qAttr;
  pwr_tStatus sts;

  sts = gdh_Init("ds_fast");
  if ( EVEN(sts)) {
    errh_Fatal( "gdh_Init, %m", sts);
    exit(sts);
  }

  errh_Init("pwr_fast", errh_eAnix_fast);
  errh_SetStatus( PWR__SRVSTARTUP);

  if (!qcom_Init(&sts, 0, "pwr_fast")) {
    errh_Fatal("qcom_Init, %m", sts); 
    errh_SetStatus( PWR__SRVTERM);
   exit(sts);
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, qid, &qAttr, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  } 

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, qid, &qini)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(-1);
  }
}

int main()
{
  pwr_tStatus sts;
  rt_fast fast;
  int tmo;
  char mp[2000];
  qcom_sQid qid = qcom_cNQid;
  qcom_sGet get;
  int swap = 0;
  bool first_scan = true;

  init( &qid);

  try {
    fast.open();
  }
  catch ( co_error& e) {
    errh_Error( (char *)e.what().c_str());
    errh_Fatal( "rt_fast aborting");
    errh_SetStatus( PWR__SRVTERM);
    exit(0);
  }

  aproc_TimeStamp();
  errh_SetStatus( PWR__SRUN);

  first_scan = true;
  for (;;) {
    if ( first_scan) {
      tmo = (int) (fast.scantime() * 1000 - 1);
    }

    get.maxSize = sizeof(mp);
    get.data = mp;
    qcom_Get( &sts, &qid, &get, tmo);
    if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
      if ( !swap)
	fast.scan();
    } 
    else {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.oldPlcStop && !swap) {
	errh_SetStatus( PWR__SRVRESTART);
        swap = 1;
	fast.close();
      } else if (new_event.b.swapDone && swap) {
        swap = 0;
	fast.open();
	errh_SetStatus( PWR__SRUN);
      } else if (new_event.b.terminate) {
	exit(0);
      }
    }
    first_scan = false;
  }

}






