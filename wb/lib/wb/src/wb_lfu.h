#ifndef wb_lfu_h
#define wb_lfu_h

/* wb_lfu.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	lfu_eDistrSts_Normal 		= 0,
	lfu_eDistrSts_NoRootVolume 	= 1 << 0
} lfu_eDistrSts;

typedef enum {
  lfu_mDistrComponents_UserDatabase     = 1 << 0,
  lfu_mDistrComponents_LoadFiles       	= 1 << 1,
  lfu_mDistrComponents_ApplFile		= 1 << 2,
  lfu_mDistrComponents_PwrpAliasFile   	= 1 << 3,
  lfu_mDistrComponents_IncludeFiles    	= 1 << 4,
  lfu_mDistrComponents_GraphFiles     	= 1 << 5,
  lfu_mDistrComponents_XttHelpFile     	= 1 << 6,
  lfu_mDistrComponents_XttResourceFile 	= 1 << 7,
  lfu_mDistrComponents_XttSetupFile    	= 1 << 8,
  lfu_mDistrComponents_FlowFiles        = 1 << 9,
  lfu_mDistrComponents_RHostsFile      	= 1 << 10,
  lfu_mDistrComponents_WebFiles        	= 1 << 11,
  lfu_mDistrComponents_PwrpStop        	= 1 << 12
} lfu_mDistrComponents;

typedef enum {
	lfu_eAccessType_StdLgi 		= 0,
	lfu_eAccessType_Proxy 		= 1,
	lfu_eAccessType_Local 		= 2
} lfu_eAccessType;

typedef struct {
	pwr_tString40	volume_name;
	pwr_tVolumeId	volume_id;
	pwr_tString40	p1;
	pwr_tString40	p2;
	pwr_tString40	p3;
	pwr_tString40	p4;
	pwr_tString40	p5;
	pwr_tString40	p6;
	} lfu_t_volumelist;

typedef struct {
	pwr_tVolumeId	volid;
	int		version;
	pwr_tClassId	classid;
	int		file_checked;
	} lfu_t_volref;

pwr_tStatus lfu_volumelist_load( 
	char *filename,
	lfu_t_volumelist **vollist,
	int *volcount
);

pwr_tStatus lfu_create_loadfile(
  ldh_tSesContext	ldhses
);

pwr_tStatus lfu_create_bootfile(
	char		*nodeconfigname,
	lfu_t_volumelist *volumelist,
	int		volumecount,
	int		debug
);

pwr_tStatus lfu_GetFileVersion( 
  char *pattern, 
  int number_of_digits,
  int *version, 
  pwr_tTime *date
);

pwr_tStatus lfu_GetPlcFileVersion(
  pwr_tVolumeId volumeid, 
  int *version, 
  pwr_tTime *date
);

pwr_tStatus lfu_IncrementAndGetVersion(
  char	*filename,
  unsigned long	*current_version
);

pwr_tStatus lfu_SaveDirectoryVolume(
  ldh_tSesContext 	ldhses,
  Widget		parent_window
);

int lfu_create_bootfiles (
  char		*nodestr,
  int		debug,
  int		allnodes
);

pwr_tStatus lfu_ReadBootFile(
  char		*filename,
  pwr_tTime	*date,
  char		*systemname,
  char		*systemgroup,
  pwr_tVolumeId **vollist,
  pwr_tString40	**volnamelist,
  int		*volcount,
  char		*plc_name
);

pwr_tStatus lfu_ReadSysObjectFile(
	char	*SystemName,
	char	*SystemGroup
);

pwr_tStatus lfu_WriteSysObjectFile(
	ldh_tSesContext	ldhses
);

pwr_tStatus	lfu_GetVolRef( 		char *filename,
			       	char *volname,
				pwr_tClassId *volclass,
			       	pwr_tTime *createtime,
			       	lfu_t_volref **volref,
	       			int *volref_count);

#ifdef __cplusplus
}
#endif

#endif





















