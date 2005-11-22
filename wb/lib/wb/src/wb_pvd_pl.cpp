/* 
 * Proview   $Id: wb_pvd_pl.cpp,v 1.5 2005-11-22 12:25:12 claes Exp $
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

#include <vector.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_wow.h"
#include "co_msgwindow.h"
#include "wb_vext.h"
#include "wb_pvd_pl.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_cnf.h"
}

static char *pwrp_status_to_string( int value)
{
  static char str[80];

  switch ( value)
  {
    case 2: strcpy( str, "Database doesn't exist"); break;
    case 3: strcpy( str, "No such project"); break;
    case 4: strcpy( str, "Project root already exist"); break;
    case 5: strcpy( str, "Project is not yet configured"); break;
    case 6: strcpy( str, "Projectlist file not found"); break;
    case 7: strcpy( str, "Project already exist"); break;
    case 8: strcpy( str, "Base already exist"); break;
    case 9: strcpy( str, "No such base"); break;
    case 10: strcpy( str, "No write access to projectlist file"); break;
    case 11: strcpy( str, "Syntax error"); break;
    case 12: strcpy( str, "No write access to project root"); break;
    case 13: strcpy( str, "Unable to create project root"); break;
    case 14: strcpy( str, "Unable to delete project root"); break;
    case 15: strcpy( str, "Unable to delete database"); break;
    case 16: strcpy( str, "Base doesn't exist"); break;
    case 17: strcpy( str, "Error in move"); break;
    case 18: strcpy( str, "Current node is not mysql server"); break;
    case 19: strcpy( str, "Unable to find mysql datadir"); break;
    case 20: strcpy( str, "Unable to find database"); break;
    case 21: strcpy( str, "Unable to find template database"); break;
    case 22: strcpy( str, "Database already exist"); break;
    case 23: strcpy( str, "Error in copy"); break;
    case 24: strcpy( str, "Error when creating database"); break;
    case 25: strcpy( str, "No project is set"); break;
    case 26: strcpy( str, "Invalid project name"); break;
    case 27: strcpy( str, "Unable to copy mysql database"); break;
    case 28: strcpy( str, "Unable to rename mysql database"); break;
    case 29: strcpy( str, "Unable to find mysql"); break;
    case 30: strcpy( str, "No such root"); break;
    default:
      sprintf( str, "Errorcode from pwrp: %d", value);
  }
  return str;
}

void wb_pvd_pl::writeAttribute( wb_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer)
{
  if ( oix >= m_list.size() || oix <= 0) {
    pcom->provideStatus( LDH__NOSUCHOBJ);
    return;
  }

  switch ( m_list[oix].cid) {
  case pwr_cClass_ProjectReg: {
    pwr_sClass_ProjectReg body;
    
    if ( offset == (unsigned int)((char *)&body.Project - (char *)&body)) {
      m_list[oix].flags |= pl_mFlags_ProjectModified;
      cdh_ToLower( buffer, buffer);      
    }
    else if ( offset == (unsigned int)((char *)&body.Path - (char *)&body)) {
      m_list[oix].flags |= pl_mFlags_PathModified;
    }
    break;
  }
  default: ;
  }

  wb_pvd_file::writeAttribute( pcom, oix, offset, size, buffer);
}

void wb_pvd_pl::createObject( wb_procom *pcom, pwr_tOix destoix, int desttype,
		     pwr_tCid cid, char *name)
{
  pwr_tOix oix = next_oix;

  wb_pvd_file::createObject( pcom, destoix, desttype, cid, name);

  switch ( cid) {
  case pwr_cClass_ProjectReg: {
    char defaultpath[80];
    pwr_tObjName lowname;
    pwr_tOix boix;
    pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[oix].body;

    // Set default values to project, version and path
    cnf_get_value( "defaultProjectRoot", defaultpath);
    cdh_ToLower( lowname, m_list[oix].name);
    strcat( defaultpath, "/");
    strcat( defaultpath, lowname);

    find( root, "Bases", &boix);
    if ( m_list[boix].lchoix != 0) {
      pwr_sClass_BaseReg *basebody = 
	(pwr_sClass_BaseReg *)m_list[m_list[boix].lchoix].body;
      strcpy( body->Version, basebody->Version);
    }
    strcpy( body->Path, defaultpath); 
    strcpy( body->Project, lowname); 

    // Store original body in userdata to detect any modifications
    m_list[oix].userdata_size = m_list[oix].body_size;
    m_list[oix].userdata = calloc( 1, m_list[oix].userdata_size);

    break;
  }
  case pwr_cClass_BaseReg:
    m_list[oix].userdata_size = m_list[oix].body_size;
    m_list[oix].userdata = calloc( 1, m_list[oix].userdata_size);
    break;
  default: ;
  }
}

void wb_pvd_pl::save( pwr_tStatus *sts)
{
  if ( !check_list(sts))
    return;
  process_list( sts);
  save_list( sts);
}

void wb_pvd_pl::confirm_actions_ok( void *ctx, void *data)
{
  wb_pvd_pl *pl = (wb_pvd_pl *)ctx;
  pwr_tStatus sts;

  pl->process_list( &sts);
  pl->save_list( &sts);
}

bool wb_pvd_pl::check_list( pwr_tStatus *sts)
{
  int error_cnt = 0;
  char msg[200];
  char text[4000] = "Do you wan't to execute these actions \n\n";
  int actions_found = 0;
  pwr_tCmd cmd;
  int csts;

  for ( int i = 0; i < (int) m_list.size(); i++) {
    if ( m_list[i].flags & pl_mFlags_Disabled)
      continue;

    switch ( m_list[i].cid) {
    case pwr_cClass_BaseReg: {
      pwr_sClass_BaseReg *body = (pwr_sClass_BaseReg *)m_list[i].body;
      pwr_sClass_BaseReg *origbody = (pwr_sClass_BaseReg *)m_list[i].userdata;
      pwr_tOid oid;

      oid.oix = m_list[i].oix;
      oid.vid = ldh_cProjectListVolume;

      if ( m_list[i].flags & pitem_mFlags_Deleted &&
	   !(m_list[i].flags & pitem_mFlags_Created)) {
      }
      else if ( !(m_list[i].flags & pitem_mFlags_Deleted)) {

	if ( strcmp( body->Path, "") == 0) {
	  sprintf( msg, "Path is missing, in object %s", longname(m_list[i].oix));
	  MsgWindow::message('E', msg, msgw_ePop_No, oid);
	  error_cnt++;
	}
	if ( strcmp( body->Version, "") == 0) {
	  sprintf( msg, "Version is missing, in object %s", longname(m_list[i].oix));
	  MsgWindow::message('E', msg, msgw_ePop_No, oid);
	  error_cnt++;
	}
	if ( strcmp( body->Path, origbody->Path) != 0) {
	  // Check new path
	}
      }
      break;
    }
    case pwr_cClass_ProjectReg: {
      pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[i].body;
      pwr_sClass_ProjectReg *origbody = (pwr_sClass_ProjectReg *)m_list[i].userdata;
      pwr_tOid oid;

      oid.oix = m_list[i].oix;
      oid.vid = ldh_cProjectListVolume;

      if ( m_list[i].flags & pitem_mFlags_Deleted &&
	   !(m_list[i].flags & pitem_mFlags_Created)) {
	// Project deleted
	sprintf( msg, "delete project %s with file tree and databases\n", body->Project);
	if ( strlen(text) + strlen(msg) < sizeof(text))
	  strcat( text, msg);
	actions_found++;
      }
      else if ( !(m_list[i].flags & pitem_mFlags_Deleted)) {

	if ( strcmp( body->Project, "") == 0) {
	  sprintf( msg, "Project is missing, in object %s", longname(m_list[i].oix));
	  MsgWindow::message('E', msg, msgw_ePop_No, oid);
	  error_cnt++;
	}
	if ( strcmp( body->Version, "") == 0) {
	  sprintf( msg, "Version is missing, in object %s", longname(m_list[i].oix));
	  MsgWindow::message('E', msg, msgw_ePop_No, oid);
	  error_cnt++;
	}
	else {
	  pwr_tOix boix;

	  find( root, "Bases", &boix);
	  if ( !find( boix, basename( body->Version), &boix)) {
	    sprintf( msg, "Version not found, in object %s", longname(m_list[i].oix));
	    MsgWindow::message('E', msg, msgw_ePop_No, oid);
	    error_cnt++;
	  }
	  if ( m_list[i].flags & pitem_mFlags_Created) {
	    if ( strcmp( body->CopyFrom, "") == 0) {
	      // Create project 
	      // Check destination path
	      sprintf( cmd, "wb_pvd_pl.sh check create %s", body->Path);
	      csts = system( cmd);
	      if ( (csts >>= 8) != 0) {
		sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
			 longname(m_list[i].oix));
		MsgWindow::message('E', msg, msgw_ePop_No);
		error_cnt++;
	      }

	      sprintf( msg, "create project %s, version %s, path %s\n", body->Project,
		       body->Version, body->Path);
	      if ( strlen(text) + strlen(msg) < sizeof(text))
		strcat( text, msg);
	      actions_found++;
	    }
	    else {
	      // Copy project 
	      // Check destination path
	      sprintf( cmd, "wb_pvd_pl.sh check create %s", body->Path);
	      csts = system( cmd);
	      if ( (csts >>= 8) != 0) {
		sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
			 longname(m_list[i].oix));
		MsgWindow::message('E', msg, msgw_ePop_No);
		error_cnt++;
	      }

	      sprintf( msg, "copy project %s to %s, path %s\n", body->CopyFrom,
		       body->Project, body->Path);
	      if ( strlen(text) + strlen(msg) < sizeof(text))
		strcat( text, msg);
	      actions_found++;
	    }
	  }
	  else if ( m_list[i].flags & pl_mFlags_PathModified &&
	       strcmp( body->Path, origbody->Path) != 0) {
	    // Move project 
	    // Check source and destination path
	    sprintf( cmd, "wb_pvd_pl.sh check move %s %s",
		     origbody->Path, body->Path);
	    csts = system( cmd);
	    if ( (csts >>= 8) != 0) {
	      sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
		       longname(m_list[i].oix));
	      MsgWindow::message('E', msg, msgw_ePop_No);
	      error_cnt++;
	    }

	    sprintf( msg, "move project %s from %s to %s\n", body->Project,
		     origbody->Path, body->Path);
	    if ( strlen(text) + strlen(msg) < sizeof(text))
	      strcat( text, msg);
	    actions_found++;
	  }
	}
      }
      break;
    }
    default: ;
    }
  }
  if ( error_cnt) {
    sprintf( msg, "%d error(s) found, Save aborted", error_cnt);
    MsgWindow::message('E', msg, msgw_ePop_Yes);
    *sts = LDH__SYNTAX;
    return false;
  }
  if ( actions_found) {
    Widget toplevel;

    MsgWindow::get_parent_widget( &toplevel);
    if ( toplevel) 
      wow_DisplayQuestion( (void *)this, toplevel, "Confirm Actions", text,
			   confirm_actions_ok, 0, 0);
    *sts = LDH__CONFIRM;
    return false;
  }

  *sts = LDH__SUCCESS;
  return true;
}

void wb_pvd_pl::process_list( pwr_tStatus *sts)
{
  char msg[200];
  pwr_tCmd cmd;
  int csts;

  for ( int i = 0; i < (int) m_list.size(); i++) {
    if ( m_list[i].flags & pl_mFlags_Disabled)
      continue;

    switch ( m_list[i].cid) {
    case pwr_cClass_ProjectReg: {
      pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[i].body;

      if ( m_list[i].flags & pitem_mFlags_Deleted &&
	   !(m_list[i].flags & pitem_mFlags_Created)) {
	printf( "Project deleted %s\n", longname(i));

	sprintf( cmd, "pwrp_env.sh delete project %s noconfirm",
		 body->Project);
	csts = system( cmd);      
	if ( (csts >>= 8) != 0) {
	  sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
		   longname(m_list[i].oix));
	  MsgWindow::message('E', msg, msgw_ePop_Yes);
	  return;
	}
      }
      else if ( m_list[i].flags & pitem_mFlags_Created &&
	   !(m_list[i].flags & pitem_mFlags_Deleted)) {
	if ( strcmp( body->CopyFrom, "") == 0) {
	  printf( "Project created %s\n", longname(i));

	  sprintf( cmd, "wb_pvd_pl.sh create project %s %s %s \"%s\" \"%s\"",
		   body->Project, body->Version, body->Path, longname(m_list[i].oix), 
		   body->Description);
	  csts = system( cmd);      
	  if ( (csts >>= 8) != 0) {
	    sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
		     longname(m_list[i].oix));
	    MsgWindow::message('E', msg, msgw_ePop_Yes);
	    return;
	  }
	}
	else {
	  sprintf( cmd, "pwrp_env.sh copy project %s %s %s %s noconfirm",
		     body->CopyFrom, body->Project, body->Path, longname(m_list[i].oix));
	  printf( cmd);
	  csts = system( cmd);      
	  if ( (csts >>= 8) != 0) {
	    sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
		     longname(m_list[i].oix));
	    MsgWindow::message('E', msg, msgw_ePop_Yes);
	    return;
	  }
	}
      }
      else if ( !(m_list[i].flags & pitem_mFlags_Deleted)) {
	pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[i].body;
	pwr_sClass_ProjectReg *origbody = (pwr_sClass_ProjectReg *)m_list[i].userdata;

	if ( m_list[i].flags & pl_mFlags_ProjectModified) {
	  printf( "Project modified %s\n", longname(i));
	}
	if ( m_list[i].flags & pl_mFlags_PathModified &&
	     strcmp( body->Path, origbody->Path) != 0) {
	  printf( "Path modified %s\n", longname(i));

	  sprintf( cmd, "pwrp_env.sh modify project %s -r %s", origbody->Project, 
		   body->Path);
	  csts = system( cmd);      
	  if ( (csts >>= 8) != 0) {
	    sprintf( msg, "%s, in object %s", pwrp_status_to_string(csts), 
		     longname(m_list[i].oix));
	    MsgWindow::message('E', msg, msgw_ePop_Yes);
	    return;
	  }
	}
      }
      break;
    }
    default: ;
    }
  }
}

void wb_pvd_pl::save_list( pwr_tStatus *sts)
{
  ofstream of;
  pwr_tFileName fname;
 

  *sts = LDH__SUCCESS;
  dcli_translate_filename( fname, "$pwra_db/pwr_projectlist.dat");
  of.open( fname);
  if ( !of) {
    *sts = LDH__FILEOPEN;
    return;
  }

  for ( int oix = m_list[0].fchoix; oix; oix = m_list[oix].fwsoix) {
    save_item( oix, of);
  }
  of.close();

  // Disable deletet objects
  for ( int i = 0; i < (int) m_list.size(); i++) {
    if ( m_list[i].flags & pitem_mFlags_Deleted)
      m_list[i].flags |= pl_mFlags_Disabled;
  }
}

void wb_pvd_pl::save_item( pwr_tOix oix, ofstream& of)
{
  if ( m_list[oix].flags & pitem_mFlags_Deleted)
    return;

  m_list[oix].flags = 0;
  if ( m_list[oix].userdata)
    memcpy( m_list[oix].userdata, m_list[oix].body, m_list[oix].userdata_size);

  switch ( m_list[oix].cid) {
  case pwr_eClass_Hier: {
    pwr_sHier *body = (pwr_sHier *)m_list[oix].body;
    if ( oix != 1)
      of << "!**Menu " << m_list[oix].name << " { // "  << body->Description << endl;
    for ( int ix = m_list[oix].fchoix; ix; ix = m_list[ix].fwsoix) {
      save_item( ix, of);
    }
    if ( oix != 1)
      of << "!**}" << endl;
    break;
  }
  case pwr_cClass_ProjectReg: {
    char hname[120];
    pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[oix].body;
    strcpy( body->CopyFrom, "");

    strcpy( hname, longname(oix));

    of << body->Project << "	" << body->Version << "	" << body->Path << "	" << hname 
       << "	\"" << body->Description << "\"" << endl;

    for ( int ix = m_list[oix].fchoix; ix; ix = m_list[ix].fwsoix) {
      save_item( ix, of);
    }
    break;
  }
  case pwr_cClass_BaseReg: {
    pwr_sClass_BaseReg *body = (pwr_sClass_BaseReg *)m_list[oix].body;

    of << "%base " << body->Version << "	" << body->Path << endl;
    break;
  }
  default: ;
  }
}

void wb_pvd_pl::load( pwr_tStatus *rsts)
{
  char line[200];
  char line_item[6][80];
  int num;
  ifstream is;
  pwr_tFileName fname;
  int line_cnt = 0;
  int menu_stack[100];
  int menu_cnt = 0;
  char description[80];
  char *s;

  *rsts = LDH__SUCCESS;


  // Create Root object
  pitem rootitem;
  strcpy( rootitem.name, "ProjectList");
  rootitem.cid = pwr_eClass_Hier;
  rootitem.oix = 0; 
  m_list.push_back(rootitem);
  menu_stack[menu_cnt] = rootitem.oix;
  menu_cnt++;

  // Create Root for BaseReg objects
  pitem brootitem;
  strcpy( brootitem.name, "Bases");
  brootitem.cid = pwr_eClass_Hier;
  brootitem.oix = next_oix++; 
  brootitem.fthoix = m_list[rootitem.oix].oix; 
  m_list[rootitem.oix].fchoix = brootitem.oix;
  m_list[rootitem.oix].lchoix = brootitem.oix;
  m_list.push_back(brootitem);

  dcli_translate_filename( fname, "$pwra_db/pwr_projectlist.dat");
  is.open( fname);
  if ( !is) {
    *rsts = LDH__NEWFILE;
    return;
  }

  while ( is.getline( line, sizeof(line))) {
    line_cnt++;
    if ( line[0] == '!') {
      if ( strncmp( line, "!**Menu", 7) == 0) {
	// Add Hier
	char *s = strstr( line, "// ");
	if ( s) {
	  strncpy( description, s+3, sizeof(description));
	  description[sizeof(description)-1] = 0;
	}
	else
	  strcpy( description, "");

	num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
	if ( num < 3) {
	  cout << "Syntax error " << fname << " row " << line_cnt << endl;
	  continue;
	}

	pitem hieritem;
	strcpy( hieritem.name, line_item[1]);
	hieritem.cid = pwr_eClass_Hier;
	hieritem.oix = next_oix++;
	hieritem.fthoix = menu_stack[menu_cnt - 1];
	hieritem.bwsoix = m_list[hieritem.fthoix].lchoix;
	hieritem.fwsoix = 0;
	hieritem.body_size = sizeof(pwr_sHier);
	pwr_sHier *hierbody = (pwr_sHier *) calloc( 1, hieritem.body_size); 
	hieritem.body = hierbody;
	strcpy( hierbody->Description, description);

	m_list.push_back(hieritem);

	if ( hieritem.bwsoix != 0)
	  m_list[hieritem.bwsoix].fwsoix = hieritem.oix;
	m_list[hieritem.fthoix].lchoix = hieritem.oix;
	if ( m_list[hieritem.fthoix].fchoix == 0)
	  m_list[hieritem.fthoix].fchoix = hieritem.oix;

	menu_stack[menu_cnt] = hieritem.oix;
	menu_cnt++;
      }
      else if ( strncmp( line, "!**}", 4) == 0) {
	if ( menu_cnt == 0) {
	  cout << "Syntax error " << fname << " row " << line_cnt << endl;
	  continue;
	}
	menu_cnt--;
      }
      continue;
    }
    dcli_trim( line, line);

    num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
    if ( strncmp( line, "%base", 5) == 0) {
      // Insert BaseReg under baseroot
      if ( num != 3) {
	cout << "Syntax error " << fname << " row " << line_cnt << endl;
	continue;
      }

      pitem baseitem;
      strcpy( baseitem.name, basename(line_item[1]));
      baseitem.cid = pwr_cClass_BaseReg;
      baseitem.oix = next_oix++;
      baseitem.fthoix = brootitem.oix;
      baseitem.bwsoix = m_list[baseitem.fthoix].lchoix;
      baseitem.fwsoix = 0;
      baseitem.body_size = sizeof(pwr_sClass_BaseReg);
      pwr_sClass_BaseReg *basebody = 
	(pwr_sClass_BaseReg *) calloc( 1, baseitem.body_size); 
      baseitem.body = basebody;
      strcpy( basebody->Description, "");
      strcpy( basebody->Version, line_item[1]);
      strcpy( basebody->Path, line_item[2]);
      // Store original body in userdata to identify any changes when saving
      baseitem.userdata_size = baseitem.body_size;
      baseitem.userdata = calloc( 1, baseitem.userdata_size);
      memcpy( baseitem.userdata, baseitem.body, baseitem.userdata_size);
      
      m_list.push_back(baseitem);
      
      if ( baseitem.bwsoix != 0)
	m_list[baseitem.bwsoix].fwsoix = baseitem.oix;
      m_list[baseitem.fthoix].lchoix = baseitem.oix;
      if ( m_list[baseitem.fthoix].fchoix == 0)
	m_list[baseitem.fthoix].fchoix = baseitem.oix;
      continue;
    }

    if ( num != 5) {
      cout << "Syntax error " << fname << " row " << line_cnt << endl;
      continue;
    }

    // For backward compability, create the path (pre V4.1)
    bool hier_created;
    pwr_tOix hieroix;
    if ( (hier_created = create_hier( line_item[3], &hieroix))) {
      menu_stack[menu_cnt++] = hieroix;
    }

    pitem projitem;
    if ( (s = strrchr( line_item[3], '-')))
      strcpy( projitem.name, s + 1);
    else
      strcpy( projitem.name, line_item[3]);

    
    projitem.body_size = sizeof(pwr_sClass_ProjectReg);
    pwr_sClass_ProjectReg *projbody = 
      (pwr_sClass_ProjectReg *) calloc( 1, projitem.body_size);

    projitem.body = projbody;
    strncpy( projbody->Version, line_item[1], sizeof( projbody->Version));
    strncpy( projbody->Project, line_item[0], sizeof( projbody->Project));

    projitem.cid = pwr_cClass_ProjectReg;
    projitem.oix = next_oix++;
    projitem.fthoix = hieroix; // menu_stack[menu_cnt - 1];
    projitem.bwsoix = m_list[projitem.fthoix].lchoix;
    projitem.fwsoix = 0;
    strcpy( projbody->Description, line_item[4]);
    strcpy( projbody->Path, line_item[2]);
    // Store original body in userdata to identify any changes when saving
    projitem.userdata_size = projitem.body_size;
    projitem.userdata = calloc( 1, projitem.userdata_size);
    memcpy( projitem.userdata, projitem.body, projitem.userdata_size);


    m_list.push_back(projitem);

    if ( projitem.bwsoix != 0)
      m_list[projitem.bwsoix].fwsoix = projitem.oix;
    m_list[projitem.fthoix].lchoix = projitem.oix;
    if ( m_list[projitem.fthoix].fchoix == 0)
      m_list[projitem.fthoix].fchoix = projitem.oix;


    if ( hier_created)
      menu_cnt--;
  }
}

// Create a path from hierarchy string
// Used to be compatible with projectlists of V4.0 and previous
bool wb_pvd_pl::create_hier( char *hier, pwr_tOix *oix)
{
  pwr_tObjName segment_name[20];
  int num;
  pwr_tOix fthoix = root;
  pwr_tOix ix;
  bool created = false;

  num = dcli_parse( hier, "-", "", (char *)segment_name,
		     sizeof(segment_name)/sizeof(segment_name[0]),
		     sizeof(segment_name[0]), 0);

  for ( int i = 0; i < num - 1; i++) {
    // Check if name exist
    if ( !find( fthoix, segment_name[i], &ix)) {
      // Create this hierarchy object
      pitem hieritem;
      strcpy( hieritem.name, segment_name[i]);
      hieritem.cid = pwr_eClass_Hier;
      hieritem.oix = next_oix++;
      hieritem.fthoix = fthoix;
      hieritem.bwsoix = m_list[hieritem.fthoix].lchoix;
      hieritem.fwsoix = 0;
      hieritem.body_size = sizeof(pwr_sHier);
      pwr_sHier *hierbody = (pwr_sHier *) calloc( 1, hieritem.body_size); 
      hieritem.body = hierbody;
      strcpy( hierbody->Description, "");

      m_list.push_back(hieritem);

      if ( hieritem.bwsoix != 0)
	m_list[hieritem.bwsoix].fwsoix = hieritem.oix;
      m_list[hieritem.fthoix].lchoix = hieritem.oix;
      if ( m_list[hieritem.fthoix].fchoix == 0)
	m_list[hieritem.fthoix].fchoix = hieritem.oix;

      fthoix = hieritem.oix;
      created = true;
    }
    else
      fthoix = ix;
  }
  *oix = fthoix;
  return created;
}

// Convert version to a valid object name
char *wb_pvd_pl::basename( char *version)
{
  char *s, *t;
  static char str[80];
  
  strcpy( str, "Base");
  for ( s = version, t = &str[strlen(str)]; *s; s++) {
    if ( *s == '.')
      continue;
    *t++ = *s;
  }
  *t = 0;
  return str;
}
