/* 
 * Proview   $Id: rt_plc_proc.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_plc_proc_h
#define rt_plc_proc_h

/* Initialize the PLC.
   At this state only direct linking can be done.
   The part of the initialization which effects parameters
   in the object body must wait until the old PLC program has
   stopped. 
   First argument = 1 -> Do the direct link part.
   First argument = 0 -> It is ok to change object parameters.

   The part of the initialization which effects parameters
   in the object body can now be done, since the old PLC program has
   stopped. 
   First argument (DirectLink) = 2 -> Do the I/O direct link part.
   First argument (DirectLink) = 0 -> It is ok to change object parameters.  */


typedef struct plc_proctbl plc_sProctbl;

struct plc_proctbl {
  pwr_tObjid	thread;
  void		(*init)(int, plc_sThread*);
  void		(*exec)(int, plc_sThread*);
};
#endif
