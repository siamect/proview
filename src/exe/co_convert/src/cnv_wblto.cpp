#include <iostream.h>
#include <fstream.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "cnv_wblto.h"

int CnvWblTo::init( char *) 
{ 
  return 1;
}
