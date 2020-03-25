/*
  Print current project status
*/

#include <stdio.h>
#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"

int main()
{
  pwr_tStatus sts, system_sts;
  pwr_tOid noid;
  pwr_tOName name;

  sts = gdh_Init("ra_status");
  if (EVEN(sts)) {
    printf("E pwrtest01 status, gdh_Init error status: %u\n", sts);
    exit(0);
  }
  
  sts = gdh_GetNodeObject(0, &noid);  
  if (ODD(sts))
    sts = gdh_ObjidToName(noid, name, sizeof(name), cdh_mName_volumeStrict);
  if (ODD(sts)) {
    strcat(name, ".SystemStatus");
    sts = gdh_GetObjectInfo(name, &system_sts, sizeof(system_sts));
  }
  if (EVEN(sts)) {
    printf("E pwrtest01 status, error status: %u\n", sts);
    exit(0);
  }

  if (ODD(system_sts))
    printf("I pwrtest01 status success: %u\n", system_sts);
  else
    printf("E pwrtest01 status error: %u\n", system_sts);
}
