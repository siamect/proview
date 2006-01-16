/* 
 * Proview   $Id: rt_io_pb_locals.h,v 1.1 2006-01-16 10:55:42 claes Exp $
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

#define IO_MAXCHAN 96

#define PB_MODULE_STATE_NOTINIT 0
#define PB_MODULE_STATE_OPERATE 1

#define PB_SLAVE_STATE_NOTINIT 0
#define PB_SLAVE_STATE_STOPPED 1
#define PB_SLAVE_STATE_OPERATE 2

#define PB_MASTER_STATE_NOTINIT 0
#define PB_MASTER_STATE_STOPPED 1
#define PB_MASTER_STATE_CLEARED 2
#define PB_MASTER_STATE_OPERATE 3

#define PB_STALLACTION_NONE	0
#define PB_STALLACTION_RESET 	1
#define PB_STALLACTION_BREAK 	2

#define PB_NUMREP_UNSIGNEDINT	0
#define PB_NUMREP_SIGNEDINT	1
#define PB_NUMREP_FLOATIEEE	2
#define PB_NUMREP_FLOATVAX	3
#define PB_NUMREP_FLOATINTEL	4

#define PB_BYTEORDERING_LE	0
#define PB_BYTEORDERING_BE	1

#define PB_ORIENTATION_BYTE	8
#define PB_ORIENTATION_WORD	16
#define PB_ORIENTATION_DWORD	32

typedef struct {
  int  Pb_fp;
} io_sAgentLocal;

typedef struct {
  int initialized;
} io_sRackLocal;

typedef struct {
  void *input_area;
  void *output_area;
  int scancount[IO_MAXCHAN];
} io_sCardLocal;
