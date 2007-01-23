/* 
 * Proview   $Id: cnv_changelog.h,v 1.3 2007-01-23 13:11:43 claes Exp $
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
 */

#ifndef cnv_changelog_h
#define cnv_changelog_h

/* cnv_changelog.h -- Read changelog files */

using namespace std;
#include <vector>

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_ctx.h"

using namespace std;

class LogEntry {
 public:
  LogEntry() 
  { 
    strcpy(signature,""); 
    strcpy(module,"");
    strcpy(component,"");
    strcpy(text,"");
  }
  LogEntry( const LogEntry& x) : time(x.time)
  {
    strcpy(signature,x.signature); 
    strcpy(module,x.module);
    strcpy(component,x.component);
    strcpy(text,x.text);
  }
  const LogEntry& operator= ( const LogEntry& x)
  {
    time = x.time;
    strcpy(signature,x.signature); 
    strcpy(module,x.module);
    strcpy(component,x.component);
    strcpy(text,x.text);
    return *this;
  }
  pwr_tTime time;
  char signature[10];
  char module[40];
  char component[40];
  char text[400];
};


class CnvChangeLog {
 public:
  vector<LogEntry> entries;
  CnvCtx 	*ctx;
  int		from;
  pwr_tTime	from_time;
  

  CnvChangeLog( CnvCtx *cnv_ctx, char *from_str);

  int read( char *module);
  void print();
  void print_docbook();
  void sort_time();
  void sort_module();
  void sort_component();
  void sort_signature();
};

#if defined __cplusplus
}
#endif
#endif



