/* 
 * Proview   $Id: wb_c_pb_dp_slave.cpp,v 1.4 2006-04-12 12:17:45 claes Exp $
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

/* wb_c_pb_dp_slave.c -- work bench methods of the Pb_DP_Slave class. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "wb_env.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

extern "C" {
#include "pwr.h"
#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "wb_nav_macros.h"
}
#include "rt_pb_gsd.h"
#include "rt_pb_gsd_attr.h"


#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "rt_pb_msg.h"
#include "wb_wnav.h"

using namespace std;

// #include "wb_api.h"


/*----------------------------------------------------------------------------*\
  Configure the slave from gsd file.
\*----------------------------------------------------------------------------*/

typedef struct {
  pb_gsd *gsd;
  GsdAttr *attr;
  ldh_tSession ldhses;
  pwr_tAttrRef aref;
  gsd_sModuleClass *mc;
  void *editor_ctx;
} slave_sCtx;
  

static void get_subcid( ldh_tSession ldhses, pwr_tCid cid, vector<pwr_tCid>& v)
{
  pwr_tCid subcid;
  pwr_tStatus sts;

  for ( sts = ldh_GetSubClass( ldhses, cid, pwr_cNCid, &subcid);
	ODD(sts);
	sts = ldh_GetSubClass( ldhses, cid, subcid, &subcid)) {
    v.push_back( subcid);
    get_subcid( ldhses, subcid, v);
  }
}

static int attr_help_cb( void *sctx, char *text)
{
  pwr_tCmd cmd;
  slave_sCtx *ctx = (slave_sCtx *)sctx;

  strcpy( cmd, "help ");
  strcat( cmd, text);
  return ((WNav *)ctx->editor_ctx)->command( cmd);
}

static void attr_close_cb( void *sctx)
{
  slave_sCtx *ctx = (slave_sCtx *)sctx;
  delete ctx->attr;
  delete ctx->gsd;
  free( (char *)ctx);
}

static int attr_save_cb( void *sctx)
{
  slave_sCtx *ctx = (slave_sCtx *)sctx;
  pwr_tStatus sts;
  pwr_tOName name;
  int size;
  char svalue[200];
  pwr_tAttrRef aaref;
  pwr_sClass_Pb_DP_Slave *slave;
  pwr_sClass_Pb_Module *module;
  int found;
  pwr_tOid oid;
  
  sts = ldh_ObjidToName(ctx->ldhses, ctx->aref.Objid, 
			ldh_eName_Hierarchy, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  // SlaveAddress
  pwr_tUInt16 address = ctx->gsd->address;
  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "SlaveAddress", &aaref);
  if ( EVEN(sts)) return sts;
  
  sts = ldh_WriteAttribute( ctx->ldhses, &aaref, &address, sizeof(address));
  if ( EVEN(sts)) return sts;

  // VendorName
  sts = ctx->gsd->get_svalue( "Vendor_Name", svalue, sizeof(svalue)); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "VendorName", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, svalue, sizeof( slave->VendorName));
    if ( EVEN(sts)) return sts;
  }

  // ModelName
  sts = ctx->gsd->get_svalue( "Model_Name", svalue, sizeof(svalue)); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "ModelName", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, svalue, sizeof( slave->ModelName));
    if ( EVEN(sts)) return sts;
  }

  // Revision
  sts = ctx->gsd->get_svalue( "Revision", svalue, sizeof(svalue)); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "Revision", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, svalue, sizeof( slave->Revision));
    if ( EVEN(sts)) return sts;
  }

  // HardwareRelease
  sts = ctx->gsd->get_svalue( "Hardware_Release", svalue, sizeof(svalue)); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "HardwareRelease", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, svalue, sizeof( slave->HardwareRelease));
    if ( EVEN(sts)) return sts;
  }

  // SoftwareRelease
  sts = ctx->gsd->get_svalue( "Software_Release", svalue, sizeof(svalue)); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "SoftwareRelease", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, svalue, sizeof( slave->SoftwareRelease));
    if ( EVEN(sts)) return sts;
  }

  // PNOIdent
  pwr_tUInt32 PNOIdent;
  sts = ctx->gsd->get_ivalue( "Ident_Number", (int *)&PNOIdent); 
  if ( ODD(sts)) {
    sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "PNOIdent", &aaref);
    if ( EVEN(sts)) return sts;

    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, &PNOIdent, sizeof( PNOIdent));
    if ( EVEN(sts)) return sts;
  }

  // PrmUserData
  pwr_tUInt8 user_prm_data[256];
  int len;

  memset( user_prm_data, 0, sizeof(user_prm_data));
  ctx->gsd->pack_ext_user_prm_data( (char *)user_prm_data, &len);

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "PrmUserData", &aaref);
  if ( EVEN(sts)) return sts;
  
  sts = ldh_WriteAttribute( ctx->ldhses, &aaref, user_prm_data, 
			    sizeof( slave->PrmUserData));
  if ( EVEN(sts)) return sts;

  // PrmUserDataLen
  pwr_tUInt16 user_prm_data_len = ctx->gsd->user_prm_data_len;
  if ( user_prm_data_len == 0)
    user_prm_data_len = len;

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "PrmUserDataLen", &aaref);
  if ( EVEN(sts)) return sts;
  
  sts = ldh_WriteAttribute( ctx->ldhses, &aaref, &user_prm_data_len, 
			    sizeof( slave->PrmUserDataLen));
  if ( EVEN(sts)) return sts;

  // ConfigData
  pwr_tUInt8 config_data[256];
  pwr_tUInt16 config_data_len;

  memset( config_data, 0, sizeof(config_data));
  ctx->gsd->pack_config( (char *)config_data, &len);
  config_data_len = len;

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "ConfigData", &aaref);
  if ( EVEN(sts)) return sts;
  
  sts = ldh_WriteAttribute( ctx->ldhses, &aaref, config_data, 
			    sizeof( slave->ConfigData));
  if ( EVEN(sts)) return sts;

  // ConfigDataLen
  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "ConfigDataLen", &aaref);
  if ( EVEN(sts)) return sts;
  
  sts = ldh_WriteAttribute( ctx->ldhses, &aaref, &config_data_len, 
			    sizeof( slave->ConfigDataLen));
  if ( EVEN(sts)) return sts;

  // Do a temporary rename all module object to avoid name collisions
  for ( sts = ldh_GetChild( ctx->ldhses, ctx->aref.Objid, &oid);
	ODD(sts);
	sts = ldh_GetNextSibling( ctx->ldhses, oid, &oid)) {
    sts = ldh_ObjidToName( ctx->ldhses,  oid, cdh_mName_object, name,
			   sizeof(name), &size);
    if ( EVEN(sts)) return sts;
    strcat( name, "__tmp");
    sts = ldh_ChangeObjectName( ctx->ldhses, oid, name);
    if ( EVEN(sts)) return sts;    
  }

  for ( int i = 0; i < ctx->gsd->module_conf_cnt; i++) {
    gsd_sModuleConf *m = &ctx->gsd->module_conf[i];
    pwr_tCid cid;
    pwr_tOid prev;

    if ( !m->module)
      continue;

    int found = 0;
    if ( cdh_ObjidIsNotNull( m->oid)) {
      // Find module object
      sts = ldh_GetObjectClass( ctx->ldhses, m->oid, &cid);
      if ( ODD(sts)) {
	if ( cid == m->cid) {
	  // Module object found and class is ok
	  found = 1;

	  // Check if name is changed
	  sts = ldh_ObjidToName( ctx->ldhses, m->oid, cdh_mName_object, name,
				 sizeof(name), &size);
	  if ( EVEN(sts)) return sts;

	  if ( strcmp( name, m->name) != 0) {
	    // Change name
	    sts = ldh_ChangeObjectName( ctx->ldhses, m->oid, m->name);
	    if ( EVEN(sts)) return sts;
	  }	  
	  
	  // Check that sibling position is right
	  sts = ldh_GetPreviousSibling( ctx->ldhses, m->oid, &prev);
	  if ( i == 0) {
	    // Should be first sibling
	    if ( ODD(sts)) {
	      // Move to first sibling
	      sts = ldh_MoveObject( ctx->ldhses, m->oid, ctx->aref.Objid, ldh_eDest_IntoFirst);
	    }
	  }
	  else {
	    if ( (ODD(sts) && cdh_ObjidIsNotEqual( ctx->gsd->module_conf[i-1].oid, prev)) ||
		 EVEN(sts)) {
	      // Move to next to i-1
	      sts = ldh_MoveObject( ctx->ldhses, m->oid, ctx->gsd->module_conf[i-1].oid, 
				    ldh_eDest_After);
	    }
	  }
	}
	else {
	  // New class, delete current object
	  sts = ldh_DeleteObjectTree( ctx->ldhses, m->oid);
	}
      }
    }
    if ( !found) {
      // Create a new module object
      if ( i == 0)
	sts = ldh_CreateObject( ctx->ldhses, &m->oid, m->name, m->cid,
				ctx->aref.Objid, ldh_eDest_IntoFirst);
      else
	sts = ldh_CreateObject( ctx->ldhses, &m->oid, m->name, m->cid,
				ctx->gsd->module_conf[i-1].oid, ldh_eDest_After);
      if ( EVEN(sts)) {
	printf( "Error creating module object, %d\n", sts);
	return 0;
      }
    }

    // Set module data
    pwr_tAttrRef maref;
    maref = cdh_ObjidToAref( m->oid);

    // ModuleName
    sts = ldh_ArefANameToAref( ctx->ldhses, &maref, "ModuleName", &aaref);
    if ( EVEN(sts)) {
      sts = ldh_ArefANameToAref( ctx->ldhses, &maref, "Super", &aaref);
      if ( ODD(sts))
	sts = ldh_ArefANameToAref( ctx->ldhses, &aaref, "ModuleName", &aaref);
      if ( EVEN(sts)) return sts;
    }
    sts = ldh_WriteAttribute( ctx->ldhses, &aaref, m->module->Mod_Name, 
			      sizeof( module->ModuleName));
    if ( EVEN(sts)) return sts;

    // sts = ldh_ArefANameToAref( ctx->ldhses, &maref, "Specification", &aaref);
    // if ( EVEN(sts)) return sts;

    // sts = ldh_WriteAttribute( ctx->ldhses, &aaref, m->module->Mod_Name, 
    //			      sizeof( module->Specification));
    // if ( EVEN(sts)) return sts;
  }

  // Remove modules that wasn't configured any more
  pwr_tOid moid[100];
  int mcnt = 0;
  for ( sts = ldh_GetChild( ctx->ldhses, ctx->aref.Objid, &oid);
	ODD(sts);
	sts = ldh_GetNextSibling( ctx->ldhses, oid, &oid)) {
    found = 0;
    for ( int i = 0; i < ctx->gsd->module_conf_cnt; i++) {
      if ( ctx->gsd->module_conf[i].module &&
	   cdh_ObjidIsEqual( ctx->gsd->module_conf[i].oid, oid)) {
	found = 1;
	break;
      }
    }
    if ( !found) {
      moid[mcnt++] = oid;
      if ( mcnt > (int) (sizeof(moid)/sizeof(moid[0])))
	break;	 
    }
  }

  for ( int i = 0; i < mcnt; i++)
    sts = ldh_DeleteObjectTree( ctx->ldhses, moid[i]);

  return PWRB__SUCCESS;
}

static pwr_tStatus load_modules( slave_sCtx *ctx)
{
  pwr_tOid oid;
  pwr_tCid cid;
  int found;
  pwr_tObjName name;
  pwr_tString40 module_name;
  int sts;
  int size;
  pwr_tAttrRef maref, aaref;

  for ( sts = ldh_GetChild( ctx->ldhses, ctx->aref.Objid, &oid);
	ODD(sts);
	sts = ldh_GetNextSibling( ctx->ldhses, oid, &oid)) {

    // Check that this is a module
    sts = ldh_GetObjectClass( ctx->ldhses, oid, &cid);
    if ( EVEN(sts)) return sts;

    found = 0;
    for ( int i = 0; ; i++) {
      if ( ctx->gsd->module_classlist[i].cid == 0)
	break;
      if ( ctx->gsd->module_classlist[i].cid == cid) {
	found = 1;
	break;
      }
    }
    if ( !found)
      // This is not a known module object
      continue;

    // Get name
    sts = ldh_ObjidToName( ctx->ldhses, oid, cdh_mName_object, name,
			   sizeof(name), &size);
    if ( EVEN(sts)) return sts;

    maref = cdh_ObjidToAref( oid);

    // Get ModuleName attribute
    sts = ldh_ArefANameToAref( ctx->ldhses, &maref, "ModuleName", &aaref);
    if ( EVEN(sts)) return sts;
      
    sts = ldh_ReadAttribute( ctx->ldhses, &aaref, module_name, sizeof(module_name));
    if ( EVEN(sts)) return sts;

    ctx->gsd->add_module_conf( cid, oid, name, module_name);
  }

  // Set address
  pwr_tUInt16 address;

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "SlaveAddress", &aaref);
  if ( EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ctx->ldhses, &aaref, &address, sizeof(address));
  if ( EVEN(sts)) return sts;

  ctx->gsd->address = address;

  // Set Ext_User_Prm_Data
  pwr_tUInt8 prm_user_data[256];
  pwr_tUInt16 prm_user_data_len;
  int len;
  
  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "PrmUserData", &aaref);
  if ( EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ctx->ldhses, &aaref, prm_user_data, sizeof(prm_user_data));
  if ( EVEN(sts)) return sts;

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "PrmUserDataLen", &aaref);
  if ( EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ctx->ldhses, &aaref, &prm_user_data_len, sizeof(prm_user_data_len));
  if ( EVEN(sts)) return sts;

  len = prm_user_data_len;
  if ( len != 0) {
    sts = ctx->gsd->unpack_ext_user_prm_data( (char *)prm_user_data, len);
    if ( EVEN(sts)) return sts;
  }
  return 1;
}

static pwr_tStatus Configure (
  ldh_sMenuCall *ip
)
{
  pwr_tOName name;
  char *gsdfile;
  int size;
  int sts;
  int lsts;
  int edit_mode;
  pwr_tFileName fname;
  ldh_sSessInfo Info;
  vector<pwr_tCid> mcv;

  sts = ldh_GetSessionInfo (ip->PointedSession, &Info);
  edit_mode = (ODD(sts) && Info.Access == ldh_eAccess_ReadWrite) &&
    ldh_LocalObject( ip->PointedSession, ip->Pointed.Objid);


  sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
			ldh_eName_Hierarchy, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "GSDfile", &gsdfile, &size);
  if ( EVEN(sts)) return sts;
  if ( strcmp( gsdfile, "") == 0) {
    free( gsdfile);  
    return 1;
  }

  slave_sCtx *ctx = (slave_sCtx *) calloc( 1, sizeof(slave_sCtx));
  ctx->ldhses = ip->PointedSession;
  ctx->aref = ip->Pointed;
  ctx->editor_ctx = ip->EditorContext;

  get_subcid( ctx->ldhses, pwr_cClass_Pb_Module, mcv);
  ctx->mc = (gsd_sModuleClass *) calloc( mcv.size() + 2, sizeof(gsd_sModuleClass));

  ctx->mc[0].cid = pwr_cClass_Pb_Module;
  sts = ldh_ObjidToName( ctx->ldhses, cdh_ClassIdToObjid(ctx->mc[0].cid), cdh_mName_object, 
			 ctx->mc[0].name, sizeof(ctx->mc[0].name), &size);
  if ( EVEN(sts)) return sts;

  for ( int i = 1; i <= (int) mcv.size(); i++) {
    ctx->mc[i].cid = mcv[i-1];
    sts = ldh_ObjidToName( ctx->ldhses, cdh_ClassIdToObjid(ctx->mc[i].cid), cdh_mName_object, 
			   ctx->mc[i].name, sizeof(ctx->mc[0].name), &size);
    if ( EVEN(sts)) return sts;
  }

  if ( strchr( gsdfile, '/') == 0) {
    strcpy( fname, "$pwrp_exe/");
    strcat( fname, gsdfile);
  }
  else
    strcpy( fname, gsdfile);
    
  ctx->gsd = new pb_gsd();
  sts = ctx->gsd->read( fname);
  if ( EVEN(sts))
    return sts;
    
  ctx->gsd->set_classes( ctx->mc);

  lsts = load_modules( ctx);
  if ( lsts != PB__USERPRMDATALEN && EVEN(lsts)) return lsts;

  ctx->attr = new GsdAttr( (Widget) ip->WindowContext, ctx, 0, ctx->gsd, edit_mode);
  ctx->attr->close_cb = attr_close_cb;
  ctx->attr->save_cb = attr_save_cb;
  ctx->attr->help_cb = attr_help_cb;

  if ( EVEN(lsts)) {
    wow_DisplayError( ctx->attr->toplevel, "Configuration load error", 
		      "Configuration load error\nCheck configuration data");
  }

  free( gsdfile);

  return 1;
}

static pwr_tStatus ConfigureFilter (
  ldh_sMenuCall *ip
)
{
#if 0
  char *gsd;
  int size;
  int sts;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "GSDfile", &gsd, &size);
  if ( EVEN(sts)) return sts;
  if ( strcmp( gsd, "") == 0) {
    free( gsd);  
    return 0;
  }
  free( gsd);  
#endif
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Pb_DP_Slave) = {
  pwr_BindMethod(Configure),
  pwr_BindMethod(ConfigureFilter),
  pwr_NullMethod
};




