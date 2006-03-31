/* 
 * Proview   $Id: wb_build.cpp,v 1.1 2006-03-31 14:24:34 claes Exp $
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


#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_msgwindow.h"
#include "co_dcli.h"
#include "co_time.h"
#include "wb_foe_msg.h"
#include "wb_pwrb_msg.h"
#include "wb_utl.h"
#include "wb_build.h"
#include "wb_name.h"
#include "wb_lfu.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_graph.h"
#include "ge.h"

void wb_build::classlist( pwr_tCid cid)
{
  pwr_tStatus 		sumsts;

  // Build all objects of specified class
  sumsts = PWRB__NOBUILT;
  for ( wb_object o = m_session.object( cid); o.oddSts(); o = o.next()) {

    // Call build method for object
    switch ( cid) {
    case pwr_cClass_plc:
      plcpgm( o.oid());
      break;
    case pwr_cClass_XttGraph:
      xttgraph( o.oid());
      break;
    case pwr_cClass_WebHandler:
      webhandler( o.oid());
      break;
    default: 
      m_sts = PWRB__NOBUILT;
    }
    if ( evenSts())
      sumsts = m_sts;
    else if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT && m_sts != PWRB__INLIBHIER)
      sumsts = m_sts;
  }
  m_sts = sumsts;
}

void wb_build::node( char *nodename, void *volumelist, int volumecnt)
{
  lfu_t_volumelist *vlist = (lfu_t_volumelist *)volumelist;
  pwr_tTime vtime;
  pwr_tTime btime;
  pwr_tFileName fname;
  pwr_tObjName vname;
  int bussid = -1;
  int rebuild = 1;
  pwr_tStatus status;

  printf( "Build node %s\n", nodename);

  if ( !opt.manual) {
    // Check if there is any new dbsfile
    rebuild = 0;
    for ( int i = 0; i < volumecnt; i++) {
      if ( cdh_NoCaseStrcmp( nodename, vlist[i].p1) == 0) {
	if ( bussid == -1) {
	  char systemname[80], systemgroup[80], pname[80];
	  pwr_tVid *vl;
	  pwr_tString40 *vnl;
	  int vcnt;
	
	  // Get time for current bootfile
	  status = sscanf( vlist[i].p3, "%d", &bussid);
	  if ( status != 1) {
	    // Bussid error
	  }
	
	  sprintf( fname, load_cNameBoot, 
		   load_cDirectory, nodename, bussid);
	  dcli_translate_filename( fname, fname);
	  status = lfu_ReadBootFile( fname, &btime, systemname, systemgroup, &vl, &vnl, 
				     &vcnt, pname);
	  if ( EVEN(status)) {
	    rebuild = 1;
	    break;
	  }	 
	}
      
	if ( vlist[i].volume_id == m_session.vid()) {
	  // Build current volume
	  volume();
	}
      
	cdh_ToLower( vname, vlist[i].volume_name);
	if ( vlist[i].volume_id >= cdh_cUserVolMin &&
	     vlist[i].volume_id <= cdh_cUserVolMax) {
	  sprintf( fname, "$pwrp_load/%s.dbs", vname);
	  dcli_translate_filename( fname, fname);
	  m_sts = dcli_file_time( fname, &vtime);
	  if ( evenSts()) {
	    // Dbs file is missing
	    return;
	  }
	  if ( vtime.tv_sec > btime.tv_sec)
	    rebuild = 1;
	}
      }
    }
  }
  if ( opt.force || opt.manual || rebuild)
    m_sts = lfu_create_bootfile( nodename, (lfu_t_volumelist *)volumelist, volumecnt,
				 opt.debug);
  else
    m_sts = PWRB__NOBUILT;

  if ( m_sts != PWRB__NOBUILT) {
    char msg[200];

    sprintf( msg, "Build:    Node     %s", nodename);
    MsgWindow::message('I', msg, msgw_ePop_No);
  }
}

void wb_build::volume()
{
  switch ( m_session.cid()) {
  case pwr_eClass_RootVolume:
  case pwr_eClass_SubVolume:
  case pwr_eClass_SharedVolume:
    rootvolume(0);
    break;
  case pwr_eClass_ClassVolume:
    classvolume(0);
    break;
  default:
    ;
  }
}

void wb_build::rootvolume( pwr_tVid vid)
{
  pwr_tStatus 		sumsts, plcsts;
  pwr_tOid		oid;
  pwr_tTime		modtime;
  pwr_tObjName		vname;
  pwr_tFileName		fname;
  pwr_tTime		dbs_time, rtt_time;
  pwr_tCmd		cmd;
  char			msg[80];

  if ( !opt.manual) {
    // Build all plcpgm
    classlist( pwr_cClass_plc);
    if ( evenSts()) return;
    plcsts = sumsts = m_sts;

    // Build all XttGraph
    classlist( pwr_cClass_XttGraph);
    if ( evenSts()) return;
    if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT)
      sumsts = m_sts;

    classlist( pwr_cClass_WebHandler);
    if ( evenSts()) return;
    if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT)
      sumsts = m_sts;
  }

  // Create loadfiles
  oid.oix = 0;
  oid.vid = m_session.vid();
  wb_attribute a = m_session.attribute( oid, "SysBody", "Modified");
  if ( !a) {
    m_sts = a.sts();
    return;
  }

  a.value( &modtime);
  if ( !a) {
    m_sts = a.sts();
    return;
  }

  cdh_ToLower( vname, m_session.name());
  sprintf( fname, "$pwrp_load/%s.dbs", vname);
  dcli_translate_filename( fname, fname);
  m_sts = dcli_file_time( fname, &dbs_time);

  if ( opt.force || opt.manual || evenSts() || time_Acomp( &modtime, &dbs_time) == 1 ||
       plcsts != PWRB__NOBUILT) {
    m_sts = lfu_create_loadfile( (ldh_tSession *) &m_session);
    if ( evenSts()) return;
    m_sts = ldh_CreateLoadFile( (ldh_tSession *) &m_session);
    if ( evenSts()) return;

    sprintf( msg, "Build:    Volume   Loadfiles created volume %s", m_session.name());
    MsgWindow::message('I', msg, msgw_ePop_No);

    sumsts = PWRB__SUCCESS;
  }
  else
    m_sts = sumsts;

  cdh_uVolumeId	uvid;
  uvid.pwr = m_session.vid();
  sprintf( fname, "$pwrp_load/" load_cNameRttCrr,
	   uvid.v.vid_3, uvid.v.vid_2, uvid.v.vid_1, uvid.v.vid_0);
  dcli_translate_filename( fname, fname);
  m_sts = dcli_file_time( fname, &rtt_time);
  if ( opt.crossref && ( evenSts() || time_Acomp( &modtime, &rtt_time) == 1)) {
    strcat( cmd, "create rttfiles");
    m_wnav->command( cmd);
    if ( ODD(sumsts))
      sumsts = PWRB__SUCCESS;
	 
    sprintf( msg, "Build:    Volume   Crossreference file generated volume %s", m_session.name());
    MsgWindow::message('I', msg, msgw_ePop_No);
  }


  m_sts = sumsts;
}

void wb_build::classvolume( pwr_tVid vid)
{
  pwr_tCmd cmd;
  pwr_tFileName fname;
  pwr_tObjName name;
  pwr_tTime wbl_time, dbs_time, h_time;
  pwr_tStatus fsts;

  if ( vid == 0) {
    // Build current volume
    cdh_ToLower( name, m_session.name());
  }
  else {
    wb_env env = m_session.env();
    wb_volume v = env.volume(vid);
    if ( !v) {
      m_sts = v.sts();
      return;
    }
    strcpy( name, v.name());
  }

  // Get time for wb_load file
  sprintf( fname, "$pwrp_db/%s.wb_load", name);
  dcli_translate_filename( fname, fname);
  m_sts = dcli_file_time( fname, &wbl_time);
  if ( evenSts())
    return;

  // Get time for dbs file
  sprintf( fname, "$pwrp_load/%s.dbs", name);
  dcli_translate_filename( fname, fname);
  fsts = dcli_file_time( fname, &dbs_time);

  // Create new loadfile
  if ( EVEN(fsts) || wbl_time.tv_sec > dbs_time.tv_sec) {
    sprintf( cmd, "create snapshot/file=\"$pwrp_db/%s.wb_load\"", name);
    m_sts = m_wnav->command( cmd);
  }
  else
    m_sts = PWRB__NOBUILT;

  // Get time for struct file
  sprintf( fname, "$pwrp_inc/pwr_%sclasses.h", name);
  dcli_translate_filename( fname, fname);
  fsts = dcli_file_time( fname, &h_time);

  // Create new struct file
  if ( EVEN(fsts) || wbl_time.tv_sec > h_time.tv_sec) {
    sprintf( cmd, "create struct/file=\"$pwrp_db/%s.wb_load\"", name);
    m_sts = m_wnav->command( cmd);
  }

  if ( m_sts != PWRB__NOBUILT) {
    char msg[80];

    sprintf( msg, "Build:    Volume   %s", name);
    MsgWindow::message('I', msg, msgw_ePop_No);
  }
}

void wb_build::planthier( pwr_tOid oid)
{
  pwr_tStatus 		sumsts;

  m_hierarchy = oid;

  // Build all plcpgm
  classlist( pwr_cClass_plc);
  if ( evenSts()) return;
  sumsts = m_sts;

  // Build all XttGraph
  classlist( pwr_cClass_XttGraph);
  if ( evenSts()) return;
  if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT)
    sumsts = m_sts;

  m_sts = sumsts;
}

void wb_build::nodehier( pwr_tOid oid)
{
  pwr_tStatus sumsts;
  m_hierarchy = oid;

  // Build all XttGraph
  classlist( pwr_cClass_XttGraph);
  if ( evenSts()) return;
  sumsts = m_sts;

  classlist( pwr_cClass_XttGraph);
  if ( evenSts()) return;
  if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT)
    sumsts = m_sts;

  classlist( pwr_cClass_WebHandler);
  if ( evenSts()) return;
  if ( sumsts == PWRB__NOBUILT && m_sts != PWRB__NOBUILT)
    sumsts = m_sts;

  m_sts = sumsts;
}

void wb_build::plcpgm( pwr_tOid oid)
{
  int check_hierarchy = cdh_ObjidIsNotNull( m_hierarchy);
  int hierarchy_found = 0;

  wb_object o = m_session.object(oid);
  if ( !o) { 
    m_sts = o.sts(); 
    return; 
  }

  // Check that no ancestor is a LibHier
  for ( wb_object p = o.parent(); p.oddSts(); p = p.parent()) {
    if ( p.cid() == pwr_eClass_LibHier) {
      m_sts =  PWRB__INLIBHIER;
      return;
    }
    if ( check_hierarchy && cdh_ObjidIsEqual( m_hierarchy, p.oid()))
      hierarchy_found = 1;
  }

  if ( check_hierarchy && !hierarchy_found) {
    m_sts = PWRB__NOBUILT;
    return;
  }

  m_sts = utl_compile( (ldh_tSession *)&m_session, ldh_SessionToWB( (ldh_tSession *)&m_session), 
		     o.longName().name(cdh_mName_volumeStrict), 0, 0, 0, 
		      !opt.force, opt.debug, 0, 0); 
  if ( oddSts() && m_sts != GSX__NOMODIF) {
    char msg[200];

    sprintf( msg, "Build:    PlcPgm    %s", o.longName().name(cdh_mName_path | cdh_mName_object));
    MsgWindow::message('I', msg, msgw_ePop_No, oid);
  }
  else if ( m_sts == GSX__NOMODIF) {
    m_sts = PWRB__NOBUILT;
  }
}

void wb_build::xttgraph( pwr_tOid oid)
{
  pwr_tFileName src_fname, dest_fname;
  pwr_tCmd	cmd;
  pwr_tString80	action;
  pwr_tTime	dest_time, src_time;
  int 		check_hierarchy = cdh_ObjidIsNotNull( m_hierarchy);
  int 		hierarchy_found = 0;
  int 		is_frame, is_applet;
  char 		java_name[80];
  pwr_tStatus  	fsts;
  int		jexport;

  wb_object o = m_session.object(oid);
  if ( !o) {
    m_sts = o.sts();
    return;
  }

  // Check that no ancestor is a LibHier
  for ( wb_object p = o.parent(); p.oddSts(); p = p.parent()) {
    if ( p.cid() == pwr_eClass_LibHier) {
      m_sts = PWRB__INLIBHIER;
      return;
    }
    if ( check_hierarchy && cdh_ObjidIsEqual( m_hierarchy, p.oid()))
      hierarchy_found = 1;
  }

  if ( check_hierarchy && !hierarchy_found) {
    m_sts = PWRB__NOBUILT;
    return;
  }

  wb_attribute a = m_session.attribute( oid, "RtBody", "Action");
  if ( !a) {
    m_sts = a.sts();
    return;
  }

  a.value( &action);
  if ( !a) {
    m_sts = a.sts();
    return;
  }

  if ( strstr( action, ".pwg")) {
    strcpy( src_fname, "$pwrp_pop/");
    strcat( src_fname, action);
    dcli_translate_filename( src_fname, src_fname);
    m_sts = dcli_file_time( src_fname, &src_time);
    if ( evenSts()) {
      m_sts = PWRB__NOBUILT;
      return;
    }

    strcpy( dest_fname, "$pwrp_exe/");
    strcat( dest_fname, action);
    dcli_translate_filename( dest_fname, dest_fname);
    m_sts = dcli_file_time( dest_fname, &dest_time);
    if ( opt.force || evenSts() || src_time.tv_sec > dest_time.tv_sec) {
      sprintf( cmd, "cp %s %s", src_fname, dest_fname);
      system( cmd);
      sprintf( cmd, "Build:    XttGraph copy $pwrp_pop/%s -> $pwrp_exe", action);
      MsgWindow::message( 'I', cmd, msgw_ePop_No, oid);
      m_sts = PWRB__SUCCESS;
    }
    else
      m_sts = PWRB__NOBUILT;

    jexport = 0;
    fsts = grow_IsJava( src_fname, &is_frame, &is_applet, java_name);
    if ( EVEN(fsts)) {
      m_sts = fsts;
      return;
    }
    if ( is_frame) {
      // Check exported java frame
      sprintf( dest_fname, "$pwrp_pop/%s.java", java_name);
      dcli_translate_filename( dest_fname, dest_fname);
      fsts = dcli_file_time( dest_fname, &dest_time);
      if ( opt.force || EVEN(fsts) || time_Acomp( &src_time, &dest_time) == 1)
	jexport = 1;
    }
    if ( is_applet) {
      // Check exported java applet
      sprintf( dest_fname, "$pwrp_pop/%s_A.java", java_name);
      dcli_translate_filename( dest_fname, dest_fname);
      fsts = dcli_file_time( dest_fname, &dest_time);
      if ( opt.force || EVEN(fsts) || time_Acomp( &src_time, &dest_time) == 1)
	jexport = 1;
    }
    if ( jexport) {
      if ( !m_window) {
	sprintf( cmd, "Build:    XttGraph  Unable to export java in this environment %s", action);
	MsgWindow::message('W', cmd, msgw_ePop_No, oid);
      }
      else {
	void *gectx = ge_new( NULL, (Widget)m_window, (ldh_tSession *)&m_session, 0, action);
	strcpy( cmd, "export java");
	ge_command( gectx, cmd);
	ge_del( gectx);

	sprintf( cmd, "Build:    XttGraph  Export java %s", action);
	MsgWindow::message('I', cmd, msgw_ePop_No, oid);

	m_sts = PWRB__SUCCESS;
      }
    }
  }
}

void wb_build::webhandler( pwr_tOid oid)
{
  pwr_tTime	modtime;
  char 		timestr[40];
  pwr_tString80 file_name, name;
  pwr_tFileName fname;
  pwr_tTime 	ftime;
  pwr_tTime	xtthelp_time, html_time;
  char 		*s;
  pwr_tStatus   fsts;

  wb_object o = m_session.object(oid);
  if ( !o) {
    m_sts = o.sts();
    return;
  }

  modtime = o.modTime();

  time_AtoAscii( &modtime, time_eFormat_DateAndTime, timestr, sizeof(timestr));
  printf( "WebHandler time: %s\n", timestr);

  wb_attribute a = m_session.attribute( oid, "RtBody", "FileName");
  if ( !a) {
    m_sts = a.sts();
    return;
  }
  a.value( &file_name);
  if ( !a) {
    m_sts = a.sts();
    return;
  }
  // Parse the name of the start page
  if ( (s = strrchr( file_name, '/')) ||
       (s = strrchr( file_name, '<')) ||
       (s = strrchr( file_name, ':')))
    strcpy( name, s+1);
  else
    strcpy( name, file_name);

  if ( (s = strrchr( name, '.')))
    *s = 0;

  sprintf( fname, "$pwrp_web/%s_opwin_menu.html", name);
  dcli_translate_filename( fname, fname);
  fsts = dcli_file_time( fname, &ftime);

  m_sts = PWRB__NOBUILT;
  if ( opt.force || EVEN(fsts) || time_Acomp( &modtime, &ftime) == 1) {
    // modtime > ftime
    m_sts = Graph::generate_web( (ldh_tSession *)&m_session);
    if ( evenSts()) return;

    char msg[200];
    sprintf( msg, "Build:    WebHandler Webpage generated %s", fname);
    MsgWindow::message( 'I', msg, msgw_ePop_No, oid);
  }

  // Check if xtthelp should be converted to html
  dcli_translate_filename( fname, "$pwrp_exe/xtt_help.dat");
  fsts = dcli_file_time( fname, &xtthelp_time);
  if ( EVEN(fsts)) return;
  
  dcli_translate_filename( fname, "$pwrp_web/xtt_help_index.html");
  fsts = dcli_file_time( fname, &html_time);
  if ( opt.force || EVEN(fsts) || time_Acomp( &xtthelp_time, &html_time) == 1) {
    system( "co_convert -d $pwrp_web -t $pwrp_exe/xtt_help.dat");

    char msg[200];
    sprintf( msg, "Build:    WebHandler xtt_help.dat converted to html");
    MsgWindow::message( 'I', msg, msgw_ePop_No, oid);
    m_sts = PWRB__SUCCESS;
  }
}





