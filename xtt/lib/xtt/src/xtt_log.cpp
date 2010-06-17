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
#include <stdio.h>
#include <iomanip>
#include <string.h>

#include "pwr.h"
#include "co_time.h"
#include "cow_log.h"
#include "co_time.h"
#include "cow_wow.h"
#include "xtt_log.h"
#include "xtt_xnav.h"
#include "rt_gdh.h"

using namespace std;

XttLog *XttLog::m_default_log = 0;


XttLog::XttLog( const char *filename) : m_level(1) 
{ 
  char category_str[20];

  dcli_translate_filename( m_filename, filename); 
  strncpy(m_pid,syi_ProcessId(),sizeof(m_pid));

  gdh_RegisterLogFunction( gdh_log_bc);

  category_to_string(xttlog_eCategory_LogStart, category_str);
  log( category_str, 0, 0, 0);
}

void XttLog::delete_default() 
{ 
  if ( m_default_log) {
    delete m_default_log;
    m_default_log = 0;
  }
}

void XttLog::gdh_log_bc( char *name, void *value, unsigned int size)
{
  char str[1000];
  unsigned int strsize = sizeof(str);
  unsigned int len = 0;
  for ( unsigned int i = 0; i < size; i++) {
    if ( i == size - 1) {
      if ( len + 4 >= strsize)
	break;
      len += sprintf( &str[i*5], "0x%02hhx", *(((unsigned char *)value)+i));
    }
    else {
      if ( len + 5 >= strsize)
	break;
      len += sprintf( &str[i*5], "0x%02hhx,", *(((unsigned char *)value)+i));		 
    }
  }
  dlog( xttlog_eCategory_SetObjectInfo, name, str, 0);
}

void XttLog::dlog( xttlog_eCategory category, const char *str, const char *value, unsigned int opt)
{  
  char category_str[40];

  category_to_string(category, category_str);
  if ( m_default_log) 
    m_default_log->log( category_str, str, value, opt);
}


void XttLog::category_to_string( xttlog_eCategory category, char *str)
{
  switch ( category) {
  case xttlog_eCategory_OpenGraph:
    strcpy( str, "OpenGraph");
    break;
  case xttlog_eCategory_CloseGraph:
    strcpy( str, "CloseGraph");
    break;
  case xttlog_eCategory_SetObjectInfo:
    strcpy( str, "SetObjectInfo");
    break;
  case xttlog_eCategory_Command:
    strcpy( str, "Command");
    break;
  case xttlog_eCategory_ApplNew:
    strcpy( str, "ApplNew");
    break;
  case xttlog_eCategory_ApplDelete:
    strcpy( str, "ApplDelete");
    break;
  case xttlog_eCategory_LogStart:
    strcpy( str, "LogStart");
    break;
  default:
    strcpy( str, "");
  }
}

void XttLog::string_to_category( char *str, xttlog_eCategory *category)
{
  if ( strcmp( str, "OpenGraph") == 0)
    *category = xttlog_eCategory_OpenGraph;
  else if ( strcmp( str, "CloseGraph") == 0)
    *category = xttlog_eCategory_CloseGraph;
  else if ( strcmp( str, "SetObjectInfo") == 0)
    *category = xttlog_eCategory_SetObjectInfo;
  else if ( strcmp( str, "Command") == 0)
    *category = xttlog_eCategory_Command;
  else if ( strcmp( str, "ApplNew") == 0)
    *category = xttlog_eCategory_ApplNew;
  else if ( strcmp( str, "ApplDelete") == 0)
    *category = xttlog_eCategory_ApplDelete;
  else if ( strcmp( str, "LogStart") == 0)
    *category = xttlog_eCategory_LogStart;
  else
    *category = xttlog_eCategory_;
}

void XttLog::log( const char *category, const char *str, const char *value, 
		  unsigned int opt)
{
  ofstream fp;
  pwr_tStatus sts;
  char timstr[40];
  char username[80];

  time_AtoAscii( 0, time_eFormat_DateAndTime, timstr, sizeof(timstr));
  sts = syi_UserName( username, sizeof(username));
  if ( EVEN(sts))
    strcpy( username, "Unknown");

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
  if ( str)
    fp << str;
  if ( value)
    fp << " \"" << value << "\"";
  fp << endl;
  fp.close();  
}

int XttLog::play( XNav *xnav, char *filename, double speed, int pid)
{
  ifstream fp;
  pwr_tFileName fname;
  char line[1000];
  int ind;
  int lpid;
  char t1[20], t2[20];
  char type[40];
  char user[40];
  char attr[512];
  char value[1000];
  xttlog_eCategory category;
  int num;
  char timstr[40];
  pwr_tTime log_time;
  pwr_tTime prev_time;
  pwr_tDeltaTime diff_time;
  float diff_time_f;
  int first = 1;

  if ( speed < 0.01)
    speed = 1;

  dcli_translate_filename( fname, filename); 
  
  fp.open( fname);
  if ( !fp)
    return 0;
 
  while( fp.getline( line, sizeof(line))) {
    num = sscanf( line, "%d %s %s %d %s %s", &ind, t1, t2, &lpid, type, user);

    if ( pid != 0 && pid != lpid)
      continue;
    
    strcpy( timstr, t1);
    strcat( timstr, " ");
    strcat( timstr, t2);
    
    time_AsciiToA( timstr, &log_time);
    if ( !first) {
      time_Adiff( &diff_time, &log_time, &prev_time);
      diff_time_f = time_DToFloat( 0, &diff_time);
    }
    else
      diff_time_f = 0;

    if ( diff_time_f / speed > 0.01) {
      xnav->wow->Wait( diff_time_f / speed);
    }

    char *s;
    int cnt = 0;
    for ( s = line; *s; s++) {
      if ( s > line && 
	   ( *(s-1) == ' ' || *(s-1) == '	') &&
	   ( *s != ' ' && *s != '	'))
	cnt++;
      if ( cnt == num)
	break;
    }

    string_to_category( type, &category);
    switch ( category) {
    case xttlog_eCategory_Command:
    case xttlog_eCategory_OpenGraph:
    case xttlog_eCategory_CloseGraph:
    case xttlog_eCategory_ApplNew:
    case xttlog_eCategory_ApplDelete: {

      int sts = xnav->command( s);
      printf( "%8.3f %-14s %9d %s\n", diff_time_f, type, sts, s);
      break;
    }
    case xttlog_eCategory_SetObjectInfo: {
      unsigned int len;
      unsigned char buf[500];
      int size;
      pwr_tStatus sts;

      sscanf( s, "%s %s", attr, value);

      unsigned int i = 0;
      while (1) {
	len = sscanf( &value[i*5+1], "0x%2hhx", &buf[i]);
	if ( len != 1)
	  break;
	i++;
	if ( value[i*5] != ',' || i >= sizeof(buf))
	  break;
      }
      size = i;
      if ( size) {
	sts = gdh_SetObjectInfo( attr, buf, size);
	if ( EVEN(sts)) {
	  printf( "gdh_SetObjectInfo error: %s %s\n", attr, value);
	}
      }
      printf( "%8.3f %-14s %9d %s %s\n", diff_time_f, type, sts, attr, value);
      break;      
    }
    case xttlog_eCategory_LogStart:
      printf( "%8.3f %-14s\n", diff_time_f, type);
      break;
    default: ;
    }

    prev_time = log_time;
    if ( first)
      first = 0;

  }
  printf( "         %-14s\n", "EndOfFile");
  return 1;
} 
