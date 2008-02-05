/* 
 * Proview   $Id: rt_pb_gsd.cpp,v 1.12 2008-02-05 08:14:59 claes Exp $
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

/* pb_gsd.cpp -- Parse gsd file */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwr_baseclasses.h"
#include "rt_pb_msg.h"
#include "rt_pb_gsd.h"
#include "co_dcli.h"
#include "co_cdh.h"

typedef enum {
  gsd_M,
  gsd_O,
  gsd_D,
  gsd_G,
  gsd_MD,		// Mandatory with depencency
  gsd_OD		// Optional with depencency
} gsd_eOpt;

typedef enum {
  gsd_mSts_Profibus_DP 	= 1 << 0,
  gsd_mSts_Module 	= 1 << 1,
  gsd_mSts_Area 	= 1 << 2,
  gsd_mSts_PrmText 	= 1 << 3,
  gsd_mSts_ExtUserPrmData = 1 << 4
} gsd_mSts;

gsd_sKeyword pb_gsd::keywordlist[] = {
  // General
  {"#Profibus_DP",	"", 		gsd_M, 	gsd_ProfibusDP,  0},
  {"GSD_Revision",	"", 		gsd_M, 	gsd_VString32,  0},
  {"Vendor_Name",	"", 		gsd_M, 	gsd_VString32,  0},
  {"Model_Name",	"", 		gsd_M, 	gsd_VString32,  0},
  {"Revision",		"", 	       	gsd_M, 	gsd_VString32,  0},
  {"Revision_Number",	"", 		gsd_O, 	gsd_Unsigned8,  0},
  {"Ident_Number",	"", 		gsd_M, 	gsd_Unsigned16, 0},
  {"Protocol_Ident",	"", 		gsd_M, 	gsd_Unsigned8, 0},
  {"Station_Type",	"", 		gsd_M, 	gsd_Unsigned8, 0},
  {"FMS_supp",		"", 		gsd_D, 	gsd_Boolean, 0},
  {"Hardware_Release",	"", 		gsd_M, 	gsd_VString32, 0},
  {"Software_Release",	"", 		gsd_M, 	gsd_VString32, 0},
  {"9.6_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"19.2_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"31.25_supp",	"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"45.45_supp",       	"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"93.75_supp",       	"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"187.5_supp",       	"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"500_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"1.5M_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"3M_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"6M_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"12M_supp",		"supp",        	gsd_G, 	gsd_Boolean, 0},
  {"MaxTsdr_9.6",      	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_19.2",     	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_31.25",     "MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_45.45",   	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_93.75",   	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_187.5",   	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_500",      	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_1.5M",    	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_3M",      	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_6M",      	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"MaxTsdr_12M",      	"MaxTsdr",      gsd_G, 	gsd_Unsigned16, 0},
  {"Redundancy",      	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Repeater_Ctrl_Sig", "",      	gsd_D, 	gsd_Boolean, 0},
  {"24V_Pins",      	"",      	gsd_D, 	gsd_Unsigned8, 0},
  {"Implementation_Type", "",      	gsd_O, 	gsd_VString32, 0},
  {"Bitmap_Device",   	"",      	gsd_O, 	gsd_VString8, 0},
  {"Bitmap_Diag",      	"",      	gsd_O, 	gsd_VString8, 0},
  {"Bitmap_SF",      	"",      	gsd_O, 	gsd_VString8, 0},
  {"Physical_Interface", "",      	gsd_O, 	gsd_Unsigned8, 0},
  {"Transmission_Delay_9.6", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_19.2", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_31.25", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_45.45", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_93.75", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_187.5", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_500", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_1.5M", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_3M", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_6M", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Transmission_Delay_12M", "Transmission_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_9.6", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_19.2", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_31.25", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_45.45", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_93.75", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_187.5", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_500", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_1.5M", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_3M", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_6M", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  {"Reaction_Delay_12M", "Reaction_Delay", gsd_G, gsd_Unsigned16, 0},
  // Master
  {"Master_Freeze_Mode_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Master_Sync_Mode_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Master_Fail_Safe_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Download_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Upload_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Act_Para_Brct_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Act_Param_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Max_MPS_Length", 	"",      	gsd_M, 	gsd_Unsigned32, 0},
  {"Max_Lsdu_MS", 	"",      	gsd_M, 	gsd_Unsigned8, 0},
  {"Min_Poll_Timeout", 	"",      	gsd_M, 	gsd_Unsigned16, 0},
  {"Trdy_9.6", 		"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_19.2",        	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_31.25",       	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_45.45",       	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_93.75",       	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_187.5",       	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_500", 		"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_1.5M",        	"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_3M", 		"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_6M", 		"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Trdy_12M", 		"Trdy",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_9.6", 		"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_19.2",        	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_31.25",       	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_45.45",       	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_93.75",       	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_187.5",       	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_500", 		"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_1.5M",        	"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_3M", 		"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_6M", 		"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tqui_12M", 		"Tqui",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_9.6", 		"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_19.2",        	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_31.25",       	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_45.45",       	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_93.75",       	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_187.5",       	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_500", 		"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_1.5M",        	"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_3M", 		"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_6M", 		"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"Tset_12M", 		"Tset",      	gsd_G, 	gsd_Unsigned8, 0},
  {"LAS_Len", 		"",      	gsd_M, 	gsd_Unsigned8, 0},
  {"Tsdi_9.6", 		"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_19.2",        	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_31.25",       	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_45.45",       	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_93.75",       	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_187.5",       	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_500", 		"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_1.5M",        	"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_3M", 		"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_6M", 		"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Tsdi_12M", 		"Tsdi",      	gsd_G, 	gsd_Unsigned16, 0},
  {"Max_Slaves_supp", 	"",      	gsd_M, 	gsd_Unsigned8, 0},
  {"Max_Master_Input_Len", "",      	gsd_O, 	gsd_Unsigned8, 0},
  {"Max_Master_Output_Len", "",      	gsd_O, 	gsd_Unsigned8, 0},
  {"Max_Master_Data_Len", "",      	gsd_O, 	gsd_Unsigned16, 0},
  // Master DP extensions
  {"DPV1_Master", 	"",   		gsd_D,  gsd_Boolean, 0},
  {"DPV1_Conformance_Class", "", 	gsd_O,	gsd_Unsigned8, 0},
  {"C1_Master_Read_Write_supp", "",   	gsd_D, 	gsd_Boolean, 0},
  {"Master_DPV1_Alarm_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Master_Diagnostic_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Process_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Pull_Plug_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Status_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Update_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Manufacturer_Alarm_supp", "Master_DPV1_Alarm_Supp", gsd_G, gsd_Boolean, 0},
  {"Master_Extra_Alarm_SAP_supp", "Master_DPV1_Alarm_Supp", 	gsd_MD, gsd_Boolean, 0},
  {"Master_Alarm_Sequence_Mode", "Master_DPV1_Alarm_Supp", 	gsd_MD, gsd_Unsigned8, 0},
  {"Master_Alarm_Type_Mode_supp", "Master_DPV1_Alarm_Supp", 	gsd_MD, gsd_Boolean, 0},
  // Master DP-V2
  {"Isochron_Mode_Synchronised", "",    gsd_D, 	gsd_Unsigned8, 0},
  {"DXB_Master_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"X_Master_Prm_SAP_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  // Slave
  {"Freeze_Mode_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Sync_Mode_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Auto_Baud_supp", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Set_Slave_Add_supp", "",      	gsd_D, 	gsd_Boolean, 0},
  {"User_Prm_Data_Len", "",      	gsd_D, 	gsd_UserPrmDataLen, 0},
  {"User_Prm_Data", 	"",      	gsd_D, 	gsd_UserPrmData, 0},
  {"Min_Slave_Intervall", "",      	gsd_M, 	gsd_Unsigned16, 0},
  {"Modular_Station", 	"",      	gsd_D, 	gsd_Boolean, 0},
  {"Max_Module", 	"Modular_Station", gsd_MD, gsd_Unsigned8, 0},
  {"Max_Input_Len", 	"Modular_Station", gsd_MD, gsd_Unsigned8, 0},
  {"Max_Output_Len", 	"Modular_Station", gsd_MD, gsd_Unsigned8, 0},
  {"Max_Data_Len", 	"Modular_Station", gsd_OD, gsd_Unsigned16, 0},
  {"Unit_Diag_Bit",  	"",      	gsd_O, 	gsd_BitVString32, 0},
  {"X_Unit_Diag_Bit",  	"",      	gsd_O, 	gsd_BitVString32, 0},
  {"Unit_Diag_Bit_Help", "",      	gsd_O, 	gsd_BitVString256, 0},
  {"X_Unit_Diag_Bit_Help", "",      	gsd_O, 	gsd_BitVString256, 0},
  {"Unit_Diag_Not_Bit", "",      	gsd_O, 	gsd_BitVString32, 0},
  {"X_Unit_Diag_Not_Bit", "",      	gsd_O, 	gsd_BitVString32, 0},
  {"Unit_Diag_Not_Bit_Help", "",    	gsd_O, 	gsd_BitVString256, 0},
  {"X_Unit_Diag_Not_Bit_Help", "",    	gsd_O, 	gsd_BitVString256, 0},
  {"Unit_Diag_Area", 	"",   		gsd_O, 	gsd_DiagArea, 0},
  {"Unit_Diag_Area_End", "",   		gsd_O, 	gsd_DiagAreaEnd, 0},
  {"X_Unit_Diag_Area", 	"",   		gsd_O, 	gsd_DiagArea, 0},
  {"Unit_Diag_Type", 	"",      	gsd_O, 	gsd_DiagType, 0},
  {"Value", 		"Unit_Diag_Area", gsd_O, gsd_Value, 0},
  {"Module", 		"",            	gsd_O, 	gsd_Module, 0},
  {"EndModule", 	"",      	gsd_O, 	gsd_EndModule, 0},
  {"Module_Reference", 	"Module",      	gsd_O, 	gsd_Unsigned16, 0},
  {"Ext_Module_Prm_Data_Len", "Module", gsd_O, 	gsd_Unsigned16, 0},
  {"X_Ext_Module_Prm_Data_Len", "Module", gsd_O, gsd_Unsigned16, 0},
  {"F_Ext_Module_Prm_Data_Len", "Module", gsd_O, gsd_Unsigned16, 0},
  {"Data_Area_Beg", 	"",            	gsd_O, 	gsd_DataAreaBegin, 0},
  {"Data_Area_End", 	"",      	gsd_O, 	gsd_DataAreaEnd, 0},
  {"Related_CFG_Identifier", "Data_Area", gsd_MD, 	gsd_Unsigned8, 0},
  {"IO_Direction", 	"Data_Area",    gsd_MD, 	gsd_Boolean, 0},
  {"Length", 		"Data_Area",   	gsd_MD, 	gsd_Unsigned8, 0},
  {"Consistency", 	"Data_Area",   	gsd_MD, 	gsd_Unsigned8, 0},
  {"Publisher_allowed", "Data_Area",   	gsd_MD, 	gsd_Boolean, 0},
  {"DP_Master_allowed", "Data_Area",   	gsd_MD, 	gsd_Boolean, 0},
  {"Data_Type", 	"Data_Area",   	gsd_MD, 	gsd_Unsigned8, 0},
  {"Channel_Diag", 	"",      	gsd_O, 	gsd_BitVString32, 0},
  {"Channel_Diag_Help", "",      	gsd_O, 	gsd_BitVString256, 0},
  {"Fail_Safe", 	"",      	gsd_O, 	gsd_Boolean, 0},
  {"Max_Diag_Data_Len", "",      	gsd_M, 	gsd_Unsigned8, 0},
  {"Modul_Offset", 	"",      	gsd_D, 	gsd_Unsigned8, 0},
  {"Slave_Family", 	"",      	gsd_M, 	gsd_SlaveFamily, 0},
  {"Family_Name", 	"",      	gsd_O, 	gsd_VString32, 0},
  {"OrderNumber", 	"",      	gsd_O, 	gsd_VString32, 0},
  {"Diag_Update_Delay", "",      	gsd_D, 	gsd_Unsigned8, 0},
  {"Fail_Safe_required", "",      	gsd_D, 	gsd_Boolean, 0},
  {"Info_Text", 	"",      	gsd_O, 	gsd_VString256, 0},
  {"PrmText", 		"",      	gsd_O, 	gsd_PrmText, 0},
  {"EndPrmText",       	"",      	gsd_O, 	gsd_EndPrmText, 0},
  {"ExtUserPrmData",   	"",      	gsd_O, 	gsd_ExtUserPrmData, 0},
  {"EndExtUserPrmData", "",      	gsd_O, 	gsd_EndExtUserPrmData, 0},
  {"Prm_Text_Ref", 	"ExtUserPrmData", gsd_O, gsd_Unsigned16, 0},
  {"Changeable", 	"ExtUserPrmData", gsd_O, gsd_Boolean, 0},
  {"Visible", 		"ExtUserPrmData", gsd_O, gsd_Boolean, 0},
  {"Text", 		"",      	gsd_O, 	gsd_Text, 0},
  {"Max_User_Prm_Data_Len", "",      	gsd_M, 	gsd_MaxUserPrmDataLen, 0},
  {"Ext_User_Prm_Data_Ref", "",      	gsd_M, 	gsd_ExtUserPrmDataRef, 0},
  {"Ext_User_Prm_Data_Const", "",      	gsd_M, 	gsd_ExtUserPrmDataConst, 0},

  // Slave DP extentions
  {"DPV1_Slave", "",      	gsd_D, 	gsd_Boolean, 0},
  {"C1_Read_Write_supp", "",    gsd_D, 	gsd_Boolean, 0},
  {"C2_Read_Write_supp", "",  	gsd_D, 	gsd_Boolean, 0},
  {"C1_Max_Data_Len", "",      	gsd_D, 	gsd_Unsigned8, 0},
  {"C2_Max_Data_Len", "",      	gsd_D, 	gsd_Unsigned8, 0},
  {"C1_Response_Timeout", "",  	gsd_D, 	gsd_Unsigned16, 0},
  {"C2_Response_Timeout", "",  	gsd_D, 	gsd_Unsigned16, 0},
  {"C1_Read_Write_required", "", gsd_D,	gsd_Boolean, 0},
  {"C2_Read_Write_required", "", gsd_D, gsd_Boolean, 0},
  {"C2_Max_Count_Channels", "",	gsd_D, 	gsd_Unsigned8, 0},
  {"Max_Initiate_PDU_Length", "", gsd_D, gsd_Unsigned8, 0},
  {"Diagnostic_Alarm_supp", "", gsd_D, 	gsd_Boolean, 0},
  {"Process_Alarm_supp", "", 	gsd_D, 	gsd_Boolean, 0},
  {"Pull_Plug_Alarm_supp", "",	gsd_D, 	gsd_Boolean, 0},
  {"Status_Alarm_supp", "",     gsd_D, 	gsd_Boolean, 0},
  {"Update_Alarm_supp", "",   	gsd_D, 	gsd_Boolean, 0},
  {"Manufacturer_Specific_Alarm_supp", "", gsd_D, gsd_Boolean, 0},
  {"Extra_Alarm_SAP_supp", "", 	gsd_D, 	gsd_Boolean, 0},
  {"Alarm_Sequence_Mode_Count", "", gsd_D, gsd_Unsigned8, 0},
  {"Alarm_Type_Mode_supp", "",  gsd_D, 	gsd_Boolean, 0},
  {"Diagnostic_Alarm_required", "", gsd_D, gsd_Boolean, 0},
  {"Process_Alarm_required", "", gsd_D, gsd_Boolean, 0},
  {"Pull_Plug_Alarm_required", "", gsd_D, gsd_Boolean, 0},
  {"Status_Alarm_required", "", gsd_D, 	gsd_Boolean, 0},
  {"Update_Alarm_required", "",	gsd_D, 	gsd_Boolean, 0},
  {"Manufacturer_Specific_Alarm_required", "", gsd_D, 	gsd_Boolean, 0},
  {"DPV1_Data_Types", "",      	gsd_O, 	gsd_Boolean, 0},
  {"WB_Base_1ms_supp", "",     	gsd_D, 	gsd_Boolean, 0},
  {"Check_Cfg_Mode", "",      	gsd_D, 	gsd_Boolean, 0},

  {"","",0,gsd_End,0}
};


#if 0
// malloc check
static void t_free( void *a1) { printf("free:   %d\n", a1); free(a1);}

static void *t_calloc( int a1, int a2) { 
  void *p = calloc(a1,a2);
  printf("calloc: %d   %d   %d\n", p, (char *)p + a2*a1, a2*a1); 
  return p;
}
static void *t_malloc( int a1) { 
  void *p = malloc(a1);
  printf("malloc: %d   %d   %d\n", p, (char *)p + a1, a1); 
  return p;
}

#define calloc( a1, a2) t_calloc(a1,a2)
#define malloc( a1) t_malloc(a1)
#define free(a1) t_free(a1)
#endif


pb_gsd::pb_gsd() :
  dptype(0), modular_station(0), max_module(0),
  user_prm_data_len(0), max_user_prm_data_len(0), items_user_prm_data_len(0),
  status(0), address(0),
  datalist(0), modulelist(0), prm_textlist(0), extuserprmdatalist(0), 
  extuserprmdatareflist(0), current_module(0), current_area(0), current_prm_text(0), 
  current_extuserprmdata(0), extuserprmdataconst(0), prm_dataitems(0), module_conf(0), 
  module_conf_cnt(0), module_classlist(0), copy_buff(0), modified(0), byte_order(0)
{
  datalist = (gsd_sData *) calloc( sizeof(keywordlist)/sizeof(keywordlist[0]), sizeof(gsd_sData));
}
    
pb_gsd::~pb_gsd()
{
  if ( datalist) {
    for ( int i = 0; i < (int)(sizeof(keywordlist)/sizeof(keywordlist[0])); i++) {
      if ( datalist[i].data)
	free( datalist[i].data);
    }
    free( (char *)datalist);
  }

  gsd_sExtUserPrmDataRef *rp, *rnext;
  for ( rp = extuserprmdatareflist; rp; rp = rnext) {
    rnext = rp->next;
    free( (char *) rp);
  }

  gsd_sExtUserPrmData *dp, *dnext;
  for ( dp = extuserprmdatalist; dp; dp = dnext) {
    dnext = dp->next;
    free( (char *) dp);
  }

  gsd_sModule *mp, *mnext;
  for ( mp = modulelist; mp; mp = mnext) {
    mnext = mp->next;

    if ( mp->extuserprmdataconst)
      free( (char *) mp->extuserprmdataconst);
    
    for ( rp = mp->extuserprmdatareflist; rp; rp = rnext) {
      rnext = rp->next;
      free( (char *) rp);
    }

    if ( mp->Config)
      free( mp->Config);
    free( (char *)mp);
  }

  gsd_sPrmText *ptp, *ptnext;
  gsd_sText *tp, *tnext;
  for ( ptp = prm_textlist; ptp; ptp = ptnext) {
    ptnext = ptp->next;
    for ( tp = ptp->text_list; tp; tp = tnext) {
      tnext = tp->next;
      free( (char *)tp);
    }
    free ( (char *)ptp);
  }

  if ( extuserprmdataconst)
    free( (char *)extuserprmdataconst);

  if ( prm_dataitems)
    free( (char *)prm_dataitems);
  if ( module_conf)
    free( (char *)module_conf);

  if ( copy_buff) {
    if ( copy_buff->prm_dataitems)
      free( (char *)copy_buff->prm_dataitems);
    free( copy_buff);
  }
    
}

int pb_gsd::read( char *filename)
{
  pwr_tFileName fname;
  int state;
  char line[1000];
  gsd_sKeyword *keyp;
  gsd_sData *datap;
  char	line_part[6][500];
  char	idxline_part[2][500];
  int idxpart_cnt;
  int has_idx;
  char idx_str[80];
  int part_cnt;
  int sts;
  FILE *fp;
  int found;
  int idx;
  int read_line_done = 0;

  printf( "-- Processing file %s\n", filename);

  dcli_translate_filename( fname, filename);
  fp = fopen( fname, "r");
  if ( !fp)
    return PB__GSDFILE;

  state = 0;

  line_cnt = 0;
  while( 1) {
    if ( !read_line_done) {
      sts = read_line( line, sizeof(line), fp);
      if ( !sts)
	break;
    }
    else
      read_line_done = 0;


    compress( line);
    part_cnt = dcli_parse( line, " 	=", "", (char *)line_part,
			   sizeof( line_part) / sizeof( line_part[0]), 
			   sizeof( line_part[0]), 0);

    has_idx = 0;
    if ( strchr( line_part[0], '(') != 0) {
      idxpart_cnt = dcli_parse( line_part[0], " 	()", "", (char *)idxline_part,
				sizeof( idxline_part) / sizeof( idxline_part[0]), 
				sizeof( idxline_part[0]), 0);	
      if ( idxpart_cnt == 2) {
	strcpy( line_part[0], idxline_part[0]);
	strcpy( idx_str, idxline_part[1]);
	has_idx = 1;
      }
    }

    found = 0;
    idx = 0;
    for ( keyp = keywordlist; keyp->type != gsd_End; keyp++) {
      if ( cdh_NoCaseStrcmp( line_part[0], keyp->name) == 0) {
	found = 1;
	break;
      }
      idx++;
    }
    if ( !found) {
      printf( "Keyword not found, line %d, %s\n", line_cnt, line_part[0]);
      continue;
    }
    
    datap = &datalist[idx];
    
    switch ( keyp->type) {
    case gsd_Boolean:
    case gsd_Unsigned8:
    case gsd_Unsigned16:
    case gsd_Unsigned32:
    case gsd_UserPrmDataLen:
    case gsd_MaxUserPrmDataLen:
      if ( line_part[1][0] == '0' && line_part[1][1] == 'x')
	sts = sscanf( &line_part[1][2], "%x", &datap->value);
      else
	sts = sscanf( line_part[1], "%d", &datap->value);
      if ( sts != 1)
	printf( "Syntax error, line %d %d (%s) (%s)\n", line_cnt, sts, line_part[0], line_part[1]);
      datap->found = 1;
      if ( keyp->type == gsd_UserPrmDataLen)
	user_prm_data_len = datap->value;
      else if ( keyp->type == gsd_MaxUserPrmDataLen)
	max_user_prm_data_len = datap->value;
      
      if ( strcmp( keyp->group, "Module") == 0) {
	// Insert in module data
	if ( !current_module) {
	  printf( "Module keyword outside module, line %d\n", line_cnt);
	  break;
	}
	if ( strcmp( keyp->name, "Module_Reference") == 0) 
	  current_module->Module_Reference = datap->value;
	else if ( strcmp( keyp->name, "Ext_Module_Prm_Data_Len") == 0) 
	  current_module->Ext_Module_Prm_Data_Len = datap->value;
	else if ( strcmp( keyp->name, "X_Ext_Module_Prm_Data_Len") == 0) 
	  current_module->X_Ext_Module_Prm_Data_Len = datap->value;
	else if ( strcmp( keyp->name, "F_Ext_Module_Prm_Data_Len") == 0) 
	  current_module->F_Ext_Module_Prm_Data_Len = datap->value;
      }
      else if ( strcmp( keyp->group, "Area") == 0) {
	// Insert in area data
	if ( !current_area) {
	  printf( "Area keyword outside area, line %d\n", line_cnt);
	  break;
	}
	if ( strcmp( keyp->name, "Related_CFG_Identifier") == 0) 
	  current_area->Related_CFG_Identifier = datap->value;
	else if ( strcmp( keyp->name, "IO_Direction") == 0) 
	  current_area->IO_Direction = datap->value;
	else if ( strcmp( keyp->name, "Length") == 0) 
	  current_area->Length = datap->value;
	else if ( strcmp( keyp->name, "Consistency") == 0) 
	  current_area->Consistency = datap->value;
	else if ( strcmp( keyp->name, "Publisher_allowed") == 0) 
	  current_area->Publisher_allowed = datap->value;
	else if ( strcmp( keyp->name, "DP_Master_allowed") == 0) 
	  current_area->DP_Master_allowed = datap->value;
	else if ( strcmp( keyp->name, "Data_Type") == 0) 
	  current_area->Data_Type[current_area->data_type_cnt++] = datap->value;
      }
      else if ( strcmp( keyp->group, "ExtUserPrmData") == 0) {
	// Insert in area data
	if ( !current_extuserprmdata) {
	  printf( "Prm data keyword outside ExtUserPrmData, line %d\n", line_cnt);
	  break;
	}
	if ( strcmp( keyp->name, "Prm_Text_Ref") == 0) 
	  current_extuserprmdata->Prm_Text_Ref = datap->value;
	else if ( strcmp( keyp->name, "Changeable") == 0) 
	  current_extuserprmdata->Changeable = datap->value;
	else if ( strcmp( keyp->name, "Visible") == 0) 
	  current_extuserprmdata->Visible = datap->value;
      }
      else if ( strcmp( keyp->name, "Max_Module") == 0) {
	dptype = gsd_DpSlave;
	max_module = datap->value;
      }
      else if ( strcmp( keyp->name, "Modular_Station") == 0) {
	dptype = gsd_DpSlave;
	modular_station = datap->value;
      }
      else if ( strcmp( keyp->name, "Max_MPS_Length") == 0) {
	dptype = gsd_DpMaster;
      }
      break;
    case gsd_VString8:
    case gsd_VString32:
    case gsd_VString256:
      datap->data = malloc( 256);
      strcpy( (char *)datap->data, line_part[1]);
      datap->found = 1;
      
      if ( strcmp( keyp->group, "Area") == 0) {
	// Insert in area data
	if ( !current_area) {
	  printf( "Area keyword outside area, line %d\n", line_cnt);
	  break;
	}
	if ( strcmp( keyp->name, "Area_Name") == 0) 
	  strcpy( current_area->Area_Name, (char *)datap->data);
      }
      break;
    case gsd_UserPrmData:
    case gsd_OString: {
      char *s = strchr( line, '=');
      str_to_ostring( (unsigned char **)&datap->data, s+1, 256, &datap->len);
      datap->found = 1;
      break;
    }
    case gsd_Module: {
      gsd_sModule *mp;
      gsd_sModule *m = (gsd_sModule *) calloc( 1, sizeof(gsd_sModule));
      strncpy( m->Mod_Name, line_part[1], sizeof(m->Mod_Name));
      if ( part_cnt > 2)
	str_to_ostring( &m->Config, line_part[2], 244, &m->config_size);
      
      if ( !modulelist)
	modulelist = m;
      else {
	mp = modulelist;
	while ( mp->next) 
	  mp = mp->next;
	mp->next = m;
      }
      datap->found = 1;
      status |= gsd_mSts_Module;
      current_module = m;
      
      // Read Module_Reference
      sts = read_line( line, sizeof(line), fp);
      if ( !sts) break;
      
      sts = sscanf( line, "%d", &m->Module_Reference);
      if ( sts != 1) {
	// Module_Reference is missing
	read_line_done = 1;
      }
      
      break;
    }
    case gsd_EndModule:
      status &= ~gsd_mSts_Module;
      current_module = 0;
      break;
    case gsd_DataAreaBegin: {
      gsd_sArea *ap;
      gsd_sArea *a = (gsd_sArea *) calloc( 1, sizeof(gsd_sArea));
      
      if ( !current_module) {
	printf( "Area outside module, line %d\n", line_cnt);
	break;
      }
      if ( !current_module->arealist)
	current_module->arealist = a;
      else {
	ap = current_module->arealist;
	while ( ap->next) 
	  ap = ap->next;
	ap->next = a;
      }
      datap->found = 1;
      status |= gsd_mSts_Area;
      current_area = a;
      break;
    }
    case gsd_DataAreaEnd:
      status &= ~gsd_mSts_Area;
      current_area = 0;
      break;
    case gsd_PrmText: {
      gsd_sPrmText *p, *pp;
      
      p = (gsd_sPrmText *) calloc( 1, sizeof(gsd_sPrmText));
      sts = sscanf( line_part[1], "%d", &p->Reference_Number);
      if ( sts != 1)
	printf( "Syntax error, line %d %d (%s) (%s)\n", line_cnt, sts, line_part[0], line_part[1]);
      if ( !prm_textlist)
	prm_textlist = p;
      else {
	pp = prm_textlist;
	while ( pp->next) 
	  pp = pp->next;
	pp->next = p;
      }
      current_prm_text = p;
      status |= gsd_mSts_PrmText;
      break;
    }
    case gsd_EndPrmText: {
      status &= ~gsd_mSts_PrmText;
      current_prm_text = 0;
      break;
    }
    case gsd_Text: {
      gsd_sText *t, *tp;
      
      t = (gsd_sText *) calloc( 1, sizeof(gsd_sText));
      if ( !has_idx || !current_prm_text || part_cnt < 2) {
	printf( "Syntax error, line %d (%s)\n", line_cnt, line_part[0]);
	break;
      }
      dcli_remove_blank( idx_str, idx_str);
      if ( idx_str[0] == '0' && idx_str[1] == 'x')
	sts = sscanf( idx_str, "%x", &t->Prm_Data_Value);
      else
	sts = sscanf( idx_str, "%d", &t->Prm_Data_Value);
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line_part[0]);
      strcpy( t->Text, line_part[1]);
      if ( !current_prm_text->text_list)
	current_prm_text->text_list = t;
      else {
	tp = current_prm_text->text_list;
	while ( tp->next) 
	  tp = tp->next;
	tp->next = t;
      }
      break;
    }
    case gsd_ExtUserPrmData: {
      gsd_sExtUserPrmData *e, *ep;
      char *t;
      char minval[20], maxval[20];
      
      e = (gsd_sExtUserPrmData *) calloc( 1, sizeof(gsd_sExtUserPrmData));
      sts = sscanf( line_part[1], "%d", &e->Reference_Number);
      e->Changeable = 1;
      e->Visible = 1;
      
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line);
      if ( part_cnt > 2)
	strncpy( e->Ext_User_Prm_Data_Name, line_part[2], 
		 sizeof(e->Ext_User_Prm_Data_Name));

      // Read Data_Type
      sts = read_line( line, sizeof(line), fp);
      if ( !sts) break;
      
      part_cnt = dcli_parse( line, " 	=", "", (char *)line_part,
			     sizeof( line_part) / sizeof( line_part[0]), 
			     sizeof( line_part[0]), 0);
      
      if ( strchr( line_part[0], '(') != 0) {
	idxpart_cnt = dcli_parse( line_part[0], " 	()", "", (char *)idxline_part,
				  sizeof( idxline_part) / sizeof( idxline_part[0]), 
				  sizeof( idxline_part[0]), 0);	
	if ( idxpart_cnt == 2) {
	  strcpy( line_part[0], idxline_part[0]);
	  strcpy( idx_str, idxline_part[1]);
	}

	if ( cdh_NoCaseStrcmp( line_part[0], "Bit") == 0) {
	  sts = sscanf( idx_str, "%d", &e->bit_num);
	  if ( sts != 1)
	    printf( "Syntax error, line %d (%s)\n", line_cnt, line);	  
	}
	else if ( cdh_NoCaseStrcmp( line_part[0], "BitArea") == 0) {
	  if ( (t = strrchr( idx_str, '-')) && t != idx_str) {
	    strncpy( minval, idx_str, (int)(t - idx_str));
	    minval[t-idx_str] = 0;
	    strcpy( maxval, t+1);
	  
	    sts = sscanf( minval, "%d", &e->bitarea_min);
	    if ( sts != 1)
	      printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	  
	    sts = sscanf( maxval, "%d", &e->bitarea_max);
	    if ( sts != 1)
	      printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	  }
	  else
	    printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	}
      }
      
      strcpy( e->Data_Type_Name, line_part[0]);
      if ( part_cnt > 1) {
	sts = sscanf( line_part[1], "%d", &e->Default_Value);
	if ( sts != 1)
	  printf( "Syntax error, line %d (%s)\n", line_cnt, line);
      }	  
      if ( part_cnt > 2) {
	
	if ( (t = strrchr( line_part[2], '-')) && t != line_part[2]) {
	  strncpy( minval, line_part[2], (int)(t - line_part[2]));
	  minval[t-line_part[2]] = 0;
	  strcpy( maxval, t+1);
	  
	  sts = sscanf( minval, "%d", &e->Min_Value);
	  if ( sts != 1)
	    printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	  
	  sts = sscanf( maxval, "%d", &e->Max_Value);
	  if ( sts != 1)
	    printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	}
	else {
	  char allstr[16][10];
	  int all_cnt;
	  
	  all_cnt = dcli_parse( line_part[2], " 	,", "", (char *)allstr,
				sizeof( allstr) / sizeof( allstr[0]), 
				sizeof( allstr[0]), 0);
	  for ( int i = 0; i < all_cnt; i++) {
	    sts = sscanf( allstr[i], "%d", &e->Allowed_Values[i]);
	    if ( sts != 1)
	      printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	  }
	  e->allowed_cnt = all_cnt;
	}
      }
      
      if ( !extuserprmdatalist)
	extuserprmdatalist = e;
      else {
	ep = extuserprmdatalist;
	while ( ep->next) 
	  ep = ep->next;
	ep->next = e;
      }
      current_extuserprmdata = e;
      status |= gsd_mSts_ExtUserPrmData;
      break;
    }
    case gsd_EndExtUserPrmData: {
      status &= ~gsd_mSts_ExtUserPrmData;
      current_extuserprmdata = 0;
      break;
    }
    case gsd_ExtUserPrmDataRef: {
      gsd_sExtUserPrmDataRef *e, *ep;
      
      e = (gsd_sExtUserPrmDataRef *) calloc( 1, sizeof(gsd_sExtUserPrmDataRef));
      sts = sscanf( line_part[1], "%d", &e->Reference_Number);
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line);
      
      dcli_remove_blank( idx_str, idx_str);
      if ( idx_str[0] == '0' && idx_str[1] == 'x')
	sts = sscanf( idx_str, "%x", &e->Reference_Offset);
      else
	sts = sscanf( idx_str, "%d", &e->Reference_Offset);
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line_part[0]);
      
      if ( status & gsd_mSts_Module) {
	if ( !current_module->extuserprmdatareflist)
	  current_module->extuserprmdatareflist = e;
	else {
	  ep = current_module->extuserprmdatareflist;
	  while ( ep->next) 
	    ep = ep->next;
	  ep->next = e;
	}
      }
      else {
	if ( !extuserprmdatareflist)
	  extuserprmdatareflist = e;
	else {
	  ep = extuserprmdatareflist;
	  while ( ep->next) 
	    ep = ep->next;
	  ep->next = e;
	}
      }
      break;
    }
    case gsd_ExtUserPrmDataConst: {
      gsd_sExtUserPrmDataConst *e;
      unsigned char *t;
      char *s;
      int new_data = 0;
      int const_offset;
      int data_len;
      
      if ( status & gsd_mSts_Module) {	
	// Addition module data
	if ( current_module->extuserprmdataconst)
	  e = current_module->extuserprmdataconst;
	else
	  new_data = 1;
      }
      else {
	if ( extuserprmdataconst)
	  // Additional slave data
	  e = extuserprmdataconst;
	else
	  new_data = 1;
      }

      if ( new_data)
	e = (gsd_sExtUserPrmDataConst *) calloc( 1, sizeof(gsd_sExtUserPrmDataConst));
 

      dcli_remove_blank( idx_str, idx_str);
      if ( idx_str[0] == '0' && idx_str[1] == 'x')
	sts = sscanf( idx_str, "%x", &const_offset);
      else
	sts = sscanf( idx_str, "%d", &const_offset);
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line_part[0]);
      
      if ( new_data) {
	e->Const_Offset = const_offset;
      
	if ( (s = strchr( line, '='))) {
	  str_to_ostring( &t, s+1, sizeof(e->Const_Prm_Data), &e->len);
	  if ( e->len + e->Const_Offset > (int) sizeof(e->Const_Prm_Data))
	    printf( "Const length too large, line %d (%s)\n", line_cnt, line_part[0]);
	  memcpy( (char *)e->Const_Prm_Data + e->Const_Offset, (char *)t, 
		  sizeof(e->Const_Prm_Data) - e->Const_Offset);
	  free( t);
	}
      }
      else {
	if ( (s = strchr( line, '='))) {
	  str_to_ostring( &t, s+1, sizeof(e->Const_Prm_Data), &data_len);
	  memcpy( (char *)e->Const_Prm_Data + const_offset, (char *)t, data_len);
	  e->len = const_offset + data_len - e->Const_Offset;
	  if ( e->len + e->Const_Offset > (int) sizeof(e->Const_Prm_Data))
	    printf( "Const length too large, line %d (%s)\n", line_cnt, line_part[0]);
	  free( t);
	}
      }

      if ( status & gsd_mSts_Module)
	current_module->extuserprmdataconst = e;
      else
	extuserprmdataconst = e;

      break;
    }
    case gsd_ProfibusDP:
      status |= gsd_mSts_Profibus_DP;
      break;
    case gsd_SlaveFamily: {
      char fam_part[20][4];
      int fam_cnt;
      int family;
      
      fam_cnt = dcli_parse( line_part[1], " 	@", "", (char *)fam_part,
			    sizeof( fam_part) / sizeof( fam_part[0]), 
			    sizeof( fam_part[0]), 0);
      if ( fam_cnt < 1) {
	printf( "Syntax error, line %d (%s)\n", line_cnt, line);
	break;
      }
      sts = sscanf( fam_part[0], "%d", &family);
      if ( sts != 1)
	printf( "Syntax error, line %d (%s)\n", line_cnt, line);
      
      datap->data = malloc( 256);
      datap->found = 1;
      
      switch ( family) {
      case 0:
	strcpy( (char *)datap->data, "General");
	break;
      case 1:
	strcpy( (char *)datap->data, "Drives");
	break;
      case 2:
	strcpy( (char *)datap->data, "Switching devices");
	break;
      case 3:
	strcpy( (char *)datap->data, "I/O");
	break;
      case 4:
	strcpy( (char *)datap->data, "Valves");
	break;
      case 5:
	strcpy( (char *)datap->data, "Controllers");
	break;
      case 6:
	strcpy( (char *)datap->data, "HMI");
	break;
      case 7:
	strcpy( (char *)datap->data, "Encoders");
	break;
      case 8:
	strcpy( (char *)datap->data, "NC/RC");
	break;
      case 9:
	strcpy( (char *)datap->data, "Gateway");
	break;
      case 10:
	strcpy( (char *)datap->data, "Programmable Logic Controllers");
	break;
      case 11:
	strcpy( (char *)datap->data, "Ident systems");
	break;
      case 12:
	strcpy( (char *)datap->data, "Profibus PA Profile");
	break;
      default:
	strcpy( (char *)datap->data, "Unknown");
      }
      
      for ( int i = 1; i < fam_cnt; i++) {
	strcat( (char *)datap->data, " / ");
	strcat( (char *)datap->data, fam_part[i]);
      }
      break;
    }
    case gsd_BitVString32:
    case gsd_BitVString256:
    case gsd_DiagArea:
    case gsd_DiagAreaEnd:
    case gsd_DiagType:
    case gsd_Value:
      if ( part_cnt >= 2) {
	// printf( "Line %s %s\n", keyp->name, line_part[1]);
      }
      else {
	// printf( "Line %s\n", keyp->name);
      }
      break;
    default:
      printf("Unknown type, line %d\n", line_cnt);
    }
  }
    
  build();
  return 1;
}

int pb_gsd::build()
{ 
  // Link ExtUserPrmData to PrmText
  for ( gsd_sExtUserPrmData *dp = extuserprmdatalist; dp; dp = dp->next) {
    if ( dp->Prm_Text_Ref != 0) {
      for ( gsd_sPrmText *tp = prm_textlist; tp; tp = tp->next) {
	if ( dp->Prm_Text_Ref == tp->Reference_Number) {
	  dp->prmtext = tp;
	  break;
	}
      }
      if ( !dp->prmtext) 
	printf( "** Referenced PrmText not found %d\n", dp->Prm_Text_Ref);
    }
  }

  // Translate ExtUserPrmData Data_Type_Name
  for ( gsd_sExtUserPrmData *dp = extuserprmdatalist; dp; dp = dp->next) {
    if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Bit") == 0)
      dp->data_type = gsd_Bit;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "BitArea") == 0)
      dp->data_type = gsd_BitArea;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Unsigned8") == 0)
      dp->data_type = gsd_Unsigned8;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Unsigned16") == 0)
      dp->data_type = gsd_Unsigned16;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Unsigned32") == 0)
      dp->data_type = gsd_Unsigned32;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Signed8") == 0)
      dp->data_type = gsd_Signed8;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Signed16") == 0)
      dp->data_type = gsd_Signed16;
    else if ( cdh_NoCaseStrcmp( dp->Data_Type_Name, "Signed32") == 0)
      dp->data_type = gsd_Signed32;
    else
      printf( "** Unknown Data_Type_Name \"%s\" in ExtUserPrmData %s\n", dp->Data_Type_Name, 
	      dp->Ext_User_Prm_Data_Name);
  }

  // Link ExtUserPrmDataRef to ExtUserPrmData
  for ( gsd_sExtUserPrmDataRef *ep = extuserprmdatareflist; ep; ep = ep->next) {
    for ( gsd_sExtUserPrmData *dp = extuserprmdatalist; dp; dp = dp->next) {
      if ( ep->Reference_Number == dp->Reference_Number) {
	ep->prm_data = dp;
	break;
      }
    }
    if ( !ep->prm_data)
      printf( "** Referenced ExtUserPrmData not found %d\n", ep->Reference_Number);
  }

  // Create data items for slave ExtPrmData
  prm_dataitems_cnt = 0;
  for ( gsd_sExtUserPrmDataRef *ep = extuserprmdatareflist; ep; ep = ep->next)
    prm_dataitems_cnt++;

  prm_dataitems = (gsd_sPrmDataItem *) calloc( prm_dataitems_cnt, sizeof(gsd_sPrmDataItem));
  int i = 0;
  for ( gsd_sExtUserPrmDataRef *ep = extuserprmdatareflist; ep; ep = ep->next) {
    prm_dataitems[i].ref = ep;
    i++;
  }    
    
  // Link Module UserPrmDataRef to ExtUserPrmData
  for ( gsd_sModule *mp = modulelist; mp; mp = mp->next) {
    for ( gsd_sExtUserPrmDataRef *ep = mp->extuserprmdatareflist; ep; ep = ep->next) {
      for ( gsd_sExtUserPrmData *dp = extuserprmdatalist; dp; dp = dp->next) {
	if ( ep->Reference_Number == dp->Reference_Number) {
	  ep->prm_data = dp;
	  break;
	}
      }
      if ( !ep->prm_data)
	printf( "** Referenced ExtUserPrmData not found %d\n", ep->Reference_Number);
    }
  }

  // Calculate ExtUserPrmData length
  items_user_prm_data_len = prm_len( prm_dataitems, prm_dataitems_cnt);

  if ( extuserprmdataconst) {
    if ( items_user_prm_data_len < 
	 extuserprmdataconst->len + extuserprmdataconst->Const_Offset)
      items_user_prm_data_len = extuserprmdataconst->len + 
	extuserprmdataconst->Const_Offset;
  }
  if ( max_user_prm_data_len) {
    if ( extuserprmdataconst && 
	 max_user_prm_data_len < 
         extuserprmdataconst->len + extuserprmdataconst->Const_Offset)
      printf( "ExtUserPrmDataConst exceeds Max_User_Prm_Data_Len, line %d\n", line_cnt);
  }

  // Check Module UserPrmDataLen
  for ( gsd_sModule *mp = modulelist; mp; mp = mp->next) {
    if ( mp->Ext_Module_Prm_Data_Len == 0 && mp->extuserprmdataconst)
      mp->Ext_Module_Prm_Data_Len = mp->extuserprmdataconst->len +
	mp->extuserprmdataconst->Const_Offset;
    else if ( mp->extuserprmdataconst && 
	      mp->Ext_Module_Prm_Data_Len < 
	      mp->extuserprmdataconst->len + mp->extuserprmdataconst->Const_Offset) {
      printf( "** ExtUserPrmDataLen differs (%d) (%d), Module %s\n", mp->Ext_Module_Prm_Data_Len,
	      mp->extuserprmdataconst->len, mp->Mod_Name);
    }
  }

  if ( dptype == gsd_DpSlave) {
    // Create data items for modules
    if ( !modular_station)
      module_conf_cnt = 1;
    else
      module_conf_cnt = max_module;
    module_conf = (gsd_sModuleConf *) calloc( module_conf_cnt, sizeof(gsd_sModuleConf));
  }

  if ( !extuserprmdataconst)
    extuserprmdataconst = (gsd_sExtUserPrmDataConst *) calloc( 1, sizeof(gsd_sExtUserPrmDataConst));

  // Set default values to items
  prm_items_set_default_data( prm_dataitems, prm_dataitems_cnt);

  return 1;
}

int pb_gsd::prm_len( gsd_sPrmDataItem *item, int item_size)
{
  gsd_sExtUserPrmData *pd;
  int len = 0;
  int size;

  for ( int i = 0; i < item_size; i++) {
    pd = item[i].ref->prm_data;

    // Check value
    switch ( pd->data_type) {
    case gsd_Bit:
    case gsd_BitArea:
    case gsd_Unsigned8:
    case gsd_Signed8:
      size = 1;
      break;
    case gsd_Signed16:
    case gsd_Unsigned16:
      size = 2;
      break;
    case gsd_Unsigned32:
    case gsd_Signed32:
      size = 4;
      break;
    default: 
      size = 0;
    }
    if ( item[i].ref->Reference_Offset + size > len)
      len = item[i].ref->Reference_Offset + size;
  }
  return len;
}

int pb_gsd::prm_items_set_default_data( gsd_sPrmDataItem *item, int item_size)
{
  gsd_sExtUserPrmData *pd;

  for ( int i = 0; i < item_size; i++) {
    pd = item[i].ref->prm_data;

    switch ( pd->data_type) {
    case gsd_BitArea:
    case gsd_Unsigned8:
    case gsd_Signed8:
    case gsd_Unsigned16:
    case gsd_Signed16:
    case gsd_Unsigned32:
    case gsd_Signed32:
      item[i].value = pd->Default_Value;
      break;
    default: ;
    }
  }
  return 1;
}


int pb_gsd::prm_items_to_data( gsd_sPrmDataItem *item, int item_size, 
			       unsigned char *data, int data_size)
{
  gsd_sExtUserPrmData *pd;

  for ( int i = 0; i < item_size; i++) {
    pd = item[i].ref->prm_data;

    // Check value
    if ( pd->allowed_cnt == 0) {
      if ( item[i].value < pd->Min_Value)
	printf( "** Value too low \"%s\" %d (%d-%d)\n", pd->Ext_User_Prm_Data_Name, 
		item[i].value, pd->Min_Value, pd->Max_Value);
      else if ( item[i].value > pd->Max_Value)
	printf( "** Value too high \"%s\" %d (%d-%d)\n", pd->Ext_User_Prm_Data_Name, 
		item[i].value, pd->Min_Value, pd->Max_Value);
    }
    else {
      int found = 0;
      for ( int j = 0; j < pd->allowed_cnt; j++) {
	if ( pd->Allowed_Values[j] == item[i].value) {
	  found = 1;
	  break;
	}
      }
      if ( !found)
	printf( "** Value not allowed\n");
    }
    switch ( pd->data_type) {
    case gsd_Bit: {
      if ( item[i].ref->Reference_Offset >= data_size) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      unsigned char v = 1 << pd->bit_num;
      if ( item[i].value)
	data[item[i].ref->Reference_Offset] |= v;
      else
	data[item[i].ref->Reference_Offset] &= ~v;
      break;
    }
    case gsd_BitArea: {
      if ( item[i].ref->Reference_Offset >= data_size) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      unsigned char mask = 0;
      unsigned char v = (unsigned char) item[i].value;

      for ( int j = 0; j <= pd->bitarea_max - pd->bitarea_min; j++)
	mask = (mask << 1) + 1;
      mask = mask << pd->bitarea_min;
      v = v << pd->bitarea_min;
      data[item[i].ref->Reference_Offset] &= ~mask;
      data[item[i].ref->Reference_Offset] |= v;

      break;
    }
    case gsd_Unsigned8: {
      if ( item[i].ref->Reference_Offset >= data_size) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      unsigned char v = (unsigned char) item[i].value;
      memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      break;
    }
    case gsd_Signed8: {
      if ( item[i].ref->Reference_Offset >= data_size) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      char v = (char) item[i].value;
      memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      break;
    }
    case gsd_Unsigned16: {
      if ( item[i].ref->Reference_Offset >= data_size - 1) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      unsigned short v = (unsigned short) item[i].value;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[2];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[1];
	data[item[i].ref->Reference_Offset+1] = b[0];
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[2];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[1];
	data[item[i].ref->Reference_Offset+1] = b[0];
      }
#endif
      break;
    }
    case gsd_Signed16: {
      if ( item[i].ref->Reference_Offset >= data_size - 1) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      short v = (short) item[i].value;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[2];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[1];
	data[item[i].ref->Reference_Offset+1] = b[0];
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[2];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[1];
	data[item[i].ref->Reference_Offset+1] = b[0];
      }
#endif
      break;
    }
    case gsd_Unsigned32: {
      if ( item[i].ref->Reference_Offset >= data_size - 3) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      unsigned int v = (unsigned int) item[i].value;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian) 
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[4];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[3];
	data[item[i].ref->Reference_Offset+1] = b[2];
	data[item[i].ref->Reference_Offset+2] = b[1];
	data[item[i].ref->Reference_Offset+3] = b[0];
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian) 
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[4];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[3];
	data[item[i].ref->Reference_Offset+1] = b[2];
	data[item[i].ref->Reference_Offset+2] = b[1];
	data[item[i].ref->Reference_Offset+3] = b[0];
      }
#endif
      break;
    }
    case gsd_Signed32: {
      if ( item[i].ref->Reference_Offset >= data_size - 3) {
	printf( "** Prm Reference_Offset out of range\n");
	return 0;
      }

      int v = (int) item[i].value;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian) 
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[4];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[3];
	data[item[i].ref->Reference_Offset+1] = b[2];
	data[item[i].ref->Reference_Offset+2] = b[1];
	data[item[i].ref->Reference_Offset+3] = b[0];
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian) 
	memcpy( &data[item[i].ref->Reference_Offset], &v, sizeof(v));
      else {
	unsigned char b[4];
	memcpy( b, &v, sizeof(b));
	data[item[i].ref->Reference_Offset] = b[3];
	data[item[i].ref->Reference_Offset+1] = b[2];
	data[item[i].ref->Reference_Offset+2] = b[1];
	data[item[i].ref->Reference_Offset+3] = b[0];
      }
#endif
      break;
    }
    default: 
      printf( "** Unknown data type\n");;
    }
  }

  // printf( "To   Data: ");
  // for ( int i = 0; i < data_size; i++)
  //   printf( "0x%x,", data[i]);
  // printf("\n");

  return 1;
}

int pb_gsd::prm_data_to_items( gsd_sPrmDataItem *item, int item_size, 
			       unsigned char *data, int data_size, int set_default)
{
  gsd_sExtUserPrmData *pd;

  for ( int i = 0; i < item_size; i++) {
    pd = item[i].ref->prm_data;

    switch ( pd->data_type) {
    case gsd_Bit: {
      unsigned char mask = 1 << pd->bit_num;
      item[i].value = ( (data[item[i].ref->Reference_Offset] & mask) != 0);
      break;
    }
    case gsd_BitArea: {
      unsigned char mask = 0;
      unsigned char v = (unsigned char) item[i].value;

      for ( int j = 0; j <= pd->bitarea_max - pd->bitarea_min; j++)
	mask = (mask << 1) + 1;
      mask = mask << pd->bitarea_min;

      v = data[item[i].ref->Reference_Offset] & mask;
      v = v >> pd->bitarea_min;
      item[i].value = v;

      break;
    }
    case gsd_Unsigned8: {
      unsigned char v;
      memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      item[i].value = v;
      break;
    }
    case gsd_Signed8: {
      char v;
      memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      item[i].value = v;
      break;
    }
    case gsd_Unsigned16: {
      unsigned short v;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[2];
	b[1] = data[item[i].ref->Reference_Offset];
	b[0] = data[item[i].ref->Reference_Offset+1];
	memcpy( &v, b, sizeof(v));
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[2];
	b[1] = data[item[i].ref->Reference_Offset];
	b[0] = data[item[i].ref->Reference_Offset+1];
	memcpy( &v, b, sizeof(v));
      }
#endif
      item[i].value = v;
      break;
    }
    case gsd_Signed16: {
      short v;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[2];
	b[1] = data[item[i].ref->Reference_Offset];
	b[0] = data[item[i].ref->Reference_Offset+1];
	memcpy( &v, b, sizeof(v));
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[2];
	b[1] = data[item[i].ref->Reference_Offset];
	b[0] = data[item[i].ref->Reference_Offset+1];
	memcpy( &v, b, sizeof(v));
      }
#endif
      item[i].value = v;
      break;
    }
    case gsd_Unsigned32: {
      unsigned int v;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[4];
	b[3] = data[item[i].ref->Reference_Offset];
	b[2] = data[item[i].ref->Reference_Offset+1];
	b[1] = data[item[i].ref->Reference_Offset+2];
	b[0] = data[item[i].ref->Reference_Offset+3];
	memcpy( &v, b, sizeof(v));
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[4];
	b[3] = data[item[i].ref->Reference_Offset];
	b[2] = data[item[i].ref->Reference_Offset+1];
	b[1] = data[item[i].ref->Reference_Offset+2];
	b[0] = data[item[i].ref->Reference_Offset+3];
	memcpy( &v, b, sizeof(v));
      }
#endif
      item[i].value = v;
      break;
    }
    case gsd_Signed32: {
      int v;
#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
      if ( byte_order == pwr_eByteOrderingEnum_LittleEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[4];
	b[3] = data[item[i].ref->Reference_Offset];
	b[2] = data[item[i].ref->Reference_Offset+1];
	b[1] = data[item[i].ref->Reference_Offset+2];
	b[0] = data[item[i].ref->Reference_Offset+3];
	memcpy( &v, b, sizeof(v));
      }
#elif (pwr_dHost_byteOrder == pwr_dBigEndian)
      if ( byte_order == pwr_eByteOrderingEnum_BigEndian)
	memcpy( &v, &data[item[i].ref->Reference_Offset], sizeof(v));
      else {
	unsigned char b[4];
	b[3] = data[item[i].ref->Reference_Offset];
	b[2] = data[item[i].ref->Reference_Offset+1];
	b[1] = data[item[i].ref->Reference_Offset+2];
	b[0] = data[item[i].ref->Reference_Offset+3];
	memcpy( &v, b, sizeof(v));
      }
#endif
      item[i].value = v;
      break;
    }
    default: 
      printf( "** Unknown data type\n");;
    }

    // Set default value 
    switch ( pd->data_type) {
    case gsd_BitArea:
    case gsd_Unsigned8:
    case gsd_Signed8:
    case gsd_Unsigned16:
    case gsd_Signed16:
    case gsd_Unsigned32:
    case gsd_Signed32:
      if ( set_default) {
	if ( item[i].value < pd->Min_Value || item[i].value > pd->Max_Value)
	  item[i].value = pd->Default_Value;
      }
      break;
    default: ;
    }
  }
  // printf( "From Data: ");
  // for ( int i = 0; i < data_size; i++)
  //   printf( "0x%x,", data[i]);
  // printf("\n");
  return 1;
}

int pb_gsd::prm_text_val_to_str( gsd_sPrmText *prmtext, int val, char *str)
{
  for ( gsd_sText *tp = prmtext->text_list; tp; tp = tp->next) {
    if ( tp->Prm_Data_Value == val) {
      strcpy( str, tp->Text);
      return 1;
    }
  }
  strcpy( str, "");
  return 0;
}

int pb_gsd::get_svalue( char *key, char *value, int size)
{
  gsd_sKeyword *keyp;
  gsd_sData *datap;

  for ( keyp = keywordlist, datap = datalist; keyp->type != gsd_End; keyp++, datap++) {
    if ( cdh_NoCaseStrcmp( key, keyp->name) == 0) {
      if ( !datap->found)
	return 0;
      
      switch ( keyp->type) {
      case gsd_VString8:
      case gsd_VString32:
      case gsd_VString256:
      case gsd_SlaveFamily:
	strncpy( value, (char *)datap->data, size);
	return 1;
      case gsd_UserPrmData: {
	int len = size < datap->len ? size : datap->len;
	memcpy( value, (char *)datap->data, len);
	return 1;
      }
      default:
	return 0;
      }
    }
  }
  return 0;
}

int pb_gsd::get_ivalue( char *key, int *value)
{
  gsd_sKeyword *keyp;
  gsd_sData *datap;

  for ( keyp = keywordlist, datap = datalist; keyp->type != gsd_End; keyp++, datap++) {
    if ( cdh_NoCaseStrcmp( key, keyp->name) == 0) {
      if ( !datap->found)
	return 0;
      
      switch ( keyp->type) {
      case gsd_ProfibusDP:
      case gsd_Boolean:
      case gsd_Unsigned8:
      case gsd_Unsigned16:
      case gsd_Unsigned32:
      case gsd_UserPrmDataLen:
	*value = datap->value;
	return 1;
      default:
	return 0;
      }
    }
  }
  return 0;
}

int pb_gsd::print()
{
  gsd_sKeyword *keyp;
  gsd_sData *datap;
  unsigned int val;

  for ( keyp = keywordlist, datap = datalist; keyp->type != gsd_End; keyp++, datap++) {
    if ( datap->found) {
      switch ( keyp->type) {
      case gsd_ProfibusDP:
      case gsd_Boolean:
      case gsd_Unsigned8:
      case gsd_Unsigned16:
      case gsd_Unsigned32:
      case gsd_UserPrmDataLen:
	printf( "%s %d\n", keyp->name, datap->value);
	break;
      case gsd_VString8:
      case gsd_VString32:
      case gsd_VString256:
      case gsd_SlaveFamily:
	printf( "%s %s\n", keyp->name, (char *)datap->data);
	break;
      case gsd_OString:
      case gsd_UserPrmData: {
	int size = 256;
	if ( keyp->type == gsd_UserPrmData)
	  size = user_prm_data_len;

	printf( "%s ", keyp->name);
	for ( int i = 0; i < size; i++) {
	  val = *((unsigned char *)datap->data + i);
	  printf( "0x%x,", val);
	}
	printf("\n");
	break;
      }
      case gsd_Module: {
	break;
      }
      case gsd_BitVString32:
      case gsd_BitVString256:
      case gsd_DiagArea:
      case gsd_DiagType:
      case gsd_EndModule:
      case gsd_DataAreaBegin:
      case gsd_DataAreaEnd:
      case gsd_Value:
      case gsd_MaxUserPrmDataLen:
	break;
      default:
	printf("Unknown type %s\n", keyp->name);
      }
    }
  }


  // Print modules
  for ( gsd_sModule *mp = modulelist; mp; mp = mp->next) {
    printf( "Module %s ", mp->Mod_Name);
    for ( int i = 0; i < mp->config_size; i++) {
      val = *(mp->Config + i);
      printf( "0x%x,", val);
    }
    printf("\n");	  
    printf( "  Module_Reference %d\n", mp->Module_Reference);

    // Print ExtUserPrmDataRef
    for ( gsd_sExtUserPrmDataRef *ep = mp->extuserprmdatareflist; ep; ep = ep->next) {
      printf( "  ExtUserPrmDataRef %d %d\n", ep->Reference_Offset, ep->Reference_Number);
    }	

    printf( "  Ext_Module_Prm_Data_Len %d\n", mp->Ext_Module_Prm_Data_Len);
    if ( mp->extuserprmdataconst) {
      printf( "  ExtUserPrmDataConst %d   ", mp->extuserprmdataconst->Const_Offset);
      for ( int i = 0; i < mp->extuserprmdataconst->len; i++)
	printf( "%d,", mp->extuserprmdataconst->Const_Prm_Data[i]);
      printf( "\n");
    }

    // Print areas
    for ( gsd_sArea *ap = mp->arealist; ap; ap = ap->next) {
      printf("Area %s\n", ap->Area_Name);
      printf("  Related_CFG_Identifier %d\n", ap->Related_CFG_Identifier);
      printf("  IO_Direction %d\n", ap->IO_Direction);
      printf("  Length %d\n", ap->Length);
      printf("  Publisher_allowed %d\n", ap->Publisher_allowed);
      printf("  DP_Master_allowed %d\n", ap->DP_Master_allowed);
      for ( int i = 0; i < ap->data_type_cnt; i++)
	printf("  Data_Type %d\n", ap->Data_Type[i]);
    }
  }	

  // Print PrmText
  for ( gsd_sPrmText *tp = prm_textlist; tp; tp = tp->next) {
    printf( "PrmText %d\n", tp->Reference_Number);

    // Print Text
    for ( gsd_sText*xp = tp->text_list; xp; xp = xp->next) {
      printf("  Text %d \"%s\"\n", xp->Prm_Data_Value, xp->Text);
    }
  }	

  // Print ExtUserPrmData
  for ( gsd_sExtUserPrmData *ep = extuserprmdatalist; ep; ep = ep->next) {
    printf( "ExtPrmUserData %d \"%s\"\n", ep->Reference_Number, ep->Ext_User_Prm_Data_Name);
    printf( "  Data_Type_Name %s", ep->Data_Type_Name);
    if ( cdh_NoCaseStrcmp( ep->Data_Type_Name, "Bit") == 0)
      printf( " %d\n", ep->bit_num);
    else if ( cdh_NoCaseStrcmp( ep->Data_Type_Name, "BitArea") == 0)
      printf( " %d-%d\n", ep->bitarea_min, ep->bitarea_max);
    else
      printf( "\n");
    printf( "  Min_Value %d\n", ep->Min_Value);
    printf( "  Max_Value %d\n", ep->Max_Value);
    if ( ep->allowed_cnt) {
      printf( "  Allowed_Values ");
      for ( int i = 0; i < ep->allowed_cnt; i++)
	printf( "%d,", ep->Allowed_Values[i]);
      printf( "\n");
    }
    printf( "  Prm_Text_Ref %d\n", ep->Prm_Text_Ref);
    printf( "  Changeable %d\n", ep->Changeable);
    printf( "  Visible %d\n", ep->Visible);
  }

  // Print ExtUserPrmRef
  for ( gsd_sExtUserPrmDataRef *ep = extuserprmdatareflist; ep; ep = ep->next) {
    printf( "ExtUserPrmDataRef %d %d\n", ep->Reference_Offset, ep->Reference_Number);
  }	

  if ( extuserprmdataconst) {
    printf( "ExtUserPrmDataConst %d   ", extuserprmdataconst->Const_Offset);
    for ( int i = 0; i < extuserprmdataconst->len; i++)
      printf( "%d,", extuserprmdataconst->Const_Prm_Data[i]);
    printf( "\n");
  }

  return 1;
}

int pb_gsd::str_to_ostring( unsigned char **data, char *str, int size, int *rsize)
{
  char valstr[40];
  int valcnt;
  unsigned int val;
  char *s, *t;
  int sts;

  *data = (unsigned char *) malloc( size);
  t = str;
  valcnt = 0;
  for ( s = str;; s++) {
    if ( valcnt > size) {
      printf( "** Size error");
      break;
    }
    if ( *s == ',' || *s == 0) {
      strncpy( valstr, t, s - t);
      valstr[s - t] = 0;
      dcli_remove_blank( valstr, valstr);
      if ( valstr[0] == '0' && valstr[1] == 'x')
	sts = sscanf( &valstr[2], "%x", &val);
      else
	sts = sscanf( valstr, "%d", &val);
      *(*data + valcnt++) = (unsigned char) val;
      if ( sts != 1)
	printf( "Syntax error, line %d\n", line_cnt);
 
      t = s+1;
    }
    if ( *s == 0)
      break;
  }
  if ( rsize)
    *rsize = valcnt;
  return 1;
}

int pb_gsd::read_line( char *line, int lsize, FILE *fp)
{
  int sts;
  int in_string;
  char *s;

  while ( 1) {
    sts = dcli_read_line( line, lsize, fp);
    if ( !sts)
      return sts;
    else {
      line_cnt++;
      
      // Remove comments
      in_string = 0;
      for ( s = line; *s; s++) {
	if ( *s == '"')
	  in_string = !in_string;
	if ( *s == ';' && !in_string) {
	  *s = 0;
	  break;
	}
      }

      dcli_remove_blank( line, line);
      if ( line[strlen(line)-1] == '\r')
	line[strlen(line)-1] = 0;
      if ( strcmp( line, "") == 0 || (line[0] == '\r' && line[1] == '\0'))
	continue;
      
      while (1) {
	if ( line[strlen(line)-1] == '\\') {
	  // Add next line
	  sts = dcli_read_line( &line[strlen(line)-1], lsize-strlen(line), fp);
	  if ( !sts)
	    break;
	  line_cnt++;
	  
	  // Remove comments
	  in_string = 0;
	  for ( s = line; *s; s++) {
	    if ( *s == '"')
	      in_string = !in_string;
	    if ( *s == ';' && !in_string)
	      *s = 0;
	  }
	  dcli_remove_blank( line, line);
	}
	else if ( line[strlen(line)-2] == '\\') {
	  // Add next line
	  sts = dcli_read_line( &line[strlen(line)-2], lsize-strlen(line), fp);
	  if ( !sts)
	    break;
	  line_cnt++;
	  dcli_remove_blank( line, line);
	}
	else 
	  break;
      }
    }
    break;
  }
  return sts;
}

int pb_gsd::add_module_conf( pwr_tCid cid, pwr_tOid oid, char *name, char *module_name)
{
  int idx;
  gsd_sModule *mp;
  int i;
  int found;

  // Find first free module conf
  for ( idx = 0; idx < module_conf_cnt; idx++) {
    if ( module_conf[idx].module == 0)
      break;
  }
  if ( module_conf[idx].module != 0)
    // All is occupied
    return PB__MODOCC;

  module_conf[idx].oid = oid;
  module_conf[idx].cid = cid;
  strcpy( module_conf[idx].name, name);

  i = 0;
  found = 0;
  for ( mp = modulelist; mp; mp = mp->next) {
    if ( strcmp( module_name, mp->Mod_Name) == 0) {
      found = 1;
      break;
    }
    i++;
  }
  if ( !found)
    return 0;

  module_conf[idx].idx = i + 1;
  configure_module( &module_conf[idx]);
  return 1;
}

int pb_gsd::move_module_conf( gsd_sModuleConf *src, gsd_sModuleConf *dest)
{
  int src_idx = ((char *)src - (char *)module_conf) / sizeof(gsd_sModuleConf);
  int dest_idx;
  if ( !dest)
    dest_idx = 0;
  else 
    dest_idx = ((char *)dest - (char *)module_conf) / sizeof(gsd_sModuleConf);

  if ( src_idx == dest_idx)
    return 1;

  if ( src_idx < 0 || src_idx >= module_conf_cnt ||
       dest_idx < 0 || dest_idx >= module_conf_cnt)
    return 0;

  gsd_sModuleConf tmp = module_conf[src_idx];
  if ( src_idx > dest_idx) {
    for ( int i = src_idx-1; i >= dest_idx; i--) {
      module_conf[i+1] = module_conf[i];
    }
    module_conf[dest_idx] = tmp;
  }
  else {
    for ( int i = src_idx; i < dest_idx; i++) {
      module_conf[i] = module_conf[i+1];
    }
    module_conf[dest_idx] = tmp;
  }

  return 1;
}

int pb_gsd::copy_module_conf( gsd_sModuleConf *m)
{
  if ( copy_buff && copy_buff->prm_dataitems) {
    free( copy_buff->prm_dataitems);
  }
  else if ( !copy_buff)
    copy_buff = (gsd_sModuleConf *) calloc( 1, sizeof(gsd_sModuleConf));

  // Copy m to copy_buff
  *copy_buff = *m;
  copy_buff->oid = pwr_cNOid;
  if ( m->prm_dataitems) {
    copy_buff->prm_dataitems = (gsd_sPrmDataItem *) calloc( m->prm_dataitems_cnt,
							    sizeof(gsd_sPrmDataItem));
    memcpy( copy_buff->prm_dataitems, m->prm_dataitems, m->prm_dataitems_cnt *
	    sizeof(gsd_sPrmDataItem));
  }
  return 1;
}

int pb_gsd::cut_module_conf( gsd_sModuleConf *m)
{
  int idx = ((char *)m - (char *)module_conf) / sizeof(gsd_sModuleConf);

  if ( copy_buff && copy_buff->prm_dataitems) {
    free( copy_buff->prm_dataitems);
  }
  else if ( !copy_buff)
    copy_buff = (gsd_sModuleConf *) calloc( 1, sizeof(gsd_sModuleConf));

  // Copy m to copy_buff
  *copy_buff = *m;
  copy_buff->oid = pwr_cNOid;

  // Shift modules of higher index
  for ( int i = idx; i < module_conf_cnt-1; i++) {
    module_conf[i] = module_conf[i+1];
  }
  memset( &module_conf[module_conf_cnt-1], 0, sizeof(gsd_sModuleConf));
  return 1;
}

int pb_gsd::paste_module_conf( gsd_sModuleConf *m)
{
  int idx = ((char *)m - (char *)module_conf) / sizeof(gsd_sModuleConf);

  if ( !copy_buff)
    // Nothing copied
    return 0;

  // Last module will be lost
  if ( module_conf[module_conf_cnt-1].prm_dataitems)
    free( (char *) module_conf[module_conf_cnt-1].prm_dataitems);

  // Shift to make space for new module
  for ( int i = module_conf_cnt - 1; i >= idx + 1; i--)
    module_conf[i] = module_conf[i-1];
  
  // Insert module
  module_conf[idx] = *copy_buff;
  if ( copy_buff->prm_dataitems) {
    module_conf[idx].prm_dataitems = 
      (gsd_sPrmDataItem *) calloc( module_conf[idx].prm_dataitems_cnt, sizeof(gsd_sPrmDataItem));
    memcpy( module_conf[idx].prm_dataitems, copy_buff->prm_dataitems,
	    module_conf[idx].prm_dataitems_cnt * sizeof(gsd_sPrmDataItem));
  }
  return 1;
}

int pb_gsd::configure_module( gsd_sModuleConf *m)
{
  if ( m->idx == 0) {
    // Module removed
    if ( m->module)
      m->module = 0;
    if ( m->prm_dataitems) {
      free( m->prm_dataitems);
      m->prm_dataitems = 0;
      m->prm_dataitems_cnt = 0;
    }
  }
  else {
    int new_module = 1;
    gsd_sModule *mp;
    int i = 1;

    for ( mp = modulelist; mp; mp = mp->next) {
      if ( i == m->idx)
	break;
      i++;
    }
    if ( !mp) {
      printf( "** Module index out of range\n");
      return PB__MODULEIDX;
    }

    if ( m->module) {
      // Check if new module index

      if ( mp != m->module) {
	// New module configured, remove previous module
	if ( m->module)
	  m->module = 0;
	if ( m->prm_dataitems) {
	  free( m->prm_dataitems);
	  m->prm_dataitems = 0;
	  m->prm_dataitems_cnt = 0;
	}
      }
      else
	new_module = 0;
    }

    if ( new_module) {
      m->module = mp;

      // Create data items for module ExtPrmData
      m->prm_dataitems_cnt = 0;
      for ( gsd_sExtUserPrmDataRef *ep = mp->extuserprmdatareflist; ep; ep = ep->next)
	m->prm_dataitems_cnt++;

      m->prm_dataitems = (gsd_sPrmDataItem *) calloc( m->prm_dataitems_cnt,
						      sizeof(gsd_sPrmDataItem));
      int i = 0;
      for ( gsd_sExtUserPrmDataRef *ep = mp->extuserprmdatareflist; ep; ep = ep->next) {
	m->prm_dataitems[i].ref = ep;
	i++;
      }
      if ( m->module->extuserprmdataconst) {
	memcpy( m->prm_data, m->module->extuserprmdataconst->Const_Prm_Data,
		m->module->extuserprmdataconst->len);
 	prm_data_to_items( m->prm_dataitems, m->prm_dataitems_cnt, 
			   m->prm_data,
			   m->module->Ext_Module_Prm_Data_Len, 1);
	// Test Remove this !!!
	prm_items_to_data( m->prm_dataitems, m->prm_dataitems_cnt, 
			   m->prm_data,
			   m->module->Ext_Module_Prm_Data_Len);
      }
    }
  }

  if ( m->module) {
    if ( strcmp( m->name, "") == 0) {
      int idx = ((char *)m - (char *)module_conf) / sizeof(gsd_sModuleConf);
      sprintf( m->name, "M%d", idx+1);
    }
  }
  else
    strcpy( m->name, "");

  return 1;
}

void pb_gsd::pack_config( char *config, int *len)
{
  int i;
  short conf_idx;

  conf_idx = 2;
  for ( i = 0; i < module_conf_cnt; i++) {
    if ( !module_conf[i].module || !module_conf[i].module->Config)
      continue;

    memcpy( &config[conf_idx], module_conf[i].module->Config, 
	    module_conf[i].module->config_size);
    conf_idx += module_conf[i].module->config_size;
  }

  // Length in first two bytes
  config[0] = 0;
  config[1] = conf_idx;
  *len = conf_idx;
}

void pb_gsd::pack_ext_user_prm_data( char *data, int *len)
{
  int i;
  short data_idx;

  prm_items_to_data( prm_dataitems, prm_dataitems_cnt,
		     extuserprmdataconst->Const_Prm_Data,
		     items_user_prm_data_len);

  data_idx = 0;
  memcpy( &data[data_idx], extuserprmdataconst->Const_Prm_Data,
	  items_user_prm_data_len);
  data_idx += items_user_prm_data_len;

  for ( i = 0; i < module_conf_cnt; i++) {
    if ( !module_conf[i].module || !module_conf[i].module->extuserprmdataconst)
      continue;

    if ( module_conf[i].module->extuserprmdataconst)
      memcpy( module_conf[i].prm_data + 
	      module_conf[i].module->extuserprmdataconst->Const_Offset, 
	      module_conf[i].module->extuserprmdataconst->Const_Prm_Data,
	      module_conf[i].module->extuserprmdataconst->len);
    prm_items_to_data( module_conf[i].prm_dataitems, module_conf[i].prm_dataitems_cnt,
		     module_conf[i].prm_data,
		     module_conf[i].module->Ext_Module_Prm_Data_Len);
    memcpy( &data[data_idx], module_conf[i].prm_data,
	    module_conf[i].module->Ext_Module_Prm_Data_Len);
    data_idx += module_conf[i].module->Ext_Module_Prm_Data_Len;
  }
  *len = data_idx;
}

int pb_gsd::get_user_prm_data( char *data, int *len, int size)
{
  int sts;

  sts = get_ivalue( "User_Prm_Data_Len", len);
  if ( EVEN(sts)) {
    *len = 0;
    return 0;
  }

  sts = get_svalue( "User_Prm_Data", data, size);
  if ( EVEN(sts)) {
    *len = 0;
    return 0;
  }

  return 1;
}

int pb_gsd::unpack_ext_user_prm_data( char *data, int len)
{
  int i;
  short data_idx;

  data_idx = 0;
  memcpy( extuserprmdataconst->Const_Prm_Data, &data[data_idx], 
	  items_user_prm_data_len);
  data_idx += items_user_prm_data_len;

  prm_data_to_items( prm_dataitems, prm_dataitems_cnt,
		     extuserprmdataconst->Const_Prm_Data,
		     items_user_prm_data_len);

  for ( i = 0; i < module_conf_cnt; i++) {
    if ( !module_conf[i].module || !module_conf[i].module->extuserprmdataconst)
      continue;

    memcpy( module_conf[i].prm_data, &data[data_idx],
	    module_conf[i].module->Ext_Module_Prm_Data_Len);
    data_idx += module_conf[i].module->Ext_Module_Prm_Data_Len;

    prm_data_to_items( module_conf[i].prm_dataitems, module_conf[i].prm_dataitems_cnt,
		     module_conf[i].prm_data,
		     module_conf[i].module->Ext_Module_Prm_Data_Len);
  }


  if ( len != data_idx)
    return PB__USERPRMDATALEN;
  return 1;
}

// Remove spaces inside () and after ','
void pb_gsd::compress( char *line)
{
  char *tmp = (char *) malloc( strlen(line)+1);
  char *s, *t;
  int inpar = 0;
  int instr = 0;

  for ( s = line, t = tmp; *s; s++) {
    if ( *s == '(' && !instr)
      inpar = 1;
    else if ( *s == ')' && !instr)
      inpar = 0;
    else if ( *s == '"')
      instr = !instr;

    if ( inpar && *s == ' ')
      continue;

    if ( !instr && *s == ',') {
      while ( *(t-1) == ' ')
	t--;
      *t = *s;
      t++;
      s++;
      while ( *s == ' ')
	s++;
      s--;
      continue;
    }
    *t = *s;
    t++;
  }
  *t = 0;
  strcpy( line, tmp);
  free( tmp);
}

int pb_gsd::syntax_check( int *idx)
{
  for ( int i = 0; i < module_conf_cnt; i++) {
    if ( module_conf[i].module) {
      // Check that name is not null
      if ( strcmp( module_conf[i].name, "") == 0) {
	*idx = i;
	return PB__NOMODULENAME;
      }
      // Check that name is unic
      for ( int j = 0; j < module_conf_cnt; j++) {
	if ( i != j && 
	     cdh_NoCaseStrcmp( module_conf[i].name, module_conf[j].name) == 0) {
	  *idx = i;
	  return PB__DUPLMODULENAME;
	}
      }

      // Check that class is not null
      if ( module_conf[i].cid == pwr_cNCid) {
	*idx = i;
	return PB__NOMODULECLASS;
      }
    }
  }
  return PB__SUCCESS;
}

#if 0
int main()
{
  pb_gsd gsd;

  gsd.read("test.gsd");
  gsd.print();
}
#endif


