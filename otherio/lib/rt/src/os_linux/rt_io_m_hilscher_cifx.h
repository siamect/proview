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

#ifndef rt_io_hilscher_cifx_h
#define rt_io_hilscher_cifx_h

typedef struct {
  unsigned long channel;
  unsigned long board;
  CIFXHANDLE chan;
  unsigned int diag_cnt;
  unsigned int diag_interval;
  unsigned int dev_init;
  unsigned int dev_init_cnt;
  unsigned int dev_init_limit;
  int softlimit_logged;
  int input_area_size;
  int output_area_size;
  void *input_area;
  void *output_area;
} io_sLocalHilscher_cifX_Master;

typedef struct {
  int byte_ordering;
  int float_representation;
} io_sLocalHilscher_cifX_Device;

typedef struct {
  unsigned long channel;
  unsigned long board;
  CIFXHANDLE chan;
  unsigned int diag_cnt;
  unsigned int diag_interval;
  unsigned int dev_init;
  unsigned int dev_init_cnt;
  unsigned int dev_init_limit;
  int softlimit_logged;
  int input_area_size;
  int output_area_size;
  void *input_area;
  void *output_area;
} io_sLocalHilscher_cifX_PnController;

#endif
