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

#ifndef co_log_h
#define co_log_h

#include <string.h>
#include "pwr.h"
#include "co_dcli.h"
#include "co_syi.h"

typedef enum {
  log_mOption_Comment = 1
} log_mOption;

class CoLog;
class CoWow;

class CoLog
{
 protected:
  pwr_tFileName m_filename;
  char m_pid[40];
  CoWow *m_wow;
  static CoLog *m_default_log;
  int m_level;


 public:
  CoLog( const char *filename) : m_wow(0), m_level(1)
    { dcli_translate_filename( m_filename, filename); 
      strncpy(m_pid,syi_ProcessId(),sizeof(m_pid));}
  ~CoLog() { if ( this == m_default_log) m_default_log = 0;}

  void set_default() { m_default_log = this;}
  void log( const char *category, const char *str, const char *cmt, unsigned int opt = 0);
  void get( char categories[][20], char *item, 
	    void item_cb( void *, pwr_tTime, char *, char *, char *, char *), void *ctx);
  void push() { m_level++;}
  void pull() { 
    m_level--; 
    if ( m_level < 1) 
      m_level = 1;
  }

  static void dlog( const char *category, const char *str, const char *cmt, unsigned int opt) { 
    if ( m_default_log) m_default_log->log( category, str, cmt, opt);}
  static void dpush() { 
    if ( m_default_log) m_default_log->push();}
  static void dpull() { 
    if ( m_default_log) m_default_log->pull();}
  static void dget( char categories[][20], char *item, void item_cb( void *, pwr_tTime, char *, char *, char *, char *), void *ctx) { 
    if ( m_default_log) m_default_log->get( categories, item, item_cb, ctx);}
};

#endif

