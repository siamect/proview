/* 
 * Proview   $Id: wb_export.h,v 1.5 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_export_h
#define wb_export_h

class wb_import;

class wb_export
{
public:

  virtual bool exportVolume(wb_import &e) = 0;
    
  virtual bool exportHead(wb_import &e) = 0;

  virtual bool exportRbody(wb_import &e) = 0;
    
  virtual bool exportDbody(wb_import &e) = 0;
    
  virtual bool exportDocBlock(wb_import &e) = 0;
    
  virtual bool exportMeta(wb_import &e) = 0;
};

#endif
