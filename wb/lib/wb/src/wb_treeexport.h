/* 
 * Proview   $Id: wb_treeexport.h,v 1.4 2005-12-13 15:15:53 claes Exp $
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

#ifndef wb_treeexport_h
#define wb_treeexport_h

class wb_treeimport;

class wb_treeexport
{
public:

  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid) = 0;

  virtual ~wb_treeexport() {}
};

#endif

