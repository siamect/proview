/* 
 * Proview   $Id: xtt_c_plcpgm.cpp,v 1.4 2007-01-04 08:22:46 claes Exp $
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

/* wb_c_plcpgm.c -- work bench methods of the PlcPgm class. */

#include "pwr_baseclasses.h"
#include "flow_std.h"
#include "co_cdh.h"
#include "xtt_menu.h"
#include "xtt_xnav.h"
#include "rt_xnav_msg.h"
#include "pwr_privilege.h"

// Open trace
static pwr_tStatus OpenTrace( xmenu_sMenuCall *ip)
{
  pwr_tOName name;
  pwr_tCmd cmd;
  int sts;

  printf( "OpenProgram method called\n");

  //foe_Open (ip->EditorContext, ip->WindowContext, ip->PointedSession,
  //  ip->Pointed.Objid);

  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  sprintf( cmd, "open trace/name=%s", name);
  ((XNav *)ip->EditorContext)->command( cmd);
  return 1;
}


//
// Set scan
//
static pwr_tStatus SetScan( xmenu_sMenuCall *ip)
{
  pwr_tObjid child;
  int sts;
  pwr_tBoolean local;
  pwr_tObjid io_handler;
  pwr_sClass_IOHandler *io_handler_p;
  pwr_tBoolean value = 0;
  pwr_tOName name;
  pwr_tClassId classid;

  if ( strcmp( "Enable", ip->ItemList[ip->ChosenItem].MethodArguments[0]) == 0)
    value = 0;
  else if ( strcmp( "Disable", ip->ItemList[ip->ChosenItem].MethodArguments[0]) == 0)
    value = 1;
  else
    return XNAV__SUCCESS;
    
  // Only on local objects
  sts = gdh_GetObjectLocation( ip->Pointed.Objid, &local);
  if ( EVEN(sts) || !local)
    return 0;  

  // Only if simulate
  sts = gdh_GetClassList( pwr_cClass_IOHandler, &io_handler);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( io_handler, (void **) &io_handler_p);
  if ( EVEN(sts)) return sts;

  if ( !io_handler_p->IOSimulFlag)
    return 0;

  sts = gdh_GetChild( ip->Pointed.Objid, &child);
  while( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_windowplc) {
      sts = gdh_ObjidToName( child, name, sizeof(name),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      strcat( name, ".ScanOff");
      sts = gdh_SetObjectInfo( name, (void *) &value, sizeof( value));
      if ( EVEN(sts)) return sts;

      return XNAV__SUCCESS;
    }
  }
  return 0;
}
//
// Set Scan filter
//
static pwr_tStatus SetScanFilter( xmenu_sMenuCall *ip)
{
  pwr_tObjid child;
  int sts;
  pwr_tBoolean local;
  pwr_tObjid io_handler;
  pwr_sClass_IOHandler *io_handler_p;
  pwr_sClass_windowplc *window_p;
  pwr_tClassId classid;
  int enable;

  if ( !ip->Priv & pwr_mPrv_System)
    return XNAV__INSENSITIVE;

  if ( strcmp( "Enable", ip->ItemList[ip->ChosenItem].FilterArguments[0]) == 0)
    enable = 1;
  else if ( strcmp( "Disable", ip->ItemList[ip->ChosenItem].FilterArguments[0]) == 0)
    enable = 0;
  else
    return XNAV__SUCCESS;

  // Only on local objects
  sts = gdh_GetObjectLocation( ip->Pointed.Objid, &local);
  if ( EVEN(sts) || !local)
    return XNAV__INSENSITIVE;

  // Only if simulate
  sts = gdh_GetClassList( pwr_cClass_IOHandler, &io_handler);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( io_handler, (void **) &io_handler_p);
  if ( EVEN(sts)) return sts;

  if ( !io_handler_p->IOSimulFlag)
    return XNAV__INSENSITIVE;

  sts = gdh_GetChild( ip->Pointed.Objid, &child);
  while( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_windowplc) {
      sts = gdh_ObjidToPointer( child, (void **) &window_p);
      if ( EVEN(sts)) return sts;

      if ( (enable && window_p->ScanOff) ||
           (!enable && !window_p->ScanOff))
        return XNAV__SUCCESS;
      else
        return XNAV__INSENSITIVE;
    }
  }
  return XNAV__INVISIBLE;
}

static pwr_tStatus SimulateOn( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid io_handler;
  pwr_sClass_IOHandler *io_handler_p;

  // Only if simulate
  sts = gdh_GetClassList( pwr_cClass_IOHandler, &io_handler);
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  sts = gdh_ObjidToPointer( io_handler, (void **) &io_handler_p);
  if ( EVEN(sts)) return sts;

  if ( !io_handler_p->IOSimulFlag)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to xtt should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindXttMethods(PlcPgm) = {
  pwr_BindXttMethod(OpenTrace),
  pwr_BindXttMethod(SetScan),
  pwr_BindXttMethod(SetScanFilter),
  pwr_BindXttMethod(SimulateOn),
  pwr_NullMethod
};





