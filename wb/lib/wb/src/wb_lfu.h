/* 
 * Proview   $Id: wb_lfu.h,v 1.11 2008-06-25 07:56:04 claes Exp $
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
 **/

#ifndef wb_lfu_h
#define wb_lfu_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef co_wow_h
#include "co_wow.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	lfu_mDistrOpt_NoRootVolume 	= 1 << 0,
	lfu_mDistrOpt_RSH 		= 1 << 1
} lfu_mDistrOpt;

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
	pwr_tVid	vid;
	pwr_tTime      	version;
        char		name[80];
	} lfu_t_volref;

pwr_tStatus lfu_volumelist_load( char *filename, 
				 lfu_t_volumelist **vollist,
				 int *volcount);

pwr_tStatus lfu_create_loadfile( ldh_tSesContext ldhses);

pwr_tStatus lfu_create_bootfile( char *nodeconfigname,
				 lfu_t_volumelist *volumelist,
				 int volumecount,
				 int debug);

pwr_tStatus lfu_GetFileVersion( char *pattern, 
				int number_of_digits,
				int *version, 
				pwr_tTime *date);

pwr_tStatus lfu_GetPlcFileVersion( pwr_tVolumeId volumeid, 
				   int *version, 
				   pwr_tTime *date);

pwr_tStatus lfu_IncrementAndGetVersion( char *filename,
					unsigned long *current_version);

pwr_tStatus lfu_SaveDirectoryVolume( ldh_tSesContext ldhses,
				     CoWow *wow);

int lfu_create_bootfiles( char *nodestr,
			  int debug,
			  int allnodes);

pwr_tStatus lfu_ReadBootFile( char *filename,
			      pwr_tTime	*date,
			      char *systemname,
			      char *systemgroup,
			      pwr_tVolumeId **vollist,
			      pwr_tString40 **volnamelist,
			      int *volcount,
			      char *plc_name);

pwr_tStatus lfu_ReadSysObjectFile( char	*SystemName,
				   char	*SystemGroup);

pwr_tStatus lfu_WriteSysObjectFile( ldh_tSesContext	ldhses);

pwr_tStatus lfu_GetVolRef( char *filename,
			   lfu_t_volref **volref,
			   int *volref_count);
pwr_tStatus lfu_GetVolume( char *filename,
			   char *name,
			   pwr_tVid *vid,
			   pwr_tCid *cid,
			   pwr_tTime *time);
pwr_tStatus lfu_GetVolumeCnf( char *name, pwr_tVid *vid, pwr_tCid *cid, ldh_eVolRep *volrep,
			      char *server);

pwr_tStatus lfu_ParseDbmsServer( char *server, char *user, char *passw, 
				 unsigned int *port, char *host);

pwr_tStatus lfu_check_appl_file( char *nodename, int bus_number);
pwr_tStatus lfu_check_opt_file( char *nodename, int bus_number, pwr_mOpSys opsys);

#ifdef __cplusplus
}
#endif

#endif





















