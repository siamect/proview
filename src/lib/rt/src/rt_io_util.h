/* 
 * Proview   $Id: rt_io_util.h,v 1.1 2005-12-30 15:45:31 claes Exp $
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

#ifndef rt_io_util_h
#define rt_io_util_h

/* rt_io_util.h -- includefile for io util. */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

pwr_tStatus io_GetIoTypeClasses( 
  io_eType	type,
  pwr_tClassId 	**classes,
  int		*size
);

int io_CheckClassIoType( 
  io_eType	type,
  pwr_tCid 	cid
);
#endif
