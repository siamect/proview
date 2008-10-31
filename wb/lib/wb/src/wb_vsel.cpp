/* 
 * Proview   $Id: wb_vsel.cpp,v 1.20 2008-10-31 12:51:32 claes Exp $
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

/* wb_vsel -- select volume */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_systemclasses.h"
#include "rt_load.h"

#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "co_cdh.h"
#include "co_dcli.h"
#include "wb.h"
#include "wb_ldh.h"
#include "wb_vsel.h"
#include "wb_foe.h"
#include "wb_utl_api.h"
#include "wb_lfu.h"
#include "co_wow.h"
#include "co_login.h"
#include "co_login_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh_msg.h"
#include "co_msgwindow.h"

#define	BEEP	    putchar( '\7' );


/*************************************************************************
*
* Name:		int	vsel_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char *	name		I	Name of the created object
*
* Description:
*	Create a new login window
**************************************************************************/

WVsel::WVsel (
  pwr_tStatus   *status,
  void		*wv_parent_ctx,
  const char   	*wv_name,
  ldh_tWBContext wv_wbctx,
  char		*volumename,
  int		(*bc_success)( void *, pwr_tVolumeId *, int),
  void		(*bc_cancel)(),
  int		(*bc_time_to_exit)( void *),
  int		show_volumes,
  wb_eType      wv_wb_type
  ) : parent_ctx(wv_parent_ctx), wbctx(wv_wbctx),
      vsel_bc_success(bc_success), vsel_bc_cancel(bc_cancel),
      vsel_bc_time_to_exit( bc_time_to_exit), volume_count(0), all(0),
      write_priv(0), wb_type(wv_wb_type), wow(0)
{
  strcpy( name, wv_name);
  *status = LOGIN__SUCCESS;
}



WVsel::~WVsel()
{
}


/*************************************************************************
*
* Name:		load_list()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Load the volumelist.
*
**************************************************************************/

pwr_tStatus WVsel::load_volumelist()
{
  pwr_tVolumeId	volume;
  pwr_tClassId	classid;
  char		name[80];
  int		size;
  pwr_tStatus	sts;
  int		i;
  bool            local_wbvolume = false;
  ldh_sVolumeInfo info;
  char		str[120];

  list_clear();
  volume_count = 0;

  if ( wb_type != wb_eType_Buffer)
    sts = ldh_GetVolumeList( wbctx, &volume);
  else
    sts = ldh_GetBufferList( wbctx, &volume);
  while ( ODD(sts) ) {
    sts = ldh_GetVolumeClass( wbctx, volume, &classid);
    if (EVEN(sts)) return sts;

    if ( !all) {
      if ( cdh_isClassVolumeClass( classid) ||
	   classid == pwr_eClass_WorkBenchVolume  ||
	   volume == ldh_cRtVolume) {
	sts = ldh_GetNextVolume( wbctx, volume, &volume);
	continue;
      }
    }
    sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
			      &size);
    if (EVEN(sts)) return sts;

    strcpy( str, name);

    strcat( str, " ");
    for ( i = strlen(str); i < 20; i++)
      strcat( str, " ");

    sts = ldh_GetVidInfo( wbctx, volume, &info);
    if (EVEN(sts)) return sts;

    switch ( info.VolRep) {
    case ldh_eVolRep_Db: 
      strcat( str, "Db     ");
      break;
    case ldh_eVolRep_Dbs: 
      strcat( str, "Dbs    ");
      break;
    case ldh_eVolRep_Dbms: 
      strcat( str, "Dbms    ");
      break;
    case ldh_eVolRep_Wbl: 
      strcat( str, "Wbl    ");
      break;
    case ldh_eVolRep_Mem: 
      strcat( str, "Mem    ");
      break;
    case ldh_eVolRep_Ref: 
      strcat( str, "Ref    ");
      break;
    case ldh_eVolRep_Ext: 
      strcat( str, "Ext    ");
      break;
    case ldh_eVolRep_Ced: 
      strcat( str, "Ced    ");
      break;
    }
    
    switch (classid) {
    case pwr_eClass_RootVolume:
      strcat( str, "RootVolume");
      break;
    case pwr_eClass_SubVolume:
      strcat( str, "SubVolume");
      break;
    case pwr_eClass_SharedVolume:
      strcat( str, "SharedVolume");
      break;
    case pwr_eClass_ClassVolume:
      strcat( str, "ClassVolume");
      break;
    case pwr_eClass_DetachedClassVolume:
      strcat( str, "DetachedClassVolume");
      break;
    case pwr_eClass_WorkBenchVolume:
      strcat( str, "WorkBenchVolume");
      break;
    case pwr_eClass_DirectoryVolume:
      strcat( str, "DirectoryVolume");
      break;
    case pwr_eClass_ExternVolume:
      strcat( str, "ExternVolume");
      break;
    }
    list_add_item( str);

    // Store the volume
    volumes[ volume_count] = volume;
    volume_count++;
    if ( volume_count >= VSEL_MAX_VOLUMES)
      break;
    if ( local_wbvolume)
      break;

    sts = ldh_GetNextVolume( wbctx, volume, &volume);
    if ( EVEN(sts) && all) {
      // Look for local workbench volume
      volume = ldh_cWBVolLocal;
      sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
				&size);
      local_wbvolume = true;
    }
  }
  return 1;
}


/*************************************************************************
*
* Name:		check_volumelist()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Load the volumelist.
*
**************************************************************************/

pwr_tStatus WVsel::check_volumelist(
  int		quiet,
  int		*display_window
)
{
  pwr_tVolumeId	volume;
  pwr_tClassId	classid;
  char		name[80];
  int		size;
  pwr_tStatus	sts;
  int		i;
  lfu_t_volumelist *volumelist;
  lfu_t_volumelist *volumelist_ptr;
  int		volumecount;	
  int		class_error;
  char		volname_conf[80];
  char		volname_db[80];
  int		volume_found;
  int		error_count;
  int		errlen=0;
  int		errlen_old = 0;
  char		errstr[800];
  
  error_count = 0;
  *display_window = 0;
	
  if ( !(CoLogin::privilege() & pwr_mPrv_DevConfig ))
    // No privileges for edit, don't check the volumes
    return 1;

  // Load the configured volume list
  sts = lfu_volumelist_load( load_cNameVolumeList,  &volumelist, 
			     &volumecount);
  if (sts == FOE__NOFILE) {
    if ( !quiet) {
      printf( "** Error, project is not configured\n");
      BEEP;
      *display_window = 1;
    }
    return 1;
  }
  else if (EVEN(sts))
    return 1;

  sts = ldh_GetVolumeList( wbctx, &volume);
  while ( ODD(sts) ) {
    sts = ldh_GetVolumeClass( wbctx, volume, &classid);
    if (EVEN(sts)) return sts;

    sts = ldh_VolumeIdToName( wbctx, volume, name, sizeof(name),
			      &size);
    if (EVEN(sts)) return sts;

    if ( classid == pwr_eClass_RootVolume ||
	 classid == pwr_eClass_SubVolume ||
	 (classid == pwr_eClass_SharedVolume && volume != ldh_cRtVolume) ||
	 (cdh_isClassVolumeClass(classid) && 
	  (cdh_cUserClassVolMin <= volume && volume <= cdh_cUserClassVolMax))) {
      // This volume should be configured
      volume_found = 0;
      volumelist_ptr = volumelist;
      for ( i = 0; i < volumecount; i++) {
	if ( volumelist_ptr->volume_id == volume) {
	  // Mark that volume is found
	  volume_found = 1;
	  strcpy( volumelist_ptr->p3, "Found");

	  // Check volume name
	  utl_toupper( volname_conf, volumelist_ptr->volume_name);
	  utl_toupper( volname_db, name);
	  if ( strcmp( volname_db, volname_conf)) {
	    // Volume name differs
	    if ( !quiet && errlen < (int)(sizeof(errstr)-100)) {
	      errlen += sprintf( &errstr[errlen], 
			  "** Error, Volume %s is configured with another name '%s'\n", 
				 name, volumelist_ptr->volume_name);
	      MsgWindow::message( 'E', &errstr[errlen_old], msgw_ePop_No);
	      errlen_old = errlen;
	      BEEP;
	    }
	    error_count++;
	  }
	  class_error = 0;
	  switch( classid) {
	  case pwr_eClass_RootVolume :
	    if ( strcmp( volumelist_ptr->p1, "RootVolume"))
	      class_error = 1;
	    break;
	  case pwr_eClass_SubVolume :
	    if ( strcmp( volumelist_ptr->p1, "SubVolume"))
	      class_error = 1;
	    break;
	  case pwr_eClass_ClassVolume :
	    if ( strcmp( volumelist_ptr->p1, "ClassVolume"))
	      class_error = 1;
	    break;
	  case pwr_eClass_DetachedClassVolume :
	    if ( strcmp( volumelist_ptr->p1, "DetachedClassVolume"))
	      class_error = 1;
	    break;
	  case pwr_eClass_SharedVolume :
	    if ( strcmp( volumelist_ptr->p1, "SharedVolume"))
	      class_error = 1;
	    break;
	  }
	  if ( class_error) {
	    if ( !quiet && errlen < (int)(sizeof(errstr)-100)) {
	      errlen += sprintf( &errstr[errlen], 
	                "** Error, Volume %s is configured with another class '%s'\n", 
				 name, volumelist_ptr->p1);
	      MsgWindow::message( 'E', &errstr[errlen_old], msgw_ePop_No);
	      errlen_old = errlen;
	      BEEP;
	    }
	    error_count++;
	  }
	  break;
	}
	volumelist_ptr++;
      }
      if ( !volume_found) {
	if ( !quiet && errlen < (int)(sizeof(errstr)-100)) {
	  errlen += sprintf( &errstr[errlen], 
		  "** Error, Volume %s (%s) is not configured in the ProjectVolume\nCheck volume name and identity\n", 
			     name, cdh_VolumeIdToString( 0, volume, 1, 0));
	  MsgWindow::message( 'E', &errstr[errlen_old], msgw_ePop_No);
	  errlen_old = errlen;
	  BEEP;
	}	
	error_count++;
      }
    }
    sts = ldh_GetNextVolume( wbctx, volume, &volume);
  }

  if ( error_count) {
    *display_window = 1;
    if ( !quiet) {
      errlen += sprintf( &errstr[errlen],
			 "\n   %d syntax error%s found\n", 
			 error_count, (error_count == 1) ? "" : "s");
      MsgWindow::message( 'E', &errstr[errlen_old], msgw_ePop_No);
      errlen_old = errlen;
      wow->DisplayError( "Syntax control", errstr);
    }
  }
  return 1;
}









