#ifndef co_nav_crr_h
#define co_nav_crr_h

/* co_nav_crr.h -- Display crossreferenses

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif


typedef enum {
	navc_eItemType_Crossref,
	navc_eItemType_Header,
	navc_eItemType_Text
	} navc_eItemType;


class NavCrr {
  public:
    NavCrr( void *c_parent_ctx, void *c_parent_node) 
      : parent_ctx(c_parent_ctx), parent_node(c_parent_node) 
      {};
    void 	        *parent_ctx;
    void                *parent_node;
    void 		(*insert_cb)( void *, void *, navc_eItemType,
			     char *, char *, int);
    int                 (*name_to_objid_cb)( void *, char *, pwr_tObjid *);
    int                 (*get_volume_cb)( void *, pwr_tVolumeId *);
    int	crr_signal( char *filename, char *signalname);
    int	crr_object( char *filename, char *objectname);
    int	crr_code( char *filename, char *str, int brief, int func,
		  int case_sensitive);
};


#if defined __cplusplus
}
#endif
#endif








