/* 
 * Proview   $Id: rt_io_pb_locals.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

#define IO_MAXCHAN 32
 
typedef struct {
  int  Pb_fp;
} io_sAgentLocal;

typedef struct {
  int initialized;
} io_sRackLocal;

typedef struct {
  void *input_area;
  void *output_area;
  short byte_swap;
  int initialized;
  int scancount[IO_MAXCHAN];
} io_sCardLocal;
