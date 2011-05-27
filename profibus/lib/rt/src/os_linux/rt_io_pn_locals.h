/* 
 * Proview   $Id$
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

#ifndef rt_io_pn_locals_h
#define rt_io_pn_locals_h

/* rt_io_pn_locals.h -- Profinet io handling locals. */

typedef struct _io_sPnRackLocal {
  unsigned short  bytes_of_input;
  unsigned short  bytes_of_output;
  unsigned char  *inputs;
  unsigned char  *outputs;
} io_sPnRackLocal;

typedef struct _io_sPnCardLocal {
  unsigned int input_area_size;
  unsigned int output_area_size;
  unsigned char *input_area;
  unsigned char *output_area;
} io_sPnCardLocal;

#endif
