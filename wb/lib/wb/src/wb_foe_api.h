/** 
 * Proview   $Id: wb_foe_api.h,v 1.2 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_foe_api_h
#define wb_foe_api_h

/* wb_foe_api.h -- functional object editor
   This module contains the api for the Functional Object Editor (PLC).  */

#include "pwr_class.h"
#include "wb_ldh.h"

typedef void *foe_tContext;

foe_tContext
foe_Open (
  void		    *ParentEditorContext,
  void		    *ParentWindowContext,
  ldh_tSesContext   SessCtx,
  pwr_tObjDId	    PlcPgmObject
);

void
foe_Close (
  foe_tContext FoeContext
);

#endif
