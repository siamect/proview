/* 
 * Proview   $Id: wb_wccm.cpp,v 1.6 2008-10-28 09:53:30 claes Exp $
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

/* wb_wccm.c -- Buildtin script functions for wb */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_ccm.h"
#include "co_cdh.h"
#include "co_api_user.h"
#include "wb_utl_api.h"
#include "wb_trv.h"
#include "wb_cmd_msg.h"
#include "co_ccm_msg.h"
#include "wb_foe_msg.h"
#include "wb_wnav.h"
//#include "wb_cmd.h"


static int 		wccm_ccm_func_registred = 0;
static ldh_tSesContext	stored_ldhses = 0;
static ldh_tWBContext	stored_wbctx = 0;
static void		*stored_client_data;
int (* wccm_get_wbctx_cb)( void *, ldh_tWBContext *);
int (* wccm_get_ldhsession_cb)( void *, ldh_tSesContext *);

/*____Local function prototypes_______________________________________*/

static int wccm_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string
);
static int wccm_cut_segments (
  char	*outname,
  char	*name,
  int	segments
);

void wccm_store_client( void *client_data)
{
  stored_client_data = client_data;
}

void wccm_store_ldhses( void *client_data, ldh_tSesContext ldhsession)
{
  stored_ldhses = ldhsession;
  stored_client_data = client_data;
  if ( !stored_wbctx)
    stored_wbctx = ldh_SessionToWB( ldhsession);
}

int wccm_get_ldhses( ldh_tSesContext *ldhses)
{
  int sts;

  if ( !stored_ldhses)
  {
    sts = (wccm_get_ldhsession_cb)( stored_client_data, &stored_ldhses);
    if (EVEN(sts)) return sts;
  }
  *ldhses = stored_ldhses;
  return 1;
}

int wccm_get_wbctx( ldh_tWBContext *wbctx)
{
  int sts;

  if ( !stored_wbctx)
  {
    sts = (wccm_get_wbctx_cb)( stored_client_data, &stored_wbctx);
    if (EVEN(sts)) return sts;
  }
  *wbctx = stored_wbctx;
  return 1;
}


static int wccm_getattribute_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg	*arg_p2;
  int	sts;
  int	value_decl;
  int	value_int;
  float	value_float;
  char	value_string[80];

  if ( !(arg_count == 2 || arg_count == 1))
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_count == 2 && arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  sts = wccm_attribute_func( arg_list->value_string, &value_decl,
	&value_float, &value_int, value_string);
  if ( EVEN(sts))
  {
    if ( arg_count == 2)
    {
      arg_p2->value_int = 0;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
    *return_decl = CCM_DECL_UNKNOWN;
  }
  else
  {
    if ( value_decl == CCM_DECL_INT)
    {
      *return_int = value_int;
      *return_decl = CCM_DECL_INT;
    }
    else if ( value_decl == CCM_DECL_FLOAT)
    {
      *return_float = value_float;
      *return_decl = CCM_DECL_FLOAT;
    }
    else if ( value_decl == CCM_DECL_STRING)
    {
      strcpy( return_string, value_string);
      *return_decl = CCM_DECL_STRING;
    }
    if ( arg_count == 2)
    {
      arg_p2->value_int = 1;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
  }

  return 1;
}

static int wccm_getchild_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &parent_objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetChild ( ldhses, parent_objid, &child_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( ldhses, child_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getparent_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &child_objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetParent( ldhses, child_objid, &parent_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( ldhses, parent_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getnextsibling_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tObjid	objid;
  pwr_tObjid	next_objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetNextSibling ( ldhses, objid, &next_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( ldhses, next_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getclasslist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tCid	cid;
  pwr_tObjid	oid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_ClassNameToId( ldhses, &cid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetClassList( ldhses, cid, &oid);
    if (ODD(sts))
      sts = ldh_ObjidToName( ldhses, oid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getnextobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tObjid	objid;
  pwr_tObjid	next_objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetNextObject( ldhses, objid, &next_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( ldhses, next_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getrootlist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  pwr_tObjid	objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = ldh_GetRootList( ldhses, &objid);
  if (ODD(sts))
    sts = ldh_ObjidToName( ldhses, objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getobjectclass_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tClassId	cid;
  pwr_tObjid	objid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetObjectClass( ldhses, objid, &cid);
    if ( ODD(sts))
      sts = ldh_ObjidToName( ldhses, cdh_ClassIdToObjid( cid), 
		ldh_eName_Object, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_objectexist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tObjid	objid;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    *return_int = 0;
    *return_decl = CCM_DECL_INT;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_NameToObjid( ldhses, &objid, arg_list->value_string);
  *return_int =  ODD(sts);
  *return_decl = CCM_DECL_INT;

  return 1;
}

static int wccm_getvolumelist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tVolumeId	volid;
  int		size;
  ldh_tWBContext wbctx;

  sts = wccm_get_wbctx( &wbctx);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOWBATTACHED;
  }

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = ldh_GetVolumeList( wbctx, &volid);
  if (ODD(sts))
    sts = ldh_VolumeIdToName( wbctx, volid,
		name, sizeof( name), &size);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getnextvolume_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tVolumeId	volid;
  pwr_tVolumeId	next_volid;
  int		size;
  ldh_tWBContext wbctx;

  sts = wccm_get_wbctx( &wbctx);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOWBATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_VolumeNameToId( wbctx, arg_list->value_string, &volid);
  if ( ODD(sts))
  {
    sts = ldh_GetNextVolume( wbctx, volid, &next_volid);
    if (ODD(sts))
      sts = ldh_VolumeIdToName( wbctx, next_volid,
		name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getvolumeclass_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tClassId	cid;
  pwr_tVolumeId	volid;
  int		size;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  sts = ldh_VolumeNameToId( ldh_SessionToWB(ldhses), arg_list->value_string, &volid);
  if ( ODD(sts))
  {
    sts = ldh_GetVolumeClass( ldh_SessionToWB(ldhses), volid, &cid);
    if ( ODD(sts))
      sts = ldh_ObjidToName( ldhses, cdh_ClassIdToObjid( cid), 
		ldh_eName_Object, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_cutobjectname_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg 	*arg_p2; 

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  wccm_cut_segments( return_string, arg_list->value_string, 
	arg_p2->value_int);

  *return_decl = CCM_DECL_STRING;
  
  return 1;
}


static int wccm_getnodeobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tOName   	name;
  int		size;
  unsigned long	node_count;
  pwr_tObjid	*nodelist;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts))
  {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = trv_get_rtnodes( ldhses, &node_count, &nodelist);
  if ( ODD(sts))
  {
    if ( node_count > 0)
    {
      sts = ldh_ObjidToName( ldhses, *nodelist, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
      free( (char *)nodelist);
    }
    else
      sts = 0;
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_getprojectname_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		projectname[80];

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = utl_get_projectname( projectname);
  if ( ODD(sts))
    strcpy( return_string, projectname);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_setattribute_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg *arg_p2; 
  int sts;
  char buf[400];
  pwr_tAName name;
  pwr_tOName attr;
  char *s;
  ldh_tSesContext ldhses;

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts)) {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 2)
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;
  switch ( arg_p2->value_decl) {
    case CCM_DECL_STRING:
      strcpy( buf, arg_p2->value_string);
      break;
    case CCM_DECL_INT:
      sprintf( buf, "%d", arg_p2->value_int);
      break;
    case CCM_DECL_FLOAT:
      sprintf( buf, "%f", arg_p2->value_float);
      break;
  }

  strcpy( name, arg_list->value_string);
  if ( (s = strrchr( name, '.'))) {
    *s = 0;
    strcpy( attr, s+1);
  }
  else
    return CMD__ATTRIBUTE;

  sts = utl_set_object_parameter( ldhses, 0, 0, name, attr, buf, 0, "", 0, 0);

  *return_int = sts;
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wccm_checksystemgroup_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int sts;
  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;

  sts = user_CheckSystemGroup( arg_list->value_string);

  *return_int = ODD(sts);
  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wccm_getnextfreeuservid_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  pwr_tVid 	next_vid;
  pwr_tVid	start_vid = cdh_cUserVolMin;

  if ( arg_count > 1)
    return CCM__ARGMISM;

  if ( arg_count > 0) {
    if ( arg_list->value_decl != CCM_DECL_INT)
      return CCM__VARTYPE;

    if ( (pwr_tVid) arg_list->value_int > start_vid)
      start_vid = arg_list->value_int;
  }

  sts = WNav::get_next_free_vid( start_vid, cdh_cUserVolMax, &next_vid);
  if ( EVEN(sts))
    strcpy( return_string, "");
  else
    strcpy( return_string, cdh_VolumeIdToString( 0, next_vid, 0, 0));

  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int wccm_checknewvid_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_INT)
    return CCM__VARTYPE;

  sts = WNav::check_new_vid( arg_list->value_int);
  if ( EVEN(sts))
    *return_int = 0;
  else
    *return_int = 1;

  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wccm_checknewvolumename_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__VARTYPE;

  sts = WNav::check_new_volumename( arg_list->value_string);
  if ( EVEN(sts))
    *return_int = 0;
  else
    *return_int = 1;

  *return_decl = CCM_DECL_INT;
  
  return 1;
}

static int wccm_getcurrentvolume_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  int		size;
  ldh_tSesContext ldhses;
  ldh_sSessInfo info;
  ldh_tWBContext wbctx;

  sts = wccm_get_wbctx( &wbctx);
  if ( EVEN(sts)) {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOWBATTACHED;
  }

  sts = wccm_get_ldhses( &ldhses);
  if ( EVEN(sts)) {
    strcpy( return_string, "");
    *return_decl = CCM_DECL_STRING;
    return CMD__NOVOLATTACHED;
  }

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = ldh_GetSessionInfo( ldhses, &info);
  if (ODD(sts))
    sts = ldh_VolumeIdToName( wbctx, info.Vid,
			      name, sizeof( name), &size);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

/*************************************************************************
*
* Name:		wccm_register()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

int	wccm_register( 
	int (* get_wbctx_cb)( void *, ldh_tWBContext *),
	int (* get_ldhsession_cb)( void *, ldh_tSesContext *))
{
  int		sts;

  wccm_get_wbctx_cb = get_wbctx_cb;
  wccm_get_ldhsession_cb = get_ldhsession_cb;

  if ( !wccm_ccm_func_registred)
  {
    sts = ccm_register_function( "GetAttribute", wccm_getattribute_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetChild", wccm_getchild_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetParent", wccm_getparent_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetNextSibling", wccm_getnextsibling_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetClassList", wccm_getclasslist_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetNextObject", wccm_getnextobject_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetRootList", wccm_getrootlist_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetObjectClass", wccm_getobjectclass_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetVolumeList", wccm_getvolumelist_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetNextVolume", wccm_getnextvolume_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetVolumeClass", wccm_getvolumeclass_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "CutObjectName", wccm_cutobjectname_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetNodeObject", wccm_getnodeobject_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "ObjectExist", wccm_objectexist_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetProjectName", wccm_getprojectname_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "SetAttribute", wccm_setattribute_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "CheckSystemGroup", wccm_checksystemgroup_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetNextFreeUserVid", wccm_getnextfreeuservid_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "CheckNewVid", wccm_checknewvid_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "CheckNewVolumeName", wccm_checknewvolumename_func);
    if ( EVEN(sts)) return sts;
    sts = ccm_register_function( "GetCurrentVolume", wccm_getcurrentvolume_func);
    if ( EVEN(sts)) return sts;

    sts = ccm_create_external_var( "cmd_status", CCM_DECL_INT, 0, 1, 
		NULL);

    wccm_ccm_func_registred = 1;
  }

  return CMD__SUCCESS;
}


/*************************************************************************
*
* Name:		wccm_set_status()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

void	wccm_set_status( pwr_tStatus	sts)
{
	int	status;

	if ( ODD(sts))
	  status = 1;
	else
	  status = sts;
	ccm_set_external_var( "cmd_status", CCM_DECL_INT, 0, status, NULL);
}



/*************************************************************************
*
* Name:		wccm_attribute_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	
*
**************************************************************************/

static int wccm_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string
)
{
	int		sts, size, i, j;
	pwr_tClassId	cid;
	pwr_tAName     	hier_name;
	char		*hier_name_p;
	ldh_sParDef 	*bodydef;
	int		rows;
	char		body[20];	
	char		*object_par;
	char		*object_element;
	int		elements;
	int		element;
	char		elementstr[20];
	int		found;
	char		*s;
	char		*t;
	char		upper_name[80];
	pwr_tObjid	objid;
	char		objname[120];
	char		attrname[32];
	int		len;
	int		decl;
	int		int_val;
	float		float_val;
	char		string_val[80];
	ldh_tSesContext ldhses;

	sts = wccm_get_ldhses( &ldhses);
	if ( EVEN(sts))
	{
  	  strcpy( return_string, "");
  	  *return_decl = CCM_DECL_STRING;
	  return CMD__NOVOLATTACHED;
	}

	/* Parse the string in name and attribute */
	strcpy( objname, name);
	s = strrchr( objname, '.');
        if ( s == 0)
	  return FOE__NOPAR;
	*s = 0;

	strcpy( attrname, s+1);
	cdh_ToUpper( attrname, attrname);

	/* Check index in parameter */
	s = strchr( attrname, '[');
	if ( s == 0)
	  element = 0;
	else
	{
	  t = strchr( attrname, ']');
	  if ( t == 0)
	    return FOE__PARSYNT;
	  else
	  {
	    len = t - s - 1;
	    strncpy( elementstr, s + 1, len);
	    elementstr[ len ] = 0;
	    sscanf( elementstr, "%d", &element);
	    *s = '\0';
	    if ( (element < 0) || (element > 100) )
	      return FOE__PARELSYNT;
	  }
	}
	
        sts = ldh_NameToObjid( ldhses, &objid, objname);
        if ( EVEN(sts)) return sts;

	/* Get the class of the object */
	sts = ldh_GetObjectClass( ldhses, objid, &cid);

	found = 0;
	for ( i = 0; i < 3; i++ )
	{
	  if ( i == 0)
	    strcpy( body, "RtBody");
	  else if ( i == 1 )
	    strcpy( body, "DevBody");
	  else
	    strcpy( body, "SysBody");

    	  /* Get the parameters for this body */
	  sts = ldh_GetObjectBodyDef(ldhses, cid, body, 1, 
	  		&bodydef, &rows);
	  if ( EVEN(sts) ) continue;

	  for ( j = 0; j < rows; j++)
	  {
	    /* Convert parname to upper case */
	    cdh_ToUpper( upper_name, bodydef[j].ParName);

	    if (strcmp( attrname, upper_name) == 0)
	    {
	      found = 1;
	      break;
	    }
	  }
	  if ( found )
	    break;
	  free((char *) bodydef);	
	}
	if ( !found)
	{
	  /* Parametern fanns ej */
	  return FOE__NOPAR;
	}

	strcpy( attrname, bodydef[j].ParName);

	if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_ARRAY )
	  elements = bodydef[j].Par->Output.Info.Elements;
	else
	  elements = 1;

	/* Get the parameter value in object */
	sts = ldh_GetObjectPar( ldhses, objid, body,   
			attrname, (char **)&object_par, &size); 
	if ( EVEN(sts)) return sts;

	object_element = object_par + element * size / elements;

        switch ( bodydef[j].Par->Output.Info.Type )
        {
          case pwr_eType_Boolean:
          {
	    int_val = *(pwr_tBoolean *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Float32:
          {
	    float_val = *(pwr_tFloat32 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Float64:
          {
	    float_val = *(pwr_tFloat64 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Char:
          {
	    int_val = *(pwr_tChar *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int8:
          {
	    int_val = *(pwr_tInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int16:
          {
	    int_val = *(pwr_tInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int32:
          {
	    int_val = *(pwr_tInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int64:
          {
	    int_val = *(pwr_tInt64 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt8:
          {
	    int_val = *(pwr_tUInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt16:
          {
	    int_val = *(pwr_tUInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt32:
          case pwr_eType_Enum:
          case pwr_eType_Mask:
	  case pwr_eType_ClassId:
	  case pwr_eType_TypeId:
	  case pwr_eType_VolumeId:
	  case pwr_eType_ObjectIx:
          {
	    int_val = *(pwr_tUInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt64:
	  {
	    int_val = *(pwr_tUInt64 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_String:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Text:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_ObjDId:
          {
            /* Get the object name from ldh */
            sts = ldh_ObjidToName( ldhses, 
                 	*(pwr_tObjid *)object_element, ldh_eName_Hierarchy,
   		        hier_name, sizeof( hier_name), &size);
 	    if ( EVEN(sts))
	      strcpy( string_val, "Undefined Object");
	    else
	      strncpy( string_val, hier_name, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
	  case pwr_eType_AttrRef:
          {
            /* Get the object name from ldh */
	    sts = ldh_AttrRefToName( ldhses, 
	           	(pwr_sAttrRef *)object_element,  ldh_eName_Aref, 
			&hier_name_p, &size);
 	    if ( EVEN(sts))
	      strcpy( string_val, "Undefined attribute");
	    else
	      strncpy( string_val, hier_name_p, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Time:
          {
	    /* Convert time to ascii */	
	    sts = time_AtoAscii((pwr_tTime *)object_element, 
                      time_eFormat_DateAndTime, 
                      string_val, sizeof(string_val));
	    string_val[20] = 0;
	    decl = CCM_DECL_STRING;
	    break;
          }
          default:
            ;
        }
	free((char *) bodydef);	

	*return_decl = decl;
	if ( decl == CCM_DECL_INT)
	  *return_int = int_val;
	else if ( decl == CCM_DECL_FLOAT)
	  *return_float = float_val;
	else if ( decl == CCM_DECL_STRING)
	  strcpy( return_string, string_val);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		wccm_cut_segments()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	
*
**************************************************************************/

static int wccm_cut_segments (
  char	*outname,
  char	*name,
  int	segments
)
{
	char	*s[20];
	int	i, j, last_i;

	for( i = 0; i < segments; i++)
	{
	  s[i] = strrchr( name, '-');
	  if ( s[i] == 0)
	  {
	    last_i = i;
	    break;
	  }
	  *s[i] = '+';
	  last_i = i;
	}
	for ( j = 0; j <= last_i; j++)
	{
	  if ( s[j] != 0)
	    *s[j] = '-';
	}
	if ( s[last_i] == 0)
	  strcpy( outname, name);
	else
	  strcpy( outname, s[last_i] + 1);

	return FOE__SUCCESS;
}

