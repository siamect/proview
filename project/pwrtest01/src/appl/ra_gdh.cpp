/*
  Test gdh functions.
*/

#include <stdio.h>
#include "ra_gdh_test.h"

void GetRootList()
{
}


int main()
{
  pwr_tStatus sts, system_sts;
  pwr_tOid noid;
  pwr_tOName name;

  sts = gdh_Init("ra_gdh");
  if (EVEN(sts)) {
    printf("E pwrtest01 status, gdh_Init error status: %u\n", sts);
    exit(0);
  }
  
  GetRootList();
}
