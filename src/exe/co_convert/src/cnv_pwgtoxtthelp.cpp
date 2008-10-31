/* 
 * Proview   $Id: cnv_pwgtoxtthelp.cpp,v 1.3 2008-10-31 12:51:30 claes Exp $
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

/* cnv_pwgtoxtthelp.cpp --
   Extract comment from pwg files and write as xtthelp file. */

/*_Include files_________________________________________________________*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "pwr_version.h"
#include "cnv_pwgtoxtthelp.h"
#include "co_lng.h"
#include "cnv_ctx.h"

using namespace std;

char *CnvPwgToXtthelp::fname_to_topic( char *fname)
{
  static char topic[200];

  char *s = strrchr( fname, '/');
  if ( s)
    s++;
  else
    s = fname;

  strcpy( topic, s);
  if ( (s = strrchr( topic, '.')))
    *s = 0;
       
  return topic;
}

CnvPwgToXtthelp::CnvPwgToXtthelp( CnvCtx *cnv_ctx) :
  ctx( cnv_ctx)
{
  // Get files
  static pwr_tFileName filenames[10] = {
    "$pwre_croot/xtt/lib/ge/src/pwr_*.pwg", 
    "$pwre_croot/xtt/mmi/sis/src/sis*.pwg", 
    "$pwre_croot/xtt/mmi/ssg/src/ssg*.pwg", 
    "$pwre_croot/bcomp/mmi/bcomp/src/bcomp*.pwg", 
    "$pwre_croot/abb/mmi/mcomp/src/mcomp*.pwg", 
    ""};
  static pwr_tFileName titles[10] = {
    "General", 
    "Ventilation", 
    "CircuitDiagram", 
    "BaseComponent", 
    "ABB", 
    ""};

  pwr_tFileName found_file;
  pwr_tStatus sts;
  char title[200];
  char group[200];
  pwr_tFileName outfile = "$pwr_exe/man_subgraph.dat";
  char *s;
  int idx;

  // Open output file
  if ( strcmp( ctx->dir, "") == 0)
    strcpy( outfile, "$pwr_exe/man_subgraph.dat");
  else {
    strcpy( outfile, ctx->dir);
    strcat( outfile, "man_subgraph.dat");
  }
      
  dcli_translate_filename( outfile, outfile);
  m_fp.open( outfile);

  // Print pdf header
  header();

  // Print index topic
  print_disable();
  m_fp << 
    "<topic> index" << endl <<
    "Subgraph documentation" << endl;

  for ( int i = 0; i < int(sizeof(filenames)/sizeof(filenames[0])); i++) {
    if ( strcmp( filenames[i], "") == 0)
      break;
    m_fp << "<b>" << titles[i] << " <link>" << titles[i] << endl;
  }
  m_fp << "</topic>" << endl;
  print_enable();

  for ( int i = 0; i < int(sizeof(filenames)/sizeof(filenames[0])); i++) {
    if ( strcmp( filenames[i], "") == 0)
      break;
    m_filelist.clear();
    strcpy( m_current_title, titles[i]);

    for ( sts = dcli_search_file( filenames[i], found_file, DCLI_DIR_SEARCH_INIT);
	  ODD(sts);
	  sts = dcli_search_file( filenames[i], found_file, DCLI_DIR_SEARCH_NEXT)) {
      
      if ( (s = strstr( found_file, "__p")) && sscanf( s+3, "%d", &idx))
	// Skip page
	continue;

      // Get title for this subgraph
      if ( !get_title( found_file, title, sizeof(title), group, sizeof(group)))
	strcpy( title, fname_to_topic( found_file));
      if ( strcmp( title, "") == 0)
	strcpy( title, fname_to_topic( found_file));

      PwgFile pf( found_file, title, group);
      m_filelist.push_back( pf);
    }
    dcli_search_file( filenames[i], found_file, DCLI_DIR_SEARCH_END);

    sort();

    chapter();

    exec_filelist( titles[i]);
    headerlevel();
    for ( int j = 0; j < (int)m_filelist.size(); j++) {
      if ( j == 0 ||
	   strcmp( m_filelist[j].m_group, m_filelist[j-1].m_group) != 0) {
	if ( j != 0) {
	  headerlevel_end();
	}
	m_fp << "<topic>" << topic_name(m_filelist[j].m_group) << "__dummy <style> function" << endl <<
	  "Group " << m_filelist[j].m_group << endl <<
	  "</topic>" << endl;
	headerlevel();
      }

      exec_file( m_filelist[j].m_fname);
    }
    headerlevel_end();
    headerlevel_end();
    chapter_end();
  }
}

int CnvPwgToXtthelp::exec_filelist( char *title)
{
  int idx, next_idx;

  // Print index
  m_fp << 
    "<topic> " << title << endl <<
    title << endl;

  print_disable();
  for ( int j = 0; j < (int)m_filelist.size(); j++) {
    if ( j == 0 ||
	 strcmp( m_filelist[j].m_group, m_filelist[idx].m_group) != 0) {
      idx = j;
    
      m_fp << "<b>Group " << m_filelist[j].m_group << " <link>" << m_current_title << "_" << topic_name( m_filelist[j].m_group) << endl;
    }
  }
  m_fp << "</topic>" << endl << endl;  

  // Print topic for groups
  
  idx = 0;
  for (;;) {
    exec_group( idx, &next_idx);
    idx = next_idx;
    if ( idx >= (int) m_filelist.size())
      break;
  }

  print_enable();

  return 1;
}

int CnvPwgToXtthelp::exec_group( int idx, int *next_idx)
{
  // Print index
  m_fp << 
    "<topic> " << m_current_title << "_" << topic_name( m_filelist[idx].m_group) << endl <<
    "Group " << m_filelist[idx].m_group << endl;

  for ( int j = idx; j < (int)m_filelist.size(); j++) {
    if ( strcmp( m_filelist[j].m_group, m_filelist[idx].m_group) != 0) {
      break;
    }
    m_fp << m_filelist[j].m_title << " <link>" << 
      fname_to_topic( m_filelist[j].m_fname) << endl;
    *next_idx = j + 1;
  }
  m_fp << "</topic>" << endl << endl;  

  return 1;
}

int CnvPwgToXtthelp::exec_file( char *fname)
{
  int type;
  char line[200];
  int incomment = 0;

  ifstream ifp( fname);
  if ( !ifp)
    return 0;

  ifp >> type;

  if ( type != 199)
    return 1;

  m_fp << "<topic> " << fname_to_topic( fname) << " <style> function" << endl;

  ifp.getline( line, sizeof(line));
  for (;;) {
    ifp.getline( line, sizeof(line));
    if ( strstr( line, "!*/") != 0)
      break;
    else if ( line[0] != '!')
      break;

    if ( incomment) {
      m_fp << (char *)&line[1] << endl;
    }
    if ( strstr( line, "!/**") != 0)
      incomment = 1;
  }
  ifp.close();

  m_fp << "</topic>" << endl << endl;
  return 1;
}

int CnvPwgToXtthelp::get_title( char *fname, char *title, int tsize,
				char *group, int gsize)
{
  char line[200];
  int type;
  int incomment = 0;
  int found = 0;
  char *s;

  ifstream ifp( fname);
  if ( !ifp)
    return found;

  ifp >> type;

  if ( type != 199)
    return 0;

  ifp.getline( line, sizeof(line));
  for (;;) {
    ifp.getline( line, sizeof(line));
    if ( strstr( line, "!*/") != 0)
      break;
    else if ( line[0] != '!')
      break;

    if ( incomment) {
      strncpy( title, (char *)&line[1], tsize);
      dcli_trim( title, title);

      ifp.getline( line, sizeof(line));
      if ( (s = strstr( line, "Group"))) {
	strncpy( group, s+6, gsize);
	dcli_trim( group, group);
      }
      else
	strcpy( group, "");

      found = 1;
      break;
    }
    if ( strstr( line, "!/**") != 0)
      incomment = 1;
  }
  ifp.close();
  return found;
}

void CnvPwgToXtthelp::sort()
{
  for ( int i = m_filelist.size() - 1; i > 0; i--) {
    for ( int j = 0; j < i; j++) {
      if ( strcmp( m_filelist[i].m_group, m_filelist[j].m_group) == 0) {
	//  Same group, sort by title
	if ( strcmp( m_filelist[i].m_title, m_filelist[j].m_title) < 0) {
	  PwgFile tmp = m_filelist[i];
	  m_filelist[i] = m_filelist[j];
	  m_filelist[j] = tmp;
	}
      }
      else {
	// Different groups, sort by group
	if ( strcmp( m_filelist[i].m_group, m_filelist[j].m_group) < 0) {
	  PwgFile tmp = m_filelist[i];
	  m_filelist[i] = m_filelist[j];
	  m_filelist[j] = tmp;
	}
      }
    }
  }
}



void CnvPwgToXtthelp::header()
{
  char timestr[40];
  char year[40];
  char *s;
  
  time_AtoAscii( 0, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  if ( (s = strchr( timestr, ' ')))
    *s = 0;
  strcpy( year, timestr);
  if ( (s = strchr( year, '-')))
    *s = 0;

  m_fp << 
    "<topic> __DocumentTitlePage" << endl << endl <<
    "<image> pwr_logga.gif" <<  endl << endl << endl << endl << endl << endl << endl <<
    "<h1> Graphic Symbol Library" << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl<<
    "<hr>" << endl <<
    timestr << endl <<
    "Version " << pwrv_cPwrVersionStr << endl <<
    "<hr>" << endl <<
    "</topic>" << endl << endl <<
    "<topic> __DocumentInfoPage" << endl << endl << endl <<
    "Copyright (C) "<< year << " SSAB Oxelösund AB" << endl << endl <<
    "Permission is granted to copy, distribute and/or modify this document" << endl <<
    "under the terms of the GNU Free Documentation License, Version 1.2" << endl <<
    "or any later version published by the Free Software Foundation;" << endl <<
    "with no Invariant Sections, no Front-Cover Texts, and no Back-Cover" << endl <<
    "Texts." << endl <<
    "</topic>" << endl;
}

void CnvPwgToXtthelp::chapter()
{
  m_fp << "<chapter>" << endl;
}

void CnvPwgToXtthelp::chapter_end()
{
  m_fp << "</chapter>" << endl;
}

void CnvPwgToXtthelp::headerlevel()
{
  m_fp << "<headerlevel>" << endl;
}

void CnvPwgToXtthelp::headerlevel_end()
{
  m_fp << "</headerlevel>" << endl;
}

void CnvPwgToXtthelp::print_disable()
{
  m_fp << "<option> printdisable" << endl;
}

void CnvPwgToXtthelp::print_enable()
{
  m_fp << "<option> printenable" << endl;
}

char *CnvPwgToXtthelp::topic_name( char *str)
{
  static char res[200];
  char *s, *t;

  // Replace '/' to '_'
  for ( s = str, t = res; *s; s++,t++) {
    if ( *s == '/')
      *t = '_';
    else
      *t = *s;
  }
  *t = 0;
  return res;
}
