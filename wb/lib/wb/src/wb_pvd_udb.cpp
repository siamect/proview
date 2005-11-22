/* 
 * Proview   $Id: wb_pvd_udb.cpp,v 1.2 2005-11-22 12:25:40 claes Exp $
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
#include "wb_pvd_udb.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

void wb_pvd_udb::save( pwr_tStatus *sts)
{
  gu->clear();

  for ( int i = 1; i < (int)m_list.size(); i++) {
    if ( m_list[i].flags & pitem_mFlags_Deleted)
      continue;

    switch ( m_list[i].cid) {
    case pwr_cClass_SystemGroupReg: {
      pwr_sClass_SystemGroupReg *body = (pwr_sClass_SystemGroupReg *)m_list[i].body;

      *sts = gu->add_system( groupname(longname( m_list[i].oix)), body->Attributes);
      if ( EVEN(*sts)) return;
      break;
    }
    case pwr_cClass_UserReg: {
      pwr_sClass_UserReg *body = (pwr_sClass_UserReg *)m_list[i].body;
      char gname[120];
      char *s;

      strcpy( gname, longname( m_list[i].oix)); 
      if ( (s = strrchr( gname, '-')))
	*s = 0;
	   
      *sts = gu->add_user( groupname(gname), m_list[i].name, body->Password, 
			   body->Privileges);
      if ( EVEN(*sts)) return;
      break;
    }
    default: ;
    }
  }
  *sts = gu->save();
}

char *wb_pvd_udb::groupname( char *name)
{
  static char str[200];
  char *s, *t;

  for ( s = name, t = str; *s; s++,t++) {
    if ( *s == '-')
      *t = '.';
    else
      *t = *s;
  }
  *t = 0;
  return str;
}

void wb_pvd_udb::load( pwr_tStatus *rsts)
{
  char		filename[256];
  int		sts;

  if ( gu)
    gu->clear();
  else
    gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  gu->load( filename);
 
  // Create Root object
  pitem rootitem;
  strcpy( rootitem.name, "UserDatabase");
  rootitem.cid = pwr_eClass_PlantHier;
  rootitem.oix = 0; 
  m_list.push_back(rootitem);
  menu_stack[menu_cnt] = rootitem.oix;
  menu_cnt++;

  SystemList *systemgroup = gu->root_system();
  while ( systemgroup) {
    load_systemgroup( systemgroup);

    systemgroup = systemgroup->next_system();
  }
}

void wb_pvd_udb::load_systemgroup( SystemList *systemgroup)
{
  pitem item;
  pwr_sClass_SystemGroupReg *body;
  char sname[120];
  char *s;

  item.oix = next_oix++;
  item.cid = pwr_cClass_SystemGroupReg;
  item.fthoix = menu_stack[menu_cnt - 1];
  item.bwsoix = m_list[item.fthoix].lchoix;
  if ( item.bwsoix)
    m_list[item.bwsoix].fwsoix = item.oix;
  if ( m_list[item.fthoix].fchoix == 0)
    m_list[item.fthoix].fchoix = item.oix;
  m_list[item.fthoix].lchoix = item.oix;

  item.body_size = sizeof(pwr_sClass_SystemGroupReg);
  body = (pwr_sClass_SystemGroupReg *) calloc( 1, item.body_size);
  item.body = body;
  gu->get_system_name( systemgroup, sname);
  if (( s = strrchr( sname, '.')))
    strcpy( item.name, s+1);
  else
    strcpy( item.name, sname);

  gu->get_system_data( sname, &body->Attributes);
  menu_stack[menu_cnt] = item.oix;
  menu_cnt++;
  m_list.push_back( item);

  UserList *user = systemgroup->first_user();
  while ( user) {
    load_user( user, systemgroup);

    user = user->next_user();
  }
  SystemList *sg = systemgroup->first_system();
  while ( sg) {
    load_systemgroup( sg);

    sg = sg->next_system();
  }
  menu_cnt--;
}

void wb_pvd_udb::load_user( UserList *user, SystemList *sg)
{
  pitem item;
  pwr_sClass_UserReg *body;

  item.oix = next_oix++;
  item.cid = pwr_cClass_UserReg;
  item.fthoix = menu_stack[menu_cnt - 1];
  item.bwsoix = m_list[item.fthoix].lchoix;
  if ( item.bwsoix)
    m_list[item.bwsoix].fwsoix = item.oix;
  if ( m_list[item.fthoix].fchoix == 0)
    m_list[item.fthoix].fchoix = item.oix;
  m_list[item.fthoix].lchoix = item.oix;

  item.body_size = sizeof(pwr_sClass_UserReg);
  body = (pwr_sClass_UserReg *) calloc( 1, item.body_size);
  item.body = body;
  strcpy( item.name, user->get_name());
  user->get_data( body->Password, &body->Privileges);

  m_list.push_back( item);
}








