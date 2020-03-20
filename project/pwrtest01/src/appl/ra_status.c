/*
  Print current project status
*/

#include <stdio.h>
#include "pwr.h"
#include "rt_gdh.h"

int main()
{
  pwr_tStatus sts, system_sts;

  sts = gdh_Init("ra_status");
  if (EVEN(sts)) {
    printf("E pwrtest01 status, gdh_Init error status: %u\n", sts);
    exit(0);
  }
  
  sts = gdh_GetObjectInfo("Nodes-PwrTest01.SystemStatus", &system_sts,
			  sizeof(system_sts));
  if (EVEN(sts)) {
    printf("E pwrtest01 status, gdh_GetObjectInfo error status: %u\n", sts);
    exit(0);
  }

  if (ODD(system_sts))
    printf("I pwrtest01 status success: %u\n", system_sts);
  else
    printf("E pwrtest01 status error: %u\n", system_sts);
}
