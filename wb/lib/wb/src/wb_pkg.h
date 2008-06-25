/* 
 * Proview   $Id: wb_pkg.h,v 1.8 2008-06-25 07:56:10 claes Exp $
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

#ifndef wb_pkg_h
#define wb_pkg_h

#include <iostream.h>
#include <vector.h>
#include <string>
#include "pwr.h"
#include "pwr_class.h"
#include "co_wow.h"
#include "wb_lfu.h"

class pkg_node;

class pkg_file {
  friend class pkg_node;

 private:
  char m_source[200];
  char m_target[200];
  char m_arname[80];
  pwr_tTime m_date;

 public:
  pkg_file( char *source, char *target);
  char *source() { return m_source;}
  char *target() { return m_target;}
  pwr_tTime date() { return m_date;}

};

class pkg_pattern {
  friend class pkg_node;

 private:
  char m_source[200];
  char m_target[200];  
  vector<pkg_file> m_filelist;
  pkg_node *m_node;
  char m_severity;

 public:
  pkg_pattern( char *source, char *target, char severity = 'W') 
    : m_node(0), m_severity(severity) {
    strcpy( m_source, source);
    strcpy( m_target, target);
  }
  pkg_pattern( char *source) : m_node(0), m_severity('W') {
    strcpy( m_source, source);
    strcpy( m_target, "");
  }
  pkg_pattern( const pkg_pattern& x) : m_filelist(x.m_filelist), m_node(x.m_node),
    m_severity(x.m_severity) {
    strcpy( m_source, x.m_source);
    strcpy( m_target, x.m_target);
  }
  char *source() { return m_source;}
  char *target() { return m_target;}
  bool hasTarget() { return m_target[0] != 0;}
  void fetchFiles();
  void node( pkg_node *node) { m_node = node;}
};

class pkg_volume {
  friend class pkg_node;

 private:
  char m_name[80];
  char m_filename[200];  
  pwr_tVid m_vid;
  pwr_tTime m_time;
  bool m_isSystem;

 public:
  pkg_volume( char *name, char *filename, pwr_tVid vid, pwr_tTime time) :
    m_vid(vid), m_time(time), m_isSystem(false)
  {
    strcpy( m_name, name);
    strcpy( m_filename, filename);
    if ( strncmp( m_filename, "$pwr_load/", 10) == 0)
      m_isSystem = true;
  }
};

class pkg_node {
 private:
  vector<pkg_pattern> m_pattern;
  vector<pkg_file> m_filelist;
  vector<pkg_volume> m_volumelist;
  char m_name[80];
  char m_bootnode[80];
  pwr_mOpSys m_opsys;
  int m_bus;
  pwr_tMask m_dstatus;
  bool m_valid;
  int m_errors;
  int m_warnings;

 public:
  pkg_node( char *name): m_opsys(pwr_mOpSys__), m_bus(0),
    m_dstatus(0), m_valid(false), m_errors(0), m_warnings(0)
    { strcpy( m_name, name); strcpy( m_bootnode, "-");}
  pkg_node( char *name, pwr_mOpSys opsys, int bus, 
	    pwr_tMask dstatus, char *bootnode) :
    m_opsys(opsys), m_bus(bus), m_dstatus(dstatus),
    m_valid(true), m_errors(0), m_warnings(0)
    { strcpy( m_name, name); strcpy( m_bootnode, bootnode);}
  char *name() { return m_name;}
  pwr_mOpSys opsys() { return m_opsys;}
  int bus() { return m_bus;}
  pwr_tMask dstatus() { return m_dstatus;}
  char *bootnode() { return m_bootnode;}
  bool valid() { return m_valid;}
  void setOpsys( pwr_mOpSys opsys) { m_opsys = opsys;}
  void setBus( int bus) { m_bus = bus;}
  void setDStatus( pwr_tMask dstatus) { m_dstatus = dstatus;} 
  void setBootnode( char *bootnode) { strcpy( m_bootnode, bootnode);} 
  void setValid() { m_valid = true;}
  void push_back( pkg_pattern& pattern) { 
    pattern.node( this);
    m_pattern.push_back( pattern);
  }
  void checkVolume( char *filename);
  void fetchFiles( bool distribute);
  void copyPackage( char *pkg_name);
  void incrWarnings() { m_warnings++;}
  void incrErrors() { m_errors++;}
};

class wb_pkg {
 private:
  vector<pkg_node> m_nodelist;
  bool m_allnodes;

  void readConfig();

 public:
  wb_pkg( char *nodelist, bool distribute = true, bool config_only = false);
  pkg_node& getNode( char *name);
  void fetchFiles( bool distribute) {
    for ( int i = 0; i < (int)m_nodelist.size(); i++) m_nodelist[i].fetchFiles( distribute);
  }

  static void copyPackage( char *pkg_name);
};

#endif




