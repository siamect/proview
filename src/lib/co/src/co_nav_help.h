#ifndef co_nav_help_h
#define co_nav_help_h

/* co_nav_help.h -- Help file parser

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
	navh_eItemType_Image
	} navh_eItemType;


class NavHelp {
  public:
    NavHelp( void *h_parent_ctx, char *h_base_file, char *h_project_file) 
      : parent_ctx(h_parent_ctx) 
      { strcpy(project_file, h_project_file);
        strcpy(base_file, h_base_file);};
    void 	        *parent_ctx;
    void 		*(*insert_cb)( void *, navh_eItemType, char *, 
			     char *, char *, char *, char *, char *,
			     navh_eHelpFile, int, char *);
    void 		(*draw_cb)( void *, int, void *);
    char                base_file[120];
    char                project_file[120];

    int help( char *key, char *help_bookmark, navh_eHelpFile file_type, 
	      char *file_name, void **bookmark, bool strict = false);
    int	help_index( navh_eHelpFile file_type, char *file_name);
};


#if defined __cplusplus
}
#endif
#endif








