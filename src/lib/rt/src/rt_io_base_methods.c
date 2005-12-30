
#include "rt_io_base.h"

pwr_dImport pwr_BindIoMethods(Node);
pwr_dImport pwr_BindIoMethods(Pb_Profiboard);
pwr_dImport pwr_BindIoMethods(Pb_DP_Slave);
pwr_dImport pwr_BindIoMethods(Pb_Module);
pwr_dImport pwr_BindIoMethods(Pb_Di);
pwr_dImport pwr_BindIoMethods(Pb_Do);
pwr_dImport pwr_BindIoMethods(Pb_Ai);
pwr_dImport pwr_BindIoMethods(Pb_Ao);
pwr_dImport pwr_BindIoMethods(Pb_Ii);
pwr_dImport pwr_BindIoMethods(Pb_Io);

pwr_BindIoClasses(Base) = {
  pwr_BindIoClass(Node),
  pwr_BindIoClass(Pb_Profiboard),
  pwr_BindIoClass(Pb_DP_Slave),
  pwr_BindIoClass(Pb_Module),
  pwr_BindIoClass(Pb_Di),
  pwr_BindIoClass(Pb_Do),
  pwr_BindIoClass(Pb_Ai),
  pwr_BindIoClass(Pb_Ao),
  pwr_BindIoClass(Pb_Ii),
  pwr_BindIoClass(Pb_Io),
  pwr_NullClass
};
