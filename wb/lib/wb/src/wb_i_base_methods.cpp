/* 
 * Proview   $Id: wb_i_base_methods.cpp,v 1.4 2008-10-15 06:04:55 claes Exp $
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

/* wb_i_base_methods.c -- work bench base class methods. */

#include "wb_pwrs.h"

pwr_dImport pwr_BindMethods(ASup);
pwr_dImport pwr_BindMethods(DsFast);
pwr_dImport pwr_BindMethods(SevHist);
pwr_dImport pwr_BindMethods(DsTrend);
pwr_dImport pwr_BindMethods(DSup);
pwr_dImport pwr_BindMethods(PlcPgm);
pwr_dImport pwr_BindMethods(ProjectReg);
pwr_dImport pwr_BindMethods(RootVolumeConfig);
pwr_dImport pwr_BindMethods(SubVolumeConfig);
pwr_dImport pwr_BindMethods(SharedVolumeConfig);
pwr_dImport pwr_BindMethods(ClassVolumeConfig);
pwr_dImport pwr_BindMethods(ChanDi);
pwr_dImport pwr_BindMethods(ChanDo);
pwr_dImport pwr_BindMethods(ChanAi);
pwr_dImport pwr_BindMethods(ChanAit);
pwr_dImport pwr_BindMethods(ChanAo);
pwr_dImport pwr_BindMethods(ChanCo);
pwr_dImport pwr_BindMethods(ChanIi);
pwr_dImport pwr_BindMethods(ChanIo);
pwr_dImport pwr_BindMethods(Di);
pwr_dImport pwr_BindMethods(Do);
pwr_dImport pwr_BindMethods(Ai);
pwr_dImport pwr_BindMethods(Ao);
pwr_dImport pwr_BindMethods(Co);
pwr_dImport pwr_BindMethods(Po);
pwr_dImport pwr_BindMethods(Ii);
pwr_dImport pwr_BindMethods(Io);
pwr_dImport pwr_BindMethods(XttGraph);
pwr_dImport pwr_BindMethods(WebGraph);
pwr_dImport pwr_BindMethods(OpPlace);
pwr_dImport pwr_BindMethods(WebHandler);
pwr_dImport pwr_BindMethods(PlcProcess);
pwr_dImport pwr_BindMethods(PlcThread);
pwr_dImport pwr_BindMethods(NodeConfig);
pwr_dImport pwr_BindMethods(SevNodeConfig);
pwr_dImport pwr_BindMethods(Application);

pwr_dExport pwr_BindClasses(Base) = {
  pwr_BindClass(ASup),
  pwr_BindClass(DsFast),
  pwr_BindClass(SevHist),
  pwr_BindClass(DsTrend),
  pwr_BindClass(DSup),
  pwr_BindClass(PlcPgm),
  pwr_BindClass(ProjectReg),
  pwr_BindClass(RootVolumeConfig),
  pwr_BindClass(SubVolumeConfig),
  pwr_BindClass(SharedVolumeConfig),
  pwr_BindClass(ClassVolumeConfig),
  pwr_BindClass(ChanDi),
  pwr_BindClass(ChanDo),
  pwr_BindClass(ChanAi),
  pwr_BindClass(ChanAit),
  pwr_BindClass(ChanAo),
  pwr_BindClass(ChanCo),
  pwr_BindClass(ChanIi),
  pwr_BindClass(ChanIo),
  pwr_BindClass(Di),
  pwr_BindClass(Do),
  pwr_BindClass(Ai),
  pwr_BindClass(Ao),
  pwr_BindClass(Co),
  pwr_BindClass(Po),
  pwr_BindClass(Ii),
  pwr_BindClass(Io),
  pwr_BindClass(XttGraph),
  pwr_BindClass(WebGraph),
  pwr_BindClass(OpPlace),
  pwr_BindClass(WebHandler),
  pwr_BindClass(PlcProcess),
  pwr_BindClass(PlcThread),
  pwr_BindClass(NodeConfig),
  pwr_BindClass(SevNodeConfig),
  pwr_BindClass(Application),
  pwr_NullClass
};

















