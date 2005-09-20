/* 
 * Proview   $Id: wb_pvd_pl.cpp,v 1.1 2005-09-20 13:14:28 claes Exp $
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
#include "wb_vext.h"
#include "wb_pvd_pl.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

void wb_pvd_file::writeAttribute( wb_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer)
{
  if ( oix >= m_list.size() || oix <= 0) {
    pcom->provideStatus( LDH__NOSUCHOBJ);
    return;
  }

  switch ( m_list[oix].cid) {
  case pwr_cClass_ProjectReg: {
    pwr_sClass_ProjectReg body;
    
    if ( offset == (unsigned int)((char *)&body.Project - (char *)&body))
      m_list[oix].flags |= pl_mFlags_ProjectModified;
    else if ( offset == (unsigned int)((char *)&body.Path - (char *)&body))
      m_list[oix].flags |= pl_mFlags_PathModified;
    break;
  }
  default: ;
  }

  wb_pvd_file::writeAttribute( pcom, oix, offset, size, buffer);
}

void wb_pvd_pl::save( pwr_tStatus *sts)
{
  process_list( sts);
  save_list( sts);
}

void wb_pvd_pl::process_list( pwr_tStatus *sts)
{
  for ( int i = 0; i < (int) m_list.size(); i++) {
    switch ( m_list[i].cid) {
    case pwr_cClass_ProjectReg: {
      if ( m_list[i].flags & pitem_mFlags_Deleted &&
	   !(m_list[i].flags & pitem_mFlags_Created)) {
	printf( "Project deleted %s\n", longname(i));
      }
      else if ( m_list[i].flags & pitem_mFlags_Created &&
	   !(m_list[i].flags & pitem_mFlags_Deleted)) {
	printf( "Project deleted %s\n", longname(i));
      }
      else if ( !(m_list[i].flags & pitem_mFlags_Deleted)) {
	if ( m_list[i].flags & pl_mFlags_ProjectModified) {
	  printf( "Project modified %s\n", longname(i));
	}
	if ( m_list[i].flags & pl_mFlags_PathModified) {
	  printf( "Path modified %s\n", longname(i));
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
  dcli_translate_filename( fname, "$pwra_db/pwr_projectlist.tst");
  of.open( fname);
  if ( !of) {
    *sts = LDH__FILEOPEN;
    return;
  }

  for ( int oix = m_list[0].fchoix; oix; oix = m_list[oix].fwsoix) {
    if ( !(m_list[oix].flags & pitem_mFlags_Deleted))
      save_item( oix, of);
  }
  of.close();
}

void wb_pvd_pl::save_item( pwr_tOix oix, ofstream& of)
{
  switch ( m_list[oix].cid) {
  case pwr_eClass_Hier: {
    pwr_sHier *body = (pwr_sHier *)m_list[oix].body;
    if ( oix != 1)
      of << "!**Menu " << m_list[oix].name << " { // "  << body->Description << endl;
    for ( int ix = m_list[oix].fchoix; ix; ix = m_list[ix].fwsoix) {
      if ( !(m_list[ix].flags & pitem_mFlags_Deleted))
	save_item( ix, of);
    }
    if ( oix != 1)
      of << "!**}" << endl;
    break;
  }
  case pwr_cClass_ProjectReg: {
    char hname[120];
    pwr_sClass_ProjectReg *body = (pwr_sClass_ProjectReg *)m_list[oix].body;

    strcpy( hname, longname(oix));

    of << body->Project << "	" << body->Version << "	" << body->Path << "	" << hname 
       << "	\"" << body->Description << "\"" << endl;

    for ( int ix = m_list[oix].fchoix; ix; ix = m_list[ix].fwsoix) {
      if ( !(m_list[ix].flags & pitem_mFlags_Deleted))
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

  dcli_translate_filename( fname, "$pwra_db/pwr_projectlist.tst");
  is.open( fname);
  if ( !is) {
    *rsts = LDH__NEWFILE;
    return;
  }

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
    projitem.fthoix = menu_stack[menu_cnt - 1];
    projitem.bwsoix = m_list[projitem.fthoix].lchoix;
    projitem.fwsoix = 0;
    strcpy( projbody->Description, line_item[4]);
    strcpy( projbody->Path, line_item[2]);

    m_list.push_back(projitem);

    if ( projitem.bwsoix != 0)
      m_list[projitem.bwsoix].fwsoix = projitem.oix;
    m_list[projitem.fthoix].lchoix = projitem.oix;
    if ( m_list[projitem.fthoix].fchoix == 0)
      m_list[projitem.fthoix].fchoix = projitem.oix;

  }
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
