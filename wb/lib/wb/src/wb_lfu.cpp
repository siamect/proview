/* 
 * Proview   $Id: wb_lfu.cpp,v 1.10 2008-02-04 13:34:49 claes Exp $
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

/* wb_lfu.cpp -- load file utitities
   Utitlites to handle loadfiles, bootfiles, volumelists etc */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#if defined OS_VMS
#include <starlet.h>
#include <libdef.h>
#include <libdtdef.h>
#include <lib$routines.h>
#include <clidef.h>
#include <descrip.h>
#endif
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "wb.h"
#include "flow.h"
#include "flow_ctx.h"
#include "rt_load.h"
#include "co_wow.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_regex.h"
#include "wb_ldh.h"
#include "wb_utl_api.h"
#include "wb_dir.h"
#include "wb_lfu_msg.h"
#include "wb_foe_msg.h"
#include "wb.h"
#include "wb_gcg.h"
#include "wb_trv.h"
#include "wb_lfu.h"
#include "co_dbs.h"
#include "co_msgwindow.h"
#include "co_cnf.h"

#include "wb_env.h"
#include "wb_erep.h"
#include "wb_session.h"
#include "wb_object.h"
#include "wb_vrepdb.h"


#define LFU_MAX_NODE_VOLUMES  	100
#define LFU_MAX_BUS  		50
#define LFU_MAX_VOLREF 		40

class lfu_nodeconf {
public:
  lfu_nodeconf() : isfriend(0), port(0), vid(0)
  { strcpy( address, ""); strcpy( nodename, "");}
  int isfriend;
  pwr_tOid oid;
  pwr_tEnum operatingsystem;
  pwr_tString80 nodename;
  pwr_tString16 address;
  pwr_tUInt32 port;
  pwr_tVid vid;
};
      

typedef struct {
  char name[32];
  pwr_tVid vid;
  pwr_tCid cid;
  ldh_eVolRep volrep;
  char server[80];
  ldh_tSesContext ldhses;
} lfu_sCreaDb;

static void lfu_creadb_qb_yes ( 
  void *ctx,
  void *data
);

pwr_tStatus lfu_volumelist_load( 
	char	*filename,
	lfu_t_volumelist **vollist,
	int 	*volcount
)
{
	FILE	*file;
	char	line[200];
	char	param[8][40];
	int	nr;
	int	line_count;
	int	list_alloc;
	lfu_t_volumelist *list;
	lfu_t_volumelist *list_ptr;
	int	sts;
	int	count;
	int	syntax_error;
	char	fname[120];

	syntax_error = 0;
	dcli_translate_filename( fname, filename);
	file = fopen( fname, "r");
	if ( !file)
	{
	  printf( "-- Error, unable to open file '%s'\n", fname);
	  *volcount = 0;
	  return LFU__NOFILE;
	}

	list_alloc = 0;
	line_count = 0;
	count = 0;
	while( 
	   ODD( sts = utl_read_line( line, sizeof(line), file, &line_count)))
	{
	  nr = utl_parse( line, " 	", "", (char *)param, 
		sizeof( param) / sizeof( param[0]), 
		sizeof( param[0]));
	  if ( nr < 3)
	  {
	    printf("-- Syntax error in file '%s' line %d, \n", filename, line_count);
	    printf("        \"%s\"\n", line);
	    count++;
	    syntax_error = 1;
	    continue;
	  }
	  utl_realloc_s( (char **) &list, count, sizeof( *list),
			&list_alloc);
	  list_ptr = list + count;
	  strcpy( list_ptr->volume_name, param[0]);
	  if ( nr > 2)
	    strcpy( list_ptr->p1, param[2]);
	  else
	    strcpy( list_ptr->p1, "");
	  if ( nr > 3)
	    strcpy( list_ptr->p2, param[3]);
	  else
	    strcpy( list_ptr->p2, "");
	  if ( nr > 4)
	    strcpy( list_ptr->p3, param[4]);
	  else
	    strcpy( list_ptr->p3, "");
	  if ( nr > 5)
	    strcpy( list_ptr->p4, param[5]);
	  else
	    strcpy( list_ptr->p4, "");
	  if ( nr > 6)
	    strcpy( list_ptr->p5, param[6]);
	  else
	    strcpy( list_ptr->p5, "");
	  if ( nr > 7)
	    strcpy( list_ptr->p6, param[7]);
	  else
	    strcpy( list_ptr->p6, "");
	  sts = cdh_StringToVolumeId( param[1], &list_ptr->volume_id);
	  if ( EVEN(sts))
	  {
	    printf("-- Syntax error in file '%s' line %d, \n", filename, line_count);
	    printf("        \"%s\"\n", line);
	    count++;
	    syntax_error = 1;
	    continue;
	  }
	  count++;
	}
	fclose( file);

	*vollist = list;
	*volcount = count;
	if ( syntax_error)
	  return LFU__VOLLIST;
	else
	  return LFU__SUCCESS;
}

/************************************************************************
*
* Name: 	lfu_create_loadfile
*
* Type:	void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create loadfile for a volume.
*
*************************************************************************/
pwr_tStatus lfu_create_loadfile(
  ldh_tSesContext	ldhses
)
{
	pwr_tStatus	sts;
	pwr_tClassId	cid;
	ldh_sVolumeInfo volinfo;

	sts = ldh_GetVolumeInfo( ldh_SessionToVol( ldhses), &volinfo);
	if (EVEN(sts)) return sts;
	
	sts = ldh_GetVolumeClass( ldh_SessionToWB( ldhses), 
			volinfo.Volume, &cid);

	if ( cid == pwr_eClass_RootVolume ||
	     cid == pwr_eClass_SubVolume )
	{
	  sts = gcg_comp_volume( ldhses);
	  if (EVEN(sts)) return sts;
	}

	return LFU__SUCCESS;
}

/************************************************************************
*
* Name: 	lfu_create_bootfile
*
* Type:	void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create bootfile for one node.
*
*************************************************************************/
pwr_tStatus lfu_create_bootfile(
	char		*nodeconfigname,
	lfu_t_volumelist *volumelist,
	int		volumecount,
	int		debug)
{
	int j;
	FILE *file;
	lfu_t_volumelist *volumelist_ptr;
	lfu_t_volumelist *first_volumelist_ptr;
	pwr_tString40 nodename;
	pwr_tStatus sts;
	char filename[120];
	pwr_tUInt32 bus;
	pwr_tVolumeId node_vollist[ LFU_MAX_NODE_VOLUMES];
	int	node_vollist_count;
	pwr_mOpSys 	    os;
	unsigned long errorcount;
	unsigned long warningcount;
	unsigned long plc_version;
	pwr_tFloat32 single_scantime;
	char nodeconfigname_upper[80];
	char vollistname_upper[80];
	char systemname[80];
	char systemgroup[80];
	char timstr[32];
	char plcname[80];

	volumelist_ptr = volumelist;
	utl_toupper( nodeconfigname_upper, nodeconfigname);
        for ( j = 0; j < volumecount; j++)
        {
	  utl_toupper( vollistname_upper, volumelist_ptr->p1);
          if ( !strcmp( nodeconfigname_upper, vollistname_upper)) {
	    strcpy( nodename, volumelist_ptr->p2);
	    sscanf( volumelist_ptr->p3, "%d", &bus);
	    sscanf( volumelist_ptr->p4, "%d", (int *)&os);
	    sscanf( volumelist_ptr->p5, "%f", &single_scantime);

	    node_vollist_count = 0;
	    first_volumelist_ptr = volumelist_ptr;

	    while( !strcmp( nodeconfigname_upper, vollistname_upper)) {
	      if ( node_vollist_count < LFU_MAX_NODE_VOLUMES) {
	        node_vollist[node_vollist_count] = volumelist_ptr->volume_id;
	        node_vollist_count++;
	      }
	      else {
	        printf( "** Error, max number of volumes exceeded\n");
	        return LFU__NOFILE;
	      }
              volumelist_ptr++;
	      utl_toupper( vollistname_upper, volumelist_ptr->p1);
	    }

	    /* Get data for plc and print volumes on terminal */
	    printf( "-- Creating bootfile for node %s\n", nodename);
	    sprintf( filename, load_cNamePlcVersion, nodeconfigname);
	    cdh_ToLower( filename, filename);
	    sts = lfu_IncrementAndGetVersion( filename, &plc_version);
	    if ( EVEN(sts)) return sts;

	    sprintf( plcname, load_cNamePlc, "", cdh_Low(nodename), bus, (int)plc_version);
	    printf( "      %s\n", plcname);

	    sts = gcg_comp_rtnode( nodename, os,
			bus, 1, &errorcount, &warningcount, debug, node_vollist, 
			node_vollist_count, plc_version, single_scantime);
	    if ( EVEN(sts)) return sts;
	    if ( sts == GSX__NOPLC)
	      plc_version = 0;

	    /* Open the file and print boot data */
	    sprintf( filename, load_cNameBoot, 
			load_cDirectory, cdh_Low(nodename), bus);
	    dcli_translate_filename( filename, filename);
	    file = fopen( filename, "w");
	    if ( !file) {
	      printf( "** Error, Unable to open bootfile, %s", filename);
	      return LFU__NOFILE;
	    }

	    time_AtoAscii(NULL, time_eFormat_DateAndTime, timstr,sizeof(timstr));
	    fprintf( file, "%s\n", timstr);

	    sts = lfu_ReadSysObjectFile( systemname, systemgroup);
	    if ( EVEN(sts)) {
	      fprintf( file, "\n");
	      fprintf( file, "\n");
	    }
	    else {
	      fprintf( file, "%s\n", systemname);
	      fprintf( file, "%s\n", systemgroup);
	    }

	    fprintf( file, "%s %s\n", plcname, cdh_VolumeIdToString( NULL, 0, 0, 0));

	    node_vollist_count = 0;
	    volumelist_ptr = first_volumelist_ptr;
	    utl_toupper( vollistname_upper, volumelist_ptr->p1);
	    while( !strcmp( nodeconfigname_upper, vollistname_upper)) {
	      fprintf( file, "%s %s\n",
		       volumelist_ptr->volume_name,
		       cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0));
              volumelist_ptr++;
	      utl_toupper( vollistname_upper, volumelist_ptr->p1);
	    }
	    fclose( file);

	    break;
          }
          volumelist_ptr++;
        }
	return LFU__SUCCESS;	  
}


/*************************************************************************
*
* Name:		lfu_GetPlcFileVersion
*
* Type		pwr_tStatus
*
* Description: 
*	Find the highest version of a plc-file.
*
* Parameters
*
**************************************************************************/
pwr_tStatus lfu_GetPlcFileVersion(
  pwr_tVolumeId volumeid, 
  int *version, 
  pwr_tTime *date
)
{
	pwr_tStatus sts;
	char	filename[80];
	unsigned char	volid[4];

	memcpy( &volid, &volumeid, sizeof(volid));
#if defined OS_VMS
	sprintf( filename, "pwrp_load:plc_%3.3u_%3.3u_%3.3u_%3.3u_%%%%%%%%%%.exe.0",
		volid[3], volid[2], volid[1], volid[0]);
#else
	sprintf( filename, "pwrp_load:plc_%3.3u_%3.3u_%3.3u_%3.3u_*.exe",
		volid[3], volid[2], volid[1], volid[0]);
#endif
	sts = lfu_GetFileVersion( filename, 5, version, date);
	return sts;
}


/*************************************************************************
*
* Name:		lfu_GetFileVersion
*
* Type		pwr_tStatus
*
* Description: 
*		Finds the highest version for a file, and returns
*		proview-version and date for this file.
*		Filename is given as a pattern, and the version
*		has to be in the filename befor the extention
*		with the number of digits given in num_of_digits.
*		ex "pwrp_load:ld_vol_000_000_001_001_%%%%%.dat.0"
*
* Parameters
*
**************************************************************************/

pwr_tStatus lfu_GetFileVersion( 
  char *pattern, 
  int number_of_digits,
  int *version, 
  pwr_tTime *date
)
{
	pwr_tStatus sts;
	char	found_file[160];
	pwr_tTime	found_date;
	int		vms_vers;
	int		size;
	char		version_str[10];
	int		highest_version = 0;
	char		highest_filename[80] = "";
	pwr_tTime	highest_date = {0,0};
	int		found;
	int		nr;
	int		found_version;
	char		*s;
	unsigned long	search_ctx;
	char		trn_pattern[80];

	found = 0;
	search_ctx = 0;
	dcli_translate_filename( trn_pattern, pattern);
	sts = dcli_search_file( trn_pattern, found_file, DCLI_DIR_SEARCH_INIT);
	while( ODD(sts))
	{
	  found = 1;

	  sts = dir_get_fileinfo( found_file, &found_date, &size, &vms_vers,
		NULL);
	  if ( EVEN(sts)) return sts;

	  /* Get the version from the file name */
	  s = strrchr( found_file, '.');
	  s -= number_of_digits;
	  strncpy( version_str, s, number_of_digits);
	  version_str[number_of_digits] = 0;
	  nr = sscanf( version_str, "%d", &found_version);
	  if ( nr == 1)
	  {
	    if ( found_version > highest_version)
	    {
	      highest_version = found_version;
	      if (time_Acomp(&found_date, &highest_date) < 0)
	      {
	        printf( "** Warning, %s has later creation time but lower version than %s\n",
 			highest_filename, found_file);
	      }
	      highest_date = found_date;
	      strcpy( highest_filename, found_file);
	    }
	    else
	    {
	      if (time_Acomp(&found_date, &highest_date) > 0)
	      {
	        printf( "** Warning, %s has later creation time but lower version than %s\n",
			found_file, highest_filename);
	      }
	    }
	  }
	  sts = dcli_search_file( trn_pattern, found_file, DCLI_DIR_SEARCH_NEXT);
	}
	sts = dcli_search_file( trn_pattern, found_file, DCLI_DIR_SEARCH_END);
	*version = highest_version;
	*date = highest_date;
	if ( !found)
	  return sts;

	return 1;
}

pwr_tStatus lfu_IncrementAndGetVersion(
  char	*filename,
  unsigned long	*current_version)
{
	FILE 	*file;
	int	nr;
	fpos_t	pos;
	unsigned long	version;
	char	fname[120];

	dcli_translate_filename( fname, filename);
	file = fopen( fname, "r+");
	if ( !file)
	{
	  file = fopen( fname, "w");
	  if ( !file ) return LFU__NOFILE;
	  version = 1;
	}
	else
	{
	  fgetpos( file, &pos);
	  nr = fscanf( file, "%ld", &version);
	  if ( nr != 1) return LFU__PLCVERSION;

	  version++;
	  fsetpos( file, &pos);
	}
	fprintf( file, "%ld", version);
	fclose(file);
	*current_version = version;
	return 1;
}


/*************************************************************************
*
* Name:		lfu_ReadSysObjectFile
*
* Type		pwr_tStatus
*
* Description: 
*		Read info of the systemobject in the directory db.
*
**************************************************************************/

pwr_tStatus lfu_ReadSysObjectFile(
	char	*SystemName,
	char	*SystemGroup
)
{
	FILE 	*file;
	int	nr;
	char	fname[120];

	dcli_translate_filename( fname, load_cNameSysObject);
	file = fopen( fname, "r");
	if ( file == 0)
	{
          strcpy( SystemName, "");
          strcpy( SystemGroup, "");
          return LFU__NOFILE;
	}
	else
	{
	  nr = fscanf( file, "%s", SystemName);
	  if ( nr != 1) return LFU__FILECRP;

	  nr = fscanf( file, "%s", SystemGroup);
//	  if ( nr != 1) return LFU__FILECRP;
	}
	fclose(file);
	return LFU__SUCCESS;
}

pwr_tStatus lfu_WriteSysObjectFile(
	ldh_tSesContext	ldhses
)
{
	FILE 	*file;
	int	sts;
	pwr_tObjid	systemobjid;
	char		systemname[80];
	char		systemgroup[80];
	char		fname[120];

	/* Get system object */
	sts = utl_get_systemobject( ldhses, &systemobjid, systemname,
		systemgroup);
	if ( EVEN(sts)) return sts;

	dcli_translate_filename( fname, load_cNameSysObject);
	file = fopen( fname, "w");
	if ( !file )
	{
	  printf( "** Error, unable to open file %s\n",
			load_cNameSysObject);
	  return LFU__NOFILE;
	}
	fprintf( file, "%s\n", systemname);
	fprintf( file, "%s", systemgroup);
	fclose(file);
	return LFU__SUCCESS;
}


/************************************************************************
*
* Name: lfu_save_directoryvolume
*
* Type: int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Syntax control and file generation when a directory volume is saved.
*
*************************************************************************/
pwr_tStatus lfu_SaveDirectoryVolume(
  ldh_tSesContext 	ldhses,
  CoWow			*wow
)
{
  pwr_tStatus 	sts;
  lfu_t_volumelist *volumelist;
  lfu_t_volumelist *volumelist_ptr;
  int		volumecount;	
  int		i;
  pwr_tString80 volname;
  pwr_tString80 volume_name;
  pwr_tString80 name;
  pwr_tString80	nodeconfig_name;
  pwr_tString80	bus_name;
  int		syntax_error;
  int		found;
  pwr_tObjid	envobjid;
  pwr_tObjid	dbobjid;
  pwr_tClassId	cid;
  pwr_tObjid	volobjid;
  int		size;
  pwr_tString80	*path_ptr;
  pwr_tString80	classname;
  FILE		*file;
  FILE          *fpath;
  pwr_tObjid	nodeobjid;
  pwr_tObjid	busobjid;
  char		*nodename_ptr;
  char		*bootnode_ptr;
  pwr_tUInt32	*os_ptr;
  pwr_tUInt32	os;
  pwr_tUInt32	*bus_number_ptr;
  pwr_tBoolean	*single_scan_ptr;
  pwr_tFloat32	*scantime_ptr;
  pwr_tFloat32	scantime;
  pwr_tBoolean	*distr_disable_ptr;
  char		*null_nodename = "-";
  char		filename[200];
  char		found_file[200];
  char		text[200];
  pwr_tObjid	systemobjid;
  char		systemname[80];
  char		systemgroup[80];
  char		os_str[20];
  pwr_tObjid	applobjid;
  char		appl_name[80];
  char		*source_ptr;
  char		*target_ptr;
  char		*targetnode_ptr;
  char		*targetproject_ptr;
  pwr_tClassId	class_vect[4];
  utl_t_objidlist *obj_ptr;
  utl_t_objidlist *objlist;
  int		objcount;
  trv_tCtx	trvctx;
  pwr_tObjid	distrobjid;
  pwr_tVolumeId	volume_id;
  lfu_eDistrSts	distr_status;
  char		fname[200];
  char          path[80];
  int           path_file_created = 0;

  syntax_error = 0;

  /* Write system object */
  sts = lfu_WriteSysObjectFile( ldhses);
  if ( sts == LFU__NOFILE)
    syntax_error = 1;

  /* Load the volume list */
  sts = lfu_volumelist_load( load_cNameGblVolumeList,  &volumelist, 
		&volumecount);
  if (EVEN(sts))
  {
    syntax_error = 1;
    if (sts == LFU__NOFILE)
      return sts;
  }

  dcli_translate_filename( fname, load_cNameVolumeList);
  file = fopen( fname, "w");
  if ( file == 0)
  {
    printf("** Error, Unable to open file %s\n", fname);
    return LFU__NOFILE;
  }

  /* Check that the configured volumes exist */
  for ( int k = 0; k < 2; k++) {
    // Check class volumes in the first round, and other volumes after

    for ( sts = ldh_GetRootList( ldhses, &envobjid);
	  ODD(sts);
	  sts = ldh_GetNextSibling( ldhses, envobjid, &envobjid)) {

      sts = ldh_GetObjectClass( ldhses, envobjid, &cid);
      if ( EVEN(sts)) return sts;

      if ( cid == pwr_cClass_WbEnvironment) {
	if ( k != 1)
	  continue;

	/* Create directory list file */
	sts = ldh_ObjidToName( ldhses, envobjid, ldh_eName_Object,
			       name, sizeof(name), &size);
	if ( EVEN(sts)) return sts;
	
	sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				"Path", (char **) &path_ptr, &size);
	if ( EVEN(sts)) return sts;
	
	dcli_translate_filename( filename, load_cNameFilePath);

	fpath = fopen( filename, "w");
	if ( !fpath) {
	  printf( "** Unable to open path file %s\n", filename);
	  syntax_error = 1;
	  free( (char *)path_ptr);
	  return LFU__NOFILE;
	}

	path_file_created = 1;

	for ( i = 0; i < 20; i++) {
	  dcli_trim( path, path_ptr[i]);
	  if ( strcmp( path, "") != 0)
	    fprintf( fpath, "%s\n", path);
	}
	fclose( fpath);
	free( (char *)path_ptr);

	// Get xxxVolumeLoad objects
	sts = ldh_GetChild( ldhses, envobjid, &volobjid);
	while ( ODD(sts)) {
	  sts = ldh_GetObjectClass( ldhses, volobjid, &cid);
	  if ( EVEN(sts)) return sts;

	  if ( cid == pwr_cClass_RootVolumeLoad ||
	       cid == pwr_cClass_SubVolumeLoad ||
	       cid == pwr_cClass_ClassVolumeLoad ||
	       cid == pwr_cClass_SharedVolumeLoad ) {
	    sts = ldh_ObjidToName( ldhses, volobjid, ldh_eName_Object,
				   volume_name, sizeof(volume_name), &size);
	    if ( EVEN(sts)) return sts;
	    utl_toupper( name, volume_name);

	    /* Check that the name is in the global volume list */
	    found = 0;
	    volumelist_ptr = volumelist;
	    for ( i = 0; i < volumecount; i++) {
	      utl_toupper( volname, volumelist_ptr->volume_name);
	      if ( !strcmp( name, volname)) {
		found = 1;
	      
		switch (cid) {
	        case pwr_cClass_RootVolumeLoad :
	          strcpy( classname, "RootVolume");
	          break;
	        case pwr_cClass_SubVolumeLoad :
	          strcpy( classname, "SubVolume");
	          break;
	        case pwr_cClass_ClassVolumeLoad :
	          strcpy( classname, "ClassVolume");
	          break;
	        case pwr_cClass_SharedVolumeLoad :
	          strcpy( classname, "SharedVolume");
	          break;
		}

		fprintf( file, "%s %s %s load\n",
			 volume_name,
			 cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			 classname);
		break;
	      }
	      volumelist_ptr++;
	    }
	    if ( !found) {
	      char msg[200];
	      sprintf( msg, "Error in VolumeConfig object,  '%s' is not configured in the global\
 volume list", name);
	      MsgWindow::message( 'E', msg, msgw_ePop_Default);
	      syntax_error = 1;
	    }
	  }
	  sts = ldh_GetNextSibling( ldhses, volobjid, &volobjid);
	}
      }
      else if ( cid == pwr_cClass_RootVolumeConfig ||
		cid == pwr_cClass_SubVolumeConfig ||
		cid == pwr_cClass_ClassVolumeConfig ||
		cid == pwr_cClass_SharedVolumeConfig ||
		cid == pwr_cClass_ExternVolumeConfig ) {
	if ( cid != pwr_cClass_ClassVolumeConfig && k == 0)
	  continue;
	if ( cid == pwr_cClass_ClassVolumeConfig && k == 1)
	  continue;

	sts = ldh_ObjidToName( ldhses, envobjid, ldh_eName_Object,
			       volume_name, sizeof(volume_name), &size);
	if ( EVEN(sts)) return sts;
	utl_toupper( name, volume_name);

	/* Check that the name is in the global volume list */
	found = 0;
	volumelist_ptr = volumelist;
	for ( i = 0; i < volumecount; i++) {
	  utl_toupper( volname, volumelist_ptr->volume_name);
	  if ( !strcmp( name, volname)) {
	    found = 1;
	      
	    switch (cid) {
  	    case pwr_cClass_RootVolumeConfig :
	      strcpy( classname, "RootVolume");
	      break;
	    case pwr_cClass_SubVolumeConfig :
	      strcpy( classname, "SubVolume");
	      break;
	    case pwr_cClass_ClassVolumeConfig :
	      strcpy( classname, "ClassVolume");
	      break;
	    case pwr_cClass_SharedVolumeConfig :
	      strcpy( classname, "SharedVolume");
	      break;
	    case pwr_cClass_ExternVolumeConfig :
	      strcpy( classname, "ExternVolume");
	      break;
	    }
	    
	    switch (cid) {
	    case pwr_cClass_RootVolumeConfig :
	    case pwr_cClass_SubVolumeConfig :
	    case pwr_cClass_ClassVolumeConfig :
	    case pwr_cClass_SharedVolumeConfig : {
	      int *dbenum = 0;
	      char *server = 0;

	      sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				"Database", (char **) &dbenum, &size);
	      if ( EVEN(sts)) return sts;

	      if (( cid == pwr_cClass_ClassVolumeConfig && *dbenum == 2) ||
		  ( cid != pwr_cClass_ClassVolumeConfig && *dbenum == 1)) {
		sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
					"Server", (char **) &server, &size);
		if ( EVEN(sts)) return sts;
	      }
									 
	      fprintf( file, "%s %s %s cnf %d",
		       volume_name,
		       cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
		       classname, *dbenum);
	      if ( server) {
		fprintf( file, " %s\n", server);
		free( server);
	      }
	      else
		fprintf( file, "\n");
	      free( dbenum);
	      break;
	    }
	    case pwr_cClass_ExternVolumeConfig : {
	      char *devprovider;
	      char *rtprovider;
	      
	      sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				      "DevProvider", (char **)&devprovider, &size);
	      if ( EVEN(sts)) return sts;
	      sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				      "RtProvider", (char **)&rtprovider, &size);
	      if ( EVEN(sts)) return sts;

	      fprintf( file, "%s %s %s cnf %s %s\n",
		       volume_name,
		       cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
		       classname, devprovider, rtprovider);
	      free( devprovider);
	      free( rtprovider);
	      break;
	    }
	    }
	    break;
	  }
	  volumelist_ptr++;
	}

	if ( !found) {
	  char msg[200];
	  sprintf( msg, "Error in VolumeConfig object,  '%s' is not configured in the global\
 volume list", name);
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  syntax_error = 1;
	}
	else {
	  switch (cid) {
	  case pwr_cClass_RootVolumeConfig :
	  case pwr_cClass_SubVolumeConfig :
	  case pwr_cClass_SharedVolumeConfig : {
	    /* Check if the databas is created */
	    int *dbenum_p = 0;
	    char *server_p = 0;
	    ldh_eVolRep volrep;
	    pwr_tCid vcid;
	    pwr_tString40 server = "";
	    
	    switch ( cid) {
	    case pwr_cClass_RootVolumeConfig:
	      vcid = pwr_eClass_RootVolume;
	      break;
	    case pwr_cClass_SubVolumeConfig:
	      vcid = pwr_eClass_SubVolume;
	      break;
	    case pwr_cClass_SharedVolumeConfig:
	      vcid = pwr_eClass_SharedVolume;
	      break;
	    default: ;
	    }

	    sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				    "Database", (char **) &dbenum_p, &size);
	    if ( EVEN(sts)) return sts;

	    if ( *dbenum_p == pwr_eVolumeDatabaseEnum_BerkeleyDb)
	      volrep = ldh_eVolRep_Db;
	    else if ( *dbenum_p == pwr_eVolumeDatabaseEnum_MySql)
	      volrep = ldh_eVolRep_Dbms;
	    else {
	      free( dbenum_p);
	      break;
	    }

	    free( dbenum_p);

	    if ( volrep == ldh_eVolRep_Db) {
	      sprintf( filename, "$pwrp_db/%s.db/info", volume_name);
	      cdh_ToLower( filename, filename);
	      dcli_translate_filename( filename, filename);
	      sts = dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_INIT);
	      dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
	      if (EVEN(sts)) {
		if ( wow) {
		  lfu_sCreaDb *data;
		  data = (lfu_sCreaDb *) calloc( 1, sizeof(*data));
		  strcpy( data->name, volumelist_ptr->volume_name);
		  data->vid = volumelist_ptr->volume_id;
		  data->cid = vcid;
		  data->ldhses = ldhses;
		  data->volrep = volrep;
		  strcpy( data->server, "");

		  sprintf( text, "   Volume '%s' is not yet created.\n \n\
   Do you want to create this volume.\n",
			   volume_name);
		  wow->DisplayQuestion( NULL,
					"Create volume", text,
					lfu_creadb_qb_yes, NULL, (void *)data);
		}
		else {
		  char msg[200];
		  sprintf( msg, "Error, Volume '%s' is not yet created.", 
			   volume_name);
		  MsgWindow::message( 'E', msg, msgw_ePop_Default);
		}
	      }
	    }
	    if ( volrep == ldh_eVolRep_Dbms) {
	      sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				      "Server", (char **) &server_p, &size);
	      if ( EVEN(sts)) return sts;

	      strcpy( server, server_p);
	      free( server_p);

	      sprintf( filename, "$pwrp_db/%s.dbms/connection.dmsql", volume_name);
	      cdh_ToLower( filename, filename);
	      dcli_translate_filename( filename, filename);
	      sts = dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_INIT);
	      dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
	      if (EVEN(sts)) {
		if ( wow) {
		  lfu_sCreaDb *data;
		  data = (lfu_sCreaDb *) calloc( 1, sizeof(*data));
		  strcpy( data->name, volumelist_ptr->volume_name);
		  data->vid = volumelist_ptr->volume_id;
		  data->cid = vcid;
		  data->ldhses = ldhses;
		  data->volrep = volrep;
		  strcpy( data->server, server);
		  
		  sprintf( text, "   Volume '%s' is not yet created.\n \n\
   Do you want to create this volume.\n",
			   volume_name);
		  wow->DisplayQuestion( NULL,
					"Create volume", text,
					lfu_creadb_qb_yes, NULL, (void *)data);
		}
		else {
		  char msg[200];
		  sprintf( msg, "Error, Volume '%s' is not yet created.", 
			   volume_name);
		  MsgWindow::message( 'E', msg, msgw_ePop_Default);
		}
	      }
	    }
	    break;
	  }
	  case pwr_cClass_ClassVolumeConfig : {
	    int *dbenum_p = 0;
	    char *server_p = 0;
	    ldh_eVolRep volrep;
	    pwr_tString40 server = "";
	    
	    sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				"Database", (char **) &dbenum_p, &size);
	    if ( EVEN(sts)) return sts;

	    if ( *dbenum_p == pwr_eClassVolumeDatabaseEnum_WbLoad)
	      volrep = ldh_eVolRep_Wbl;
	    if ( *dbenum_p == pwr_eClassVolumeDatabaseEnum_BerkeleyDb)
	      volrep = ldh_eVolRep_Db;
	    else if ( *dbenum_p == pwr_eClassVolumeDatabaseEnum_MySql)
	      volrep = ldh_eVolRep_Dbms;
	    else {
	      free( dbenum_p);
	      break;
	    }
	    free( dbenum_p);

	    if ( volrep == ldh_eVolRep_Db) {
	      /* Check if the databas is created */
	      sprintf( filename, "$pwrp_db/%s.db/info", volume_name);
	      cdh_ToLower( filename, filename);
	      dcli_translate_filename( filename, filename);
	      sts = dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_INIT);
	      dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
	      if (EVEN(sts)) {
		if ( wow) {
		  lfu_sCreaDb *data;
		  data = (lfu_sCreaDb *) calloc( 1, sizeof(*data));
		  strcpy( data->name, volumelist_ptr->volume_name);
		  data->vid = volumelist_ptr->volume_id;
		  data->cid = pwr_eClass_ClassVolume;
		  data->ldhses = ldhses;
		  data->volrep = volrep;
		  strcpy( data->server, "");
		
		  sprintf( text, "   ClassVolume '%s' is not yet created.\n \n\
   Do you want to create this volume.\n",
			   volume_name);
		  wow->DisplayQuestion( NULL,
				     "Create Classvolume", text,
				     lfu_creadb_qb_yes, NULL, (void *)data);
		}
	      }
	    }
	    else if ( volrep == ldh_eVolRep_Dbms) {
	      // MySql...
	      sts = ldh_GetObjectPar( ldhses, envobjid, "RtBody",
				      "Server", (char **) &server_p, &size);
	      if ( EVEN(sts)) return sts;

	      strcpy( server, server_p);
	      free( server_p);

	      sprintf( filename, "$pwrp_db/%s.dbms/connection.dmsql", volume_name);
	      cdh_ToLower( filename, filename);
	      dcli_translate_filename( filename, filename);
	      sts = dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_INIT);
	      dcli_search_file( filename, found_file, DCLI_DIR_SEARCH_END);
	      if (EVEN(sts)) {
		if ( wow) {
		  lfu_sCreaDb *data;
		  data = (lfu_sCreaDb *) calloc( 1, sizeof(*data));
		  strcpy( data->name, volumelist_ptr->volume_name);
		  data->vid = volumelist_ptr->volume_id;
		  data->cid = pwr_eClass_ClassVolume;
		  data->ldhses = ldhses;
		  data->volrep = volrep;
		  strcpy( data->server, server);
		
		  sprintf( text, "   ClassVolume '%s' is not yet created.\n \n\
   Do you want to create this volume.\n",
			   volume_name);
		  wow->DisplayQuestion( NULL,
				     "Create Classvolume", text,
				     lfu_creadb_qb_yes, NULL, (void *)data);
		}
	      }
	    }
	    else if ( volrep == ldh_eVolRep_Wbl) {
	      // Check wbload-file...
	      FILE *wblfile;
	    
	      sprintf( fname, "$pwrp_db/%s.wb_load", volume_name);
	      cdh_ToLower( fname, fname);
	      dcli_translate_filename( fname, fname);
	      wblfile = fopen( fname, "r");
	      if ( wblfile == 0) {
		wblfile = fopen( fname, "w");
		if ( wblfile == 0) {
		  char msg[200];
		  sprintf( msg, "Error, unable to create file %s, ", fname);
		  MsgWindow::message( 'E', msg, msgw_ePop_Default);
		  break;
		}	    
	      
		fprintf( wblfile, "Volume %s pwr_eClass_ClassVolume %s\nEndVolume\n", 
			 volume_name, cdh_VolumeIdToString( 0, volumelist_ptr->volume_id, 0, 0));
		fclose( wblfile);
	      }
	    }
	    break;
	  }
	  }
	}
      }
    }
  }
  fclose( file);
#if defined OS_VMS
  system( "purge/nolog " load_cNameVolumeList);
#endif

  if ( !path_file_created) {
    // Create an empty path file
    dcli_translate_filename( filename, load_cNameFilePath);
    fpath = fopen( filename, "w");
    if ( !fpath) {
      char msg[200];
      sprintf( msg, "Unable to open path file %s\n", filename);
      MsgWindow::message( 'E', msg, msgw_ePop_Default);
      syntax_error = 1;
    }
    else
      fclose( fpath);
  }

  /* Generate data for bootfiles */
  dcli_translate_filename( fname, load_cNameBootList);
  file = fopen( fname, "w");
  if ( file == 0)
  {
    char msg[200];
    sprintf( msg, "** Error, Unable to open file %s", fname);
    MsgWindow::message( 'E', msg, msgw_ePop_Default);
    return LFU__NOFILE;
  }

  /* Get the configured nodes */
  sts = ldh_GetRootList( ldhses, &busobjid);
  while ( ODD(sts)) {
    sts = ldh_GetObjectClass( ldhses, busobjid, &cid);
    if ( EVEN(sts)) return sts;

    if ( cid == pwr_cClass_BusConfig) {
      /* Check Bus attribute */
      sts = ldh_GetObjectPar( ldhses, busobjid, "RtBody",
			"BusNumber", (char **)&bus_number_ptr, &size);
      if (EVEN(sts)) return sts;
      sts = ldh_GetObjectPar( ldhses, busobjid, "RtBody",
			"BusNumber", (char **)&bus_number_ptr, &size);
      if (EVEN(sts)) return sts;

      sts = ldh_ObjidToName( ldhses, busobjid, ldh_eName_Object, 
			     bus_name, sizeof(bus_name), &size);
      if ( EVEN(sts)) return sts;

      if ( *bus_number_ptr == 0) {
	char msg[200];
        sprintf( msg, "Error in Bus object '%s', BusNumber is missing", bus_name);
	MsgWindow::message( 'E', msg, msgw_ePop_Default);
        syntax_error = 1;
      }

      sts = ldh_GetChild( ldhses, busobjid, &nodeobjid);
      while ( ODD(sts)) {

	sts = ldh_GetObjectClass( ldhses, nodeobjid, &cid);
	if ( EVEN(sts)) return sts;

	if ( cid == pwr_cClass_NodeConfig) {
	  sts = ldh_ObjidToName( ldhses, nodeobjid, ldh_eName_Object, 
				 nodeconfig_name, sizeof(nodeconfig_name), &size);
	  if ( EVEN(sts)) return sts;
	  
	  /* Check NodeName attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "NodeName", &nodename_ptr, &size);
	  if (EVEN(sts)) return sts;

	  if ( !strcmp( nodename_ptr, "")) {
	    char msg[200];
	    sprintf( msg, "Error in NodeConfig object '%s', NodeName is not valid", nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	    free( nodename_ptr);
	    nodename_ptr = null_nodename;
	  }

	  /* Check OperatingSystem attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "OperatingSystem", (char **)&os_ptr, &size);
	  if (EVEN(sts)) return sts;

	  os = *os_ptr;
	  if ( !(os == pwr_mOpSys_VAX_ELN ||
		 os == pwr_mOpSys_VAX_VMS ||
		 os == pwr_mOpSys_AXP_VMS ||
		 os == pwr_mOpSys_PPC_LYNX ||
		 os == pwr_mOpSys_X86_LYNX ||
		 os == pwr_mOpSys_PPC_LINUX ||
		 os == pwr_mOpSys_X86_LINUX ||
		 os == pwr_mOpSys_AXP_LINUX)) {
	    char msg[200];
	    sprintf( msg, "Error in NodeConfig object '%s', OperatingSystem is not valid", nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	    os = 0;
	  }

	  /* Check SimulateSingleProcess attribute */

	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "SimulateSingleProcess", (char **)&single_scan_ptr, &size);
	  if (EVEN(sts)) return sts;

	  if ( *single_scan_ptr != 0) {
	    sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				    "SimulateSingleScanTime", (char **)&scantime_ptr, &size);
	    if ( EVEN(sts)) return sts;
	    if (EVEN(sts)) return sts;

	    if ( *scantime_ptr == 0) {
	      char msg[200];
	      sprintf( msg, "Error in NodeConfig object '%s', SimulateSingleScanTime is missing", nodeconfig_name);
	      MsgWindow::message( 'E', msg, msgw_ePop_Default);
	      syntax_error = 1;
	    }
	    scantime = *scantime_ptr;
	    free( (char *) scantime_ptr);
	  }
	  else
	    scantime = 0;
	  free( (char *) single_scan_ptr);

	  /* Find the volumes in this node */
	  sts = ldh_GetChild( ldhses, nodeobjid, &volobjid);
	  while ( ODD(sts)) {
	    sts = ldh_GetObjectClass( ldhses, volobjid, &cid);
	    if ( EVEN(sts)) return sts;

	    if ( cid == pwr_cClass_RootVolumeLoad ||
		 cid == pwr_cClass_SubVolumeLoad ||
		 cid == pwr_cClass_ClassVolumeLoad ||
		 cid == pwr_cClass_SharedVolumeLoad ) {
	      sts = ldh_ObjidToName( ldhses, volobjid, ldh_eName_Object,
				     volume_name, sizeof(volume_name), &size);
	      if ( EVEN(sts)) return sts;
	      utl_toupper( name, volume_name);

	      /* Check that the name is in the global volume list */
	      found = 0;
	      volumelist_ptr = volumelist;
	      for ( i = 0; i < volumecount; i++) {
		utl_toupper( volname, volumelist_ptr->volume_name);
		if ( !strcmp( name, volname)) {
		  found = 1;

		  switch (cid) {
		  case pwr_cClass_RootVolumeLoad :
		    strcpy( classname, "RootVolume");
		    break;
		  case pwr_cClass_SubVolumeLoad :
		    strcpy( classname, "SubVolume");
		    break;
		  case pwr_cClass_ClassVolumeLoad :
		    strcpy( classname, "ClassVolume");
		    break;
		  case pwr_cClass_SharedVolumeLoad :
		    strcpy( classname, "SharedVolume");
		    break;
		  }

		  fprintf( file, "%s %s %s %s %d %d %f\n",
			   volume_name,
			   cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			   nodeconfig_name,
			   nodename_ptr,
			   *bus_number_ptr,
			   os,
			   scantime);
		  break;
		}
		volumelist_ptr++;
	      }
	      if ( !found) {
		char msg[200];
		sprintf( msg, "Error in VolumeLoad object,  '%s' is not configured in the global\
 volume list", name);
		MsgWindow::message( 'E', msg, msgw_ePop_Default);
		syntax_error = 1;
	      }
	    }
	    sts = ldh_GetNextSibling( ldhses, volobjid, &volobjid);
	  }
	  if ( nodename_ptr != null_nodename)
	    free( nodename_ptr);
	  free( (char *) os_ptr);
	}
	sts = ldh_GetNextSibling( ldhses, nodeobjid, &nodeobjid);
      }
      free( (char *) bus_number_ptr);
    }
    sts = ldh_GetNextSibling( ldhses, busobjid, &busobjid);
  }

  fclose( file);
#if defined OS_VMS
  system( "purge/nolog " load_cNameBootList);
#endif

  // Generate data for nodefiles */

  // Find all BusConfig objects */
  wb_session *sp = (wb_session*)ldhses;
  pwr_tUInt32 bus_number;

  for ( wb_object buso = sp->object(); buso; buso = buso.after()) {
    if ( buso.cid() != pwr_cClass_BusConfig)
      continue;

    wb_attribute a = sp->attribute( buso.oid(), "RtBody", "BusNumber");
    if ( !a) return a.sts();

    a.value( &bus_number);
    if ( !a) return a.sts();

    if ( bus_number == 0) 
      continue;

    // Get all nodeconfig and friendnodes for this bus
    vector<lfu_nodeconf> nodevect;
    int node_cnt = 0;
    for ( wb_object nodeo = buso.first(); nodeo; nodeo = nodeo.after()) {

      switch ( nodeo.cid()) {
      case pwr_cClass_NodeConfig: {
	pwr_tString80 volstr;
	lfu_nodeconf nc;

	nc.oid = nodeo.oid();

	// Get attribute NodeName
	a = sp->attribute( nodeo.oid(), "RtBody", "NodeName");
	if ( !a) return a.sts();

	a.value( nc.nodename);
	if ( !a) return sts;

	// Get attribute OperatingSystem
	a = sp->attribute( nodeo.oid(), "RtBody", "OperatingSystem");
	if ( !a) return a.sts();

	a.value( &nc.operatingsystem);
	if ( !a) return sts;

	// Get attribute Address
	a = sp->attribute( nodeo.oid(), "RtBody", "Address");
	if ( !a) return a.sts();

	a.value( nc.address);
	if ( !a) return sts;

	// Get attribute Port
	a = sp->attribute( nodeo.oid(), "RtBody", "Port");
	if ( !a) return a.sts();

	a.value( &nc.port);
	if ( !a) return sts;
	
	if ( !strcmp( nc.nodename, "")) {
	  char msg[200];
	  sprintf( msg, "Error in NodeConfig object '%s', NodeName is missing\n",
		   nodeo.longName().c_str());
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  syntax_error = 1;
	}

	// Get rootvolume
	found = 0;
	for ( wb_object volo = nodeo.first(); volo; volo = volo.after()) {
	  if ( volo.cid() == pwr_cClass_RootVolumeLoad) {
	    strcpy( volstr, volo.name());

	    /* Check that the name is in the global volume list */
	    found = 0;
	    volumelist_ptr = volumelist;
	    for ( i = 0; i < volumecount; i++) {
	      if ( cdh_NoCaseStrcmp( volstr, volumelist_ptr->volume_name) == 0) {
		nc.vid = volumelist_ptr->volume_id;
		found = 1;
		break;
	      }
	      volumelist_ptr++;
	    }
	  }
	}
	if ( !found) {
	  char msg[200];
	  sprintf( msg, "No valid RootVolume configured for object '%s'\n",
		   nodeo.longName().c_str());
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  syntax_error = 1;
	}

	nodevect.push_back( nc);
	node_cnt++;

	break;
      }
      case pwr_cClass_FriendNodeConfig: {
	pwr_tString80 volstr;
	lfu_nodeconf nc;

	nc.oid = nodeo.oid();
	nc.isfriend = 1;

	// Get attribute NodeName
	a = sp->attribute( nodeo.oid(), "RtBody", "NodeName");
	if ( !a) return a.sts();

	a.value( nc.nodename);
	if ( !a) return sts;

	// Get attribute Address
	a = sp->attribute( nodeo.oid(), "RtBody", "Address");
	if ( !a) return a.sts();

	a.value( nc.address);
	if ( !a) return sts;

	// Get attribute Port
	a = sp->attribute( nodeo.oid(), "RtBody", "Port");
	if ( !a) return a.sts();

	a.value( &nc.port);
	if ( !a) return sts;
	
	// Get attribute Volume
	a = sp->attribute( nodeo.oid(), "RtBody", "Volume");
	if ( !a) return a.sts();

	a.value( volstr);
	if ( !a) return sts;
	
	/* Check that the name is in the global volume list */
	found = 0;
	volumelist_ptr = volumelist;
	for ( i = 0; i < volumecount; i++) {
	  if ( cdh_NoCaseStrcmp( volstr, volumelist_ptr->volume_name) == 0) {
	    nc.vid = volumelist_ptr->volume_id;
	    found = 1;
	    break;
	  }
	  volumelist_ptr++;
	}

	if ( !found) {
	  char msg[200];
	  sprintf( msg, "Error in FriendNodeConfig object '%s', Unknown volume", nodeconfig_name);
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  syntax_error = 1;
	}

	if ( !strcmp( nc.nodename, "")) {
	  char msg[200];
	  sprintf( msg, "Error in NodeConfig object '%s', NodeName is missing\n",
		   nodeo.longName().c_str());
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  syntax_error = 1;
	}
	nodevect.push_back( nc);
	node_cnt++;
	break;
      }
      default: ;
      }
    }
    // Print file
    for ( wb_object nodeo = buso.first(); nodeo; nodeo = nodeo.after()) {

      switch ( nodeo.cid()) {
      case pwr_cClass_NodeConfig: {
	FILE *fp;
	int idx;
	int found = 0;

	for ( idx = 0; idx < (int)nodevect.size(); idx++) {
	  if ( cdh_ObjidIsEqual( nodevect[idx].oid, nodeo.oid())) {
	    found = 1;
	    break;
	  }
	}	       
	if ( !found)
	  return 0;

	sprintf( filename, load_cNameNode, load_cDirectory, cdh_Low(nodevect[idx].nodename), 
		 bus_number);
	dcli_translate_filename( fname, filename);
        fp = fopen( fname, "w");
	if ( fp == 0) {
	  char msg[200];
	  sprintf( msg, "Error, Unable to open file %s\n", fname);
	  MsgWindow::message( 'E', msg, msgw_ePop_Default);
	  return LFU__NOFILE;
	}

	for ( int i = 0; i < (int)nodevect.size(); i++) {
	  lfu_nodeconf nc = nodevect[i];
	  if ( nc.port == 0)
	    fprintf( fp, "%s %s %s\n", nc.nodename, 
		     cdh_VolumeIdToString( NULL, nc.vid, 0, 0), nc.address);
	  else
	    fprintf( fp, "%s %s %s %d\n", nc.nodename, 
		     cdh_VolumeIdToString( NULL, nc.vid, 0, 0), nc.address, nc.port);
	}

	// Add specific FriendNodes for the node
	for ( wb_object fnodeo = nodeo.first(); fnodeo; fnodeo = fnodeo.after()) {
	  switch ( fnodeo.cid()) {
	  case pwr_cClass_FriendNodeConfig: {
	    pwr_tString80 volstr;
	    lfu_nodeconf nc;

	    nc.isfriend = 1;

	    // Get attribute NodeName
	    a = sp->attribute( fnodeo.oid(), "RtBody", "NodeName");
	    if ( !a) return a.sts();
	  
	    a.value( nc.nodename);
	    if ( !a) return sts;

	    // Get attribute Address
	    a = sp->attribute( fnodeo.oid(), "RtBody", "Address");
	    if ( !a) return a.sts();

	    a.value( nc.address);
	    if ( !a) return sts;

	    // Get attribute Port
	    a = sp->attribute( fnodeo.oid(), "RtBody", "Port");
	    if ( !a) return a.sts();

	    a.value( &nc.port);
	    if ( !a) return sts;
	  
	    // Get attribute Volume
	    a = sp->attribute( fnodeo.oid(), "RtBody", "Volume");
	    if ( !a) return a.sts();

	    a.value( volstr);
	    if ( !a) return sts;
	
	    /* Check that the name is in the global volume list */
	    found = 0;
	    volumelist_ptr = volumelist;
	    for ( int j = 0; j < volumecount; j++) {
	      if ( cdh_NoCaseStrcmp( volstr, volumelist_ptr->volume_name) == 0) {
		nc.vid = volumelist_ptr->volume_id;
		found = 1;
		break;
	      }
	      volumelist_ptr++;
	    }

	    if ( !found) {
	      char msg[200];
	      sprintf( msg, "Error in FriendNodeConfig object '%s', Unknown volume", nodeconfig_name);
	      MsgWindow::message( 'E', msg, msgw_ePop_Default);
	      syntax_error = 1;
	    }

	    if ( nc.port == 0)
	      fprintf( fp, "%s %s %s\n", nc.nodename, 
		       cdh_VolumeIdToString( NULL, nc.vid, 0, 0), nc.address);
	    else
	      fprintf( fp, "%s %s %s %d\n", nc.nodename, 
		      cdh_VolumeIdToString( NULL, nc.vid, 0, 0), nc.address, nc.port);
	    break;
	  }
	  default: ;
	  }
	}
	fclose( fp);
	break;
      }
      default: ;
      }
    }
    
  }
    
#if 0

  sts = ldh_GetRootList( ldhses, &busobjid);
  while ( ODD(sts)) {
    FILE *nodefile;

    sts = ldh_GetObjectClass( ldhses, busobjid, &cid);
    if ( EVEN(sts)) return sts;

    if ( cid == pwr_cClass_BusConfig) {

      /* Get Bus attribute */
      sts = ldh_GetObjectPar( ldhses, busobjid, "RtBody",
			"BusNumber", (char **)&bus_number_ptr, &size);
      if (EVEN(sts)) return sts;

      if ( *bus_number_ptr == 0) {
        free( (char *) bus_number_ptr);
        sts = ldh_GetNextSibling( ldhses, busobjid, &busobjid);
        continue;
      }

      sprintf( filename, load_cNameNode, load_cDirectory, *bus_number_ptr);
      dcli_translate_filename( fname, filename);
      nodefile = fopen( fname, "w");
      if ( nodefile == 0) {
	char msg[200];
	sprintf( msg, "Error, Unable to open file %s\n", fname);
	MsgWindow::message( 'E', msg, msgw_ePop_Default);
	return LFU__NOFILE;
      }

      sts = ldh_GetChild( ldhses, busobjid, &nodeobjid);
      while ( ODD(sts)) {
	sts = ldh_GetObjectClass( ldhses, nodeobjid, &cid);
	if ( EVEN(sts)) return sts;

	if ( cid == pwr_cClass_NodeConfig) {
	  sts = ldh_ObjidToName( ldhses, nodeobjid, ldh_eName_Object, 
			       nodeconfig_name, sizeof(nodeconfig_name), &size);
	  if ( EVEN(sts)) return sts;

	  /* Check NodeName attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				"NodeName", &nodename_ptr, &size);
	  if (EVEN(sts)) return sts;
	
	  if ( !strcmp( nodename_ptr, "")) {
	    char msg[200];
	    sprintf( msg, "Error in NodeConfig object '%s', NodeName is missing\n",
		     nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	    free( nodename_ptr);
	    nodename_ptr = null_nodename;
	  }

	  /* Check OperatingSystem attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				"Address", &address_ptr, &size);
	  if (EVEN(sts)) return sts;

	  if ( strcmp( address_ptr, "") == 0) {
	    char msg[200];
	    sprintf( msg, "Error in NodeConfig object '%s', Address is not valid", nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	  }

	  /* Check Port attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "Port", (char **)&port_ptr, &size);
	  if (EVEN(sts)) return sts;
	
	  /* Find the rootvolume for this node */
	  sts = ldh_GetChild( ldhses, nodeobjid, &volobjid);
	  while ( ODD(sts)) {
	    sts = ldh_GetObjectClass( ldhses, volobjid, &cid);
	    if ( EVEN(sts)) return sts;

	    if ( cid == pwr_cClass_RootVolumeLoad) {
	      sts = ldh_ObjidToName( ldhses, volobjid, ldh_eName_Object,
				     volume_name, sizeof(volume_name), &size);
	      if ( EVEN(sts)) return sts;
	      utl_toupper( name, volume_name);

	      /* Check that the name is in the global volume list */
	      found = 0;
	      volumelist_ptr = volumelist;
	      for ( i = 0; i < volumecount; i++) {
		utl_toupper( volname, volumelist_ptr->volume_name);
		if ( !strcmp( name, volname)) {
		  if ( *port_ptr == 0)
		    fprintf( nodefile, "%s %s %s\n",
			     nodename_ptr,
			     cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			     address_ptr);
		  else
		    fprintf( nodefile, "%s %s %s %d\n",
			     nodename_ptr,
			     cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			     address_ptr,
			     *port_ptr);
		  break;
		}
		volumelist_ptr++;
	      }
	    }
	    sts = ldh_GetNextSibling( ldhses, volobjid, &volobjid);
	  }
	  if ( nodename_ptr != null_nodename)
	    free( nodename_ptr);
	  free( address_ptr);
	  free( (char *) port_ptr);
	}
	else if ( cid == pwr_cClass_FriendNodeConfig) {
	  sts = ldh_ObjidToName( ldhses, nodeobjid, ldh_eName_Object, 
				 nodeconfig_name, sizeof(nodeconfig_name), &size);
	  if ( EVEN(sts)) return sts;
	  
	  /* Check NodeName attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "NodeName", &nodename_ptr, &size);
	  if (EVEN(sts)) return sts;
	  
	  if ( !strcmp( nodename_ptr, "")) {
	    char msg[200];
	    sprintf( msg, "Error in FriendNodeConfig object '%s', NodeName is missing",
		     nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	    free( nodename_ptr);
	    nodename_ptr = null_nodename;
	  }

	  /* Check Address attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "Address", &address_ptr, &size);
	  if (EVEN(sts)) return sts;
	  
	  if ( strcmp( address_ptr, "") == 0) {
	    char msg[200];
	    sprintf( msg, "Error in FriendNodeConfig object '%s', Address is not valid", nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	  }
	  
	  /* Check Port attribute */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "Port", (char **)&port_ptr, &size);
	  if (EVEN(sts)) return sts;
	  
	  /* Find the rootvolume for this node */
	  sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
				  "Volume", (char **)&volume_ptr, &size);
	  if (EVEN(sts)) return sts;
	  
	  if ( *volume_ptr == 0) {
	    char msg[200];
	    sprintf( msg, "Error in FriendNodeConfig object '%s', Volume is not valid", nodeconfig_name);
	    MsgWindow::message( 'E', msg, msgw_ePop_Default);
	    syntax_error = 1;
	  }
	  else {
	    strcpy( volume_name, volume_ptr);
	    utl_toupper( name, volume_name);
	    
	    /* Check that the name is in the global volume list */
	    found = 0;
	    volumelist_ptr = volumelist;
	    for ( i = 0; i < volumecount; i++) {
	      utl_toupper( volname, volumelist_ptr->volume_name);
	      if ( !strcmp( name, volname)) {
		if ( *port_ptr == 0)
		  fprintf( nodefile, "%s %s %s\n",
			   nodename_ptr,
			   cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			   address_ptr);
		else
		  fprintf( nodefile, "%s %s %s %d\n",
			   nodename_ptr,
			   cdh_VolumeIdToString( NULL, volumelist_ptr->volume_id, 0, 0),
			   address_ptr,
			   *port_ptr);
		found = 1;
		break;
	      }
	      volumelist_ptr++;
	    }
	    if ( !found) {
	      char msg[200];
	      sprintf( msg, "Error in FriendNodeConfig object '%s', Unknown volume", nodeconfig_name);
	      MsgWindow::message( 'E', msg, msgw_ePop_Default);
	      syntax_error = 1;
	    }
	  }
	  if ( nodename_ptr == null_nodename)
	    free( nodename_ptr);
	  free( volume_ptr);
	  free( address_ptr);
	  free( (char *) port_ptr);
	}
	sts = ldh_GetNextSibling( ldhses, nodeobjid, &nodeobjid);
      }
      free( (char *) bus_number_ptr);	
      fclose( nodefile);
    }
    sts = ldh_GetNextSibling( ldhses, busobjid, &busobjid);
  }
#endif

  /* Generate data for distribution */
  dcli_translate_filename( fname, load_cNameDistribute);
  file = fopen( fname, "w");
  if ( file == 0)
  {
    char msg[200];
    sprintf( msg, "Error, Unable to open file %s", fname);
    MsgWindow::message( 'E', msg, msgw_ePop_Default);
    return LFU__NOFILE;
  }

  /* Get the system name */
  sts = utl_get_systemobject( ldhses, &systemobjid, systemname,
		systemgroup);
  if ( EVEN(sts))
  {
    char msg[200];
    sprintf( msg, "Error in System object, object is missing");
    MsgWindow::message( 'E', msg, msgw_ePop_Default);
    syntax_error = 1;
  }
  if ( !strcmp( systemname, ""))
  {
    char msg[200];
    sprintf( msg, "Error in System object, SystemName is missing");
    MsgWindow::message( 'E', msg, msgw_ePop_Default);
    syntax_error = 1;
  }

  fprintf( file, "project %s\n", systemname);

  /* Get the Distribute data of the node config objects */
  sts = ldh_GetRootList( ldhses, &busobjid);
  while ( ODD(sts)) {
    sts = ldh_GetObjectClass( ldhses, busobjid, &cid);
    if ( EVEN(sts)) return sts;

    if ( cid == pwr_cClass_BusConfig) {
      /* Check DmqBus attribute */
      sts = ldh_GetObjectPar( ldhses, busobjid, "RtBody",
			"BusNumber", (char **)&bus_number_ptr, &size);
      if ( EVEN(sts)) return sts;

      sts = ldh_GetChild( ldhses, busobjid, &nodeobjid);
      while ( ODD(sts)) {

        sts = ldh_GetObjectClass( ldhses, nodeobjid, &cid);
        if ( EVEN(sts)) return sts;

        if ( cid == pwr_cClass_NodeConfig) {
          sts = ldh_ObjidToName( ldhses, nodeobjid, ldh_eName_Object, 
			nodeconfig_name, sizeof(nodeconfig_name), &size);
          if ( EVEN(sts)) return sts;

          /* Check if Disabled attribute */
          sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
			"DistributeDisable", (char **)&distr_disable_ptr, &size);
          if (ODD(sts)) {
            if ( *distr_disable_ptr) {
	      /* Distribution is disabled, goto next node */
               sts = ldh_GetNextSibling( ldhses, nodeobjid, &nodeobjid);
	       continue;
            }
          }

          /* Check NodeName attribute */
          sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
			"NodeName", &nodename_ptr, &size);
          if (EVEN(sts)) return sts;

          if ( !strcmp( nodename_ptr, "")) {
            free( nodename_ptr);
            nodename_ptr = null_nodename;
          }

          /* Check BootNode attribute */
          sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
			"BootNode", &bootnode_ptr, &size);
          if (EVEN(sts)) return sts;

          if ( !strcmp( bootnode_ptr, "") || !strcmp( bootnode_ptr, "-")) {
            free( bootnode_ptr);
            bootnode_ptr = null_nodename;
          }

          /* Check OperatingSystem attribute */
          sts = ldh_GetObjectPar( ldhses, nodeobjid, "RtBody",
			"OperatingSystem", (char **)&os_ptr, &size);
	  if ( EVEN(sts)) return sts;

	  os = *os_ptr;
	  sprintf( os_str, "%d", os);

	  /* Check that there is a rootvolume for this node */
	  found = 0;
	  sts = ldh_GetChild( ldhses, nodeobjid, &volobjid);
	  while ( ODD(sts)) {
	    sts = ldh_GetObjectClass( ldhses, volobjid, &cid);
	    if ( EVEN(sts)) return sts;

	    if ( cid == pwr_cClass_RootVolumeLoad) {
	      found = 1;
	      break;
	    }
	    sts = ldh_GetNextSibling( ldhses, volobjid, &volobjid);
	  }
	  distr_status = lfu_eDistrSts_Normal;
	  if ( !found)  
	    distr_status = (lfu_eDistrSts)((int)distr_status | lfu_eDistrSts_NoRootVolume);

	  fprintf( file, "node %s %s %d %d %s\n",
		   nodename_ptr,
		   os_str,
		   *bus_number_ptr,
		   distr_status,
		   bootnode_ptr);

	  /* Find the applications for this node */
	  class_vect[0] = pwr_cClass_Distribute;
	  class_vect[1] = pwr_cClass_ApplDistribute;
	  class_vect[3] = 0;
	  
	  objcount = 0;
	  objlist = 0;

	  sts = trv_create_ctx( &trvctx, ldhses, nodeobjid, class_vect, NULL,
				NULL);
	  if ( EVEN(sts)) return sts;

	  sts = trv_object_search( trvctx,
				   &utl_objidlist_insert, &objlist, 
				   &objcount, 0, 0, 0);
	  if ( EVEN (sts)) return sts;

	  sts = trv_delete_ctx( trvctx);

	  for ( obj_ptr = objlist; obj_ptr; obj_ptr = obj_ptr->next) {
	    applobjid = obj_ptr->objid;
	    sts = ldh_GetObjectClass( ldhses, applobjid, &cid);
	    if ( EVEN(sts)) return sts;

	    switch ( cid) {
	    case pwr_cClass_Distribute: {
	      lfu_mDistrComponents *components_ptr;

	      sts = ldh_ObjidToName( ldhses, applobjid, ldh_eName_Object,
				     appl_name, sizeof(appl_name), &size);
	      if ( EVEN(sts)) return sts;

	      /* Check Components attribute */
	      sts = ldh_GetObjectPar( ldhses, applobjid, "DevBody",
				      "Components", (char **)&components_ptr, &size);
	      if (EVEN(sts)) return sts;

	      if ( *components_ptr & lfu_mDistrComponents_LoadFiles &&
		   !(distr_status & lfu_eDistrSts_NoRootVolume)) {
		fprintf( file, "load %s\n", nodename_ptr);
	      }
	      if ( *components_ptr & lfu_mDistrComponents_UserDatabase)
		fprintf( file, "appl %s W $pwrp_src/%s/pwr_user.dat:$pwra_db/pwr_user.dat $pwra_db/pwr_user.dat\n",
			 nodename_ptr, nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_ApplFile)
		fprintf( file, "appl %s W "load_cNameAppl"\n",
			 nodename_ptr, "$pwrp_load/", nodename_ptr, *bus_number_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_PwrpAliasFile)
		fprintf( file, "appl %s W $pwrp_load/pwrp_alias.dat $pwrp_load/pwrp_alias.dat\n", 
		     nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_IncludeFiles)
		fprintf( file, "appl %s W $pwrp_inc/*.h\n", nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_GraphFiles)
		fprintf( file, "appl %s W $pwrp_exe/*.pwg\n", nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_XttHelpFile)
		fprintf( file, "appl %s W $pwrp_src/%s/xtt_help.dat:$pwrp_exe/xtt_help.dat $pwrp_exe/xtt_help.dat\n", 
			 nodename_ptr, nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_XttResourceFile)
		fprintf( file, "appl %s W $pwrp_src/%s/Rt_xtt:$pwrp_pop/Rt_xtt /home/b55/Rt_xtt\n", 
			 nodename_ptr, nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_XttSetupFile)
		fprintf( file, "appl %s W $pwrp_src/%s/xtt_setup.rtt_com:$pwrp_pop/xtt_setup.rtt_com /home/b55/xtt_setup.rtt_com\n", 
			 nodename_ptr, nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_FlowFiles)
		fprintf( file, "appl %s W $pwrp_load/*.flw\n", nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_RHostsFile)
		fprintf( file, "appl %s W $pwrp_src/%s/.rhosts:$pwra_db/.rhosts /home/pwrp/.rhosts\n",
			 nodename_ptr, nodename_ptr);
	      if ( *components_ptr & lfu_mDistrComponents_WebFiles) {
		fprintf( file, "appl %s W $pwrp_web/*.html\n", nodename_ptr);
		fprintf( file, "appl %s W $pwrp_web/*.jar\n", nodename_ptr);
		fprintf( file, "appl %s S $pwrp_web/*.gif\n", nodename_ptr);
		fprintf( file, "appl %s S $pwrp_web/*.jpg\n", nodename_ptr);
		fprintf( file, "appl %s S $pwrp_web/*.pdf\n", nodename_ptr);
	      }
	      if ( *components_ptr & lfu_mDistrComponents_PwrpStop)
		fprintf( file, "appl %s W $pwrp_src/%s/pwrp_stop.sh:$pwrp_exe/pwrp_stop.sh $pwrp_exe/pwrp_stop.sh\n", 
			 nodename_ptr, nodename_ptr);


	      free( components_ptr);
	      
	      break;
	    }
	    case pwr_cClass_ApplDistribute:
	      sts = ldh_ObjidToName( ldhses, applobjid, ldh_eName_Object,
				     appl_name, sizeof(appl_name), &size);
	      if ( EVEN(sts)) return sts;
	      
	      /* Check Source attribute */
	      sts = ldh_GetObjectPar( ldhses, applobjid, "DevBody",
				      "Source", &source_ptr, &size);
	      if (EVEN(sts)) return sts;

	      if ( !strcmp( source_ptr, "")) {
		char msg[200];
		free( source_ptr);
		source_ptr = null_nodename;
		sprintf( msg, "Error in ApplDistribute object '%s', Source is missing", 
			 appl_name);
		MsgWindow::message( 'E', msg, msgw_ePop_Default);
		syntax_error = 1;
	      }         
	      /* Check Target attribute */
	      sts = ldh_GetObjectPar( ldhses, applobjid, "DevBody",
				      "Target", &target_ptr, &size);
	      if (EVEN(sts)) return sts;
	      
	      if ( !strcmp( target_ptr, ""))
		fprintf( file, "appl %s E %s\n",
			 nodename_ptr,
			 source_ptr);
	      else
		fprintf( file, "appl %s E %s %s\n",
			 nodename_ptr,
			 source_ptr,
			 target_ptr);
	      if ( source_ptr != null_nodename)
		free( source_ptr);
	      free( target_ptr);
	      break;
	    default: ;
	    }
	  }
	  if ( nodename_ptr != null_nodename)
	    free( nodename_ptr);
	  if ( bootnode_ptr != null_nodename)
	    free( bootnode_ptr);
	  free( (char *) os_ptr);
	  utl_objidlist_free( objlist);
	}
	sts = ldh_GetNextSibling( ldhses, nodeobjid, &nodeobjid);
      }
      free( (char *) bus_number_ptr);
    }
    sts = ldh_GetNextSibling( ldhses, busobjid, &busobjid);
  }

  /* Get the DbDistribute objects */
  sts = ldh_GetRootList( ldhses, &dbobjid);
  while ( ODD(sts))
  {
    sts = ldh_GetObjectClass( ldhses, dbobjid, &cid);
    if ( EVEN(sts)) return sts;

    if ( cid == pwr_cClass_RootVolumeConfig ||
	 cid == pwr_cClass_SubVolumeConfig ||
	 cid == pwr_cClass_ClassVolumeConfig ||
	 cid == pwr_cClass_SharedVolumeConfig ) {
      sts = ldh_ObjidToName( ldhses, volobjid, ldh_eName_Object,
			     volume_name, sizeof(volume_name), &size);
      if ( EVEN(sts)) return sts;
      utl_toupper( name, volume_name);

      /* Check that the name is in the global volume list */
      found = 0;
      volumelist_ptr = volumelist;
      for ( i = 0; i < volumecount; i++) {
	utl_toupper( volname, volumelist_ptr->volume_name);
	if ( !strcmp( name, volname)) {
	  volume_id = volumelist_ptr->volume_id;
	  found = 1;
	  break;
	}
	volumelist_ptr++;
      }
      /* Get any child of class VolumeDistribute */
      sts = ldh_GetChild( ldhses, volobjid, &distrobjid);
      while ( ODD(sts)) {
	sts = ldh_GetObjectClass( ldhses, distrobjid, &cid);
	if ( EVEN(sts)) return sts;
	
	if ( cid == pwr_cClass_VolumeDistribute)
	  {
	    sts = ldh_GetObjectPar( ldhses, distrobjid, "RtBody",
				    "TargetNode", &targetnode_ptr, &size);
	    if ( EVEN(sts)) return sts;

	    sts = ldh_GetObjectPar( ldhses, distrobjid, "RtBody",
				    "TargetProject", &targetproject_ptr, &size);
	    if ( EVEN(sts)) return sts;

	    sts = ldh_GetObjectPar( ldhses, distrobjid, "RtBody",
				    "TargetOpSys", (char **)&os_ptr, &size);
	    if ( EVEN(sts)) return sts;


	    fprintf( file, "volumedistr %s %s %s %d\n",
		     cdh_VolumeIdToString( NULL, volume_id, 0, 0),
		     targetnode_ptr,
		     targetproject_ptr,
		     *os_ptr);
	    free( targetnode_ptr);
	    free( targetproject_ptr);
	  }
	sts = ldh_GetNextSibling( ldhses, distrobjid, &distrobjid);
      }
    }
    sts = ldh_GetNextSibling( ldhses, dbobjid, &dbobjid);
  }

  fclose( file);
#if defined OS_VMS
  system( "purge/nolog " load_cNameDistribute);
#endif


  if ( volumecount > 0)
    free( (char *)volumelist);

  if ( syntax_error)
    return LFU__SYNTAX;
  else
    return LFU__SUCCESS;
}


/************************************************************************
*
* Name: lfu_creatdb_qb_yes
*
* Type: int
*
* Type		Parameter	IOGF	Description
* hier_ctx	hier		I	Context variable
*
* Description:
*	Backcall when yes in the questionbox is activated on the question
*	"Should a new database be created".
*
*************************************************************************/
static void lfu_creadb_qb_yes( void *ctx, void *d)
{
  lfu_sCreaDb *data = (lfu_sCreaDb *)d;
  wb_session *sp = (wb_session *)data->ldhses;
    
  try {
    wb_env env = sp->env();
    wb_volume vdb = env.createVolume( data->vid, data->cid, data->name, data->volrep,
				      data->server, false);
  }
  catch ( wb_error &e) {
    MsgWindow::message( 'E', "Unable to create volume", msgw_ePop_Default);
  }
  free( (char *) data);
}


/*************************************************************************
*
* Name:		lfu_create_bootfiles()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	Create bootfiles for a number of nodes.
*
**************************************************************************/

int lfu_create_bootfiles (
  char		*nodestr,
  int		debug,
  int		allnodes
)
{
	int			sts;
	char			node_array[30][80];
	int			found;
	int			nr, i, j;
	lfu_t_volumelist	*volumelist;
	lfu_t_volumelist	*volumelist_ptr;
	int			volumecount;
	pwr_tString40 nodeconfigname;

	/* Load the bootlist */
	sts = lfu_volumelist_load( load_cNameBootList, 
		&volumelist, &volumecount);
	if (sts == LFU__NOFILE) 
	{
	  MsgWindow::message( 'E', "Project is not configured", msgw_ePop_Default);
	  return sts;
  	}
	else if (EVEN(sts)) return sts;

	/* Check if nodes */
	if ( nodestr != NULL )
	{
	  nr = utl_parse( nodestr, ", ", "", (char *)node_array, 
		sizeof( node_array) / sizeof( node_array[0]), 
		sizeof( node_array[0]));
	  if ( (nr == 0) || ( nr > 30))
	    return LFU__NODENAME;

	  /* Check that the node's exist in the bootfile */
	  for ( i = 0; i < nr; i++)
	  {
	    found = 0;
	    utl_toupper( node_array[i], node_array[i]);
	    volumelist_ptr = volumelist;
            for ( j = 0; j < volumecount; j++)
            {
	      utl_toupper( nodeconfigname, volumelist_ptr->p1);
	      if ( !strcmp( nodeconfigname, node_array[i]))
	      {
	        found = 1;
	        break;
	      }
	      volumelist_ptr++;
	    }
	    if ( !found)
	    {
	      return LFU__NODENAME;
	    }
	  }
	  /* Create the bootfiles */
	  for ( i = 0; i < nr; i++)
	  {
	    sts = lfu_create_bootfile( node_array[i], volumelist, volumecount, 
		debug);
	    if ( EVEN(sts)) return sts;
	  }
	}
	else if ( allnodes)
	{
	  strcpy( nodeconfigname, "");
	  /* Create bootfiles for all created nodes */
	  volumelist_ptr = volumelist;
	  for ( j = 0; j < volumecount; j++)
          {
	    if ( strcmp( nodeconfigname, volumelist_ptr->p1))
	    {
	      strcpy( nodeconfigname, volumelist_ptr->p1);
	      sts = lfu_create_bootfile( nodeconfigname, 
		volumelist, volumecount, debug);
	      if ( EVEN(sts)) return sts;
	    }
	    volumelist_ptr++;
	  }
	}
	return LFU__SUCCESS;
}


/*************************************************************************
*
* Name:		lfu_ReadBootFile
*
* Type		pwr_tStatus
*
* Description: 
*		Returns information in a bootfile.
*
**************************************************************************/

pwr_tStatus lfu_ReadBootFile(
  char		*filename,
  pwr_tTime	*date,
  char		*systemname,
  char		*systemgroup,
  pwr_tVolumeId **vollist,
  pwr_tString40	**volnamelist,
  int		*volcount,
  char		*plc_name
)
{
	FILE	*file;
	char	timstr[40];
	char	vol_name[40];
	char	vol_str[20];
	int	plc_found;
	pwr_tVolumeId volid;
	pwr_tStatus sts;
	char	fname[120];


	dcli_translate_filename( fname, filename);
	file = fopen( fname, "r");
	if ( file == 0) 
	  return LFU__NOFILE;

	if (fgets( timstr, sizeof( timstr), file) == NULL)
	{
	  fclose( file);
	  return LFU__FILECRP;
	}
	sts = time_AsciiToA(timstr, date);
	if ( EVEN(sts)) return sts;
	if ( EVEN(sts))
	{
	  fclose( file);
	  return sts;
	}

	if ( utl_read_line( systemname, sizeof(pwr_tString80), file, NULL) == 0)
	{
	  fclose( file);
	  return LFU__FILECRP;
	}

	if ( utl_read_line( systemgroup, sizeof(pwr_tString80), file, NULL) == 0)
	{
	  fclose( file);
	  return LFU__FILECRP;
	}

	*volcount = 0;
	*vollist = 
	   (pwr_tVolumeId *) calloc( LFU_MAX_NODE_VOLUMES, sizeof(**vollist));
	*volnamelist = 
	   (pwr_tString40 *) calloc( LFU_MAX_NODE_VOLUMES, sizeof(**volnamelist));
	plc_found = 0;
	while ( fscanf( file, "%s %s", vol_name, vol_str) == 2) {
	  sts = cdh_StringToVolumeId( vol_str, &volid);
	  if ( EVEN(sts)) {
	    fclose( file);
	    return sts;
	  }
	  if ( *volcount == 0 && volid == 0) {
	    strcpy( plc_name, vol_name);
	    plc_found = 1;
	    continue;
	  }

	  *(*vollist + *volcount) = volid;
	  strcpy( *(*volnamelist + *volcount), vol_name);
	  (*volcount)++;
	}
	fclose( file);

	if ( !plc_found)
	  strcpy( plc_name, "");

	return LFU__SUCCESS;
}


/****************************************************************************
* Name:		lfu_GetVolRef()
**************************************************************************/
pwr_tStatus lfu_GetVolRef(     	char *filename,
			       	lfu_t_volref **volref,
			       	int *volref_count)
{
  dbs_sEnv dbsenv;
  dbs_sVolRef vref;
  int idx;
  pwr_tStatus sts;
  char fname[200];
  lfu_t_volref *vr;

  dcli_translate_filename( fname, filename);
  dbs_Open( &sts, &dbsenv, fname);
  if ( EVEN(sts)) return sts;

  vr = (lfu_t_volref *) calloc( LFU_MAX_VOLREF, sizeof(lfu_t_volref));
  idx = 0;
  while ( dbs_VolRef( &sts, idx, &vref, &dbsenv)) {
    if ( idx >= LFU_MAX_VOLREF)
      break;
    if ( EVEN(sts)) {
      free( (char *)vr);
      return sts;
    }
    strcpy( vr[idx].name, vref.name);
    vr[idx].vid = vref.vid;
    vr[idx].version = vref.time;
    idx++;
  }
  dbs_Close( &sts, &dbsenv);

  *volref_count = idx;
  *volref = vr;
  return 	LFU__SUCCESS;
}


/****************************************************************************
* Name:		lfu_GetVolume()
**************************************************************************/
pwr_tStatus lfu_GetVolume(     	char *filename,
			       	char *name,
				pwr_tVid *vid,
				pwr_tCid *cid,
				pwr_tTime *time
			       	)
{
  dbs_sEnv dbsenv;
  dbs_sVolume volume;
  pwr_tStatus sts;
  char fname[200];

  dcli_translate_filename( fname, filename);
  dbs_Open( &sts, &dbsenv, fname);
  if ( EVEN(sts)) return sts;

  dbs_Volume( &sts, &volume, &dbsenv);
  if ( EVEN(sts)) return sts;

  *vid = volume.vid;
  *cid = volume.cid;
  strcpy( name, volume.name);
  *time = volume.time;
  dbs_Close( &sts, &dbsenv);

  return 	LFU__SUCCESS;
}

pwr_tStatus lfu_GetVolumeCnf( char *name, pwr_tVid *vid, pwr_tCid *cid, ldh_eVolRep *volrep,
			      char *server)
{
  pwr_tStatus sts;
  pwr_tFileName fname;
  char line[200];
  char vol_array[7][80];

  strcpy( fname, load_cNameVolumeList);
  dcli_translate_filename( fname, fname);
      
  *volrep = ldh_eVolRep_Db;

  ifstream fpm( fname, ios::in);
  if ( fpm) {
  
    while ( fpm.getline( line, sizeof(line))) {
      int nr;
      
      if ( line[0] == '#')
	continue;
      
      nr = dcli_parse( line, " ", "", (char *)vol_array,
		       sizeof(vol_array)/sizeof(vol_array[0]),
		       sizeof(vol_array[0]), 0);
      
      sts =  cdh_StringToVolumeId( vol_array[1], vid);
      if ( EVEN(sts)) return sts;

      if ( cdh_NoCaseStrcmp( vol_array[2], "RootVolume") == 0)
	*cid = pwr_eClass_RootVolume;
      else if ( cdh_NoCaseStrcmp( vol_array[2], "SubVolume") == 0)
	*cid = pwr_eClass_SubVolume;
      else if ( cdh_NoCaseStrcmp( vol_array[2], "SharedVolume") == 0)
	*cid = pwr_eClass_SharedVolume;
      else if ( cdh_NoCaseStrcmp( vol_array[2], "ClassVolume") == 0)
	*cid = pwr_eClass_ClassVolume;

      switch ( *cid) {
      case pwr_eClass_RootVolume:
      case pwr_eClass_SubVolume:
      case pwr_eClass_SharedVolume:
 	*volrep = ldh_eVolRep_Db;
	if ( cdh_NoCaseStrcmp( vol_array[0], name) == 0) {
	  if ( nr > 4 && strcmp( vol_array[4], "1") == 0) {
	    *volrep = ldh_eVolRep_Dbms;
	    if ( nr > 5)
	      strcpy( server, vol_array[5]);
	  }
	  break;
	}
      case pwr_eClass_ClassVolume:
 	*volrep = ldh_eVolRep_Wbl;
	if ( cdh_NoCaseStrcmp( vol_array[0], name) == 0) {
	  if ( nr > 4 && strcmp( vol_array[4], "1") == 0)
	    *volrep = ldh_eVolRep_Db;
	  else if ( nr > 4 && strcmp( vol_array[4], "2") == 0) {
	    *volrep = ldh_eVolRep_Dbms;
	    if ( nr > 5)
	      strcpy( server, vol_array[5]);
	  }
	  break;
	}
      default: ;
      }
    }
    fpm.close();
  }
  return LFU__SUCCESS;
}

pwr_tStatus lfu_ParseDbmsServer( char *server, char *user, char *password, 
				 unsigned int *port, char *host)
{
  if ( strcmp( server, "") == 0) {
    if ( !cnf_get_value( "mysqlServer", host)) {
      printf( "** mysql Server not defined\n");
      return LDH__NOSERVER;
    }
  }

  // Parse server string: username:password@port:host
  char lhost[80];
  char luser[80] = "pwrp";
  char lpassword[80] = "";
  char str1[2][80];
  char str2[2][80];
  int nr;
      
  nr = dcli_parse( server, "@", "", (char *)str1, 
		   sizeof( str1) / sizeof( str1[0]), sizeof( str1[0]), 0);

  if ( nr == 1)
    strncpy( lhost, server, sizeof(lhost));
  else if ( nr >= 2) {
    strncpy( lhost, str1[1], sizeof(lhost));
    
    nr = dcli_parse( str1[0], ":", "", (char *)str2, 
		     sizeof( str2) / sizeof( str2[0]), sizeof( str2[0]), 0);
	
    if ( nr == 1)
      strncpy( luser, str1[0], sizeof(luser));
    else if ( nr >= 2) {
      strncpy( luser, str2[0], sizeof(luser));
      strncpy( lpassword, str2[1], sizeof(lpassword));
    }
  }
  nr = dcli_parse( host, ":", "", (char *)str1, 
		   sizeof( str1) / sizeof( str1[0]), sizeof( str1[0]), 0);
  if ( nr >= 2) {
    nr = sscanf( str1[0], "%u", port);
    if ( nr != 1) {
      printf("** Syntax error in mysql Server port\n");
      return LDH__NOSERVER;
    }
    strncpy( lhost, str1[1], sizeof(lhost));
  }

  if ( strcmp( host, "localhost") == 0)
    strcpy( lhost, "");

  strcpy( host, lhost);
  strcpy( user, luser);
  strcpy( password, lpassword);
  return LFU__SUCCESS;
}



