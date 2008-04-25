/** 
 * Proview   $Id: xtt_fileview.h,v 1.1 2008-04-25 11:08:06 claes Exp $
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

#ifndef xtt_fileview_h
#define xtt_fileview_h

/* xtt_fileview.cpp -- view and select files */

typedef enum {
  fileview_eType_Open,
  fileview_eType_Save
} fileview_eType;

class XttFileview {
 public:
  XttFileview() {}
  ~XttFileview() {}
  
};

#endif
