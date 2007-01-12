/* 
 * Proview   $Id: rt_io_profiboard.h,v 1.2 2007-01-12 13:28:32 claes Exp $
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

#ifndef __RT_IO_M_PB_UTILS__
#define __RT_IO_M_PB_UTILS__

#define PB_OK			0
#define PB_DEVICE_ERROR		1
#define PB_READ_ERROR		2
#define PB_WRITE_ERROR		3
#define PB_NO_CON_IND_RECEIVED	4
#define PB_NEG_CONF		5

extern
unsigned short swap16(unsigned short in);

extern
unsigned int swap32(unsigned int in); 

#endif

