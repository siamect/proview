/* 
 * Proview   $Id: qbus_io.h,v 1.1 2006-01-12 05:54:27 claes Exp $
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


#define QBUS_IOCTL_PHYSBADLA 	1

typedef struct {
	unsigned int	Address;
	unsigned int	Data;
} qbus_io_read;

typedef struct {
        unsigned int    Address;
        unsigned int    Data;
} qbus_io_write;

typedef struct {
        unsigned int    PhysBadla;
} qbus_io_ioctl;

