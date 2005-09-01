/* 
 * Proview   $Id: wb_cmd.cpp,v 1.6 2005-09-01 14:57:48 claes Exp $
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

/* wb_cmd.c -- command file processing
   The main program of pwrc.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
#include <X11/cursorfont.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

extern "C" {
#include "pwr.h"
#include "pwr_class.h"
#include "co_dcli.h"
#include "co_mrm_util.h"
#include "wb_utl.h"
#include "wb_lfu.h"
#include "wb_login.h"

#include "ge.h"
#include "rt_load.h"
#include "wb_foe_msg.h"
#include "co_dcli_input.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_wnav.h"
#include "wb_wnav_item.h"
#include "wb_pal.h"
#include "wb_watt.h"
#include "wb_wnav_msg.h"
#include "wb_cmdc.h"
#include "wb.h"

static char cmd_volume[80];
static char *cmd_volume_p = 0;

static void usage()
{
  cout << endl << endl <<
    "pwrc        Proview workbench commands" << endl << endl << 
    "Arguments:" << endl <<
    "  -a             Load all configured databases." << endl <<
    "  -v 'volume'    Load volume 'volume'." << endl <<
    "  -h             Print usage." << endl << endl <<
    "  Other arguments are treated as a command and passed to the command parser" << endl <<
    "  If a command is given as an argument, the command will be executed and the" << endl <<
    "  program is then terminated." << endl <<
    "  If no command is given, pwrc will prompt for a command." << endl << endl <<
    "Examples:" << endl <<
    "  $ pwrc -v MyVolume" << endl <<
    "  pwrc>" << endl << endl <<
    "  $ pwrc -a show volume" << endl <<
    "  directory     Attached Db  $DirectoryVolume 254.254.254.253" << endl <<
    "  MyVolume               Db  $RootVolume        0.1.99.20" << endl <<
    "  $" << endl;
}

static int cmd_get_wbctx( void *ctx, ldh_tWBContext *wbctx)
{
  Cmd	*cmd = (Cmd *) ctx;
  int	sts;

  if ( cmd->wbctx)
  {
    *wbctx = cmd->wbctx;
    sts = 1;
  }
  else
  {
    sts = ldh_OpenWB( &cmd->wbctx, cmd_volume_p);
    if ( ODD(sts))
      *wbctx = cmd->wbctx;
  }
  return sts;
}

static int cmd_attach_volume_cb(
		void *ctx,
		pwr_tVolumeId	volid,
		int pop)
{
  Cmd *cmd = (Cmd *) ctx;
  int	sts;
  pwr_tVolumeId	vid;
  pwr_tClassId	classid;

  if ( cmd->ldhses)
  {
//    cmd->wnav->message( 'E', "Other volume is already attached");
    return WNAV__VOLATTACHED;
  }

  if ( !cmd->wbctx)
  {
    sts = cmd_get_wbctx( (void *)cmd, &cmd->wbctx);
    if ( EVEN(sts)) return sts;
  }

  if ( volid == 0)
  {
    if ( cmd_volume_p != 0) {
      // Attach argument volume
      ldh_VolumeNameToId( cmd->wbctx, cmd_volume_p, &volid);
      if ( EVEN(sts)) return sts;
    }
    else {
      // Attach first rootvolume, or if no rootvolume exist some other volume
      sts = ldh_GetVolumeList( cmd->wbctx, &vid);
      while ( ODD(sts)) {
	volid = vid;
	sts = ldh_GetVolumeClass( cmd->wbctx, vid, &classid);
	if ( EVEN(sts)) return sts;

	if ( classid == pwr_eClass_RootVolume)
	  break;
	sts = ldh_GetNextVolume( cmd->wbctx, vid, &vid);
      }
      if ( volid == 0) return sts;
    }
  }

  cmd->volid = volid;

  // Open ldh session
  sts = ldh_AttachVolume( cmd->wbctx, cmd->volid, &cmd->volctx);
  if ( EVEN(sts)) return sts;

  sts = ldh_OpenSession( &cmd->ldhses,
    cmd->volctx,
    ldh_eAccess_ReadWrite,
    ldh_eUtility_Pwr);
  if ( EVEN(sts)) {
    // Try read access
    sts = ldh_OpenSession( &cmd->ldhses,
			   cmd->volctx,
			   ldh_eAccess_ReadOnly,
			   ldh_eUtility_Pwr);
    if ( EVEN(sts)) return sts;
  }

  cmd->wnav->volume_attached( cmd->wbctx, cmd->ldhses, pop);

  return 1;
}

int Cmd::detach_volume()
{
  int sts;

  if ( !ldhses)
    return WNAV__NOVOLATTACHED;

  sts = ldh_CloseSession( ldhses);
  if ( EVEN(sts))
  {
    wnav->message( 'E', wnav_get_message( sts));
    return 0;
  }

  sts = ldh_DetachVolume( wbctx, volctx);
  if ( EVEN(sts))
  {
    wnav->message( 'E', wnav_get_message( sts));
    return 0;
  }
  volid = 0;
  volctx = 0;
  ldhses = 0;

  wnav->volume_detached();
  return 1;
}

static int cmd_detach_volume_cb( void *ctx)
{
  Cmd *cmd = (Cmd *) ctx;

  return cmd->detach_volume();
}

static void cmd_save_cb( void *ctx)
{
  Cmd *cmd = (Cmd *) ctx;
  int sts;

  if ( !cmd->ldhses) {
    cmd->wnav->message( 'E', "Cmd is not attached to a volume");
    return;
  }
  sts = ldh_SaveSession( cmd->ldhses);
  if ( EVEN(sts)) {
    cmd->wnav->message( 'E', wnav_get_message( sts));
    return;
  }

  ldh_sVolumeInfo info;
  pwr_tCid volcid;

  ldh_GetVolumeInfo( ldh_SessionToVol( cmd->ldhses), &info);
  ldh_GetVolumeClass( cmd->wbctx, info.Volume, &volcid);
  
  if ( volcid == pwr_eClass_DirectoryVolume) {
    sts = lfu_SaveDirectoryVolume( cmd->ldhses, 0);
    if ( EVEN(sts)) {
      cmd->wnav->message( 'E', "Syntax error");
      return;
    }
  }
  cmd->wnav->message( 'E', "Session saved");
}

static void cmd_revert_ok( Cmd *cmd)
{
  int sts;

  sts = ldh_RevertSession ( cmd->ldhses);
  if ( EVEN(sts))
    cmd->wnav->message( 'E', wnav_get_message( sts));
  else
    cmd->wnav->message( 'E', "Session reverted");
}

static void cmd_revert_cb( void *ctx, int confirm)
{
  Cmd *cmd = (Cmd *) ctx;

  if ( !cmd->ldhses)
  {
    cmd->wnav->message( 'E', "Cmd is not attached to a volume");
    return;
  }
  cmd_revert_ok( cmd);
}

static void cmd_close_cb( void *ctx)
{
  Cmd *cmd = (Cmd *) ctx;

  dcli_input_end( &cmd->chn, cmd->recall_buf);
  exit(0);
}

Cmd::Cmd() :
  ldhses(0), wbctx(0), volctx(0), volid(0), wnav(0), wb_type(0)
{
  Widget	w;
  pwr_tStatus	sts;

  wnav = new WNav( (void *)this, (Widget)0,"","",&w, ldhses, 
	(wnav_sStartMenu *)0, wnav_eWindowType_No, &sts);
  wnav->attach_volume_cb = &cmd_attach_volume_cb;
  wnav->detach_volume_cb = &cmd_detach_volume_cb;
  wnav->get_wbctx_cb = &cmd_get_wbctx;
  wnav->save_cb = &cmd_save_cb;
  wnav->revert_cb = &cmd_revert_cb;
  wnav->close_cb = &cmd_close_cb;

}

int main(int argc, char *argv[])
{

  pwr_tStatus	sts;
  int		i;
  char 		str[256] ;
  Cmd		*cmd;
  
  cmd = new Cmd;


  /* If arguments, treat them as a command and then exit */
  // Open directory volume as default
  strcpy( cmd_volume, "directory");
  cmd_volume_p = cmd_volume;

  str[0] = 0;
  for ( i = 1; i < argc; i++) {
    if ( argv[i][0] == '-') {
      switch ( argv[i][1]) {
      case 'h':
	usage();
	exit(0);
      case 'a':
	// Load all volumes
	cmd_volume_p = 0;
	break;
      case 'v':
	// Load specified volume
	if ( argc >= i) {
	  strcpy( cmd_volume, argv[i+1]);
	  cmd_volume_p = cmd_volume;
	  i++;
	  continue;
	}
	else
	  cout << "Syntax error, volume is missing" << endl;
	break;
      default:
	cout << "Unknown argument: " << argv[i] << endl;
      }
    }
    else {
      if ( str[0] != 0)
	strcat( str, " ");
      strcat( str, argv[i]);
    }
  }

  if ( str[0] != 0) {
    dcli_remove_blank( str, str);
    sts = cmd->wnav->command(str);
    if ( ODD(sts))
      return 0;
    exit(sts);
  }
  sts = dcli_input_init( &cmd->chn, &cmd->recall_buf);
  if ( EVEN(sts)) exit(sts);

  // Init input

  while ( 1 )
  {
    /* get and parse the command */

    /* get input */
    dcli_qio_set_attr( &cmd->chn);
    sts = dcli_get_input_command( &cmd->chn, "pwr> ", str, 
		sizeof(str), cmd->recall_buf);
    dcli_qio_reset( &cmd->chn);

//    sts = scanf( "%s", str);
      
    if ( strcmp( str, "") == 0)
      continue;

    dcli_remove_blank( str, str);
    sts = cmd->wnav->command(str);

  }
  dcli_input_end( &cmd->chn, cmd->recall_buf);
}
