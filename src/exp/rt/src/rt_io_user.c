/* 
 * Proview   $Id: rt_io_user.c,v 1.2 2005-09-01 14:57:49 claes Exp $
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

/* rt_io_user.c -- dummy for user methods.
*/

#include "pwr.h"
#include "rt_io_base.h"

/* Dummy for declaration of user methods, when no methods are defined 	*/

/* Example of declaration of user methods 				*/
/*									*/
/* 	pwr_dImport pwr_BindIoUserMethods($Node);			*/
/* 	pwr_dImport pwr_BindIoUserMethods(Co_PI24BO);			*/
/*									*/
/* 	pwr_BindIoUserClasses(User) = {					*/
/* 	  pwr_BindIoUserClass($Node),					*/
/* 	  pwr_BindIoUserClass(Co_PI24BO),				*/
/* 	  pwr_NullClass							*/
/* 	};								*/

pwr_dExport pwr_BindIoUserClasses(User) = {
  pwr_NullClass
};


