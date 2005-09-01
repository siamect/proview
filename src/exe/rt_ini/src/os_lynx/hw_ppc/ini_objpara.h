/* 
 * Proview   $Id: ini_objpara.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef ini_objpara_h
#define ini_objpara_h

/* ini_objpara.h -- <short description>

   Routines used to initialize parameters, *ActualValue and
   ValueIndex in AI, AO, DI, DO, DV and AV-object.

   Initialize parameters, *RawValue, *AbsValue and ValueIndex
   in CO-object. 

   Count number of AI, AO, DI, DO, DV, CO and AV-object and 
   store it in RTDB (rtdb_node_config_datatype-object).  */

pwr_tInt32	ini_ObjPara();

#endif
