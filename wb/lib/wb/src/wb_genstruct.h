/** 
 * Proview   $Id: wb_genstruct.h,v 1.2 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_genstruct_h
#define wb_genstruct_h

/* wb_genstruct.h
   This include file contains definitions and function prototypes
   needed to use ds_ldh_genstruct.  */

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

int genstr_process_top (
  ldh_tSesContext sesctx,
  char *volname
);
int genstr_psts (
  unsigned long int sts,
  FILE *logfile
);

#endif
