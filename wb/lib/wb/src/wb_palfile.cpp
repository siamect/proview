/* 
 * Proview   $Id: wb_palfile.cpp,v 1.4 2008-10-15 06:04:55 claes Exp $
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

/* wb_palfile.cpp -- Palette of configurator or plc-editor */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

#include "wb_palfile.h"

extern "C" {
#include "wb_ldh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
}


PalFileMenu *PalFile::config_tree_build( ldh_tSession ldhses, char *filename, 
				   pal_eNameType keytype, char *keyname, PalFileMenu *menu)
{
  ifstream	fp;
  int		line_cnt = 0;
  char		line[140];
  char		type[120];
  char		name[120];
  int           nr;
  PalFileMenu     *menu_tree = NULL;
  PalFileMenu     *menu_p, *mp;
  char          fname[120];

  dcli_translate_filename( fname, filename);

  fp.open( fname);
#ifndef OS_VMS
  if ( !fp)
    return menu_tree;
#endif

  line_cnt = 0;
  while ( 1)
  {
    if ( !fp.getline( line, sizeof( line)))
      break;
    dcli_remove_blank( line, line);
    line_cnt++;
    if ( line[0] == 0)
      continue;
    if ( line[0] == '!' || line[0] == '#')
      continue;
    nr = sscanf( line, "%s %s", type, name);
    if ( nr < 1 )
      printf( "** Syntax error in file %s, line %d\n", filename, line_cnt);

    if ( keytype != pal_eNameType_All) {
      if ( (keytype == pal_eNameType_TopObjects && 
	    cdh_NoCaseStrcmp( type, "topobjects") == 0 && 
	    cdh_NoCaseStrcmp( name, keyname) == 0) ||
	   (keytype == pal_eNameType_Palette && 
	    cdh_NoCaseStrcmp( type, "palette") == 0 && 
	    cdh_NoCaseStrcmp( name, keyname) == 0)) {
	if ( nr != 2)
	  printf( "** Syntax error in file %s, line %d\n", filename, line_cnt);

	if ( !menu)
	  menu_tree = new PalFileMenu( name, pal_eMenuType_Menu, NULL);
	else
	  menu_tree = menu;

	if ( !fp.getline( line, sizeof( line)))
	  break;
	line_cnt++;
	if ( strcmp( line, "{") != 0)
	  printf( "** Syntax error in file %s, line %d\n", filename, line_cnt);

	menu_p = config_tree_build_children( ldhses, &fp, &line_cnt, filename,
					     menu_tree);
	if ( !menu_tree->child_list)
	  menu_tree->child_list = menu_p;
	break;
      }
    }
    else {
      if ( nr != 2)
	printf( "** Syntax error in file %s, line %d\n", filename, line_cnt);

      if ( cdh_NoCaseStrcmp( type, "topobjects") == 0)
	menu_p = new PalFileMenu( name, pal_eMenuType_TopObjects, NULL);
      else if ( cdh_NoCaseStrcmp( type, "palette") == 0)
	menu_p = new PalFileMenu( name, pal_eMenuType_Palette, NULL);
      
      if ( !menu_tree)
	menu_tree = menu_p;
      else {
	for ( mp = menu_tree; mp->next; mp = mp->next)
	  ;
	mp->next = menu_p;
      }

      if ( !fp.getline( line, sizeof( line)))
	break;
      line_cnt++;
      if ( strcmp( line, "{") != 0)
	printf( "** Syntax error in file %s, line %d\n", filename, line_cnt);
      
      menu_p->child_list = config_tree_build_children( ldhses, &fp, &line_cnt,
						       filename, menu_p);
    }
  }
  fp.close();
  return menu_tree;
}

PalFileMenu *PalFile::config_tree_build_children( ldh_tSession ldhses, ifstream *fp, 
	       int *line_cnt, char *filename, PalFileMenu *parent)
{
  PalFileMenu	        *menu_p, *prev, *mp;
  PalFileMenu	        *return_menu = NULL;
  int			first = 1;
  int			nr;
  char			line[140];
  char			type[120];
  char			name[120];
  char			p1[120];
  char			p2[120];
  int                   found;
  int                   hide_children = 0;

    // Children might already exist
  if ( parent) {
    for ( menu_p = parent->child_list; menu_p; menu_p = menu_p->next) {
      prev = menu_p;
      first = 0;
    }
  }
  menu_p = 0;

  while ( 1)
  {
    if ( !fp->getline( line, sizeof( line)))
      break;
    dcli_remove_blank( line, line);
    (*line_cnt)++;
    if ( line[0] == 0)
      continue;
    if ( line[0] == '!' || line[0] == '#')
      continue;
    nr = sscanf( line, "%s %s %s %s", type, name, p1, p2);
    if ( nr < 1 )
      printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);

    if ( strcmp( type, "{") == 0)
    {
      if ( !hide_children) {
        if ( nr != 1 || !menu_p)
          printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);
        else
          mp = config_tree_build_children( ldhses, fp, line_cnt, filename,
		menu_p);
        if ( !menu_p->child_list)
          menu_p->child_list = mp;
      }
      else
	hide_children = 0;
    }
    else if ( strcmp( type, "}") == 0)
    {
      if ( nr != 1 )
        printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);
      return return_menu;
    }
    else if ( cdh_NoCaseStrcmp( type, "menu") == 0)
    {
      if ( !(nr == 2 || nr == 3))
        printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);

      if ( nr == 3 && ( ldhses && !check_volume( ldhses, p1))) {
        hide_children = 1;
        break;
      }

      // Check if it already exist
      found = 0;
      for ( menu_p = parent->child_list; menu_p; menu_p = menu_p->next) {
        if ( strcmp( menu_p->title, name) == 0) {
          found = 1;
          break;
        }
      } 

      if ( !found) {
        menu_p = new PalFileMenu( name, pal_eMenuType_Menu, parent);
        if ( first) {
          return_menu = menu_p;
          first = 0;
        }
        else
          prev->next = menu_p;
        prev = menu_p;
      }
    }
    else if ( cdh_NoCaseStrcmp( type, "class") == 0) {
      if ( !( nr == 2 || nr == 3))
        printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);

      if ( nr == 2 || ( nr == 3 && (!ldhses || check_volume( ldhses, p1)))) {
        menu_p = new PalFileMenu( name, pal_eMenuType_Class, parent);
        if ( first) {
          return_menu = menu_p;
          first = 0;
        }
        else
          prev->next = menu_p;
        prev = menu_p;
      }
    }
    else if ( cdh_NoCaseStrcmp( type, "classvolume") == 0)
    {
      if ( !(nr == 2 || nr == 3))
        printf( "** Syntax error in file %s, line %d\n", filename, *line_cnt);

      if ( nr == 2 || ( nr == 3 && (!ldhses || check_volume( ldhses, p1)))) {
        menu_p = new PalFileMenu( name, pal_eMenuType_ClassVolume, parent);
        if ( first) {
          return_menu = menu_p;
          first = 0;
        }
        else
          prev->next = menu_p;
        prev = menu_p;
      }
    }
  }

  return return_menu;
}

int PalFile::check_volume( ldh_tSession ldhses, char *name)
{
  pwr_tClassId classid;
  pwr_tVolumeId volume;
  int sts;
  int size;
  char volume_name[80];

  // Find a class volume with this name
  sts = ldh_GetVolumeList( ldh_SessionToWB(ldhses), &volume);
  while ( ODD(sts))
  {
    sts = ldh_GetVolumeClass( ldh_SessionToWB(ldhses), volume, &classid);
    if ( EVEN(sts)) return 0;

    if ( cdh_isClassVolumeClass( classid))
    {
      sts = ldh_VolumeIdToName( ldh_SessionToWB(ldhses),
		volume, volume_name, sizeof(volume_name), &size);
      if ( EVEN(sts)) return 0;

      if ( !cdh_NoCaseStrcmp( volume_name, name))
        return 1;
    }
    sts = ldh_GetNextVolume( ldh_SessionToWB(ldhses), volume, &volume);
  }
  return 0;
}

void PalFile::config_tree_free( PalFileMenu *menu_tree)
{
  if ( menu_tree)
    config_tree_free_children( menu_tree);
}

void PalFile::config_tree_free_children( PalFileMenu *first_child)
{
  PalFileMenu *menu_p, *next;

  menu_p = first_child;
  while( menu_p) {
    next = menu_p->next;
    if ( menu_p->child_list)
      config_tree_free_children( menu_p->child_list);
    delete menu_p;
    menu_p = next;
  }
}

void PalFile::config_tree_print( char *filename, PalFileMenu *menu_tree, pwr_tStatus *sts)
{
  pwr_tFileName fname;
  ofstream fp;
  PalFileMenu *menu_p;

  *sts = 1;
  dcli_translate_filename( fname, filename);
  fp.open( fname);
#ifndef OS_VMS
  if ( !fp) {
    *sts = 0;
    return;
  }
#endif

  for ( menu_p = menu_tree;
	 menu_p;
	 menu_p = menu_p->next)
    config_tree_print_item( menu_p, fp, 0);

  fp.close();
}

void PalFile::config_tree_print_item( PalFileMenu *item, ofstream& fp, int level)
{
  PalFileMenu *menu_p;
  char ind_str[80] = "";
  
  for ( int i = 0; i < level; i++)
    strcat( ind_str, "  ");

  switch ( item->item_type) {
  case pal_eMenuType_TopObjects:
    fp << "topobjects " << item->title << endl << "{" << endl;
    break;
  case pal_eMenuType_Palette:
    fp << "palette " << item->title << endl << "{" << endl;
    break;
  case pal_eMenuType_Menu:
    fp << ind_str << "menu " << item->title << endl << ind_str << "{" << endl;
    break;
  case pal_eMenuType_Class:
    fp << ind_str << "class " << item->title << endl;
    break;
  case pal_eMenuType_ClassVolume:
    fp << ind_str << "classvolume " << item->title << endl;
    break;
  }

  for ( menu_p = item->child_list; 
	 menu_p; 
	 menu_p = menu_p->next) {
    config_tree_print_item( menu_p, fp, level + 1);
  }

  switch ( item->item_type) {
  case pal_eMenuType_TopObjects:
  case pal_eMenuType_Palette:
  case pal_eMenuType_Menu:
    fp << ind_str << "}" << endl;
    break;
  default: ;
  }
}

