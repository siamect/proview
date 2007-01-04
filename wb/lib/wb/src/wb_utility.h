/* 
 * Proview   $Id: wb_utility.h,v 1.1 2007-01-04 07:29:04 claes Exp $
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

#ifndef wb_utility_h
#define wb_utility_h

/* wb_utility.h -- Baseclass for a workbench utility */

typedef enum {
	wb_eUtility_Wtt 		= 999,
	wb_eUtility_PlcEditor 		= 1000,
	wb_eUtility_AttributeEditor	= 1001,
	wb_eUtility_Distributor		= 1002,
	wb_eUtility_SpreadsheetEditor  	= 1003,
	wb_eUtility_Cmd  		= 1004,
	wb_eUtility_WNav  		= 1005
} wb_eUtility;

class WUtility {
 public:
  WUtility( wb_eUtility type) : utype(type) {}
  virtual ~WUtility() {}
  wb_eUtility	utype;
};

#endif

