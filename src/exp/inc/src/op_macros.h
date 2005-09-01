/* 
 * Proview   $Id: op_macros.h,v 1.2 2005-09-01 14:57:49 claes Exp $
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
 */

#ifndef op_macros_h
#define op_macros_h

/* op_macros.h -- <short description>
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
