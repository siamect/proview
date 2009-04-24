/* 
 * Proview   $Id$
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

/* wb_c_pndevice.c -- work bench methods of the PnDevice class. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "wb_env.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_xhelp.h"
#include "rt_pn_gsdml.h"
#include "rt_pn_gsdml_attr.h"
#include "rt_pn_gsdml_attrnav.h"
#include "wb_c_pndevice.h"

#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "rt_pb_msg.h"
#include "wb_wnav.h"

using namespace std;


/*----------------------------------------------------------------------------*\
  Configure the slave from gsd file.
\*----------------------------------------------------------------------------*/

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

int pndevice_help_cb( void *sctx, const char *text)
{
  pwr_tCmd cmd;
  device_sCtx *ctx = (device_sCtx *)sctx;

  strcpy( cmd, "help ");
  strcat( cmd, text);
  return ((WNav *)ctx->editor_ctx)->command( cmd);
}

void pndevice_close_cb( void *sctx)
{
  device_sCtx *ctx = (device_sCtx *)sctx;
  delete ctx->attr;
  delete ctx->gsdml;
  free( (char *)ctx);
}

int pndevice_save_cb( void *sctx)
{
  device_sCtx *ctx = (device_sCtx *)sctx;
  pwr_tStatus sts;
  pwr_tOName name;
  int size;
  pwr_tOid oid;
  
  sts = ldh_ObjidToName(ctx->ldhses, ctx->aref.Objid, 
			ldh_eName_Hierarchy, name, sizeof(name), &size);
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

  for ( unsigned int i = 1; i < ctx->attr->attrnav->dev_data.slot_data.size(); i++) {
    GsdmlSlotData *slot = ctx->attr->attrnav->dev_data.slot_data[i];
    pwr_tCid cid;
    pwr_tOid prev;
    pwr_tObjName mname;

    int found = 0;
    sprintf( mname, "M%d", slot->slot_idx);

    if ( cdh_ObjidIsNotNull( slot->module_oid)) {

      // Find module object
      sts = ldh_GetObjectClass( ctx->ldhses, slot->module_oid, &cid);
      if ( ODD(sts)) {
	if ( cid == slot->module_class) {
	  // Module object found and class is ok
	  found = 1;

	  // Check if name is changed
	  sts = ldh_ObjidToName( ctx->ldhses, slot->module_oid, cdh_mName_object, name,
				 sizeof(name), &size);
	  if ( EVEN(sts)) return sts;

	  if ( strcmp( name, mname) != 0) {
	    // Change name
	    sts = ldh_ChangeObjectName( ctx->ldhses, slot->module_oid, mname);
	    if ( EVEN(sts)) return sts;
	  }	  
	  
	  // Check that sibling position is right
	  sts = ldh_GetPreviousSibling( ctx->ldhses, slot->module_oid, &prev);
	  if ( i == 0) {
	    // Should be first sibling
	    if ( ODD(sts)) {
	      // Move to first sibling
	      sts = ldh_MoveObject( ctx->ldhses, slot->module_oid, ctx->aref.Objid, ldh_eDest_IntoFirst);
	    }
	  }
	  else {
	    if ( (ODD(sts) && 
		  cdh_ObjidIsNotEqual( ctx->attr->attrnav->dev_data.slot_data[i-1]->module_oid, prev)) ||
		 EVEN(sts)) {
	      // Move to next to i-1
	      sts = ldh_MoveObject( ctx->ldhses, slot->module_oid, 
				    ctx->attr->attrnav->dev_data.slot_data[i-1]->module_oid, 
				    ldh_eDest_After);
	    }
	  }
	}
	else {
	  // New class, delete current object
	  sts = ldh_DeleteObjectTree( ctx->ldhses, slot->module_oid, 0);
	}
      }
    }
    if ( !found && slot->module_class != pwr_cNCid) {
      // Create a new module object
      if ( i == 1)
	sts = ldh_CreateObject( ctx->ldhses, &slot->module_oid, mname, slot->module_class,
				ctx->aref.Objid, ldh_eDest_IntoFirst);
      else
	sts = ldh_CreateObject( ctx->ldhses, &slot->module_oid, mname, slot->module_class,
				ctx->attr->attrnav->dev_data.slot_data[i-1]->module_oid, 
				ldh_eDest_After);
      if ( EVEN(sts)) {
	printf( "Error creating module object, %d\n", sts);
	return 0;
      }
    }
  }

  // Remove modules that wasn't configured any more
  pwr_tOid moid[100];
  int mcnt = 0;
  int found;
  for ( sts = ldh_GetChild( ctx->ldhses, ctx->aref.Objid, &oid);
	ODD(sts);
	sts = ldh_GetNextSibling( ctx->ldhses, oid, &oid)) {
    found = 0;
    for ( unsigned int i = 0; i < ctx->attr->attrnav->dev_data.slot_data.size(); i++) {
      if ( cdh_ObjidIsEqual( ctx->attr->attrnav->dev_data.slot_data[i]->module_oid, oid)) {
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
    sts = ldh_DeleteObjectTree( ctx->ldhses, moid[i], 0);

  return PWRB__SUCCESS;
}

static pwr_tStatus load_modules( device_sCtx *ctx)
{
#if 0
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

  // Set byte order
  pwr_tByteOrderingEnum byte_order;

  sts = ldh_ArefANameToAref( ctx->ldhses, &ctx->aref, "ByteOrdering", &aaref);
  if ( EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ctx->ldhses, &aaref, &byte_order, sizeof(byte_order));
  if ( EVEN(sts)) return sts;

  ctx->gsd->byte_order = byte_order;

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
#endif
  return 1;
}

pwr_tStatus pndevice_create_ctx( ldh_tSession ldhses, pwr_tAttrRef aref, 
				    void *editor_ctx, device_sCtx **ctxp)
{
  pwr_tOName name;
  char *gsdmlfile;
  int size;
  int sts;
  pwr_tFileName fname;
  ldh_sSessInfo Info;
  vector<pwr_tCid> mcv;

  sts = ldh_GetSessionInfo( ldhses, &Info);


  sts = ldh_ObjidToName( ldhses, aref.Objid, 
			 ldh_eName_Hierarchy, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectPar( ldhses, aref.Objid, "RtBody",
			  "GSDMLfile", &gsdmlfile, &size);
  if ( EVEN(sts)) return sts;
  if ( strcmp( gsdmlfile, "") == 0) {
    free( gsdmlfile);  
    return PB__GSDATTR;
  }

  device_sCtx *ctx = (device_sCtx *) calloc( 1, sizeof(device_sCtx));
  ctx->ldhses = ldhses;
  ctx->aref = aref;
  ctx->editor_ctx = editor_ctx;
  ctx->edit_mode = (ODD(sts) && Info.Access == ldh_eAccess_ReadWrite) &&
    ldh_LocalObject( ldhses, aref.Objid);

  get_subcid( ctx->ldhses, pwr_cClass_PnModule, mcv);
  ctx->mc = (gsdml_sModuleClass *) calloc( mcv.size() + 2, sizeof(gsdml_sModuleClass));

  ctx->mc[0].cid = pwr_cClass_PnModule;
  sts = ldh_ObjidToName( ctx->ldhses, cdh_ClassIdToObjid(ctx->mc[0].cid), cdh_mName_object, 
			 ctx->mc[0].name, sizeof(ctx->mc[0].name), &size);
  if ( EVEN(sts)) return sts;

  for ( int i = 1; i <= (int) mcv.size(); i++) {
    ctx->mc[i].cid = mcv[i-1];
    sts = ldh_ObjidToName( ctx->ldhses, cdh_ClassIdToObjid(ctx->mc[i].cid), cdh_mName_object, 
			   ctx->mc[i].name, sizeof(ctx->mc[0].name), &size);
    if ( EVEN(sts)) return sts;
  }

  if ( strchr( gsdmlfile, '/') == 0) {
    strcpy( fname, "$pwrp_exe/");
    strcat( fname, gsdmlfile);
  }
  else
    strcpy( fname, gsdmlfile);
  free( gsdmlfile);
    
  ctx->gsdml = new pn_gsdml();
  sts = ctx->gsdml->read( fname);
  if ( EVEN(sts))
    return sts;
  ctx->gsdml->build();
  ctx->gsdml->set_classes( ctx->mc);

  *ctxp = ctx;
  return 1;
}

pwr_tStatus pndevice_init( device_sCtx *ctx)
{
  pwr_tObjName module_name;
  pwr_tOid module_oid;
  int corrupt = 0;
  unsigned int idx;
  pwr_tStatus sts;
  int size;
  
  // Identify module objects
  for ( sts = ldh_GetChild( ctx->ldhses, ctx->aref.Objid, &module_oid);
	ODD(sts);
	sts = ldh_GetNextSibling( ctx->ldhses, module_oid, &module_oid)) {
    sts = ldh_ObjidToName( ctx->ldhses,  module_oid, cdh_mName_object, module_name,
			   sizeof(module_name), &size);
    if ( EVEN(sts)) return sts;

    if ( !(sscanf(  module_name, "M%d", &idx) == 1)) {
      corrupt = 1;
      continue;
    }
    if ( idx >= ctx->attr->attrnav->dev_data.slot_data.size()) {
      corrupt = 1;
      continue;
    }
    ctx->attr->attrnav->dev_data.slot_data[idx]->module_oid = module_oid;
  }
  if ( corrupt) {
    ctx->attr->wow->DisplayError( "Configuration corrupt", 
		      "Configuration of module objects doesn't match device configuration");
  }
  return 1;
}


