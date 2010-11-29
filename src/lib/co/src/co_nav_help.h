/** 
 * Proview   $Id: co_nav_help.h,v 1.9 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_nav_help_h
#define co_nav_help_h

/* co_nav_help.h -- Help file parser */

#include <string.h>

#if defined __cplusplus
extern "C" {
#endif

typedef enum {
	navh_eHelpFile_,
	navh_eHelpFile_Base,
	navh_eHelpFile_Project,
	navh_eHelpFile_Other
	} navh_eHelpFile;

typedef enum {
	navh_eItemType_Help,
	navh_eItemType_HelpBold,
	navh_eItemType_HelpHeader,
	navh_eItemType_Header,
	navh_eItemType_HeaderLarge,
	navh_eItemType_HorizontalLine,
	navh_eItemType_Topic,
	navh_eItemType_EndTopic,
	navh_eItemType_Image,
	navh_eItemType_DocTitlePage,
	navh_eItemType_DocInfoPage,
	navh_eItemType_Chapter,
	navh_eItemType_EndChapter,
	navh_eItemType_HeaderLevel,
	navh_eItemType_EndHeaderLevel,
	navh_eItemType_PageBreak,
	navh_eItemType_Style,
	navh_eItemType_HelpCode,
	navh_eItemType_Option
	} navh_eItemType;


class NavHelp {
  public:
    NavHelp( void *h_parent_ctx, const char *h_base_file, const char *h_project_file) 
      : parent_ctx(h_parent_ctx), noprop(0)
      { strncpy(project_file, h_project_file, sizeof(project_file));
        strncpy(base_file, h_base_file, sizeof(base_file));};
    void 	        *parent_ctx;
    void 		*(*insert_cb)( void *, navh_eItemType, const char *, 
			     const char *, const char *, const char *, const char *, const char *,
			     navh_eHelpFile, int, const char *, int);
    void 		(*draw_cb)( void *, int, void *);
    pwr_tFileName       base_file;
    pwr_tFileName       project_file;
    int			noprop;

    int help( const char *key, const char *help_bookmark, navh_eHelpFile file_type, 
	      const char *file_name, void **bookmark, bool strict = false);
    int	get_next_key( const char *help_key, navh_eHelpFile file_type, const char *file_name,
		      bool strict, char *next_key);
    int get_previous_key( const char *help_key, navh_eHelpFile file_type, const char *file_name,
			  bool strict, char *prev_key);
    int	help_index( navh_eHelpFile file_type, const char *file_name);
    void set_propagate( int prop) { noprop = !prop;}
};


#if defined __cplusplus
}
#endif
#endif








