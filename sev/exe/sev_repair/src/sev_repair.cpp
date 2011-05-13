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

#if defined PWRE_CONF_MYSQL


#include "pwr.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "sev_repair.h"
#include "sev_dbms.h"
#include "rt_sev_msg.h"


int sev_repair::init()
{
  sev_dbms_env 		*env;
  pwr_tFileName 	envname;

  sprintf( envname, "$pwrp_db/%s.db", sev_dbms_env::dbName());
  dcli_translate_filename( envname, envname);
  
  env = new sev_dbms_env( envname);
  env->open( envname);
  if ( !env->exists()) {
    printf( "** Error: Database environment doesn't exist\n");
    exit(0);
  }

  if ( !env->openDb()) {
    printf("Failed to connect to database '%s'", sev_dbms_env::dbName());
    exit(0);
  }

  m_db = new sev_dbms( env);

  printf("-- Database opened '%s'\n", sev_dbms_env::dbName());

  m_db->get_items( &m_sts);
  m_db->get_objectitems(&m_sts);
  return 1;
}

int sev_repair::check()
{
  pwr_tStatus sts;
  int fail_cnt = 0;


  printf( "-- Number of tables to check: %u\n", m_db->m_items.size());
  for ( unsigned int i = 0; i < m_db->m_items.size(); i++) {
    m_db->repair_table( &sts, m_db->m_items[i].tablename);
    if ( EVEN(sts))
      fail_cnt++;
  }
  if ( fail_cnt)
    printf( "** Repair failed on %d tables\n", fail_cnt);
  else
    printf( "-- Tables successfully repaired\n");
  return 1;
}

int main (int argc, char *argv[])
{
  sev_repair rep;

  rep.init();
  rep.check();
}
#else
int main(){}
#endif
