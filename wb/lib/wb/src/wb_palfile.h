/* 
 * Proview   $Id: wb_palfile.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_palfile_h
#define wb_palfile_h

/* wb_palfile.h -- Palette configuration file */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

#define pal_cPaletteFile "$pwr_exe/pwr_wb_palette.cnf"
#define pal_cLocalPaletteFile "$pwrp_db/pwrp_cnf_palette.dat"

typedef enum {
  pal_eNameType_TopObjects,
  pal_eNameType_Palette,
  pal_eNameType_All
} pal_eNameType;

typedef enum {
  pal_eMenuType_Menu,
  pal_eMenuType_Class,
  pal_eMenuType_ClassVolume,
  pal_eMenuType_TopObjects,
  pal_eMenuType_Palette
} pal_eMenuType;

class PalFileMenu {
 public:
  PalFileMenu( char *menu_title, int menu_item_type, PalFileMenu *menu_parent) :
    item_type(menu_item_type), pixmap(0), child_list(0), parent(menu_parent), next(0)
    { strcpy( title, menu_title);}

  char	     	title[80];
  int   	item_type;
  char   	file[120];
  int    	pixmap;
  PalFileMenu	*child_list;
  PalFileMenu	*parent;
  PalFileMenu	*next;
};


class PalFile {
  public:
    static PalFileMenu *config_tree_build( ldh_tSession ldhses, char *filename, 
	       pal_eNameType keytype, char *keyname, PalFileMenu *menu);
    static PalFileMenu *config_tree_build_children( ldh_tSession ldhses, 
               ifstream *fp, int *line_cnt, char *filename, PalFileMenu *parent);
    static void config_tree_free( PalFileMenu *menu_tree);
    static void config_tree_free_children( PalFileMenu *first_child);
    static int check_volume( ldh_tSession ldhses, char *name);
    static void config_tree_print( char *filename, PalFileMenu *menu_tree, pwr_tStatus *sts);
    static void config_tree_print_item( PalFileMenu *item, ofstream& fp, int level);
};

#if defined __cplusplus
}
#endif
#endif

