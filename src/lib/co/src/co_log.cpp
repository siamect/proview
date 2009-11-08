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


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "pwr.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_log.h"
#include "co_wow.h"

using namespace std;

CoLog *CoLog::m_default_log = 0;

void CoLog::log( const char *category, const char *str, const char *cmt, unsigned int opt)
{
  ofstream fp;
  pwr_tStatus sts;
  char timstr[40];
  char username[80];
  char comment[200] = "";

  time_AtoAscii( 0, time_eFormat_DateAndTime, timstr, sizeof(timstr));
  sts = syi_UserName( username, sizeof(username));
  if ( EVEN(sts))
    strcpy( username, "Unknown");

  if ( cmt)
    strncpy( comment, cmt, sizeof(comment));
  else if ( m_level == 1 && m_wow && opt & log_mOption_Comment) {
    wow_sModalInputDialog *ret;

    ret = m_wow->CreateModalInputDialog( "Log Comment", "Comment", "Ok", "Cancel", 0, 0, sizeof(comment)-1);
    if ( ret->status == wow_eModalDialogReturn_Button1 ||
	 ret->status == wow_eModalDialogReturn_ReturnPressed)
      strncpy( comment, ret->input_str, sizeof(comment));
    free( ret);
  }
  for ( int i = 0; i < 5; i++) {
    fp.open( m_filename, ios::out | ios::app);
    if ( fp)
      break;
    sleep(1);
  }
  if ( !fp)
    return;
  
  if ( m_level == 1)
    fp << "1  ";
  else
    fp << " " << m_level << " ";

  fp << timstr << " " << m_pid;
  for ( int i = strlen(m_pid); i < 7; i++)
    fp << " ";
  fp << " " << category;
  for ( int i = strlen(category); i < 11; i++)
    fp << " ";
  fp << " ";
  fp << " " << username;
  for ( int i = strlen(username); i < 11; i++)
    fp << " ";
  fp << " ";
  fp << str;
  if ( strcmp( comment, "") != 0)
    fp << " \"" << comment << "\"";
  fp << endl;
  fp.close();  
}

void CoLog::get( char categories[][20], char *item, 
		 void item_cb( void *, pwr_tTime, char *, char *, char *, char *), void *ctx)
{
  ifstream fp;
  char line[1024];
  char line_array[8][512];
  int parts;
  int num;
  unsigned int level;
  char timestr[80];
  pwr_tTime time;
  pwr_tStatus sts;
  
  fp.open( m_filename);

  while ( fp.getline( line, sizeof(line))) {
    parts = dcli_parse( line, " ", "",
	     (char *) line_array, sizeof( line_array)/sizeof( line_array[0]), 
	     sizeof( line_array[0]), 0);

    if ( item &&
	 !(item[strlen(item)-1] == '*' && cdh_NoCaseStrncmp( line_array[6], item, strlen(item)-1) == 0) &&
	 cdh_NoCaseStrcmp( line_array[6], item) != 0)
      continue;

    int found = 0;
    for ( int i = 0; strcmp( categories[i], "") != 0; i++) {
      if ( strcmp( categories[i], line_array[4]) == 0) {
	found = 1;
	break;
      }
    }
    if ( !found)
      continue;

    num = sscanf( line_array[0], "%u", &level);
    if ( num != 1)
      continue;

    strcpy( timestr, line_array[1]);
    strcat( timestr, " ");
    strcat( timestr, line_array[2]);
    sts = time_AsciiToA( timestr, &time);
    if ( EVEN(sts))
      continue;

    if ( parts > 7)
      item_cb( ctx, time, line_array[4], line_array[5], line_array[6], line_array[7]);
    else
      item_cb( ctx, time, line_array[4], line_array[5], line_array[6], 0);
  }
}


