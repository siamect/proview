#ifndef op_macros_h
#define op_macros_h

/* op_macros.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   <Description>.  */

#include "rt_errh.h"

#define Log_Error(a, b) errh_CErrLog(PRO__ERROR, errh_ErrArgAF(b), errh_ErrArgMsg(a), NULL)
#define Log_Error_No_Sts(b) errh_CErrLog(PRO__ERROR, errh_ErrArgAF(b), NULL)
#define Log(b) errh_CErrLog(PRO__LOG, errh_ErrArgAF(b), NULL)
#define Log_Error_Exit(a, b) {Log_Error(a, b); exit(a);}
#define Log_Error_Return(a, b) {Log_Error(a, b); return (a);}
#define If_Error_Log(a, b) if ((a & 1) != 1) Log_Error(a, b)
#define If_Error_Log_Return(a, b) if ((a & 1) != 1) Log_Error_Return(a, b)
#define If_Error_Log_Exit(a, b) if ((a & 1) != 1) Log_Error_Exit(a, b)

#endif
