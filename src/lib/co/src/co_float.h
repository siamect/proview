/** 
 * Proview   $Id: co_float.h,v 1.3 2005-09-01 14:57:52 claes Exp $
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

#ifndef co_float_h
#define co_float_h

#ifdef __cplusplus
extern "C" {
#endif

/**
  @file co_float.h

  Converts an IEEE float to a VAX float and vice verca.
 */

#include "co_pdr.h"


void co_vaxf2ieee(co_eBO sbo,     /**< Source byte order, VAX float */
                  co_eBO tbo,     /**< Target byte order, IEEE float */
                  const char *sp, /**< Address to the source, VAX float */
                  char *tp);      /**< Address to the target, IEEE float */


void co_ieee2vaxf(co_eBO sbo,     /**< Source byte order, IEEE float */
                  co_eBO tbo,     /**< Target byte order, VAX float */
                  const char *sp, /**< Address to the source, IEEE float */
                  char *tp);      /**< Address to the target, VAX float */


#ifdef __cplusplus
}
#endif

#endif
