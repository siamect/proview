#ifndef wb_wnav_selformat_h
#define wb_wnav_selformat_h

/* wb_wtt.h -- Simple navigator

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

// Defines for primary selection mode
typedef enum {
  	wnav_eSelectionMode_GMS,
  	wnav_eSelectionMode_Normal,
  	wnav_eSelectionMode_Extern
	} wnav_eSelectionMode;


pwr_tBoolean wnav_format_selection( ldh_tSesContext ldhses, pwr_sAttrRef attrref, 
				   pwr_tBoolean is_class, pwr_tBoolean is_attr,
				   int select_syntax, int select_volume, 
				   int select_attr, int select_type, char *buff);



#if defined __cplusplus
}
#endif
#endif




