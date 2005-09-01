/* 
 * Proview   $Id: rt_bck_load.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_bck_load_h
#define rt_bck_load_h

/* rt_bck_load.h

   This module contains the code for reading and
   restoring the information in the backup data file.  */

/* This routine opens the backup datafile, reads the
   information and loads it into rtdb.
   This should be done before bck_Init is invoked.  */

pwr_tStatus bck_LoadBackup ();

#endif
