/* rt_cmvolsm.c -- Cached volumes

   PROVIEW/R
   Copyright (C) 1996-2002 by Cell Network AB.

   This module contains the meta cache handling routines for the server monitor.  */


#ifndef rt_cmvolsm_h
#define rt_cmvolsm_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

void
cmvolsm_GetCclass (
  qcom_sGet		*get
  );

void
cmvolsm_GetGclass (
  qcom_sGet		*get
  );

#endif
