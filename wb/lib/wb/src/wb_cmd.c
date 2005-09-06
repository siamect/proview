/* 
 * Proview   $Id: wb_cmd.c,v 1.6 2005-09-06 10:43:31 claes Exp $
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

/* wb_cmd.c -- command file processing */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_VMS
#define CLI$_NORMAL 196609
#include <clidef.h>
#include <climsgdef.h>
#include <processes.h>
#include <smgdef.h>
#include <trmdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <syidef.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_ccm.h"
#include "co_cdh.h"
#include "co_msg.h"
#include "wb_utl.h"
#include "wb_trv.h"
#include "wb_cmd_msg.h"
#include "co_ccm_msg.h"
#include "wb_foe_msg.h"
#include "wb_foe_show.h"
#include "wb_cmd.h"

typedef struct
	{
	short int	buflen;
	short int	item;
	unsigned int	bufadr;
	unsigned int	lenadr;
	} cmd_t_itemlist;

int cli$dcl_parse();
int cli$dispatch();

show_ctx	cmd_showctx;

int *wb_show_cld () ;    /* module name in wb_show_cld.cld file */

static int			cmd_ccm_func_registred = 0;

/*____Local function prototypes_______________________________________*/

static int	cmd_get_defaultfilename(
			char	*inname,
			char	*outname,
			char	*ext);
static int	cmd_get_hw( char *hw);
static int cmd_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string
);
static int cmd_cut_segments (
  char	*outname,
  char	*name,
  int	segments
);


/*************************************************************************
*
* Name:		cmd_msg()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	If the message is a error, warning or info message it is printed.
*
**************************************************************************/
void cmd_msg( pwr_tStatus sts)
{
	static char msg[256];

#if defined OS_VMS
	if ( !foe_show_message)
          return;
#endif

	if ( ( EVEN(sts)) || ((sts & 1) && (sts & 2)) )
	{
	  msg_GetMsg( sts, msg, sizeof(msg));
	  printf("%s\n", msg);
	}
}

/*************************************************************************
*
* Name:		cmd_menu_command()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* cmd_ctx	ctx		I	cmd context.
*
*
* Description:
*
**************************************************************************/
int	cmd_remove_blank( char *out_str, char *in_str)
{
	char *s;

	s = in_str;
	/* Find first not blank */
	while ( *s)
	{
	  if ( !(*s == 9 || *s == 32)) 
	    break;
	  s++;
	}
	strcpy( out_str, s);
	/* Remove at end */
	s = out_str + strlen(out_str);
	s--;
	while( s >= out_str)
	{
	  if ( !(*s == 9 || *s == 32)) 
	    break;
	  s--;
	}
	s++;
	*s = 0;
	return CMD__SUCCESS;
}

/*************************************************************************
*
* Name:		cmd_get_defaultfilename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

static int	cmd_get_defaultfilename(
			char	*inname,
			char	*outname,
			char	*ext)
{
	char	*s;
	char	*s2;

	strcpy( outname, inname);

	/* Look for extention in filename */
	if ( ext != NULL)
	{
	  s = strrchr( inname, ':');
	  if ( s == 0)
	    s = inname;

	  s2 = strrchr( s, '>');
	  if ( s2 == 0)
	  {
	    s2 = strrchr( s, ']');
	    if ( s2 == 0)
	      s2 = s;
	  }

	  s = strrchr( s2, '.');
	  if ( s == 0)
	  {
	    /* No extention found, add extention */
	    strcat( outname, ext);
	  }
	}

	return CMD__SUCCESS;
}

int	cmd_externcmd_func( char *cmd, void *client_data)
{
  long		sts;
#ifdef OS_VMS
  char 		str[256];
  struct dsc$descriptor_s	str_desc = {sizeof(str)-1,DSC$K_DTYPE_T,
					DSC$K_CLASS_S,};

  str_desc.dsc$a_pointer = str;

  strcpy( str, cmd);

  sts = cli$dcl_parse ( 
      &str_desc ,  wb_show_cld, 0,  0 , 0  ) ;
    
  /* send the command to the rtn associated */ 
  if ( sts == CLI$_NORMAL ) 
  {	 
    sts = cli$dispatch ( ) ;  
    cmd_msg( sts);
  }
  cmd_set_status( sts);
#else
  printf("cmd_externcmd_func: NYI\n");
  sts = 1;
#endif
  return sts;
}

static int cmd_getattribute_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg	*arg_p2;
  int	sts;
  int	value_decl;
  int	value_int;
  float	value_float;
  char	value_string[80];

  if ( !(arg_count == 2 || arg_count == 1))
    return CCM__ARGMISM;

  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_count == 2 && arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  sts = cmd_attribute_func( arg_list->value_string, &value_decl,
	&value_float, &value_int, value_string);
  if ( EVEN(sts))
  {
    if ( arg_count == 2)
    {
      arg_p2->value_int = 0;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
    *return_decl = CCM_DECL_UNKNOWN;
  }
  else
  {
    if ( value_decl == CCM_DECL_INT)
    {
      *return_int = value_int;
      *return_decl = CCM_DECL_INT;
    }
    else if ( value_decl == CCM_DECL_FLOAT)
    {
      *return_float = value_float;
      *return_decl = CCM_DECL_FLOAT;
    }
    else if ( value_decl == CCM_DECL_STRING)
    {
      strcpy( return_string, value_string);
      *return_decl = CCM_DECL_STRING;
    }
    if ( arg_count == 2)
    {
      arg_p2->value_int = 1;
      arg_p2->value_returned = 1;
      arg_p2->var_decl = arg_p2->value_decl;
    }
  }

  return 1;
}

static int cmd_getchild_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_NameToObjid( cmd_showctx->ldhses, &parent_objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetChild ( cmd_showctx->ldhses, parent_objid, &child_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( cmd_showctx->ldhses, child_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getparent_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	parent_objid;
  pwr_tObjid	child_objid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_NameToObjid( cmd_showctx->ldhses, &child_objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetParent( cmd_showctx->ldhses, child_objid, &parent_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( cmd_showctx->ldhses, parent_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getnextsibling_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;
  pwr_tObjid	next_objid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_NameToObjid( cmd_showctx->ldhses, &objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetNextSibling ( cmd_showctx->ldhses, objid, &next_objid);
    if (ODD(sts))
      sts = ldh_ObjidToName( cmd_showctx->ldhses, next_objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getrootlist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tObjid	objid;
  int		size;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_GetRootList( cmd_showctx->ldhses, &objid);
  if (ODD(sts))
    sts = ldh_ObjidToName( cmd_showctx->ldhses, objid, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getobjectclass_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tClassId	class;
  pwr_tObjid	objid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_NameToObjid( cmd_showctx->ldhses, &objid, arg_list->value_string);
  if ( ODD(sts))
  {
    sts = ldh_GetObjectClass( cmd_showctx->ldhses, objid, &class);
    if ( ODD(sts))
      sts = ldh_ObjidToName( cmd_showctx->ldhses, cdh_ClassIdToObjid( class), 
		ldh_eName_Object, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getvolumelist_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tVolumeId	volid;
  int		size;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_GetVolumeList( cmd_showctx->ldhwb, &volid);
  if (ODD(sts))
    sts = ldh_VolumeIdToName( cmd_showctx->ldhwb, volid,
		name, sizeof( name), &size);

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getnextvolume_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tVolumeId	volid;
  pwr_tVolumeId	next_volid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_VolumeNameToId( cmd_showctx->ldhwb, arg_list->value_string, &volid);
  if ( ODD(sts))
  {
    sts = ldh_GetNextVolume( cmd_showctx->ldhwb, volid, &next_volid);
    if (ODD(sts))
      sts = ldh_VolumeIdToName( cmd_showctx->ldhwb, next_volid,
		name, sizeof( name), &size);
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_getvolumeclass_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  pwr_tClassId	class;
  pwr_tVolumeId	volid;
  int		size;

  if ( arg_count != 1)
    return CCM__ARGMISM;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);
  sts = ldh_VolumeNameToId( cmd_showctx->ldhwb, arg_list->value_string, &volid);
  if ( ODD(sts))
  {
    sts = ldh_GetVolumeClass( cmd_showctx->ldhwb, volid, &class);
    if ( ODD(sts))
      sts = ldh_ObjidToName( cmd_showctx->ldhses, cdh_ClassIdToObjid( class), 
		ldh_eName_Object, name, sizeof( name), &size);
  }
  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

static int cmd_cutobjectname_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  ccm_s_arg 	*arg_p2; 

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;

  if ( arg_list->value_decl != CCM_DECL_STRING)
    return CCM__ARGMISM;
  if ( arg_p2->value_decl != CCM_DECL_INT)
    return CCM__ARGMISM;

  cmd_cut_segments( return_string, arg_list->value_string, 
	arg_p2->value_int);

  *return_decl = CCM_DECL_STRING;
  
  return 1;
}


static int cmd_getnodeobject_func( 
  void *filectx,
  ccm_s_arg *arg_list, 
  int arg_count,
  int *return_decl, 
  float *return_float, 
  int *return_int, 
  char *return_string)
{
  int		sts;
  char		name[80];
  int		size;
  unsigned long	node_count;
  pwr_tObjid	*nodelist;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  foe_get_ldhses( cmd_showctx);

  sts = trv_get_rtnodes( cmd_showctx->ldhses, &node_count, &nodelist);
  if ( ODD(sts))
  {
    if ( node_count > 0)
    {
      sts = ldh_ObjidToName( cmd_showctx->ldhses, *nodelist, 
		ldh_eName_Hierarchy, name, sizeof( name), &size);
      XtFree( (char *)nodelist);
    }
    else
      sts = 0;
  }

  if ( ODD(sts))
    strcpy( return_string, name);
  else
    strcpy( return_string, "");
  *return_decl = CCM_DECL_STRING;
  
  return 1;
}

int	cmd_deffilename_func( char *outfile, char *infile, void *client_data)
{
	cmd_get_defaultfilename( infile, outfile, ".pwr_com");
        return 1;
}

int	cmd_errormessage_func( char *msg, int severity, void *client_data)
{
	if ( EVEN(severity))
	  printf( "%%CCM-E-MSG, %s\n", msg);
	else
	  printf( "%%CCM-I-MSG, %s\n", msg);
        return 1;
}

/*************************************************************************
*
* Name:		cmd_get_command()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

int	cmd_readcmdfile( char		*incommand,
			 show_ctx 	showctx)
{
	char		input_str[160];
	int		sts;
	int		appl_sts;
	char		hw[20];

	if ( !cmd_ccm_func_registred)
	{
	  sts = ccm_register_function( "GetAttribute", cmd_getattribute_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetChild", cmd_getchild_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetParent", cmd_getparent_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNextSibling", cmd_getnextsibling_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetRootList", cmd_getrootlist_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetObjectClass", cmd_getobjectclass_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetVolumeList", cmd_getvolumelist_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNextVolume", cmd_getnextvolume_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetVolumeClass", cmd_getvolumeclass_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "CutObjectName", cmd_cutobjectname_func);
	  if ( EVEN(sts)) return sts;
	  sts = ccm_register_function( "GetNodeObject", cmd_getnodeobject_func);
	  if ( EVEN(sts)) return sts;

	  /* Insert some external variables */
	  sts = cmd_get_hw( hw);
          if ( EVEN(sts)) return sts;
	  sts = ccm_create_external_var( "cmd_hw", CCM_DECL_STRING, 0, 0, hw);
	  sts = ccm_create_external_var( "cmd_os", CCM_DECL_STRING, 0, 0, "os_vms");
	  sts = ccm_create_external_var( "cmd_status", CCM_DECL_INT, 0, 1, 
		NULL);

	  cmd_ccm_func_registred = 1;
        }

        cmd_showctx = showctx;
	strcpy( input_str, incommand);
	cmd_remove_blank( input_str, input_str);

	/* Read and execute the command file */
	sts = ccm_file_exec( input_str, cmd_externcmd_func,
		cmd_deffilename_func, cmd_errormessage_func, &appl_sts, 
		foe_show_verify, 0, NULL, 0, 0, NULL, NULL);
        if ( EVEN(sts)) return sts;

	return CMD__SUCCESS;
}


/*************************************************************************
*
* Name:		cmd_get_platform()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get platform for the current system.
*
**************************************************************************/

static int	cmd_get_hw( char *hw)
{
#if OS_VMS
	int		sts;
	char		hw_name[20];
	unsigned short	hw_name_len;
	cmd_t_itemlist	item_list[2] = {{ 0,SYI$_HW_NAME,0,0},
					{ 0,0,0,0}};
 	int    		pri_len;
  
	item_list[0].lenadr = (unsigned int) &hw_name_len;
	item_list[0].bufadr = (unsigned int) &hw_name;
	item_list[0].buflen = sizeof( hw_name);

	sts = sys$getsyiw(0, NULL, NULL, &item_list, 0, NULL, 0);
	if (EVEN(sts)) return sts;
	hw_name[hw_name_len] = 0;
	if ( strstr(hw_name, "VAX"))
	  strcpy( hw, "hw_vax");
	else
	  strcpy( hw, "hw_axp");
#else
	strcpy( hw, "hw_x86");
#endif
	return 1;

}

/*************************************************************************
*
* Name:		cmd_set_status()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

void	cmd_set_status( pwr_tStatus	sts)
{
	int	status;

	if ( ODD(sts))
	  status = 1;
	else
	  status = sts;
	ccm_set_external_var( "cmd_status", CCM_DECL_INT, 0, status, NULL);
}



/*************************************************************************
*
* Name:		cmd_attribute_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	
*
**************************************************************************/

static int cmd_attribute_func ( 
  char		*name,
  int		*return_decl,
  float		*return_float,
  int		*return_int,
  char		*return_string
)
{
	int		sts, size, i, j;
	pwr_tClassId	class;
	char		hier_name[80];
	char		*hier_name_p;
	ldh_sParDef 	*bodydef;
	int		rows;
	char		body[20];	
	char		*object_par;
	char		*object_element;
	int		elements;
	int		element;
	char		elementstr[20];
	int		found;
	char		*s;
	char		*t;
	char		upper_name[80];
	pwr_tObjid	objid;
	char		objname[120];
	char		attrname[32];
	int		len;
	int		decl;
	int		int_val;
	float		float_val;
	char		string_val[80];

	/* Parse the string in name and attribute */
	strcpy( objname, name);
	s = strrchr( objname, '.');
        if ( s == 0)
	  return FOE__NOPAR;
	*s = 0;

	strcpy( attrname, s+1);
	cdh_ToUpper( attrname, attrname);

	/* Check index in parameter */
	s = strchr( attrname, '[');
	if ( s == 0)
	  element = 0;
	else
	{
	  t = strchr( attrname, ']');
	  if ( t == 0)
	    return FOE__PARSYNT;
	  else
	  {
	    len = t - s - 1;
	    strncpy( elementstr, s + 1, len);
	    elementstr[ len ] = 0;
	    sscanf( elementstr, "%d", &element);
	    *s = '\0';
	    if ( (element < 0) || (element > 100) )
	      return FOE__PARELSYNT;
	  }
	}
	
        foe_get_ldhses( cmd_showctx);
        sts = ldh_NameToObjid( cmd_showctx->ldhses, &objid, objname);
        if ( EVEN(sts)) return sts;

	/* Get the class of the object */
	sts = ldh_GetObjectClass( cmd_showctx->ldhses, objid, &class);

	found = 0;
	for ( i = 0; i < 3; i++ )
	{
	  if ( i == 0)
	    strcpy( body, "RtBody");
	  else if ( i == 1 )
	    strcpy( body, "DevBody");
	  else
	    strcpy( body, "SysBody");

    	  /* Get the parameters for this body */
	  sts = ldh_GetObjectBodyDef(cmd_showctx->ldhses, class, body, 1, 
	  		&bodydef, &rows);
	  if ( EVEN(sts) ) continue;

	  for ( j = 0; j < rows; j++)
	  {
	    /* Convert parname to upper case */
	    cdh_ToUpper( upper_name, bodydef[j].ParName);

	    if (strcmp( attrname, upper_name) == 0)
	    {
	      found = 1;
	      break;
	    }
	  }
	  if ( found )
	    break;
	  free((char *) bodydef);	
	}
	if ( !found)
	{
	  /* Parametern fanns ej */
	  return FOE__NOPAR;
	}

	strcpy( attrname, bodydef[j].ParName);

	if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_ARRAY )
	  elements = bodydef[j].Par->Output.Info.Elements;
	else
	  elements = 1;

	/* Get the parameter value in object */
	sts = ldh_GetObjectPar( cmd_showctx->ldhses, objid, body,   
			attrname, (char **)&object_par, &size); 
	if ( EVEN(sts)) return sts;

	object_element = object_par + element * size / elements;

        switch ( bodydef[j].Par->Output.Info.Type )
        {
          case pwr_eType_Boolean:
          {
	    int_val = *(pwr_tBoolean *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Float32:
          {
	    float_val = *(pwr_tFloat32 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Float64:
          {
	    float_val = *(pwr_tFloat64 *)object_element;
	    decl = CCM_DECL_FLOAT;
            break;
          }
          case pwr_eType_Char:
          {
	    int_val = *(pwr_tChar *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int8:
          {
	    int_val = *(pwr_tInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int16:
          {
	    int_val = *(pwr_tInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_Int32:
          {
	    int_val = *(pwr_tInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt8:
          {
	    int_val = *(pwr_tUInt8 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt16:
          {
	    int_val = *(pwr_tUInt16 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_UInt32:
	  case pwr_eType_ClassId:
	  case pwr_eType_TypeId:
	  case pwr_eType_CastId:
	  case pwr_eType_VolumeId:
	  case pwr_eType_ObjectIx:
          {
	    int_val = *(pwr_tUInt32 *)object_element;
	    decl = CCM_DECL_INT;
            break;
          }
          case pwr_eType_String:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Text:
          {
	    strncpy( string_val, object_element, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_ObjDId:
          {
            /* Get the object name from ldh */
            sts = ldh_ObjidToName( cmd_showctx->ldhses, 
                 	*(pwr_tObjid *)object_element, ldh_eName_Hierarchy,
   		        hier_name, sizeof( hier_name), &size);
 	    if ( EVEN(sts))
	      strcpy( string_val, "Undefined Object");
	    else
	      strncpy( string_val, hier_name, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
	  case pwr_eType_AttrRef:
          {
            /* Get the object name from ldh */
	    sts = ldh_AttrRefToName( cmd_showctx->ldhses, 
	           	(pwr_sAttrRef *)object_element,  ldh_eName_Aref, 
			&hier_name_p, &size);
 	    if ( EVEN(sts))
	      strcpy( string_val, "Undefined attribute");
	    else
	      strncpy( string_val, hier_name_p, sizeof( string_val));
	    string_val[sizeof(string_val)-1] = 0;
	    decl = CCM_DECL_STRING;
            break;
          }
          case pwr_eType_Time:
          {
	    /* Convert time to ascii */	
	    sts = time_AtoAscii((pwr_tTime *)object_element, 
                      time_eFormat_DateAndTime, 
                      string_val, sizeof(string_val));
	    string_val[20] = 0;
	    decl = CCM_DECL_STRING;
	    break;
          }
          default:
            ;
        }
	free((char *) bodydef);	

	*return_decl = decl;
	if ( decl == CCM_DECL_INT)
	  *return_int = int_val;
	else if ( decl == CCM_DECL_FLOAT)
	  *return_float = float_val;
	else if ( decl == CCM_DECL_STRING)
	  strcpy( return_string, string_val);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		cmd_cut_segments()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	
*
**************************************************************************/

static int cmd_cut_segments (
  char	*outname,
  char	*name,
  int	segments
)
{
	char	*s[20];
	int	i, j, last_i;

	for( i = 0; i < segments; i++)
	{
	  s[i] = strrchr( name, '-');
	  if ( s[i] == 0)
	  {
	    last_i = i;
	    break;
	  }
	  *s[i] = '+';
	  last_i = i;
	}
	for ( j = 0; j <= last_i; j++)
	{
	  if ( s[j] != 0)
	    *s[j] = '-';
	}
	if ( s[last_i] == 0)
	  strcpy( outname, name);
	else
	  strcpy( outname, s[last_i] + 1);

	return FOE__SUCCESS;
}

