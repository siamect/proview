#ifndef rt_csup_h
#define rt_csup_h 

/* rt_csup.h -- Cycle supervision

   PROVIEW/R
   Copyright (C) 2000 by Wiromania.

*/

#ifndef rt_lst_h
# include "rt_lst.h"
#endif

typedef struct {
  lst_sEntry le;
  pwr_sClass_CycleSup *o;
  pwr_sAttrRef aref;
  pwr_tDlid dlid;
  int is_owner;
} csup_sObject;

void		csup_Exit(pwr_tStatus*, lst_sEntry*);
lst_sEntry*	csup_Init (pwr_tStatus*, pwr_tObjid, float);
int		csup_Exec(pwr_tStatus*, lst_sEntry*, pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tTime*);

#endif
