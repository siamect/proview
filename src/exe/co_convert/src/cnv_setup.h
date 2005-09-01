/* 
 * Proview   $Id: cnv_setup.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_setup_h
#define cnv_setup_h

/* cnv_setup.h -- Setup */


#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"

#define MAX_GROUPS 100

class CnvSetup {
 public:
  int 		group_cnt;
  char 	        groups[MAX_GROUPS][40];
  char 		groups_startpage[MAX_GROUPS][80];

  int setup( char *filename);
};

#if defined __cplusplus
}
#endif
#endif




