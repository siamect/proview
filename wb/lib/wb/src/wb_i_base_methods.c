/* wb_i_base_methods.c -- work bench base class methods.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "wb_pwrs.h"

pwr_dImport pwr_BindMethods(ASup);
pwr_dImport pwr_BindMethods(Ai_HVAI32);
pwr_dImport pwr_BindMethods(Ai_AI32uP);
pwr_dImport pwr_BindMethods(Ao_HVAO4);
pwr_dImport pwr_BindMethods(Ao_AO8uP);
pwr_dImport pwr_BindMethods(Backup);
pwr_dImport pwr_BindMethods(Co_PI24BO);
pwr_dImport pwr_BindMethods(Co_CO4uP);
pwr_dImport pwr_BindMethods(Di_DIX2);
pwr_dImport pwr_BindMethods(Do_HVDO32);
pwr_dImport pwr_BindMethods(DsFast);
pwr_dImport pwr_BindMethods(DsHist);
pwr_dImport pwr_BindMethods(DsTrend);
pwr_dImport pwr_BindMethods(DSup);
pwr_dImport pwr_BindMethods(PlcPgm);
pwr_dImport pwr_BindMethods(DbConfig);
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
pwr_dImport pwr_BindMethods(RttConfig);
pwr_dImport pwr_BindMethods(WebHandler);
pwr_dImport pwr_BindMethods(PlcProcess);
pwr_dImport pwr_BindMethods(PlcThread);
pwr_dImport pwr_BindMethods(NodeConfig);
pwr_dImport pwr_BindMethods(Pb_DP_Slave);

pwr_dExport pwr_BindClasses(Base) = {
  pwr_BindClass(ASup),
  pwr_BindClass(Ai_HVAI32),
  pwr_BindClass(Ai_AI32uP),
  pwr_BindClass(Ao_HVAO4),
  pwr_BindClass(Ao_AO8uP),
  pwr_BindClass(Backup),
  pwr_BindClass(Co_PI24BO),
  pwr_BindClass(Co_CO4uP),
  pwr_BindClass(Di_DIX2),
  pwr_BindClass(Do_HVDO32),
  pwr_BindClass(DsFast),
  pwr_BindClass(DsHist),
  pwr_BindClass(DsTrend),
  pwr_BindClass(DSup),
  pwr_BindClass(PlcPgm),
  pwr_BindClass(DbConfig),
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
  pwr_BindClass(RttConfig),
  pwr_BindClass(WebHandler),
  pwr_BindClass(PlcProcess),
  pwr_BindClass(PlcThread),
  pwr_BindClass(NodeConfig),
  pwr_BindClass(Pb_DP_Slave),
  pwr_NullClass
};

















