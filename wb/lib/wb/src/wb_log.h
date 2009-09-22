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

#ifndef wb_log_h
#define wb_log_h

#include "pwr.h"
#include "co_log.h"

#define wlog_cLogFile "$pwrp_db/wb_history.log"

typedef enum {
  wlog_eCategory_,
  wlog_eCategory_WbLogin,
  wlog_eCategory_ConfiguratorSave,
  wlog_eCategory_PlcSave,
  wlog_eCategory_GeSave,
  wlog_eCategory_PlcBuild,
  wlog_eCategory_GeBuild,
  wlog_eCategory_VolumeBuild,
  wlog_eCategory_NodeBuild,
  wlog_eCategory_CreatePackage,
  wlog_eCategory_CopyPackage,
  wlog_eCategory_GeExport,
  wlog_eCategory__,
} wlog_eCategory;


class wb_log;
class wb_session;

class wb_log
{
 public:
  wb_log() {}
  static void log( wb_session *session, wlog_eCategory category, pwr_tVid vid, unsigned int opt = 0);
  static void log( wb_session *session, wlog_eCategory category, pwr_tOid oid, unsigned int opt = 0);
  static void log( wb_session *session, wlog_eCategory category, pwr_tAttrRef aref, unsigned int opt = 0);
  static void log( wlog_eCategory category, const char *str, const char *comment, unsigned int opt = 0);
  static void category_to_string( wlog_eCategory category, char *str);
  static void string_to_category( char *str, wlog_eCategory *category);
  static void push() { CoLog::dpush();}
  static void pull() { CoLog::dpull();}
};

#endif

