/* 
 * Proview   $Id: wb_build_opt.h,v 1.1 2006-03-31 14:24:34 claes Exp $
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

#ifndef wb_build_opt_h
#define wb_build_opt_h

class wb_build_opt
{
 public:
  wb_build_opt() : force(0), debug(0), crossref(0), manual(0) {}
  int force;
  int debug;
  int crossref;
  int manual;
};

#endif
