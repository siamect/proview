/* wb_i_system_methods.c -- work bench system methods.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "wb_pwrs.h"

pwr_dImport pwr_BindMethods($AttrXRef);
pwr_dImport pwr_BindMethods($Node);
pwr_dImport pwr_BindMethods($Object);
pwr_dImport pwr_BindMethods($ObjXRef);
pwr_dImport pwr_BindMethods($RootVolume);
pwr_dImport pwr_BindMethods($SubVolume);

pwr_dExport pwr_BindClasses(System) = {
  pwr_BindClass($AttrXRef),
  pwr_BindClass($Node),
  pwr_BindClass($Object),
  pwr_BindClass($ObjXRef),
  pwr_BindClass($RootVolume),
  pwr_BindClass($SubVolume),
  pwr_NullClass
};
