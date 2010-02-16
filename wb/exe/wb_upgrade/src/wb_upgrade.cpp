/* 
 * Proview   $Id: wb_upgrade.c,v 1.1 2006-01-12 13:48:59 claes Exp $
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
 */

/* wb_drive_convert.c -- converts plc objects
   Program to convert plc objects from v2.0 to v2.1.

   The program should be executed after loading objects in the
   workbench of v2.1.

   Converts the grapic mask for objects with new attributes.

   In this version the following classes are changed:
      BaseClasses-Drive  */

#include <stdlib.h>
#include <iostream>
#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "wb_env.h"
#include "wb_volume.h"
#include "wb_session.h"
#include "wb_object.h"
#include "pwr_baseclasses.h"

typedef struct {
  pwr_tOName cname;
  pwr_tObjName attr[10];
} upg_sCnvAlarm;

static upg_sCnvAlarm upg_alatext[] = {
  {"BaseComponent:Class-CompModeA", {"AlarmNotReadyM", "AlarmNotReadyA", "Alarm1LocalMod", "Alarm2LocalMod", "AlarmNotManOrd", "", "", "", "", ""}},
  {"BaseComponent:Class-CompModeAM", {"AlarmLocalMode", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-CompModeD", {"AlarmNotReadyM", "AlarmNotReadyA", "Alarm1LocalMod", "Alarm2LocalMod", "AlarmNotManOrd", "", "", "", "", ""}},
  {"BaseComponent:Class-CompModeD2", {"AlarmNotReadyM", "AlarmNotReadyA", "Alarm1LocalMod", "Alarm2LocalMod", "AlarmNotManOrd", "AlarmOrderTimeOut", "", "", "", ""}},
  {"BaseComponent:Class-CompLimit", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseSupSwitch", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseTempSwitch", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseCircuitBreaker", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseActuatorA", {"DiffAlarmText", "SwitchAlarmText", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseActuatorIncrDecr", {"SwitchAlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseContactor", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseLevelSwitch", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BasePressureSwitch", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFilter", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseMValve", {"SwitchAlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseMDamper", {"SwitchAlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseM3WayValve", {"SwitchAlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseOverloadRelay", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseSafetySwitch", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO5", {"AlarmTripped", "AlarmWarning", "AlarmError", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO3", {"AlarmTripped", "AlarmWarning", "AlarmError", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO5MotorAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO5FanAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO5PumpAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO3MotorAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO3FanAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"BaseComponent:Class-BaseFcPPO3PumpAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_CircuitBreaker_MS116", {"AlarmText", "", "", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_ACS800_1", {"AlarmLocalMod", "AlarmTripped", "AlarmFanOld", "AlarmWarning", "AlarmError", "", "", "", "", ""}},
  {"ABB:Class-ABB_ACC800", {"AlarmLocalMod", "AlarmWarning", "AlarmError", "AlarmTorqFault", "AlarmBrakeFault", "AlarmTorqProvFault", "", "", "", ""}},
  {"ABB:Class-ABB_ACS800MotorAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_ACS800PumpAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_ACS800FanAggr", {"AlarmTextToOften", "AlarmTextPowOnNotOk", "AlarmTextStartNoOk", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_Converter_ACS800_PPO3", {"AlarmTripped", "AlarmWarning", "AlarmError", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_Sensor_Pb_PA", {"AlarmTextTransmError", "AlarmTextTransmWarning", "", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_TempSensor_TF12", {"AlarmTextTransmError", "AlarmTextTransmWarning", "", "", "", "", "", "", "", ""}},
  {"ABB:Class-ABB_PressureSensor_265GS", {"AlarmTextTransmError", "AlarmTextTransmWarning", "", "", "", "", "", "", "", ""}},
   {"ABB:Class-ABB_DiffPressureSensor_265DS", {"AlarmTextTransmError", "AlarmTextTransmWarning", "", "", "", "", "", "", "", ""}},
   {"ABB:Class-ABB_FlowSensor_FXE4000", {"AlarmTextTransmError", "AlarmTextTransmWarning", "", "", "", "", "", "", "", ""}}};



static void usage()
{
  printf( "\nUsage: wb_upgrade -v 'volumename'\n\n");
}

#if 0
static void process( wb_session& session, wb_object& o)
{
  pwr_sPlcNode plcnode;

  if ( o.cid() == pwr_cClass_csub) {
    printf ( "Object: %s\n", o.longName().c_str());

    wb_attribute a = session.attribute( o.oid(), "DevBody", "PlcNode");
    if ( !a) exit(0);

    a.value( &plcnode);

    printf( "subwindow:  %d, woid: %u,%u\n", plcnode.subwindow, 
	    plcnode.subwind_oid[0].vid, plcnode.subwind_oid[0].oix);

    wb_object c = o.first();
    if ( c && c.cid() == pwr_cClass_windowplc) {
      plcnode.subwind_oid[0].vid = c.oid().vid;
      plcnode.subwind_oid[0].oix = c.oid().oix;

      printf( "subwindow:  %d, woid: %u,%u\n", plcnode.subwindow, 
	      plcnode.subwind_oid[0].vid, plcnode.subwind_oid[0].oix);

      session.writeAttribute( a, &plcnode, sizeof(plcnode)); 
      if ( !a) cout << "** Write error" << endl;
    }
  }

  for ( wb_object c = o.first(); c.oddSts(); c = c.after()) {
    process( session, c);
  }
}
#endif

int main( int argc, char *argv[])
{
  pwr_tObjName volume;

  for ( int i = 1; i < argc; i++) {
    if ( argv[i][0] == '-') {
      switch ( argv[i][1]) {
      case 'h':
	usage();
	exit(0);
      case 'v':
	if ( i+1 >= argc) {
	  usage();
	  exit(0);
	}
	strncpy( volume, argv[i+1], sizeof(volume));
	i++;
	break;
      default:
	printf("Unknown argument: %s\n", argv[i]);
	usage();
	exit(0);
      }
    }
  }

  wb_env env;
  env.load( volume);

  wb_volume vol;
  for ( vol = env.volume(); vol.oddSts(); vol.next()) {
    if ( vol.cid() == pwr_eClass_RootVolume ||
	 vol.cid() == pwr_eClass_SubVolume ||
	 vol.cid() == pwr_eClass_SharedVolume) {
      break;
    }
  }
  if ( vol.evenSts()) exit( vol.sts());

  wb_session session(vol);
  if ( ! session.access( ldh_eAccess_ReadWrite)) {
    cout << "** No write access" << endl;
    exit(0);
  }

#if 0
  wb_object o;
  for ( o = session.object(); o.oddSts(); o = o.after()) {
    process( session, o);
  }
#endif


  // Replace alarmtexts with tailing ", "
  pwr_tAttrRef aref;
  pwr_tString80 text;
  pwr_tCid cid;
  
  for ( unsigned int i = 0; i < sizeof(upg_alatext)/sizeof(upg_alatext[0]); i++) {
    wb_object co = session.object( upg_alatext[i].cname);
    if ( !co) {
      printf( "** Class not loaded: %s\n", upg_alatext[i].cname);
      continue;
    }
    
    cid = cdh_ClassObjidToId( co.oid());

    for ( session.aref( cid, &aref);
	  session.oddSts();
	  session.nextAref( cid, &aref, &aref)) {

      for ( unsigned int j = 0; j < sizeof(upg_alatext[0].attr)/sizeof(upg_alatext[0].attr[0]); j++) {
	if ( strcmp( upg_alatext[i].attr[j], "") == 0)
	  break;

	try {
	  wb_attribute a = session.attribute(&aref);
	  if (!a) continue;

	  wb_attribute a_attr( a, 0, upg_alatext[i].attr[j]);
	  if ( a_attr.evenSts())
	    continue;
	  
	  a_attr.value( text);
	  if ( a_attr.evenSts())
	    continue;

	  bool mod = false;
	  if ( text[strlen(text)-2] == ',' && text[strlen(text)-1] == ' ') {
	    text[strlen(text)-2] = 0;
	    mod = true;
	  }
	  else if ( text[strlen(text)-3] == ',' && text[strlen(text)-2] == ' '&& text[strlen(text)-1] == ' ') {
	    text[strlen(text)-3] = 0;
	    mod = true;
	  }

	  if ( mod) {
	    session.writeAttribute( a_attr, text, sizeof( text));
	    printf( "Updating text \"%s\"\n", text);
	  }
	}
	catch ( wb_error& e) {
	  continue;
	}
      }      
    }
  }

  session.commit();
}

