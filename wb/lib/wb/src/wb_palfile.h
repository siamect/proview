#ifndef wb_palfile_h
#define wb_palfile_h

/* wb_palfile.h -- Palette configuration file

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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

