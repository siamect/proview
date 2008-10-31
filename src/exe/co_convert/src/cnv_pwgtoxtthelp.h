/* 
 * Proview   $Id: cnv_pwgtoxtthelp.h,v 1.2 2008-10-31 12:51:30 claes Exp $
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

#ifndef cnv_pwgxtthelp_h
#define cnv_pwgxtthelp_h

#include <vector>
#include <string.h>
#include "pwr.h"

using namespace std;

class CnvCtx;

class PwgFile {
 public:
  PwgFile( char *name, char *title, char *group) {
    strcpy( m_fname, name);
    strcpy( m_title, title);
    strcpy( m_group, group);
  }
  pwr_tFileName m_fname;
  char m_title[200];
  char m_group[200];
};

class CnvPwgToXtthelp {
 public:
  CnvPwgToXtthelp( CnvCtx *cnv_ctx);
  int exec_file( char *fname);
  int exec_filelist( char *title);
  int exec_group( int idx, int *next_idx);
  char *fname_to_topic( char *fname);
  int get_title( char *fname, char *title, int tsize, char *group, int gsize);
  void sort();
  void header();
  void chapter();
  void chapter_end();
  void headerlevel();
  void headerlevel_end();
  void print_enable();
  void print_disable();
  char *topic_name( char *str);

  vector<PwgFile> m_filelist;
  ofstream	m_fp;
  char		m_current_title[200];
  CnvCtx 	*ctx;
};

#endif

