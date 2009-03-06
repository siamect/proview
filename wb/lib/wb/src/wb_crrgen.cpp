/* 
 * Proview   $Id: wb_crrgen.cpp,v 1.2 2008-05-29 14:57:53 claes Exp $
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

#include "pwr_baseclasses.h"
#include "wb_crrgen.h"
#include "flow_ctx.h"
#include "co_dcli.h"
#include "wb_utl.h"
#include "wb_vldh.h"
#include "glow_ctx.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_dyn.h"


typedef enum {
  crrgen_eType_,
  crrgen_eType_Write,
  crrgen_eType_Read,
  crrgen_eType_Ref
} crrgen_eType;

typedef enum {
  crrgen_eTable_,
  crrgen_eTable_Object,
  crrgen_eTable_Signal
} crrgen_eTable;

typedef struct sCrrKey
{
  pwr_tAttrRef target;
  pwr_tAttrRef reference;
  crrgen_eType type;
} sArefKey;

typedef struct sCrr
{
  tree_sNode node;
  sCrrKey   key;
} sCrr;

typedef struct {
  pwr_tCid 	cid;
  pwr_tObjName 	body;
  pwr_tObjName 	attr;
  crrgen_eType	type;
  crrgen_eTable	table;
  int		is_oid;
} crrgen_tRefList;

static crrgen_tRefList reflist[] = {
    { pwr_cClass_plc, "DevBody", "ResetObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_resdv, "DevBody", "DvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_setdv, "DevBody", "DvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_stodv, "DevBody", "DvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetDv, "DevBody", "DvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetDo, "DevBody", "DoObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_resdo, "DevBody", "DoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_setdo, "DevBody", "DoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_stodo, "DevBody", "DoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetDi, "DevBody", "DiObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstoav, "DevBody", "AvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetAv, "DevBody", "AvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_stoav, "DevBody", "AvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstoao, "DevBody", "AoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetAo, "DevBody", "AoObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_stoao, "DevBody", "AoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetAi, "DevBody", "AiObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_pos3p, "DevBody", "DoOpen", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_pos3p, "DevBody", "DoClose", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_inc3p, "DevBody", "DoOpen", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_inc3p, "DevBody", "DoClose", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_stodp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_setdp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_resdp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetDp, "DevBody", "DpObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_cstoap, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetAp, "DevBody", "ApObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_stoap, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetPi, "DevBody", "CoObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_CStoIp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetIp, "DevBody", "IpObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_StoIp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_stoii, "DevBody", "IiObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_stoio, "DevBody", "IoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_stoiv, "DevBody", "IvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstoii, "DevBody", "IiObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstoio, "DevBody", "IoObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstoiv, "DevBody", "IvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetIi, "DevBody", "IiObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetIo, "DevBody", "IoObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetIv, "DevBody", "IvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetSv, "DevBody", "SvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_stosv, "DevBody", "SvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_cstosv, "DevBody", "SvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetSp, "DevBody", "SpObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_stosp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_cstosp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetATv, "DevBody", "ATvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_StoATv, "DevBody", "ATvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_CStoATv, "DevBody", "ATvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetATp, "DevBody", "ATpObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_StoATp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_CStoATp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_GetDTv, "DevBody", "DTvObject", crrgen_eType_Read, crrgen_eTable_Signal, 0},
    { pwr_cClass_StoDTv, "DevBody", "DTvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_CStoDTv, "DevBody", "DTvObject", crrgen_eType_Write, crrgen_eTable_Signal, 0},
    { pwr_cClass_GetDTp, "DevBody", "DTpObject", crrgen_eType_Read, crrgen_eTable_Object, 0},
    { pwr_cClass_StoDTp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_CStoDTp, "DevBody", "Object", crrgen_eType_Write, crrgen_eTable_Object, 0},
    { pwr_cClass_reset_so, "DevBody", "OrderObject", crrgen_eType_Read, crrgen_eTable_Object, 1},
    { pwr_cClass_GetData, "DevBody", "DataObject", crrgen_eType_Ref, crrgen_eTable_Object, 0}};

static crrgen_tRefList codelist[] = {
    { pwr_cClass_dataarithm, "DevBody", "Code", crrgen_eType_, crrgen_eTable_, 0},
    { pwr_cClass_carithm, "DevBody", "Code", crrgen_eType_, crrgen_eTable_, 0},
    { pwr_cClass_aarithm, "DevBody", "Expression", crrgen_eType_, crrgen_eTable_, 0},
    { pwr_cClass_darithm, "DevBody", "Expression", crrgen_eType_, crrgen_eTable_, 0}};

static int comp_crr(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

static int comp_crr(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
  sCrr *xp = (sCrr *) x; 
  sCrr *yp = (sCrr *) y;

  if (xp->key.target.Objid.vid > yp->key.target.Objid.vid)
    return 1;

  if (xp->key.target.Objid.vid < yp->key.target.Objid.vid)
    return -1;

  if (xp->key.target.Objid.oix > yp->key.target.Objid.oix)
    return 1;

  if (xp->key.target.Objid.oix < yp->key.target.Objid.oix)
    return -1;

  if (xp->key.target.Body > yp->key.target.Body)
    return 1;

  if (xp->key.target.Body < yp->key.target.Body)
    return -1;

  if (xp->key.target.Offset > yp->key.target.Offset)
    return 1;

  if (xp->key.target.Offset < yp->key.target.Offset)
    return -1;

  if (xp->key.target.Size > yp->key.target.Size)
    return 1;

  if (xp->key.target.Size < yp->key.target.Size)
    return -1;

  if (xp->key.type > yp->key.type)
    return 1;

  if (xp->key.type < yp->key.type)
    return 1;

  if (xp->key.reference.Objid.vid > yp->key.reference.Objid.vid)
    return 1;

  if (xp->key.reference.Objid.oix > yp->key.reference.Objid.oix)
    return 1;

  if (xp->key.reference.Objid.vid < yp->key.reference.Objid.vid)
    return -1;

  if (xp->key.reference.Objid.oix < yp->key.reference.Objid.oix)
    return -1;

  if (xp->key.reference.Body > yp->key.reference.Body)
    return 1;

  if (xp->key.reference.Body < yp->key.reference.Body)
    return -1;

  if (xp->key.reference.Offset > yp->key.reference.Offset)
    return 1;

  if (xp->key.reference.Offset < yp->key.reference.Offset)
    return -1;

  if (xp->key.reference.Size > yp->key.reference.Size)
    return 1;

  if (xp->key.reference.Size < yp->key.reference.Size)
    return -1;

  return 0;
}


wb_crrgen::wb_crrgen( wb_session *sp) : m_sp(sp)
{
  pwr_tStatus sts;

  m_object_th = tree_CreateTable(&sts, sizeof(sCrrKey), offsetof(sCrr, key), sizeof(sCrr), 1000, comp_crr);
  m_signal_th = tree_CreateTable(&sts, sizeof(sCrrKey), offsetof(sCrr, key), sizeof(sCrr), 1000, comp_crr);
}

wb_crrgen::~wb_crrgen()
{
  pwr_tStatus sts;

  tree_DeleteTable(&sts, m_object_th);
  tree_DeleteTable(&sts, m_signal_th);
}

void wb_crrgen::load( pwr_tStatus *rsts)
{
  pwr_tStatus sts;

  
  for ( int i = 0; i < int(sizeof(reflist)/sizeof(reflist[0])); i++) {
    for ( wb_object o = m_sp->object( reflist[i].cid); o; o = o.next()) {
      pwr_tAttrRef aref;

      wb_attribute a = m_sp->attribute( o.oid(), reflist[i].body, reflist[i].attr);

      if ( reflist[i].is_oid) {
	// Objid reference
	pwr_tOid oid;
	a.value( &oid);
	aref = cdh_ObjidToAref( oid);
      }
      else
	// AttrRef reference
	a.value( &aref);

      if ( cdh_ObjidIsNotNull( aref.Objid)) {
	sCrrKey key;

	wb_utl::utl_replace_symbol( (ldh_tSesContext)m_sp, o.oid(), &aref);

	key.target = aref;
	key.reference = cdh_ObjidToAref( o.oid());
	key.type = reflist[i].type;
	switch ( reflist[i].table) {
	case crrgen_eTable_Signal:
	  tree_Insert(&sts, m_signal_th, &key);
	  break;
	case crrgen_eTable_Object:
	  tree_Insert(&sts, m_object_th, &key);
	  break;
	default: ;
	}
      }
    }
  }

  // Ge references
  for ( wb_object o = m_sp->object( pwr_cClass_XttGraph); o; o = o.next()) {
    pwr_tString80 action;
    pwr_tFileName fname;
    char line[512];
    char linep[2][512];
    int tag;
    int nr;
    char *s;

    wb_attribute a = m_sp->attribute( o.oid(), "RtBody", "Action");

    a.value( action);
    if ( strstr( action, ".pwg")) {
      sprintf( fname, "$pwrp_exe/%s", action);

      dcli_translate_filename( fname, fname);

      ifstream fpg( fname);
      if ( !fpg)
	continue;

      while ( fpg.getline( line, sizeof(line))) {
	nr = dcli_parse( line , " ", "", (char *)linep, 
			 sizeof( linep) / sizeof( linep[0]), sizeof( linep[0]), 0);
	if ( nr != 2)
	  continue;

	nr = sscanf( linep[0], "%d", &tag);
	if ( nr != 1)
	  continue;

	switch ( tag) {
	case ge_eSave_DigLowColor_attribute:
	case ge_eSave_DigColor_attribute:
	case ge_eSave_DigError_attribute:
	case ge_eSave_DigWarning_attribute:
	case ge_eSave_Invisible_attribute:
	case ge_eSave_DigBorder_attribute:
	case ge_eSave_DigText_attribute:
	case ge_eSave_ValueInput_minvalue_attr:
	case ge_eSave_ValueInput_maxvalue_attr:
	case ge_eSave_Rotate_attribute:
	case ge_eSave_Move_move_x_attribute:
	case ge_eSave_Move_move_y_attribute:
	case ge_eSave_Move_scale_x_attribute:
	case ge_eSave_Move_scale_y_attribute:
	case ge_eSave_AnalogShift_attribute:
	case ge_eSave_DigShift_attribute:
	case ge_eSave_Animation_attribute:
	case ge_eSave_Bar_attribute:
	case ge_eSave_Bar_minvalue_attr:
	case ge_eSave_Bar_maxvalue_attr:
	case ge_eSave_Trend_attribute1:
	case ge_eSave_Trend_attribute2:
	case ge_eSave_Trend_minvalue_attr1:
	case ge_eSave_Trend_maxvalue_attr1:
	case ge_eSave_Trend_minvalue_attr2:
	case ge_eSave_Trend_maxvalue_attr2:
	case ge_eSave_DigFlash_attribute:
	case ge_eSave_FillLevel_attribute:
	case ge_eSave_FillLevel_minvalue_attr:
	case ge_eSave_FillLevel_maxvalue_attr:
	case ge_eSave_Table_attribute1:
	case ge_eSave_Table_sel_attribute1:
	case ge_eSave_StatusColor_attribute:
	case ge_eSave_DigSound_attribute:
	case ge_eSave_XY_Curve_x_attr:
	case ge_eSave_XY_Curve_y_attr:
	case ge_eSave_XY_Curve_y_minvalue_attr:
	case ge_eSave_XY_Curve_y_maxvalue_attr:
	case ge_eSave_XY_Curve_x_minvalue_attr:
	case ge_eSave_XY_Curve_x_maxvalue_attr:
	case ge_eSave_DigCommand_attribute:
	case ge_eSave_Slider_minvalue_attr:
	case ge_eSave_Slider_maxvalue_attr:
	case ge_eSave_Slider_insensitive_attr:
	case ge_eSave_AnalogColor_attribute:
	case ge_eSave_DigSound_soundobject:
	case ge_eSave_PopupMenu_ref_object: {
	  if ( ( s = strchr( linep[1], '#')))
	    *s = 0;

	  if ( linep[1][0] == '!')
	    strcpy( &linep[1][0], &linep[1][1]);

	  wb_attribute al = m_sp->attribute(linep[1]);
	  if (!al)
	    break;

	  int len = strlen(linep[1]);
	  if ( len > 12 &&
	       cdh_NoCaseStrcmp( &linep[1][len - 12], ".ActualValue") == 0) {
	    linep[1][len - 12] = 0;
	    al = m_sp->attribute(linep[1]);
	    if ( !al)
	      break;
	  }
	  switch ( al.tid()) {
	  case pwr_cClass_Di:
	  case pwr_cClass_Do:
	  case pwr_cClass_Dv:
	  case pwr_cClass_Ai:
	  case pwr_cClass_Ao:
	  case pwr_cClass_Av:
	  case pwr_cClass_Co:
	  case pwr_cClass_Ii:
	  case pwr_cClass_Io:
	  case pwr_cClass_Iv:
	  case pwr_cClass_Sv:
	  case pwr_cClass_ATv:
	  case pwr_cClass_DTv: {
	    sCrrKey key;

	    key.target = al.aref();
	    key.reference = cdh_ObjidToAref( o.oid());
	    key.type = crrgen_eType_Read;
	    tree_Insert(&sts, m_signal_th, &key);
	    break;
	  }
	  default: {
	    sCrrKey key;

	    key.target = al.aref();
	    key.reference = cdh_ObjidToAref( o.oid());
	    key.type = crrgen_eType_Read;
	    tree_Insert(&sts, m_object_th, &key);
	  }
	  }
	  break;
	}
	case ge_eSave_Value_attribute:
	case ge_eSave_SetDig_attribute:
	case ge_eSave_ResetDig_attribute:
	case ge_eSave_ToggleDig_attribute:
	case ge_eSave_StoDig_attribute:
	case ge_eSave_IncrAnalog_attribute:
	case ge_eSave_RadioButton_attribute:
	case ge_eSave_Slider_attribute:
	case ge_eSave_OptionMenu_attribute: {
	  if ( ( s = strchr( linep[1], '#')))
	    *s = 0;

	  if ( linep[1][0] == '!')
	    strcpy( &linep[1][0], &linep[1][1]);

	  wb_attribute al = m_sp->attribute(linep[1]);
	  if (!al)
	    break;

	  int len = strlen(linep[1]);
	  if ( len > 12 &&
	       cdh_NoCaseStrcmp( &linep[1][len - 12], ".ActualValue") == 0) {
	    linep[1][len - 12] = 0;
	    al = m_sp->attribute(linep[1]);
	    if ( !al)
	      break;
	  }
	  switch ( al.tid()) {
	  case pwr_cClass_Di:
	  case pwr_cClass_Do:
	  case pwr_cClass_Dv:
	  case pwr_cClass_Ai:
	  case pwr_cClass_Ao:
	  case pwr_cClass_Av:
	  case pwr_cClass_Co:
	  case pwr_cClass_Ii:
	  case pwr_cClass_Io:
	  case pwr_cClass_Iv:
	  case pwr_cClass_Sv:
	  case pwr_cClass_ATv:
	  case pwr_cClass_DTv: {
	    sCrrKey key;

	    key.target = al.aref();
	    key.reference = cdh_ObjidToAref( o.oid());
	    key.type = crrgen_eType_Write;
	    tree_Insert(&sts, m_signal_th, &key);
	    break;
	  }
	  default: {
	    sCrrKey key;

	    key.target = al.aref();
	    key.reference = cdh_ObjidToAref( o.oid());
	    key.type = crrgen_eType_Write;
	    tree_Insert(&sts, m_object_th, &key);
	  }
	  }
	  
	  break;
	}
	default: ;
	}
      }
      fpg.close();
    }
  }


  *rsts = 1;
}

void wb_crrgen::write( pwr_tStatus *rsts)
{
  pwr_tStatus sts;
  char type_char;
  pwr_tAttrRef prev_target;
  pwr_tFileName fname;

  memset( &prev_target, 0, sizeof(prev_target));

  sprintf( fname, "$pwrp_load/rtt_crr_%s.dat", vldh_VolumeIdToStr( m_sp->vid()));
  dcli_translate_filename( fname, fname);
  
  ofstream fps( fname);
  if ( !fps) {
    *rsts = LDH__FILEOPEN;
    return;      
  }

  sCrr *crr = (sCrr *)tree_Minimum(&sts, m_signal_th);
  while ( crr) {
    wb_attribute a_target = m_sp->attribute( &crr->key.target);
    wb_object o = m_sp->object( crr->key.reference.Objid);
    if ( !a_target) {
      printf( "** Undefined reference in: %s\n", o.longName().name(cdh_mName_volume | cdh_mName_path | cdh_mName_attribute));
      crr = (sCrr *)tree_Successor(&sts, m_signal_th, crr);
      continue;
    }
    wb_cdef cdef = m_sp->cdef(o.cid());

    if ( !(crr->key.target.Objid.vid == prev_target.Objid.vid &&
	   crr->key.target.Objid.oix == prev_target.Objid.oix &&
	   crr->key.target.Offset == prev_target.Offset &&
	   crr->key.target.Size == prev_target.Size))
      fps << a_target.longName().name(cdh_mName_volume | cdh_mName_path | cdh_mName_attribute) << endl;

    switch( crr->key.type) {
    case crrgen_eType_Write:
      type_char = 'W';
      break;
    case crrgen_eType_Read:
      type_char = 'R';
      break;
    case crrgen_eType_Ref:
      type_char = '>';
      break;
    default: ;
    }
    fps << " " << type_char << " " << o.longName().name(cdh_mName_path | cdh_mName_attribute) << " " << cdef.name() << endl;
    memcpy( &prev_target, &crr->key.target, sizeof(prev_target));
    crr = (sCrr *)tree_Successor(&sts, m_signal_th, crr);
  }
  fps.close();

  sprintf( fname, "$pwrp_load/rtt_crro_%s.dat", vldh_VolumeIdToStr( m_sp->vid()));
  dcli_translate_filename( fname, fname);
  
  ofstream fpo( fname);
  if ( !fpo) {
    *rsts = LDH__FILEOPEN;
    return;      
  }

  memset( &prev_target, 0, sizeof(prev_target));

  crr = (sCrr *)tree_Minimum(&sts, m_object_th);
  while ( crr) {
    wb_attribute a_target = m_sp->attribute( &crr->key.target);
    wb_object o = m_sp->object( crr->key.reference.Objid);
    if ( !a_target) {
      printf( "** Undefined reference in: %s\n", o.longName().name(cdh_mName_volume | cdh_mName_path | cdh_mName_attribute));
      crr = (sCrr *)tree_Successor(&sts, m_object_th, crr);
      continue;
    }
    wb_cdef cdef = m_sp->cdef(o.cid());

    if ( !(crr->key.target.Objid.vid == prev_target.Objid.vid &&
	   crr->key.target.Objid.oix == prev_target.Objid.oix &&
	   crr->key.target.Offset == prev_target.Offset &&
	   crr->key.target.Size == prev_target.Size))
      fpo << a_target.longName().name(cdh_mName_volume | cdh_mName_path | cdh_mName_attribute) << endl;

    switch( crr->key.type) {
    case crrgen_eType_Write:
      type_char = 'W';
      break;
    case crrgen_eType_Read:
      type_char = 'R';
      break;
    case crrgen_eType_Ref:
      type_char = '>';
      break;
    default: ;
    }
    fpo << " " << type_char << " " << o.longName().name(cdh_mName_path | cdh_mName_attribute) << " " << cdef.name() << endl;
    memcpy( &prev_target, &crr->key.target, sizeof(prev_target));
    crr = (sCrr *)tree_Successor(&sts, m_object_th, crr);
  }
  fpo.close();

  *rsts = 1;
}


void wb_crrgen::write_code( pwr_tStatus *rsts)
{
  pwr_tFileName fname;
  char text[1024];

  sprintf( fname, "$pwrp_load/rtt_crrc_%s.dat", vldh_VolumeIdToStr( m_sp->vid()));
  dcli_translate_filename( fname, fname);
  
  ofstream fpc( fname);
  if ( !fpc) {
    *rsts = LDH__FILEOPEN;
    return;      
  }

  for ( int i = 0; i < int(sizeof(codelist)/sizeof(codelist[0])); i++) {
    for ( wb_object o = m_sp->object( codelist[i].cid); o; o = o.next()) {

      wb_attribute a = m_sp->attribute( o.oid(), codelist[i].body, codelist[i].attr);

      a.value( text);

      if ( strcmp( text, "") != 0) {
	fpc << " _Obj_ " << o.longName().name(cdh_mName_path | cdh_mName_object)  << endl;
	fpc << text << endl;
      }
    }
  }
  fpc.close();

  *rsts = 1;
}

