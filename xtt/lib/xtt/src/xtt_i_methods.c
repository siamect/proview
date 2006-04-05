/* 
 * Proview   $Id: xtt_i_methods.c,v 1.1 2006-04-05 08:38:37 claes Exp $
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

#include "xtt_menu.h"

pwr_dImport pwr_BindXttMethods($Object);
pwr_dImport pwr_BindXttMethods(PlcPgm);
pwr_dImport pwr_BindXttMethods(Sound);

pwr_dExport pwr_BindXttClasses(Base) = {
  pwr_BindXttClass($Object),
  pwr_BindXttClass(PlcPgm),
  pwr_BindXttClass(Sound),
  pwr_NullClass
};

