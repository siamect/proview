/* 
 * Proview   $Id: wb_i_system_methods.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_i_system_methods.c -- work bench system methods. */

#include "wb_pwrs.h"

pwr_dImport pwr_BindMethods($AttrXRef);
pwr_dImport pwr_BindMethods($Node);
pwr_dImport pwr_BindMethods($Object);
pwr_dImport pwr_BindMethods($ObjXRef);
pwr_dImport pwr_BindMethods($Objid);
pwr_dImport pwr_BindMethods($RootVolume);
pwr_dImport pwr_BindMethods($SubVolume);
pwr_dImport pwr_BindMethods($ClassDef);
pwr_dImport pwr_BindMethods($ClassHier);
pwr_dImport pwr_BindMethods($Attribute);
pwr_dImport pwr_BindMethods($ObjBodyDef);
pwr_dImport pwr_BindMethods($AttrRef);
pwr_dImport pwr_BindMethods($PlantHier);
pwr_dImport pwr_BindMethods($NodeHier);

pwr_dExport pwr_BindClasses(System) = {
  pwr_BindClass($AttrXRef),
  pwr_BindClass($Node),
  pwr_BindClass($Object),
  pwr_BindClass($ObjXRef),
  pwr_BindClass($Objid),
  pwr_BindClass($RootVolume),
  pwr_BindClass($SubVolume),
  pwr_BindClass($ClassDef),
  pwr_BindClass($ClassHier),
  pwr_BindClass($Attribute),
  pwr_BindClass($ObjBodyDef),
  pwr_BindClass($AttrRef),
  pwr_BindClass($PlantHier),
  pwr_BindClass($NodeHier),
  pwr_NullClass
};
