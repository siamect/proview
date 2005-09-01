/** 
 * Proview   $Id: wb_trv.c,v 1.7 2005-09-01 14:57:59 claes Exp $
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

/* wb_trv.c -- traversing of work bench
   Traverses the workbench to get the plc's , windows ...  */

#include <stdio.h>
#include <ctype.h>

#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "wb_foe_msg.h"
#include "wb_foe_macros.h"
#include <Xm/Xm.h>
#include "wb_ldh.h"
#include "wb_vldh.h"
#include "wb_utl.h"
#include "wb_trv.h"

#define TRV_CROSS_SEARCH 0
#define TRV_CROSS_CROSSREF 1
#define TRV_CROSS_RETURN 2


/*_Local procedues_______________________________________________________*/

static int trv_get_child_object_search (
  trv_ctx	trvctx,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_plc (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  unsigned long	*plc_count,
  pwr_tObjid	**plclist
);

static int trv_get_child_rtnode ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  unsigned long	*rtnode_count,
  pwr_tObjid	**rtnodelist
);

static int trv_get_window_subwindows (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*wind_count,
  pwr_tObjid	**windlist
);

static int trv_get_child_object (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_object_class (
  ldh_tSesContext ldhses,
  pwr_tClassId	class,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_object_hi_cl_na ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*class,
  char		*name,
  pwr_tObjid	childobjdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_object_name (
  ldh_tSesContext ldhses,
  char		*name,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_object_hier_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  unsigned long	*hierflag,
  char		*name,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_object_hier_class (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  unsigned long	*hierflag,
  pwr_tClassId	class,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

static int trv_get_child_docobject (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);










/*************************************************************************
*
* Name:		trv_wildcard()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*wildname	I	wildcard name.
* char		*name		I	object name.
*
* Description:
*	Checks if the object name can be described by the
*	wildcard string.
*	Returns 0 if ok, else 1.
*
**************************************************************************/

static int trv_wildcard (
  char	*wildname,
  char	*name
)
{
	int	len;
	char	*s;
	char	*t;
	char	*u;
	char	checkstr[240];
	char	upper_name[240];
	char	namechar;

	/* Convert to upper case */
	u = name;
	t = upper_name;
	while ( *u != '\0')
	{
	  namechar = *(u++);
	  *t = _toupper( namechar);
	  if (*t == 'ö') *t = (char)'Ö';
	  else if (*t == 'ä') *t = (char)'Ä';
	  else if (*t == 'å') *t = (char)'Å';
	  t++;
	}
	*t = '\0';

	t = wildname;
	u = upper_name;
	s = strchr( t, '*');
	if ( s == 0)
	{
	  if (strcmp( t, u) == 0)
	    return 0;
	  else 
	    return 1;
	}

	len = s - t;
	if ( len > 0)
	{
	  strncpy ( checkstr, t, len);
	  checkstr[len] = '\0';
	  if ( strncmp ( checkstr, u, len) != 0)
	    return 1;
	  u += len;
	}	  
	t += len + 1;
	s = strchr( t, '*');

	while ( s != 0)
	{
	  len = s - t;
	  if ( len > 0)
	  {
	    strncpy ( checkstr, t, len);
	    checkstr[len] = '\0';
	    u = strstr( u, checkstr);
	    if ( u == 0)
	      return 1;
	    u += len;
	  }	  
	  t += len + 1;
	  s = strchr( t, '*');
	}
	strcpy ( checkstr, t);
	u = u + strlen(u) - strlen (checkstr);
	if ( strcmp ( checkstr, u ) != 0)
	      return 1;

	return 0;
}

/*************************************************************************
*
* Name:		trv_get_child_rtnode()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	ldhses		I	ldh session.
* unsigned long	objdid		I	objdid of the object
* unsigned long	* rtnode_count	IO	the number of rtnodes in rtnodelist.
* void **	rtnodelist	IO	list of found rtnodes.
*
* Description:
*	Routine used by trv_get_rtnode to find all nodes in a system.
*	Checks if the objdid is of class $Node, and if that is the case
*	adds it to the rtnodelist and increments rtnode_count.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_rtnode ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  unsigned long	*rtnode_count,
  pwr_tObjid	**rtnodelist
){
	pwr_tClassId		class;
	int			sts;
	pwr_tObjid		*rtnodelist_pointer;	

	/* Check if this is a rtnode */
	sts = ldh_GetObjectClass( ldhses, objdid, &class);
	if ( class == pwr_eClass_Node)
	{
	  /* Yes, this is a rtnode, insert in list */
	  sts = utl_realloc( (char **)rtnodelist, 
		*rtnode_count * sizeof(pwr_tObjid), 
		(*rtnode_count + 1) * sizeof(pwr_tObjid));
	  if (EVEN(sts)) return sts;

	  rtnodelist_pointer = *rtnodelist;
	  *(rtnodelist_pointer + *rtnode_count) = objdid;
	  (*rtnode_count)++;
	}

	/* Get all the children of this  object */
	sts = ldh_GetChild( ldhses, objdid, &objdid);
	while ( ODD(sts) )
	{
	  /* Get the nodes */
	  sts = trv_get_child_rtnode( ldhses, objdid, rtnode_count, rtnodelist);
	  if ( EVEN(sts)) return sts;  

	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_rtnodes()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	* rtnode_count	O	the number of rtnodes in rtnodelist.
* void **	rtnodelist	O	list of found rtnodes.
*
* Description:
*	Finds all nodes in a system.
*	Returns a pointer to list of all objects of class $Node found in the
*	workbench. The user should free the allocated space of the list
*	by a Xtfree call.
*
**************************************************************************/

int trv_get_rtnodes (
  ldh_tSesContext ldhses,
  unsigned long	  *rtnode_count,
  pwr_tObjid	  **rtnodelist
)
{
	int			sts;
	pwr_tObjid	objdid;
	pwr_tClassId	class;


	*rtnode_count = 0;
	*rtnodelist = NULL;


	sts = ldh_GetRootList( ldhses, &objdid);
	while ( ODD(sts) )
	{
	  sts = ldh_GetObjectClass( ldhses, objdid, &class);
	  if ( EVEN(sts)) return sts;

	  /* Check that the class of the node object is correct */
	  if ( class == pwr_eClass_NodeHier)
	  {
	    /* Check if the children is a rtnode */
	    sts = trv_get_child_rtnode( ldhses, objdid, rtnode_count,
			rtnodelist);
	    if ( EVEN(sts)) return sts;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_plc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	objdid		I	objdid of the object
* unsigned long	rtnode		I	objdid of the $Node object
* unsigned long	* plc_count	IO	the number of plcpgm's in plclist
* void **	plclist		IO	list of found plcpgm objects.
*
* Description:
*	Routine used by trv_get_rtnode_plc to find all plcpgm's connected
*	to a node.
*	Checks if the objdid is of class Plcpgm and if this is the case
*	and the node of the plcpgm is equivalent to the given rtnode
*	it adds the plcpgm to the plclist and increments plc_count.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_plc (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  unsigned long	*plc_count,
  pwr_tObjid	**plclist
)
{
	pwr_tClassId		class;
	int			sts;
	pwr_tObjid		*plclist_pointer;	

	/* Get all the children of this  node */
	sts = ldh_GetChild( ldhses, objdid, &objdid);
	while ( ODD(sts) )
	{
	  /* Check if plc */
	  sts = ldh_GetObjectClass( ldhses, objdid, &class);
	  if ( class == pwr_cClass_plc)
	  {
	    sts = utl_realloc( (char **)plclist, *plc_count * sizeof(pwr_tObjid), 
		(*plc_count + 1) * sizeof(pwr_tObjid));
	    if (EVEN(sts)) return sts;

	    plclist_pointer = *plclist;
	    *(plclist_pointer + *plc_count) = objdid;
	    (*plc_count)++;
	  }
	  else
	  {
	    /* Check if the children is a plc */
	    sts = trv_get_child_plc( ldhses, objdid, plc_count, 
			plclist); 
	    if ( EVEN(sts)) return sts;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		trv_get_plcpgms()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	* plc_count	O	the number of plcpgm's in plclist
* void **	plclist		O	list of found plcpgm objects.
*
* Description:
*	Finds all plcpgm's in a volume.
*	Returns a pointer to list of all objects of class Plcpgm found in the
*	workbench that is conneted to the specified node. 
*	The user should free the allocated space of the list
*	by a Xtfree call.
*
**************************************************************************/

int trv_get_plcpgms (
  ldh_tSesContext ldhses,
  unsigned long	*plc_count,
  pwr_tObjid	**plclist
)
{
	int			sts;
	pwr_tObjid	objdid;
	pwr_tClassId	class;

	*plc_count = 0;
	*plclist = NULL;

	sts = ldh_GetRootList( ldhses, &objdid);
	while ( ODD(sts) )
	{
	  sts = ldh_GetObjectClass( ldhses, objdid, &class);
	  if ( EVEN(sts)) return sts;

	  /* Check that the class of the node object is correct */
	  if ( class == pwr_eClass_PlantHier)
	  {
	    /* Check if the children is a plc */
	    sts = trv_get_child_plc( ldhses, objdid, plc_count,
			plclist);
	    if ( EVEN(sts)) return sts;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		trv_get_plc_window()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	plc		I	a object of class Plcpgm
* unsigned long	* window	O	the window of the plc
*
* Description:
*	Returns the root window of a plcprogram.
*
**************************************************************************/

int trv_get_plc_window ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	  plc,
  pwr_tObjid	  *window
)
{
	int			sts, size, window_found;
	pwr_tObjid		objdid;
	pwr_tClassId		class;
	char			*windbuffer;

	/* Check if windowobject in ldh exists */
	/* Get the first child to the plc */
	sts = ldh_GetChild( ldhses, plc, &objdid);
	window_found = 0;
	while ( ODD(sts) )
	{
	  /* Check if window */
	  sts = ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcWindow", (pwr_eClass *) &class,	
			&windbuffer, &size);
	  if ( ODD(sts)) 
	  {
	    free( windbuffer);
	    window_found = 1;	
	    break;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	if ( !window_found )
	  return GSX__NOSUBWINDOW;
 
	*window = objdid;
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_window_subwindows()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	window		I	objdid of the window object
* unsigned long	* window_count	O	the number of windows in windowlist
* void **	windowlist	O	list of found window objects.
*
*
* Description:
*	Routine used by trv_get_rtnode_plc to find all subwindows of a
*	window.
*	Checks if the objdid is of some window class and if it is
*	adds it to the windowlist and increments window_count.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_window_subwindows (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*wind_count,
  pwr_tObjid	**windlist
)
{
	int			sts, size;
	pwr_tObjid		objdid;
	pwr_tObjid		subwindow;
	pwr_tClassId		class;
	char			*windbuffer;
	pwr_tObjid		*windlist_pointer;	

	/* Insert the window in windlist */
	sts = utl_realloc( (char **)windlist, *wind_count * sizeof(pwr_tObjid), 
		(*wind_count + 1) * sizeof(pwr_tObjid));
	if (EVEN(sts)) return sts;

	windlist_pointer = *windlist;
	*(windlist_pointer + *wind_count) = window;
	(*wind_count)++;

	/* Get all the children of the window */
	sts = ldh_GetChild( ldhses, window, &objdid);
	while ( ODD(sts) )
	{
	  /* Look for a window below this child */
	  sts = ldh_GetChild( ldhses, objdid, &subwindow);
	  while ( ODD(sts) )
	  {
	    /* Check if window */
	    sts = ldh_GetObjectBuffer( ldhses,
			subwindow, "DevBody", "PlcWindow", 
			(pwr_eClass *) &class, &windbuffer, &size);
	    if ( ODD(sts)) 
	    {
	      free( windbuffer);
	      sts = trv_get_window_subwindows( ldhses, subwindow, wind_count,
			windlist);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = ldh_GetNextSibling( ldhses, subwindow, &subwindow);
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_window_windows()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	window		I	objdid of the window object
* unsigned long	* window_count	O	the number of windows in windowlist
* void **	windowlist	O	list of found window objects.
*
* Description:
*	Get a list of all subwindows under a window.
*	The rootwindow is returned first in the list.
*	Returns a pointer to list of all window objects found in the
*	workbench that is found below the specified rootwindow. 
*	The user should free the allocated space of the list
*	by a Xtfree call.
*
*
**************************************************************************/

int trv_get_window_windows (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*wind_count,
  pwr_tObjid	**windlist
)
{
	int			sts;

	*windlist = NULL;
	*wind_count = 0;

	sts = trv_get_window_subwindows( ldhses, window, wind_count,
		windlist);
	if ( EVEN(sts)) return sts;

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_window_objects()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	window		I	objdid of the window object
* unsigned long	* object_count	O	the number of objects in objectlist
* void **	objectlist	O	list of found objects.
*
* Description:
*	Returns the objects in a window ( not connections objects).
*	Returns a pointer to list of all objects found in the
*	window.
*	The user should free the allocated space of the list
*	by a Xtfree call.
*
**************************************************************************/

int trv_get_window_objects (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
)
{
	int			sts, size;
	pwr_tObjid		objdid;
	pwr_tClassId		class;
	char			*nodebuffer;
	pwr_tObjid		*objectlist_pointer;

	*object_count = 0;
	*objectlist = NULL;

	/* Get the first child to the window */
	sts = ldh_GetChild( ldhses, window, &objdid);
	while ( ODD(sts) )
	{
	  /* Check if node */
	  sts = ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcNode", (pwr_eClass *) &class,	
			&nodebuffer, &size);
	  if ( ODD(sts)) 
	  {
	    free( nodebuffer);
	    /* insert into object list */
	    sts = utl_realloc( (char **)objectlist, *object_count * sizeof(pwr_tObjid), 
		(*object_count + 1) * sizeof(pwr_tObjid));
	    if (EVEN(sts)) return sts;

	    objectlist_pointer = *objectlist;
	    *(objectlist_pointer + *object_count) = objdid;
	    (*object_count)++;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_window_connections()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	window		I	objdid of the window object
* unsigned long	* object_count	O	the number of objects in objectlist
* void **	objectlist	O	list of found objects.
*
* Description:
*	Returns the connection objects in a window.
*	Returns a pointer to list of all objects found in the
*	window.
*	The user should free the allocated space of the list
*	by a Xtfree call.
*
**************************************************************************/

int trv_get_window_connections (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
)
{
	int			sts, size;
	pwr_tObjid		objdid;
	pwr_tClassId		class;
	char			*nodebuffer;
	pwr_tObjid		*objectlist_pointer;

	*object_count = 0;

	/* Get the first child to the window */
	sts = ldh_GetChild( ldhses, window, &objdid);
	while ( ODD(sts) )
	{
	  /* Check if connection */
	  sts = ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcConnection", 
			(pwr_eClass *) &class, &nodebuffer, &size);
	  if ( ODD(sts)) 
	  {
	    free( nodebuffer);
	    /* insert into object list */
	    sts = utl_realloc( (char **)objectlist, *object_count * sizeof(pwr_tObjid), 
	  	(*object_count + 1) * sizeof(pwr_tObjid));
	    if (EVEN(sts)) return sts;

	    objectlist_pointer = *objectlist;
	    *(objectlist_pointer + *object_count) = objdid;
	    (*object_count)++;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_parentlist()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* unsigned long	object		I	objdid of the object
* unsigned long	* object_count	O	the number of parents in objectlist
* void **	objectlist	O	list of found parents.
*
* Description:
*	Returns the all parents untill the plcpgm. The plcpgm is
*	positioned last in the list and the closest parent first.
*	Returns a pointer to list of all parents found to the object
*	until a plcpgm is found.
*	The user should free the allocated space of the list
*	by a Xtfree call.
*
**************************************************************************/

int trv_get_parentlist (
  ldh_tSesContext ldhses,
  pwr_tObjid	object,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
)
{
	int			sts, size, plc_found;
	pwr_tObjid		objdid;
	pwr_tClassId		class;
	pwr_tObjid		*objectlist_pointer;
	char			*plcbuffer;

	*object_count = 0;

	/* Get the first parent to the object */
	plc_found = 0;
	sts = ldh_GetParent( ldhses, object, &objdid);
	while ( ODD(sts) || plc_found )
	{
	  /* Check if plc */
	  if ( ODD( ldh_GetObjectBuffer( ldhses,
			objdid, "DevBody", "PlcProgram", (pwr_eClass *) &class,	
			&plcbuffer, &size) ))
	  {
	    plc_found = 1;
	    free( plcbuffer);
	  }

	  /* insert into object list */
	  sts = utl_realloc( (char **)objectlist, *object_count * sizeof(pwr_tObjid), 
		(*object_count + 1) * sizeof(pwr_tObjid));
	  if (EVEN(sts)) return sts;

	  objectlist_pointer = *objectlist;
	  *(objectlist_pointer + *object_count) = objdid;
	  (*object_count)++;
	  if ( plc_found)
	    break;
	  sts = ldh_GetParent( ldhses, objdid, &objdid);
	}
	if ( !plc_found )
	{
	  return GSX__BADWIND;
	}

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects to find all objects in a system.
*	Calls a backcallroutine with the given arguments for every object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_sAttrRef		aref;

	/* call the backcall routine */
	aref = cdh_ObjidToAref( objdid);
	sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	if ( EVEN(sts)) return sts;

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &objdid);
	while ( ODD(sts) )
	{
	  sts = trv_get_child_object( ldhses, objdid, backcall, 
		arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_class()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	class		I	class of the wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_class to find all objects in a system of
*	a specified class.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_class (
  ldh_tSesContext ldhses,
  pwr_tClassId	class,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tClassId		obj_class;

	/* call the backcall routine */
	sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	if ( EVEN(sts)) return sts;

	if ( obj_class == class )
	{
	  pwr_sAttrRef aref = cdh_ObjidToAref( objdid);
	  sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &objdid);
	while ( ODD(sts) )
	{
	  sts = trv_get_child_object_class( ldhses, class, objdid, backcall, 
		arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_objects_class()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tClassId	class		I	class of the wanded objects.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Traverses the objects in the plant and nodehierarchy.
*	Calls a backcallroutine with the given arguments for every found 
*	object of the specified class. The objdid of the found object 
*	and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	pwr_tObjid	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
*
**************************************************************************/

int trv_get_objects_class (
  ldh_tSesContext ldhses,
  pwr_tClassId	class,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tObjid	objdid;
	pwr_tClassId	obj_class;

	sts = ldh_GetRootList( ldhses, &objdid);
	while ( ODD(sts) )
	{
	  sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	  if ( EVEN(sts)) return sts;

	  /* Check that the class of the node object is correct */
	  if ( !( obj_class == pwr_eClass_ClassHier ||
	          obj_class == pwr_eClass_TypeHier))
	  {
	    /* Check if the children is a plc */
	    sts = trv_get_child_object_class( ldhses, class, objdid, backcall, 
		arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	  }
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_hier_class_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tObjid	hierobjdid is found.
* pwr_tClassId	class		I	class of the wanted objects.
* char		*name		I	wildcard name of wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_hier_class
*	to find all objects in a system of
*	a specified class that has a specific object as ancestor.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class below the specified hierarchy object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_hi_cl_na (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*class,
  char		*name,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts, size;
	pwr_tClassId		obj_class;
	pwr_tObjid		childobjdid;
	int			name_ok, class_ok;
	pwr_tOName     		obj_name;
	int			i;

	/* Check class if class is specified */
	class_ok = 1;
	sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	if ( EVEN(sts)) return sts;

	if ( class != NULL )
	{
	  i = 0;
	  class_ok = 0;
	  while ( *(class + i) != 0)
	  {
	    if ( obj_class == *(class + i))
	    {
	      class_ok = 1;
	      break;
	    }
	    i++;
	  }
	}

	/* Check name if name is specified */
	name_ok = 1;
	if ( class_ok && (name != NULL) )
	{
	  /* Get the name of the object */
	  sts = ldh_ObjidToName( ldhses, objdid, ldh_eName_Hierarchy,
		obj_name, sizeof( obj_name), &size);
	  if ( EVEN(sts)) return sts;

	  if ( trv_wildcard( name, obj_name) != 0)
	    name_ok = 0;	
	}

	if ( class_ok && name_ok)
	{
	  pwr_sAttrRef aref = cdh_ObjidToAref( objdid);

	  sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}

	/* Get the first child to the object */
	if ( obj_class != pwr_eClass_MountObject)
	{
	  sts = ldh_GetChild( ldhses, objdid, &childobjdid);
	  while ( ODD(sts))
 	  {
	    sts = trv_get_child_object_hi_cl_na( ldhses, hierobjdid,
		class, name, childobjdid, backcall, arg1, arg2,
		arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_GetNextSibling( ldhses, childobjdid, &childobjdid);
	  }
	}
	return GSX__SUCCESS;
}


/*
   cpp call to trv_get_objects_hier_class_name
 */
int trv_get_objects_hcn (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*classid,
  char		*name,
  int		(*backcall)( pwr_sAttrRef *, void *, void *, void *, void *, void *),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5)
{
  return trv_get_objects_hier_class_name( ldhses, hierobjdid, classid, name,
					  backcall, arg1, arg2, arg3, arg4, arg5);
}

/*************************************************************************
*
* Name:		trv_get_objects_hier_class_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tClassId	class		I	class of the wanded objects.
* char		*name		I	wildcard name of wanted objects.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Traverses the objects in the plant and nodehierarchy.
*	Calls a backcallroutine with the given arguments for every found 
*	object of the specified class that is found below the hierobjdid in
*	the hierarchy and that  fits in a wildcard description. 
*	If hierobjdid is eq 0 the hierarchy is not tested.
*	If class is eq 0 the class is not tested.
*	If name is eq NULL the name is not tested.
*	The objdid of the found object and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	unsigned long	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
*	Calls a backcall routine for every object in the plathierarchy
*	of a specified class under a specific object in the hierarchy.
*
**************************************************************************/



int trv_get_attrobjects (
  ldh_tSesContext ldhses,
  pwr_tOid	oid,
  pwr_tCid	*cid,
  char		*name,
  trv_eDepth	depth,
  trv_tBcFunc   backcall,
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
  int i;
  pwr_tStatus sts;
  pwr_sAttrRef aref;
  char *aname;
  int size;
  pwr_tObjid child;
  pwr_tCid child_cid;

  switch ( depth) {
  case trv_eDepth_Deep:
    if ( !cid) {
      /* TODO */
    }
    else {
      for ( i = 0; cid[i]; i++) {
	for ( sts = ldh_GetClassListAttrRef( ldhses, cid[i], &aref);
	      ODD(sts);
	      sts = ldh_GetNextAttrRef( ldhses, cid[i], &aref, &aref)) {
	  if ( cdh_ObjidIsNotNull( oid) && 
	       ! cdh_ObjidIsEqual( oid, aref.Objid) &&
	       ! ldh_IsAncestor( ldhses, oid, aref.Objid))
	    continue;
	  if ( name) {
	    /* Get the name of the object */
	    sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Hierarchy,
				     &aname, &size);
	    if ( EVEN(sts)) return sts;
	    
	    if ( trv_wildcard( name, aname) == 1)
	      continue;
	  }
	
	  sts = (backcall) ( &aref, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}
      }
    }
    break;
  case trv_eDepth_Self:
    if ( !cid)
      return 0;
    for ( i = 0; cid[i]; i++) {
      for ( sts = ldh_GetObjectClassList( ldhses, cid[i], oid, &aref);
	    ODD(sts);
	    sts = ldh_GetNextObjectAttrRef( ldhses, cid[i], &aref, &aref)) {
	if ( name) {
	  /* Get the name of the object */
	  sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Hierarchy,
				   &aname, &size);
	  if ( EVEN(sts)) return sts;
	  
	  if ( trv_wildcard( name, aname) == 1)
	    continue;
	}
	
	sts = (backcall) ( &aref, arg1, arg2, arg3, arg4, arg5);
	if ( EVEN(sts)) return sts;
      }
    }
    break;
  case trv_eDepth_Children:
    for ( sts = ldh_GetChild( ldhses, oid, &child);
	  ODD(sts);
	  sts = ldh_GetNextSibling( ldhses, child, &child)) {

      if ( cid) {
	for ( i = 0; cid[i]; i++) {
	  sts = ldh_GetObjectClass( ldhses, child, &child_cid);
	  if ( EVEN(sts)) return sts;

	  if ( child_cid == cid[i]) {
	    aref = cdh_ObjidToAref( child);
	    
	    sts = (backcall) ( &aref, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	    break;
	  }
	  else {
	    for ( sts = ldh_GetObjectClassList( ldhses, cid[i], child, &aref);
		  ODD(sts);
		  sts = ldh_GetNextObjectAttrRef( ldhses, cid[i], &aref, &aref)) {
	      if ( name) {
		/* Get the name of the object */
		sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Hierarchy,
					 &aname, &size);
		if ( EVEN(sts)) return sts;
	      
		if ( trv_wildcard( name, aname) == 1)
		  continue;
	      }
	
	      sts = (backcall) ( &aref, arg1, arg2, arg3, arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }
	  }
	}
      }
      else {
	// No class
	aref = cdh_ObjidToAref( oid);

	if ( name) {
	  /* Get the name of the object */
	  sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Hierarchy,
				   &aname, &size);
	  if ( EVEN(sts)) return sts;
	      
	  if ( trv_wildcard( name, aname) == 1)
	    continue;
	}
	sts = (backcall) ( &aref, arg1, arg2, arg3, arg4, arg5);
	if ( EVEN(sts)) return sts;
      }
    }
    break;
  default:
    ;
  }

  return GSX__SUCCESS;
}

int trv_get_objects_hier_class_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*class,
  char		*name,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tObjid	objdid;
	pwr_tClassId	obj_class;

	if ( cdh_ObjidIsNotNull( hierobjdid))
	{
	  /* Hierarchy object is supplied. Check if the children */
	  sts = trv_get_child_object_hi_cl_na( ldhses, hierobjdid,
		class, name, hierobjdid, backcall, arg1, arg2, arg3, 
		arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}
	else
	{
	  sts = ldh_GetRootList( ldhses, &objdid);
	  while ( ODD(sts) )
	  {
	    sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	    if ( EVEN(sts)) return sts;

	    /* Check that the class of the node object is correct */
	    if ( !( obj_class == pwr_eClass_ClassHier ||
	            obj_class == pwr_eClass_TypeHier))
	    {
	      /* Check if the children */
	      sts = trv_get_child_object_hi_cl_na( ldhses, hierobjdid,
		class, name, objdid, backcall, arg1, arg2, arg3, 
		arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	  }
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_children_class_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tClassId	class		I	class of the wanded objects.
* char		*name		I	wildcard name of wanted objects.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Calls a backcall routine for every child to the parent object
*	of a specified class and name.
*	If class is eq 0 the class is not tested.
*	If name is eq NULL the name is not tested.
*	The objdid of the found object and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	pwr_tObjid	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
*
**************************************************************************/

int trv_get_children_class_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	parentobjdid,
  pwr_tClassId	*class,
  char		*name,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts, size;
	pwr_tObjid		objdid;
	pwr_tClassId		obj_class;
	int			name_ok, class_ok;
	pwr_tOName     		obj_name;
	int			i;


	if ( cdh_ObjidIsNotNull( parentobjdid))
	{
	  /* Parent is supplied. Check if the children */
	  /* Get the first child to the object */
	  sts = ldh_GetChild( ldhses, parentobjdid, &objdid);
	  while ( ODD(sts) )
 	  {

	    /* Check class if class is specified */
	    class_ok = 1;
	    if ( class != 0 )
	    {
	      sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	      if ( EVEN(sts)) return sts;
	    
	      i = 0;
	      class_ok = 0;
	      while ( *(class + i) != 0)
	      {
	        if ( obj_class == *(class + i))
	        {
	          class_ok = 1;
	          break;
	        }
	        i++;
	      }
	    }

	    /* Check name if name is specified */
	    name_ok = 1;
	    if ( class_ok && (name != NULL) )
	    {
	      /* Get the name of the object */
	      sts = ldh_ObjidToName( ldhses, objdid, ldh_eName_Hierarchy,
		obj_name, sizeof( obj_name), &size);
	      if ( EVEN(sts)) return sts;

	      if ( trv_wildcard( name, obj_name) != 0)
	        name_ok = 0;	
	    }

	    if ( class_ok && name_ok)
	    {
	      pwr_sAttrRef aref = cdh_ObjidToAref( objdid);

	      sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }

	    sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	  }
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* char		name		I	wildcard name of the wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_class to find all objects in a system of
*	a specified class.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_name (
  ldh_tSesContext ldhses,
  char		*name,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts, size;
	pwr_tOName     		obj_name;

	/* Get the name of the object */
	sts = ldh_ObjidToName( ldhses, objdid, ldh_eName_Hierarchy,
		obj_name, sizeof( obj_name), &size);
	if ( EVEN(sts)) return sts;

	if ( trv_wildcard( name, obj_name) == 0)
	{
	  pwr_sAttrRef aref = cdh_ObjidToAref( objdid);

	  sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &objdid);
	while ( ODD(sts) )
	{
	  sts = trv_get_child_object_name( ldhses, name, objdid, backcall, 
		arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_hier_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tObjid	hierobjdid is found.
* char		*name		I	wildcard name of the wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_hier_class
*	to find all objects in a system of that fits in a wildcard description
*	and that has a specific object as ancestor.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class below the specified hierarchy object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_hier_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  unsigned long	*hierflag,
  char		*name,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int	       	sts, size;
	pwr_tObjid	childobjdid;
	pwr_tOName     	obj_name;

	if ( *hierflag == TRV_CROSS_RETURN ) 
	  return GSX__SUCCESS;

	/* check if the objdid is the hierchy objdid */
	if ( cdh_ObjidIsEqual( objdid, hierobjdid ))
	  /* Equal */
	  *hierflag = TRV_CROSS_CROSSREF;


	if ( *hierflag == TRV_CROSS_CROSSREF )
	{
	  /* call the backcall routine */
	  /* Get the name of the object */
	  sts = ldh_ObjidToName( ldhses, objdid, ldh_eName_Hierarchy,
		obj_name, sizeof( obj_name), &size);
	  if ( EVEN(sts)) return sts;

	  if ( trv_wildcard( name, obj_name) == 0)
	  {
	    pwr_sAttrRef aref = cdh_ObjidToAref( objdid);
	    
	    sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	  }
 	}

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &childobjdid);
	while ( ODD(sts) )
 	{
	  sts = trv_get_child_object_hier_name( ldhses, hierobjdid, hierflag,
		name, childobjdid, backcall, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, childobjdid, &childobjdid);
	}

	/* if this is the hierobjdid, exit at fast as possible */
	if ( cdh_ObjidIsEqual( objdid, hierobjdid ))
	  /* Equal */
	  *hierflag = TRV_CROSS_RETURN;

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_hier_class()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tObjid	hierobjdid is found.
* pwr_tClassId	class		I	class of the wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_hier_class
*	to find all objects in a system of
*	a specified class that has a specific object as ancestor.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class below the specified hierarchy object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_hier_class (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  unsigned long	*hierflag,
  pwr_tClassId	class,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int		sts;
	pwr_tClassId	obj_class;
	pwr_tObjid	childobjdid;

	if ( *hierflag == TRV_CROSS_RETURN ) 
	  return GSX__SUCCESS;

	/* check if the objdid is the hierchy objdid */
	if ( cdh_ObjidIsEqual( objdid, hierobjdid ))
	  /* Equal */
	  *hierflag = TRV_CROSS_CROSSREF;


	if ( *hierflag == TRV_CROSS_CROSSREF )
	{
	  /* call the backcall routine */
	  sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	  if ( EVEN(sts)) return sts;

	  if ( obj_class == class )
	  {
	    pwr_sAttrRef aref = cdh_ObjidToAref( objdid);
	  
	    sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	  }
 	}

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &childobjdid);
	while ( ODD(sts) )
 	{
	  sts = trv_get_child_object_hier_class( ldhses, hierobjdid, hierflag,
		class, childobjdid, backcall, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, childobjdid, &childobjdid);
	}

	/* if this is the hierobjdid, exit at fast as possible */
	if ( cdh_ObjidIsEqual( objdid, hierobjdid ))
	  /* Equal */
	  *hierflag = TRV_CROSS_RETURN;

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_class_hier()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tClassId	class		I	class of the wanded objects.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Traverses the objects in the plant and classhierarchy.
*	Calls a backcallroutine with the given arguments for every found 
*	object of the specified class found below the hierobjdid in
*	the hierarchy. The objdid of the found object and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	pwr_tObjid	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
*
**************************************************************************/

int trv_get_class_hier (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  char		*name,
  pwr_tClassId	*class,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tObjid	objdid;
	pwr_tClassId	obj_class;

	if ( cdh_ObjidIsNotNull( hierobjdid))
	{
	  /* hierobjdid is supplied */
	  sts = trv_get_child_object_hi_cl_na( ldhses, hierobjdid,
		class, name, hierobjdid, backcall, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}
	else
	{
	  sts = ldh_GetRootList( ldhses, &objdid);
	  while ( ODD(sts) )
	  {
	    sts = ldh_GetObjectClass( ldhses, objdid, &obj_class);
	    if ( EVEN(sts)) return sts;

	    /* Check that the class of the node object is correct */
	    if ( obj_class == pwr_eClass_ClassHier)
	    {
	      /* Check if the children */
	      sts = trv_get_child_object_hi_cl_na( ldhses, hierobjdid,
		class, name, objdid, backcall, arg1, arg2, arg3, arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	  }
	}
  
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_objectdoc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects to find all documentobjects in a system.
*	Calls a backcallroutine with the given arguments for every object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_docobject (
  ldh_tSesContext ldhses,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tObjid		newobjdid;

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &newobjdid);
	while ( ODD(sts) )
	{

	  /* Check if this is a document object */
	  if ( vldh_check_document( ldhses, newobjdid))
	  {
	    pwr_sAttrRef aref = cdh_ObjidToAref( newobjdid);

	    sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	  }
	  sts = ldh_GetNextSibling( ldhses, newobjdid, &newobjdid);
	}

	/* Get the first child to the object */
	sts = ldh_GetChild( ldhses, objdid, &newobjdid);
	while ( ODD(sts) )
	{
	  sts = trv_get_child_docobject( ldhses, newobjdid, backcall, 
		arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	  sts = ldh_GetNextSibling( ldhses, newobjdid, &newobjdid);
	}
 
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_docobjects()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Traverses the objects in the planthierarchy and nodehierarchy.
*	Calls a backcallroutine with the given arguments for every found 
*	object. The objdid of the found object and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	pwr_tObjid	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
**************************************************************************/

int trv_get_docobjects ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts;
	pwr_tObjid		objdid;
	pwr_tClassId	class;

	if ( cdh_ObjidIsNull( hierobjdid))
	{
	  sts = ldh_GetRootList( ldhses, &objdid);
	  while ( ODD(sts) )
	  {
	    sts = ldh_GetObjectClass( ldhses, objdid, &class);
	    if ( EVEN(sts)) return sts;

	    /* Check that the class of the node object is correct */
	    if ( !( class == pwr_eClass_ClassHier ||
	          class == pwr_eClass_TypeHier))
	    {
	      /* Check if the children is a plc */
	      sts = trv_get_child_docobject( ldhses,
		objdid, backcall, arg1, arg2, arg3, arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = ldh_GetNextSibling( ldhses, objdid, &objdid);
	  }
	}
	else
	{
	  sts = trv_get_child_docobject( ldhses,
		hierobjdid, backcall, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}

	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		trv_create_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
**************************************************************************/

int	trv_create_ctx( 
  trv_ctx		*trvctx,
  ldh_tSesContext	ldhses,
  pwr_tObjid		hierobjid,
  pwr_tClassId		*class,
  char			*name,
  pwr_tVolumeId		*volume
)
{
	*trvctx = (trv_ctx) XtCalloc( 1, sizeof( **trvctx));
	if ( *trvctx == 0) return FOE__NOMEMORY;

	(*trvctx)->ldhses = ldhses;
	(*trvctx)->hierobjid = hierobjid;
	(*trvctx)->classid = class;
	(*trvctx)->name = name;
	(*trvctx)->volume = volume;

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_delete_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
**************************************************************************/

int	trv_delete_ctx( 
  trv_ctx		trvctx
)
{
	XtFree( (char *) trvctx);
	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_get_child_object_hier_class_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ldh_tSesContext ldhses		I	ldh session.
* pwr_tObjid	hierobjdid	I	ancestor of wanted objects.
* pwr_tObjid	hierobjdid is found.
* pwr_tClassId	class		I	class of the wanted objects.
* char		*name		I	wildcard name of wanted objects.
* pwr_tObjid	objdid		I	objdid of the object
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Routine used by trv_get_objects_hier_class
*	to find all objects in a system of
*	a specified class that has a specific object as ancestor.
*	Calls a backcallroutine with the given arguments for every object 
*	found of the specified class below the specified hierarchy object.
*	This is  a recursiv functions that calls itself for all children
*	found to the given objdid.
*
**************************************************************************/

static int trv_get_child_object_search (
  trv_ctx	trvctx,
  pwr_tObjid	objdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int			sts, size;
	pwr_tClassId		obj_class;
	pwr_tObjid		childobjdid;
	int			name_ok, class_ok;
	pwr_tOName     		obj_name;
	int			i;
	pwr_sAttrRef		aref;

	/* Check class if class is specified */
	class_ok = 1;
	sts = ldh_GetObjectClass( trvctx->ldhses, objdid, &obj_class);
	if ( EVEN(sts)) return sts;

	if ( trvctx->classid != NULL )
	{
	  i = 0;
	  class_ok = 0;
	  while ( *(trvctx->classid + i) != 0)
	  {
	    if ( obj_class == *(trvctx->classid + i))
	    {
	      class_ok = 1;
	      break;
	    }
	    i++;
	  }
	}

	/* Check name if name is specified */
	name_ok = 1;
	if ( class_ok && (trvctx->name != NULL) )
	{
	  /* Get the name of the object */
	  sts = ldh_ObjidToName( trvctx->ldhses, objdid, ldh_eName_Hierarchy,
		obj_name, sizeof( obj_name), &size);
	  if ( EVEN(sts)) return sts;

	  if ( trv_wildcard( trvctx->name, obj_name) != 0)
	    name_ok = 0;	
	}

	if ( class_ok && name_ok)
	{
	  aref = cdh_ObjidToAref( objdid);
	  sts = (backcall) (&aref, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;
	}

	/* Get the first child to the object */
	if ( !(obj_class == pwr_eClass_MountObject ||
	       obj_class == pwr_eClass_LibHier))
	{
	  sts = ldh_GetChild( trvctx->ldhses, objdid, &childobjdid);
	  while ( ODD(sts))
 	  {
	    sts = trv_get_child_object_search( trvctx, childobjdid, 
		backcall, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	    sts = ldh_GetNextSibling( trvctx->ldhses, childobjdid, &childobjdid);
	  }
	}
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		trv_object_search()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
* trv_ctx	trvctx		I	ldh session.
* int		(*backcall)()	I 	backcallroutine called for every object.
* void		*arg1		I	argument passed to the backcall routine.
* void		*arg2		I	argument passed to the backcall routine.
* void		*arg3		I	argument passed to the backcall routine.
* void		*arg4		I	argument passed to the backcall routine.
* void		*arg5		I	argument passed to the backcall routine.
*
* Description:
*	Traverses the objects in the plant and nodehierarchy.
*	Calls a backcallroutine with the given arguments for every found 
*	object of the specified class that is found below the hierobjdid in
*	the hierarchy and that  fits in a wildcard description. 
*	If hierobjdid is eq 0 the hierarchy is not tested.
*	If class is eq 0 the class is not tested.
*	If name is eq NULL the name is not tested.
*	The objdid of the found object and arguments will be 
*	passed to the backcallroutine. The backcallroutine should be
*	declared as:
*
*	int	'backcallroutine name'( objdid, arg1, arg2, arg3, arg4, arg5)
*	unsigned long	objdid;
*	void		*arg1;
*	void		*arg2;
*	void		*arg3;
*	void		*arg4;
*	void		*arg5;
*	...
*	Calls a backcall routine for every object in the plathierarchy
*	of a specified class under a specific object in the hierarchy.
*
**************************************************************************/

int trv_object_search(
  trv_ctx	trvctx,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
)
{
	int		sts;
	pwr_tObjid	objdid;
	pwr_tClassId	obj_class;
	pwr_tVolumeId	*volume_ptr;

	if ( cdh_ObjidIsNotNull( trvctx->hierobjid))
	{
	  sts = ldh_GetObjectClass( trvctx->ldhses, trvctx->hierobjid, 
			&obj_class);
	  if ( EVEN(sts)) return sts;
	  if ( !( obj_class == pwr_eClass_ClassHier ||
	            obj_class == pwr_eClass_TypeHier ||
		    obj_class == pwr_eClass_MountObject ||
	            obj_class == pwr_eClass_LibHier))
   	  {
	    /* Hierarchy object is supplied. Check if the children */
	    sts = trv_get_child_object_search( trvctx, trvctx->hierobjid,
		backcall, arg1, arg2, arg3, arg4, arg5);
	    if ( EVEN(sts)) return sts;
	  }
	}
	else if ( trvctx->volume)
	{
	  volume_ptr = trvctx->volume;
	  while( *volume_ptr)
	  {
	    sts = ldh_GetVolumeRootList( trvctx->ldhses, *volume_ptr, &objdid);
	    while ( ODD(sts) )
	    {
	      sts = ldh_GetObjectClass( trvctx->ldhses, objdid, &obj_class);
	      if ( EVEN(sts)) return sts;

	      /* Check that the class of the object is correct */
	      if ( !( obj_class == pwr_eClass_ClassHier ||
	            obj_class == pwr_eClass_TypeHier ||
		    obj_class == pwr_eClass_MountObject ||
	            obj_class == pwr_eClass_LibHier))
	      {
	        /* Check if the children */
	        sts = trv_get_child_object_search( trvctx, objdid,
			backcall, arg1, arg2, arg3, arg4, arg5);
	        if ( EVEN(sts)) return sts;
	      }
	      sts = ldh_GetNextSibling( trvctx->ldhses, objdid, &objdid);
	    }
	    volume_ptr++;
	  }
	}
	else
	{
	  sts = ldh_GetRootList( trvctx->ldhses, &objdid);
	  while ( ODD(sts) )
	  {
	    sts = ldh_GetObjectClass( trvctx->ldhses, objdid, &obj_class);
	    if ( EVEN(sts)) return sts;

	    /* Check that the class of the node object is correct */
	    if ( !( obj_class == pwr_eClass_ClassHier ||
	            obj_class == pwr_eClass_TypeHier))
	    {
	      /* Check if the children */
	      sts = trv_get_child_object_search( trvctx, objdid,
		backcall, arg1, arg2, arg3, arg4, arg5);
	      if ( EVEN(sts)) return sts;
	    }
	    sts = ldh_GetNextSibling( trvctx->ldhses, objdid, &objdid);
	  }
	}
	return GSX__SUCCESS;
}









