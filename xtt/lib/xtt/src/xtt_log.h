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

#ifndef xtt_log_h
#define xtt_log_h

#include "pwr.h"
#include "cow_log.h"

#define xttlog_cLogFile "$pwrp_log/xtt.log"

typedef enum {
  xttlog_eCategory_,
  xttlog_eCategory_Command,
  xttlog_eCategory_OpenGraph,
  xttlog_eCategory_CloseGraph,
  xttlog_eCategory_ApplNew,
  xttlog_eCategory_ApplDelete,
  xttlog_eCategory_SetObjectInfo,
  xttlog_eCategory_LogStart,
  xttlog_eCategory__,
} xttlog_eCategory;


class XttLog;
class XNav;

class XttLog
{
  pwr_tFileName m_filename;
  int m_level;
  char m_pid[40];
  static XttLog *m_default_log;

 public:
  void push() { m_level++;}
  void pull() { 
    m_level--; 
    if ( m_level < 1) 
      m_level = 1;
  }

  XttLog( const char *filename);
  ~XttLog() { if ( this == m_default_log) m_default_log = 0;}

  void set_default() { m_default_log = this;}
  void log( const char *category, const char *str, const char *value, 
		    unsigned int opt);

  static void dlog( xttlog_eCategory category, const char *str, const char *value, unsigned int opt = 0);
  static void category_to_string( xttlog_eCategory category, char *str);
  static void string_to_category( char *str, xttlog_eCategory *category);
  static void dpush() { 
    if ( m_default_log) m_default_log->push();}
  static void dpull() { 
    if ( m_default_log) m_default_log->pull();}
  static void gdh_log_bc( char *name, void *buf, unsigned int bufsize);
  static int play( XNav *xnav, char *filename, double speed, int pid);
  static void delete_default();
};

#endif

