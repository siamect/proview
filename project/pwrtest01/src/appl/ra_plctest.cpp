/*
  Print test status
*/

#include <stdio.h>
#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "co_tst_log.h"

static char item[][80] = {
  "Test01c-Plc-Logic-And",
  "Test01c-Plc-Logic-Or",
  "Test01c-Plc-Logic-XOr",
  "Test01c-Plc-Logic-Inv",
  "Test01c-Plc-Logic-True",
  "Test01c-Plc-Logic-False",
  "Test01c-Plc-Logic-FirstScan",
  "Test01c-Plc-Logic-Edge",
  "Test01c-Plc-Logic-Count",
  "Test01c-Plc-Logic-Wait",
  "Test01c-Plc-Logic-Waith",
  "Test01c-Plc-Logic-Pulse",
  "Test01c-Plc-Logic-Timer",
  "Test01c-Plc-Logic-SR_S",
  "Test01c-Plc-Logic-SR_R",
  "Test01c-Plc-Analog-Add",
  "Test01c-Plc-Analog-Sum",
  "Test01c-Plc-Analog-Minmax",
  "Test01c-Plc-Analog-Max",
  "Test01c-Plc-Analog-Min",
  "Test01c-Plc-Analog-Mul",
  "Test01c-Plc-Analog-Div",
  "Test01c-Plc-Analog-Sub",
  "Test01c-Plc-Analog-Mux",
  "Test01c-Plc-Analog-Demux",
  "Test01c-Plc-Analog-Select",
  "Test01c-Plc-Analog-Limit",
  "Test01c-Plc-Analog-Comph",
  "Test01c-Plc-Analog-Compl",
  "Test01c-Plc-Analog-ScanTime",
  "Test01c-Plc-Analog-GreaterThan",
  "Test01c-Plc-Analog-GreaterEqual",
  "Test01c-Plc-Analog-LessThan",
  "Test01c-Plc-Analog-LessEqual",
  "Test01c-Plc-Analog-Equal",
  "Test01c-Plc-Analog-NotEqual",
  "Test01c-Plc-Analog-DArithm",
  "Test01c-Plc-Analog-AArithm",
  "Test01c-Plc-Analog-CArithm",
  "Test01c-Plc-Analog-DataArithm",
  "Test01c-Plc-Analog-Curve",
  "Test01c-Plc-Analog-Adelay",
  "Test01c-Plc-Analog-Filter",
  "Test01c-Plc-Analog-Ramp",
  "Test01c-Plc-Analog-Random",
  "Test01c-Plc-Analog-Timint",
  "Test01c-Plc-Analog-TimeMean",
  "Test01c-Plc-Analog-Speed",
  "Test01c-Plc-AnalogMath-ACos",
  "Test01c-Plc-AnalogMath-ASin",
  "Test01c-Plc-AnalogMath-ATan",
  "Test01c-Plc-AnalogMath-Cos",
  "Test01c-Plc-AnalogMath-Sin",
  "Test01c-Plc-AnalogMath-Tan",
  "Test01c-Plc-AnalogMath-Exp",
  "Test01c-Plc-AnalogMath-Ln",
  "Test01c-Plc-AnalogMath-Log",
  "Test01c-Plc-AnalogMath-Mod",
  "Test01c-Plc-AnalogMath-Sqrt",
  "Test01c-Plc-AnalogMath-Abs",
  "Test01c-Plc-Integer-IAdd",
  "Test01c-Plc-Integer-IMax",
  "Test01c-Plc-Integer-IMin",
  "Test01c-Plc-Integer-IMul",
  "Test01c-Plc-Integer-IMux",
  "Test01c-Plc-Integer-IDemux",
  "Test01c-Plc-Integer-IDiv",
  "Test01c-Plc-Integer-ISub",
  "Test01c-Plc-Integer-ISel",
  "Test01c-Plc-Integer-IGreaterThan",
  "Test01c-Plc-Integer-IGreaterEqual",
  "Test01c-Plc-Integer-ILessThan",
  "Test01c-Plc-Integer-ILessEqual",
  "Test01c-Plc-Integer-IEqual",
  "Test01c-Plc-Integer-INotEqual",
  "Test01c-Plc-Integer-Even",
  "Test01c-Plc-Integer-Odd",
  "Test01c-Plc-Integer-IAbs",
  "Test01c-Plc-Integer-ILimit",
  "Test01c-Plc-Integer-BwAnd",
  "Test01c-Plc-Integer-BwOr",
  "Test01c-Plc-Integer-BwInv",
  "Test01c-Plc-Integer-BwRotateLeft",
  "Test01c-Plc-Integer-BwRotateRight",
  "Test01c-Plc-Integer-BwShiftLeft",
  "Test01c-Plc-Integer-BwShiftRight",
  "Test01c-Plc-Grafcet-InitStep",
  "Test01c-Plc-Grafcet-Step",
  "Test01c-Plc-Grafcet-Trans",
  "Test01c-Plc-Grafcet-SetCond",
  "Test01c-Plc-Grafcet-Order",
  "Test01c-Plc-Grafcet-SubStep",
  "Test01c-Plc-Grafcet-OrderAct",
  "Test01c-Plc-Grafcet-TransDiv",
  "Test01c-Plc-Grafcet-StepDiv",
  "Test01c-Plc-Other-CSub",
  "Test01c-Plc-Other-CArea",
  "Test01c-Plc-Other-CLoop"
};

int main()
{
  pwr_tStatus sts;
  unsigned int i;
  pwr_tAName aname;

  tst_log *log = new tst_log(&sts, "rt-Plc", "$pwrp_log/plc.tlog");
  if (EVEN(sts))
    printf("** Unable to open log file");

  sts = gdh_Init("ra_test");
  if (EVEN(sts)) {
    printf("E pwrtest01 status, gdh_Init error status: %u\n", sts);
    exit(0);
  }
  
  for (i = 0; i < sizeof(item)/sizeof(item[0]); i++) {
    pwr_tBoolean val;
    pwr_tInt32 state;

    strcpy(aname, item[i]);
    strcat(aname, "-TestExecuted.ActualValue");
    sts = gdh_GetObjectInfo(aname, &val, sizeof(val));
    if (EVEN(sts)) {
      log->log('E', "Can't find test object", item[i], sts);
      continue;
    }
    if (val != 1) {
      log->log('E', "Not tested", item[i]);
      continue;
    }

    strcpy(aname, item[i]);
    strcat(aname, "-ErrorDetected.ActualValue");
    sts = gdh_GetObjectInfo(aname, &val, sizeof(val));
    if (EVEN(sts)) {
      log->log('E', "gdh_GetObjectInfo", item[i], sts);
      continue;
    }
    if (val == 1) {
      char state_str[80];

      strcpy(aname, item[i]);
      strcat(aname, "-ErrorState.ActualValue");
      sts = gdh_GetObjectInfo(aname, &state, sizeof(state));
      if (EVEN(sts)) {
	log->log('E', "State not found", item[i], sts);
	state = -1;
      }
      sprintf(state_str, "error state %u", state);
      log->log('E', item[i], state_str);
    }
    else
      log->log('I', "Test ok", item[i]);
  }
}
