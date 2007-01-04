/* 
 * Proview   $Id: wb_main.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb.c -- work bench */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>

#include "wb_env.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "pwr.h"
#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "wb.h"
#include "wb_ldhld.h"
#include "wb_ldh.h"
#include "wb_login.h"
#include "wb_login_msg.h"
#include "wb_gre.h"
#include "wb_dir.h"
#include "wb_utl_api.h"

#include "wb_vsel.h"
#include "co_msgwindow.h"
#include "co_xhelp.h"
#include "co_lng.h"
#include "wb_wtt.h"

#include "wb_erep.h"
#include "wb_vrepwbl.h"
#include "wb_vrepdbs.h"
#include "wb_vrepmem.h"
#include "wb_vrepext.h"
#include "wb_main.h"

using namespace std;

Wb *Wb::main_wb = 0;

void Wb::usage()
{
  printf("\n\
Usage: wb [-a][-q][-c][-p] [-l language] [username] [password] [volume]\n\
\n\
  -a    Attach all databases.\n\
  -q    Quiet. Hide license information.\n\
  -c    Start class editor.\n\
  -p    Open project list.\n\
  -l    Language specification, sv_se or en_us.\n\
  -h    Display this help text.\n\
\n");
}

void Wb::wttlist_add( pwr_tStatus *sts, Wtt *wtt, pwr_tVid vid)
{
  wttlist_iterator it = wttlist.find( vid); 
  if ( it == wttlist.end()) {
    wttlist[vid] = wtt;
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__VOLIDALREXI;
}

void Wb::wttlist_remove( pwr_tStatus *sts, Wtt* wtt)
{
  for ( wttlist_iterator it = wttlist.begin(); it != wttlist.end(); it++) {
    if ( it->second == wtt) {
      wttlist.erase( it);
      *sts = LDH__SUCCESS;
      return;
    }
  }
  *sts = LDH__NOSUCHVOL;
}

void Wb::wttlist_find( pwr_tStatus *sts, pwr_tVid vid, Wtt **wtt)
{
  wttlist_iterator it = wttlist.find( vid);
  if ( it == wttlist.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  *sts = LDH__SUCCESS;
  *wtt = it->second;
}

int Wb::psts(unsigned long int sts, FILE *logfile)
{
  char msg[200];

  if (!(sts & 1))
  {
    msg_GetMsg( sts, msg, sizeof(msg));

    if (logfile != NULL)
      fprintf(logfile, "%s\n", msg);
    else
      printf("%s\n", msg);
  }
  return sts & 1;
}

void Wb::find_wnav_cb( void *ctx, pwr_tOid oid)
{
  Wb *wb = main_wb;
  char title[80];
  char projectname[80];
  pwr_tStatus sts;
  Wtt *wtt;
  printf( "Here in find wnav...\n");

  wb->wttlist_find( &sts, oid.vid, &wtt);
  if ( ODD(sts)) {
    printf( "Wtt Found\n");
    sts = wtt->find( oid);
    wtt->pop();
  }
  else {
    utl_get_projectname( projectname);
    strcpy( title, login_prv.username);
    strcat( title, " on ");
    strcat( title, projectname);

    wtt = wb->wtt_new( title, "Navigator", wb->wbctx, oid.vid, 0, 0, &sts);
    if (ODD(sts)) {
      wb->appl_count++;
      wtt->close_cb = Wb::wtt_close;
      wtt->open_volume_cb = Wb::wtt_open_volume;
      wtt->time_to_exit_cb = Wb::time_to_exit;
      wb->wttlist_add( &sts, wtt, oid.vid);
      sts = wtt->find( oid);
    }
  }
}

void Wb::find_plc_cb( void *ctx, pwr_tOid oid)
{
  Wb *wb = main_wb;
  char title[80];
  char projectname[80];
  pwr_tStatus sts;
  Wtt *wtt;

  wb->wttlist_find( &sts, oid.vid, &wtt);
  if ( ODD(sts)) {
    sts = wtt->find_plc( oid);
  }
  else {
    utl_get_projectname( projectname);
    strcpy( title, login_prv.username);
    strcat( title, " on ");
    strcat( title, projectname);

    wtt = wb->wtt_new( title, "Navigator", wb->wbctx, oid.vid, 0, 0, &sts);
    if (ODD(sts)) {
      wb->appl_count++;
      wtt->close_cb = Wb::wtt_close;
      wtt->open_volume_cb = Wb::wtt_open_volume;
      wtt->time_to_exit_cb = Wb::time_to_exit;
      wb->wttlist_add( &sts, wtt, oid.vid);
      sts = wtt->find_plc( oid);
    }
  }
}

void Wb::login_success()
{
  Wb *wb = main_wb;
  char title[80];
  char systemname[80];
  char systemgroup[80];
  pwr_tStatus sts;
  char msg[80];

  printf( "-- Successfull login\n");
  sprintf( msg, "User %s logged in", login_prv.username);
  MsgWindow::message( 'I', msg);

  /* Successfull login, start the volume selection */ 

  if ( login_prv.priv & pwr_mPrv_DevRead )
  {
    utl_get_systemname( systemname, systemgroup);
    strcpy( title, "PwR Navigator: ");
    strcat( title, login_prv.username);
    strcat( title, " on ");
    strcat( title, systemname);
    wb->appl_count++;
    wb->vsel_new( &sts, "PwR Volumes", wb->wbctx, NULL, 
		&Wb::vsel_success, &Wb::vsel_cancel, &Wb::time_to_exit, 0, wb_eType_Volume);
  }
  else
  {
    printf( "** Not authorized for development\n");
    exit(LOGIN__NOPRIV);
  }
}

void Wb::wtt_close( void *wttctx)
{
  Wb *wb = main_wb;
  pwr_tStatus sts;

  wb->wttlist_remove( &sts, (Wtt *)wttctx);
  wb->appl_count--;
  if ( wb->appl_count == 0)
  {
    exit(0);
  }
}

int Wb::time_to_exit( void *wttctx)
{
  Wb *wb = main_wb;

  if (wb->appl_count == 1)
    return 1;
  return 0;
}

void Wb::wtt_open_volume( void *wttctx, wb_eType type, char *filename, wow_eFileSelType file_type)
{
  Wb *wb = main_wb;
  char title[80];
  char systemname[80];
  char systemgroup[80];
  pwr_tStatus sts;

  if ( login_prv.priv & pwr_mPrv_DevRead || login_prv.priv & pwr_mPrv_Administrator)
  {
    if ( !filename) {
      utl_get_systemname( systemname, systemgroup);
      strcpy( title, "PwR Navigator: ");
      strcat( title, login_prv.username);
      strcat( title, " on ");
      strcat( title, systemname);
      wb->appl_count++;
      wb->vsel_new( &sts, "PwR Volumes", wb->wbctx, NULL, 
		&Wb::vsel_success, &Wb::vsel_cancel, &Wb::time_to_exit, 1,
		type);
    }
    else {
      // Open the file
      if ( file_type == wow_eFileSelType_Wbl) {
        printf( "Wb opening wb_load-file %s...\n", filename);

        // Load volume as extern
	wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
        wb_vrepwbl *vrep = new wb_vrepwbl( erep);
        sts = vrep->load( filename);
	if ( vrep->vid() == 0) {
	  delete vrep;
	  return;
	}
	erep->addExtern( &sts, vrep);

        // Attach extern volume
	wb_volume *vol = new wb_volume(vrep);
	pwr_tVid volume = vrep->vid();

        Wtt *wtt = wb->wtt_new( filename, "Navigator", wb->wbctx, volume, vol, 0, &sts);
        if (ODD(sts)) {
          wb->appl_count++;
          wtt->close_cb = Wb::wtt_close;
	  wtt->open_volume_cb = Wb::wtt_open_volume;
	  wtt->time_to_exit_cb = Wb::time_to_exit;
        }
      }
      else if ( file_type == wow_eFileSelType_Dbs) {
        printf( "Wb opening loadfile %s...\n", filename);

        // Load volume as extern
	wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
        wb_vrepdbs *vrep = new wb_vrepdbs( erep, filename);
	try {
          vrep->load();
	  erep->addExtern( &sts, vrep);
	}
	catch ( wb_error& e) {
	  cout << "** Error opening volume, " << e.what() << endl;
	  return;
	}

        // Attach extern volume
	wb_volume *vol = new wb_volume(vrep);
	pwr_tVid volume = vrep->vid();

        Wtt *wtt = wb->wtt_new( filename, "Navigator", wb->wbctx, volume, vol, 0, &sts);
        if (ODD(sts)) {
          wb->appl_count++;
          wtt->close_cb = Wb::wtt_close;
	  wtt->open_volume_cb = Wb::wtt_open_volume;
	  wtt->time_to_exit_cb = Wb::time_to_exit;
        }
      }
      else if ( file_type == wow_eFileSelType_WblClass) {
        printf( "Wb opening wb_load-file %s...\n", filename);

        // Load volume and import to vrepmem
	wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
	wb_vrepmem *mem = new wb_vrepmem(erep, 0);
	mem->loadWbl( filename, &sts);
	if ( EVEN(sts)) {
	  delete mem;
	  if ( sts == LDH__OTHERSESS)
	    MsgWindow::message( 'E', "Other class volume is open", msgw_ePop_Yes);
	  return;
	}
	erep->addExtern( &sts, mem);

        // Display buffer
	wb_volume *vol = new wb_volume(mem);

	// Display filename i title, without path
        char *name_p;
	if ( (name_p = strrchr( filename, '/')))
	  name_p++;
	else
	  name_p = filename;

        Wtt *wtt = wb->wtt_new( name_p, 
			    "Navigator", wb->wbctx, mem->vid(), vol, 0, &sts);
        if (ODD(sts)) {
          wb->appl_count++;
          wtt->close_cb = Wb::wtt_close;
	  wtt->open_volume_cb = Wb::wtt_open_volume;
	  wtt->time_to_exit_cb = Wb::time_to_exit;
        }
      }
      else {
	if ( strcmp( filename, "ProjectList") == 0) {
	  // Load ProjectList

	  wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
	  wb_vrepext *ext = new wb_vrepext(erep, ldh_cProjectListVolume, filename, filename);
	  erep->addExtern( &sts, ext);

	  // Display buffer
	  wb_volume *vol = new wb_volume(ext);

	  Wtt *wtt = wb->wtt_new( filename, 
			      "Navigator", wb->wbctx, ext->vid(), vol, 0, &sts);
	  if (ODD(sts)) {
	    wb->appl_count++;
	    wtt->close_cb = Wb::wtt_close;
	    wtt->open_volume_cb = Wb::wtt_open_volume;
	    wtt->time_to_exit_cb = Wb::time_to_exit;
	  }
	}
	else if ( strcmp( filename, "GlobalVolumeList") == 0) {
	  // Load GlobalVolumeList

	  wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
	  wb_vrepext *ext = new wb_vrepext(erep, ldh_cGlobalVolumeListVolume, filename, filename);
	  erep->addExtern( &sts, ext);

	  // Display buffer
	  wb_volume *vol = new wb_volume(ext);

	  Wtt *wtt = wb->wtt_new( filename, 
			      "Navigator", wb->wbctx, ext->vid(), vol, 0, &sts);
	  if (ODD(sts)) {
	    wb->appl_count++;
	    wtt->close_cb = Wb::wtt_close;
	    wtt->open_volume_cb = Wb::wtt_open_volume;
	    wtt->time_to_exit_cb = Wb::time_to_exit;
	  }
	}
	else if ( strcmp( filename, "UserDatabase") == 0) {
	  // Load UserDatabase

	  wb_erep *erep = (wb_erep *)(*(wb_env *)wb->wbctx);
	  wb_vrepext *ext = new wb_vrepext(erep, ldh_cUserDatabaseVolume, filename, filename);
	  erep->addExtern( &sts, ext);

	  // Display buffer
	  wb_volume *vol = new wb_volume(ext);

	  Wtt *wtt = wb->wtt_new( filename, 
			      "Navigator", wb->wbctx, ext->vid(), vol, 0, &sts);
	  if (ODD(sts)) {
	    wb->appl_count++;
	    wtt->close_cb = Wb::wtt_close;
	    wtt->open_volume_cb = Wb::wtt_open_volume;
	    wtt->time_to_exit_cb = Wb::time_to_exit;
	  }
	}
	else
	  printf( "Unknown file\n");
      }
    }

  }
  else {
    printf( "No privileges to enter development environment");
    if (wb->appl_count == 0)
      exit(LOGIN__NOPRIV);
  }
}

int Wb::vsel_success( void *vselctx, pwr_tVolumeId *volumelist, int volume_count)
{
  Wb *wb = main_wb;
  char		projectname[80];
  char 		title[80];
  pwr_tVolumeId	volume;
  int		i;
  Wtt	        *wtt;
  int		sts;
  pwr_tStatus	status;
  
  sts = 1;
  if ( login_prv.priv & pwr_mPrv_DevRead )
  {
    for ( i = 0; i < volume_count; i++)
    {
      volume = *volumelist++;
      utl_get_projectname( projectname);
      strcpy( title, login_prv.username);
      strcat( title, " on ");
      strcat( title, projectname);
      wtt = wb->wtt_new( title, "Navigator", wb->wbctx, volume, 0, 0, &status);
      if (ODD(status)) {
        wb->appl_count++;
        wtt->close_cb = Wb::wtt_close;
	wtt->open_volume_cb = Wb::wtt_open_volume;
	wtt->time_to_exit_cb = Wb::time_to_exit;
	wb->wttlist_add( &sts, wtt, volume);
      }
      else
        sts = status;
    }
  }
  else
  {
    exit(LOGIN__NOPRIV);
  }
  if ( ODD(sts) && wb->appl_count == 0)
    exit(0);

  return sts;
}

void Wb::vsel_cancel()
{
  Wb *wb = main_wb;

  wb->appl_count--;
  if (wb->appl_count == 0)
  {
    exit(0);
  }
}

void Wb::login_cancel()
{
  printf( "-- Login canceled\n");
  /* Not successfull login, exit */
  exit(LOGIN__AUTHFAIL);
}

