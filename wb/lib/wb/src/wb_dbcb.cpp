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

#include <string.h>

#include "pwr.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_session.h"
#include "wb_dbcb.h"
#include "co_dcli.h"


pwr_tStatus dbcb_InsertPlcThreadObject( ldh_tSesContext ldhses, pwr_tOid object)
{
  pwr_tMask process;
  int process_found = 0;
  pwr_tOid thread_oid;
  int thread_found = 0;
  pwr_tOid min_toid;
  int min_toid_found = 0;
  pwr_tFloat32 scantime;
  pwr_tFloat32 min_scantime = 1e37;

  wb_session *sp = (wb_session *)ldhses;
  wb_object o = sp->object( object);

  // Get Process and PlcThreadObject from parent
  wb_object parent = o.parent();
  if ( parent) {
    wb_attribute a = sp->attribute( parent.oid(), "RtBody", "Process");
    if ( a) {
      a.value( &process);
      if (a)
	process_found = 1;
    }

    if ( process_found && (process & pwr_mIoProcessMask_Plc)) {
      a = sp->attribute( parent.oid(), "RtBody", "ThreadObject");
      if ( a) {
	a.value( &thread_oid);
	if ( a)
	  thread_found = 1;
      }
    }
  }

  if ( process_found) {
    wb_attribute a = sp->attribute( o.oid(), "RtBody", "Process");
    if ( a) {
      sp->writeAttribute( a, &process, sizeof(process));
    }
  }

  if ( thread_found) {
    wb_attribute a = sp->attribute( o.oid(), "RtBody", "ThreadObject");
    if ( a) {
      sp->writeAttribute( a, &thread_oid, sizeof(thread_oid));
    }
  }


  if ( !process_found && !thread_found) {
    wb_attribute a = sp->attribute( o.oid(), "RtBody", "Process");
    if ( a) {
      a.value( &process);
      if (a)
	process_found = 1;
    }

    if ( process_found && (process & pwr_mIoProcessMask_Plc)) {
      for ( wb_object to = sp->object( pwr_cClass_PlcThread); to; to = to.next()) {
	wb_attribute at = sp->attribute( to.oid(), "RtBody", "ScanTime");
	if ( at) {
	  at.value( &scantime);
	  if ( scantime < min_scantime) {
	    min_scantime = scantime;
	    min_toid = to.oid();
	    min_toid_found = 1;
	  }
	}
      }
      if ( min_toid_found) {
	wb_attribute a = sp->attribute( o.oid(), "RtBody", "ThreadObject");
	if ( a) {
	  sp->writeAttribute( a, &min_toid, sizeof(min_toid));
	}
      }
    }
  }
  return PWRB__SUCCESS;
}
