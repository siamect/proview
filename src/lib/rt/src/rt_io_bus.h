/* 
 * Proview   $Id: rt_io_bus.h,v 1.1 2008-02-29 13:06:03 claes Exp $
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

#ifndef rt_io_bus_h
#define rt_io_bus_h

/* rt_io_bus.h -- includefile for io bus. */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef PWR_BASECLASSES_H
#include "pwr_baseclasses.h"
#endif

#ifndef rt_io_supervise_h
#include "rt_io_supervise.h"
#endif

#ifndef rt_io_methods_h
#include "rt_io_methods.h"
#endif

#ifndef NULL
#define NULL (void *) 0
#endif


/*----------------------------------------------------------------------------*\
  Io functions
\*----------------------------------------------------------------------------*/


int is_diag( pwr_tAttrRef *aref);

pwr_tInt32 GetChanSize(pwr_eDataRepEnum rep);

unsigned short swap16(unsigned short in);

unsigned int swap32(unsigned int in);

void io_card_read( 
  io_tCtx	         ctx,
  io_sRack	         *rp, 
  io_sCard	         *cp, 
  void                   *input_area, 
  void                   *diag_area,
  pwr_tByteOrderingEnum  byte_order,
  pwr_tFloatRepEnum      float_rep
);

void io_card_write( 
  io_tCtx	         ctx,
  io_sCard	         *cp, 
  void                   *output_area, 
  pwr_tByteOrderingEnum  byte_order,
  pwr_tFloatRepEnum      float_rep
);

#endif
