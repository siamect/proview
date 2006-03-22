/* 
 * Proview   $Id: pb_gsd.h,v 1.1 2006-03-22 14:38:11 claes Exp $
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

#ifndef pb_gsd_h
#define pb_gsd_h

#ifndef pwr_h
# include "pwr.h"
#endif

typedef enum {
  gsd_DpSlave,
  gsd_DpMaster
} gsd_eDpType;

typedef enum {
  gsd_ProfibusDP,
  gsd_Boolean,
  gsd_Bit,
  gsd_BitArea,
  gsd_VString8,
  gsd_VString32,
  gsd_VString256,
  gsd_OString,
  gsd_Signed8,
  gsd_Signed16,
  gsd_Signed32,
  gsd_Unsigned8,
  gsd_Unsigned16,
  gsd_Unsigned32,
  gsd_BitVString32,
  gsd_BitVString256,
  gsd_DiagArea,
  gsd_DiagAreaEnd,
  gsd_Value,
  gsd_DiagType,
  gsd_Module,
  gsd_EndModule,
  gsd_DataAreaBegin,
  gsd_DataAreaEnd,
  gsd_SlaveFamily,
  gsd_UserPrmDataLen,
  gsd_UserPrmData,
  gsd_ExtModulePrmDataLen,
  gsd_PrmText,
  gsd_EndPrmText,
  gsd_ExtUserPrmData,
  gsd_EndExtUserPrmData,
  gsd_MaxUserPrmDataLen,
  gsd_ExtUserPrmDataRef,
  gsd_ExtUserPrmDataConst,
  gsd_Text,
  gsd_End
} gsd_eType;

typedef struct {
  char name[50];
  char group[50];
  int option;
  int type;
  unsigned int value_default;
} gsd_sKeyword;

typedef struct {
  char name[32];
  pwr_tCid cid;
} gsd_sModuleClass;

typedef struct {
  unsigned int value;
  void *data;
  int found;
} gsd_sData;

typedef struct sArea {
  char Area_Name[33];
  int Related_CFG_Identifier;
  int IO_Direction;
  int Length;
  int Consistency;
  int Publisher_allowed;
  int DP_Master_allowed;
  int Data_Type[100];
  int data_type_cnt;
  struct sArea *next;
} gsd_sArea;

typedef struct {
  int Const_Offset;
  unsigned char Const_Prm_Data[237];
  int len;
} gsd_sExtUserPrmDataConst;

typedef struct sText {
  int Prm_Data_Value;
  char Text[33];
  struct sText *next;
} gsd_sText;

typedef struct sPrmText {
  int Reference_Number;
  gsd_sText *text_list;
  struct sPrmText *next;
} gsd_sPrmText;

typedef struct sExtUserPrmData {
  int Reference_Number;
  char Ext_User_Prm_Data_Name[33];
  char Data_Type_Name[33];
  int Default_Value;
  int Min_Value;
  int Max_Value;
  int Allowed_Values[16];
  int allowed_cnt;
  int Prm_Text_Ref;
  int Changeable;
  int Visible;
  int data_type;
  int bitarea_min;
  int bitarea_max;
  int bit_num;
  gsd_sPrmText *prmtext;
  struct sExtUserPrmData *next;
} gsd_sExtUserPrmData;

typedef struct sExtUserPrmDataRef {
  int Reference_Offset;
  int Reference_Number;
  gsd_sExtUserPrmData *prm_data;
  sExtUserPrmDataRef *next;
} gsd_sExtUserPrmDataRef;

typedef struct {
  gsd_sExtUserPrmDataRef *ref;
  int value;
} gsd_sPrmDataItem;

typedef struct sModule {
  char Mod_Name[33];
  unsigned char *Config;
  int config_size;
  int Module_Reference;
  int Ext_Module_Prm_Data_Len;
  int X_Ext_Module_Prm_Data_Len;
  int F_Ext_Module_Prm_Data_Len;
  gsd_sExtUserPrmDataConst *extuserprmdataconst;
  gsd_sArea *arealist;
  gsd_sExtUserPrmDataRef *extuserprmdatareflist;
  struct sModule *next;
} gsd_sModule;

typedef struct {
  int idx;
  gsd_sModule *module;
  char name[80];
  pwr_tOid oid;
  pwr_tCid cid;
  gsd_sPrmDataItem *prm_dataitems;
  int prm_dataitems_cnt;
} gsd_sModuleConf;

class pb_gsd {
 public:
  pb_gsd();
  ~pb_gsd();
    
  int dptype;
  int modular_station;
  int max_module;
  int user_prm_data_len;
  int max_user_prm_data_len;
  int line_cnt;
  int status;
  gsd_sData *datalist;
  gsd_sModule *modulelist;
  gsd_sPrmText *prm_textlist;
  gsd_sExtUserPrmData *extuserprmdatalist;
  gsd_sExtUserPrmDataRef *extuserprmdatareflist;
  gsd_sModule *current_module;
  gsd_sArea *current_area;
  gsd_sPrmText *current_prm_text;
  gsd_sExtUserPrmData *current_extuserprmdata;
  gsd_sExtUserPrmDataConst *extuserprmdataconst;
  gsd_sPrmDataItem *prm_dataitems;
  int prm_dataitems_cnt;
  gsd_sModuleConf *module_conf;
  int module_conf_cnt;
  gsd_sModuleClass *module_classlist;
  gsd_sModuleConf *copy_buff;
  int modified;

  static gsd_sKeyword keywordlist[];

  int read( char *filename);
  int read_line( char *line, int lsize, FILE *fp);
  int str_to_ostring( unsigned char **data, char *str, int size, int *rsize);
  int prm_text_val_to_str( gsd_sPrmText *prmtext, int val, char *str);
  int print();
  int build();
  int configure_module( gsd_sModuleConf *mclist);
  int prm_items_to_data( gsd_sPrmDataItem *item, int item_size, unsigned char *data, 
			 int data_size);
  int prm_data_to_items( gsd_sPrmDataItem *item, int item_size, unsigned char *data, 
			 int data_size, int set_default = 0);
  void set_classes( gsd_sModuleClass *mclist) { module_classlist = mclist;}
  int get_svalue( char *key, char *value, int size);
  int get_ivalue( char *key, int *value);
  int add_module_conf( pwr_tCid cid, pwr_tOid oid, char *name, char *module_name); 
  int move_module_conf( gsd_sModuleConf *src, gsd_sModuleConf *dest);
  int copy_module_conf( gsd_sModuleConf *m);
  int cut_module_conf( gsd_sModuleConf *m);
  int paste_module_conf( gsd_sModuleConf *m);
  void pack_config( char *config, int *len);
  int unpack_ext_user_prm_data( char *data, int len);
  void pack_ext_user_prm_data( char *data, int *len);
  void compress( char *line);
  int syntax_check( int *idx);
  void set_modified( int mod) { modified = mod;}
  int is_modified() { return modified;}
};

#endif

