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

#include "pwr.h"
#include "co_time.h"
#include "co_log.h"
#include "co_time.h"
#include "wb_session.h"
#include "wb_object.h"
#include "wb_log.h"


void wb_log::log( wlog_eCategory category, const char *str, const char *comment, unsigned int opt)
{  
  char category_str[40];

  category_to_string(category, category_str);

  CoLog::dlog( category_str, str, comment, opt);
}

void wb_log::log( wb_session *session, wlog_eCategory category, pwr_tVid vid, unsigned int opt)
{
  pwr_tOid oid;
  oid.oix = 0;
  oid.vid = vid;
  wb_object o = session->object( oid);

  log( category, o.name(), 0, opt);
}

void wb_log::log( wb_session *session, wlog_eCategory category, pwr_tOid oid, unsigned int opt)
{
  wb_object o = session->object( oid);

  log( category, o.longName().c_str(), 0, opt);
}

void wb_log::log( wb_session *session, wlog_eCategory category, pwr_tAttrRef aref, unsigned int opt)
{
#if 0
  wb_object o = session->object( oid);

  log( category, o.longName().c_str(), 0, opt);
#endif
}

void wb_log::category_to_string( wlog_eCategory category, char *str)
{
  switch ( category) {
  case wlog_eCategory_WbLogin:
    strcpy( str, "WbLogin");
    break;
  case wlog_eCategory_ConfiguratorSave:
    strcpy( str, "ConfigSave");
    break;
  case wlog_eCategory_PlcSave:
    strcpy( str, "PlcSave");
    break;
  case wlog_eCategory_GeSave:
    strcpy( str, "GeSave");
    break;
  case wlog_eCategory_PlcBuild:
    strcpy( str, "PlcBuild");
    break;
  case wlog_eCategory_GeBuild:
    strcpy( str, "GeBuild");
    break;
  case wlog_eCategory_VolumeBuild:
    strcpy( str, "VolumeBuild");
    break;
  case wlog_eCategory_NodeBuild:
    strcpy( str, "NodeBuild");
    break;
  case wlog_eCategory_CreatePackage:
    strcpy( str, "CreatePackage");
    break;
  case wlog_eCategory_CopyPackage:
    strcpy( str, "CopyPackage");
    break;
  case wlog_eCategory_GeExport:
    strcpy( str, "GeExport");
    break;
  default:
    strcpy( str, "");
  }
}

void wb_log::string_to_category( char *str, wlog_eCategory *category)
{
  if ( strcmp( str, "WbLogin") == 0)
    *category = wlog_eCategory_WbLogin;
  else if ( strcmp( str, "ConfigSave") == 0)
    *category = wlog_eCategory_ConfiguratorSave;
  else if ( strcmp( str, "PlcSave") == 0)
    *category = wlog_eCategory_PlcSave;
  else if ( strcmp( str, "GeSave") == 0)
    *category = wlog_eCategory_GeSave;
  else if ( strcmp( str, "PlcBuild") == 0)
    *category = wlog_eCategory_PlcBuild;
  else if ( strcmp( str, "GeBuild") == 0)
    *category = wlog_eCategory_GeBuild;
  else if ( strcmp( str, "VolumeBuild") == 0)
    *category = wlog_eCategory_VolumeBuild;
  else if ( strcmp( str, "NodeBuild") == 0)
    *category = wlog_eCategory_NodeBuild;
  else if ( strcmp( str, "CreatePackage") == 0)
    *category = wlog_eCategory_CreatePackage;
  else if ( strcmp( str, "CopyPackage") == 0)
    *category = wlog_eCategory_CopyPackage;
  else if ( strcmp( str, "GeExport") == 0)
    *category = wlog_eCategory_GeExport;
  else
    *category = wlog_eCategory_;
}

