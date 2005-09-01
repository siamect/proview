/** 
 * Proview   $Id: wb_foe_ptrn.h,v 1.2 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_foe_ptrn_h
#define wb_foe_ptrn_h

#define pattern_width 16
#define pattern_height 16
static char pattern_bits[] = {
   0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xfc, 0x1f, 0x04, 0x10, 0xa4, 0x12,
   0x44, 0x11, 0xa4, 0x12, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10,
   0xfc, 0x1f, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00};

#endif
