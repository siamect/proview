/* 
 * Proview   $Id: rt_ini_alias.c,v 1.6 2007-12-21 13:31:08 claes Exp $
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

#ifdef OS_ELN
# include ctype
# include $vaxelnc
# include stdio
# include stdlib
# include string
#else
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_cdh.h"
#include "co_cdh_msg.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_ini_msg.h"
#include "rt_ini_alias.h"

static	FILE		*ini_datafile;

static int ini_datafile_init (char *filename);

static int ini_datafile_close ();

static int ini_datafile_get_next ( 
  char *parameter, 
  char **data, 
  int *elements
);

static int ini_parse ( 
  char	*instring,
  char	*parse_char,
  char	*inc_parse_char,
  char	*outstr,
  int	max_rows,
  int 	max_cols
);
 
static int ini_read_line ( 
  char	*line,
  int	maxsize,
  FILE	*file
);


static int ini_set_attribute (
  char	*name_str,
  char	*value_str
);

static int ini_set_nodeattribute ( 
  char	*attribute_str,
  char	*value_str
);

static int ini_set_plcscan (char *value_str);


pwr_tStatus
ini_GetAlias (
  char *filename,
  char *nodename,
  char *aliasname
)
{
  pwr_tStatus	sts;
  char		*aliasname_ptr;
  int		elements;

  sts = ini_datafile_init(filename);
  if (EVEN(sts)) return sts;

  sts = ini_datafile_get_next(nodename, &aliasname_ptr, &elements);
  if (EVEN(sts)) return sts;

  strcpy(aliasname, (char *)aliasname_ptr);
  free(aliasname_ptr);

  ini_datafile_close();

  return INI__SUCCESS;
}

pwr_tStatus
ini_SetAttributeAfterPlc (
  char *filename,
  char *nodename,
  int  output
)
{
  pwr_tStatus		sts;
  int			elements;
  char			codeword[80];
  pwr_tString132	*data_ptr;
  pwr_tString132	*attribute_ptr;
  pwr_tString132	*value_ptr;

  strcpy(codeword, nodename);
  strcat(codeword, "_SETVALP");
  
  sts = ini_datafile_init(filename);
  if (EVEN(sts)) return sts;

  while (1) {
    sts = ini_datafile_get_next(codeword, (char **)&data_ptr, &elements);
    if (EVEN(sts)) break;

    if (elements != 2) {
      if ( elements > 2)
	if ( output)
	  printf("Syntax error in alias file %s\n", (char *)data_ptr);
        else
	  errh_Error("syntax error in alias file %s", (char *)data_ptr);
      else
	if ( output)
	  printf("Syntax error in alias file\n");
	else
	  errh_Error("syntax error in alias file");
      continue;

    }

    attribute_ptr = data_ptr;
    value_ptr = data_ptr;
    value_ptr++;

    sts = ini_set_attribute((char *)attribute_ptr, (char *)value_ptr);
    if (EVEN(sts)) 
      if ( output)
        printf("Unable to modify attribute '%s'\n", (char *)attribute_ptr);
      else
        errh_Error("unable to modify attribute '%s'\n%m", (char *)attribute_ptr, sts);
    else
      if ( output)
        printf("Attribute '%s' set to '%s'", (char *)attribute_ptr, (char *)value_ptr);
      else
        errh_Info("attribute '%s' set to '%s'", (char *)attribute_ptr, (char *)value_ptr);

    free(data_ptr);
  }

  ini_datafile_close();

  return INI__SUCCESS;
}

pwr_tStatus
ini_SetAttribute (
  char *filename,
  char *nodename, 
  int  output
)
{
	pwr_tStatus	sts;
	int		elements;
	char		codeword[80];
	pwr_tString132	*data_ptr;
	pwr_tString132	*attribute_ptr;
	pwr_tString132	*value_ptr;

	strcpy( codeword, nodename);
	strcat( codeword, "_SETVAL");
	

	sts = ini_datafile_init( filename);
	if ( EVEN(sts)) return sts;

	while ( 1)
	{
	  sts = ini_datafile_get_next( codeword, (char **)&data_ptr, &elements);
	  if ( EVEN(sts)) break;

	  if ( elements == 3 && strcmp( data_ptr[2], "") == 0)
	    elements = 2;
	  if ( elements != 2)
	  {
	    if ( elements > 2)
	      if ( output)
	        printf("Syntax error in alias file '%s'\n", (char *)data_ptr);
	      else
	        errh_Error("syntax error in alias file '%s'", (char *)data_ptr);
	    else
	      if ( output)
	        printf("Syntax error in alias file\n");
	      else
	        errh_Error("syntax error in alias file");
	    continue;

	  }

	  attribute_ptr = data_ptr;
	  value_ptr = data_ptr;
	  value_ptr++;

	  if ( cdh_NoCaseStrcmp( (char *)attribute_ptr, "PLCSCAN") == 0)
	  {
	    sts = ini_set_plcscan((char *)value_ptr);
	    if ( EVEN(sts)) 
	      if ( output)
	        printf("Unable to modify attribute %s\n", (char *)attribute_ptr);
	      else
	        errh_Error("unable to modify attribute %s\n%m", (char *)attribute_ptr, sts);
	    else
	      if ( output)
	        printf("Attribute '%s' set to '%s'\n", (char *)attribute_ptr, (char *)value_ptr);
	      else
	        errh_Info("attribute '%s' set to '%s'", (char *)attribute_ptr, (char *)value_ptr);
	  }
	  else if ( 	cdh_NoCaseStrcmp( (char *)attribute_ptr, "PLCSIM") == 0 ||
	  		cdh_NoCaseStrcmp( (char *)attribute_ptr, "ERRLOGFILE") == 0 ||
	  		cdh_NoCaseStrcmp( (char *)attribute_ptr, "ERRLOGTERM") == 0)

	  {
	    sts = ini_set_nodeattribute( (char *)attribute_ptr, (char *)value_ptr);
	    if ( EVEN(sts)) 
	      if ( output)
	        printf("Unable to modify attribute %s\n", (char *)attribute_ptr);
	      else
	        errh_Error("unable to modify attribute %s\n%m", (char *)attribute_ptr, sts);
	    else
	      if ( output)
	        printf("Attribute '%s' set to '%s'\n", (char *)attribute_ptr, (char *)value_ptr);
	      else
	        errh_Info("attribute '%s' set to '%s'", (char *)attribute_ptr, (char *)value_ptr);
	  }
	  else
	  {
	    sts = ini_set_attribute( (char *)attribute_ptr, (char *)value_ptr);
	    if ( EVEN(sts)) 
	      if ( output)
	        printf("Unable to modify attribute %s\n", (char *)attribute_ptr);
	      else
	        errh_Error("unable to modify attribute %s\n%m", (char *)attribute_ptr, sts);
	    else
	      if ( output)
	        printf("Attribute '%s' set to '%s'\n", (char *)attribute_ptr, (char *)value_ptr);
	      else
	        errh_Info("attribute '%s' set to '%s'", (char *)attribute_ptr, (char *)value_ptr);
	  }
	  free( data_ptr);
	}

	ini_datafile_close();

	return INI__SUCCESS;
}

/****************************************************************************
* Name:		ini_datafile_init()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/
static int ini_datafile_init ( char *filename)
{

  /* Open file */
  ini_datafile = fopen( filename, "r");
  if ( ini_datafile == 0)
  {
    return 0;
  }
  return INI__SUCCESS;
}

/****************************************************************************
* Name:		ini_datafile_close()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/
static int ini_datafile_close()
{
	/* Close file */
	fclose( ini_datafile);
	ini_datafile = 0;

	return INI__SUCCESS;
}


/****************************************************************************
* Name:		ini_datafile_get_next()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/
static int	ini_datafile_get_next( char *parameter, char **data, 
		int *elements) 
{
	char		line[256];
	char		data_array[10][132];
	int		found, nr, sts ;
        char		param[80];

	if ( ini_datafile == 0)
	  return 0;

        cdh_ToUpper( param, parameter);
	found = 0;
	while ( 1) {

	  /* Read one line */
	  sts = ini_read_line( line, sizeof( line), ini_datafile);
	  if ( EVEN(sts)) 
	    break;
	  if ( line[0] == '!')
	    continue;
	  
	  /* Parse the line */

	  nr = ini_parse( line, "=, 	", "", (char *)data_array, 
		sizeof( data_array) / sizeof( data_array[0]), 
		sizeof( data_array[0]));
	  if (nr == 0) 
	    continue;

	  if ( cdh_NoCaseStrcmp( data_array[0], param) == 0) {
	     found = 1;
	     break;
	  }
	}
	if ( !found)
	  return 0;   /* INI__PARNOTFOUND; */

	*data = calloc( 1, sizeof( data_array) - sizeof(data_array[0]));
	memcpy( *data, data_array[1], 
		sizeof( data_array) - sizeof(data_array[0]));
	*elements = nr - 1;

	return INI__SUCCESS;
}


/*************************************************************************
*
* Name:		ini_parse()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*string		I	string to be parsed.
* char		*parse_char	I	parse charachter(s).
* char		*inc_parse_char	I	parse charachter(s) that will be
*					included in the parsed string.
* char		*outstr		O	parsed strings.
* int		max_rows	I	maximum number of chars in a parsed
*					string.
* int 		max_cols	I	maximum number of parsed elements.
*
* Description:
*	Parses a string.
*
**************************************************************************/

static int ini_parse ( 
  char	*instring,
  char	*parse_char,
  char	*inc_parse_char,
  char	*outstr,
  int	max_rows,
  int 	max_cols
)
{
	char	*string;
	int	row;
	int	col;
	char	*char_ptr;	
	char	*inc_char_ptr;	
	int	parsechar_found;
	int	inc_parsechar_found;
	int	next_token;
	int	char_found;
	int	one_token = 0;

	string = instring;
	row = 0;
	col = 0;
	char_found = 0;
	next_token = 0;
	while ( *string != '\0')	
	{
	  char_ptr = parse_char;
	  inc_char_ptr = inc_parse_char;
	  parsechar_found = 0;
	  inc_parsechar_found = 0;
	  if ( *string == '"')
	  {
	    one_token = !one_token;
	    string++; 
	    continue;
	  }
	    if ( !one_token)
	    {
	    while ( *char_ptr != '\0')
	    {
	      /* Check if this is a parse charachter */
	      if ( *string == *char_ptr)
	      {
	        parsechar_found = 1;
	        /* Next token */
	        if ( col > 0)	
	        {
	          *(outstr + row * max_cols + col) = '\0';
	          row++;
	          if ( row >= max_rows )
                  {
                    cdh_ToUpper ( outstr , outstr ) ; 
	            return row;
                  }
	          col = 0;
	          next_token = 0;
	        }
	        break;
	      }
	      char_ptr++;	    
	    }
	    while ( *inc_char_ptr != '\0')
	    {
	      /* Check if this is a parse charachter */
	      if ( *string == *inc_char_ptr)
	      {
	        parsechar_found = 1;
	        inc_parsechar_found = 1;
	        /* Next token */
	        if ( col > 0)	
	        {
	          *(outstr + row * max_cols + col) = '\0';
	          row++;
	          if ( row >= max_rows )
                  {
                    cdh_ToUpper ( outstr , outstr ) ; 
	            return row;
                  }
	          col = 0;
	          next_token = 0;
	        }
	        break;
	      }
	      inc_char_ptr++;	    
	    }
	  }
	  if ( !parsechar_found && !next_token)
	  {
	    char_found++;
	    *(outstr + row * max_cols + col) = *string;
	    col++;
	  }
	  if ( inc_parsechar_found)
	  {
	    *(outstr + row * max_cols + col) = *inc_char_ptr;
	    col++;
	  }
	  string++; 
	  if ( col >= (max_cols - 1))
	    next_token = 1;	    
	}
	*(outstr + row * max_cols + col) = '\0';
	row++;

	if ( char_found == 0)
	  return 0;

        cdh_ToUpper ( outstr , outstr ) ; 
	return row;
}


/*************************************************************************
*
* Name:		ini_read_line()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read a line for a file.
*
**************************************************************************/

static int ini_read_line ( 
  char	*line,
  int	maxsize,
  FILE	*file
)
{ 
	char	*s;

	if (fgets( line, maxsize, file) == NULL)
	  return 0;
	s = strchr( line, 10);
	if ( s != 0)
	  *s = 0;

	return INI__SUCCESS;
}


/*************************************************************************
*
* Name:		ini_set_attribute()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set value of a parameter in rtdb.
*
**************************************************************************/

static int ini_set_attribute (
  char	*name_str,
  char	*value_str
)
{
  /* Get type of the parameter from info in the class object */

  int		sts;
  char		buffer[256];
  pwr_sAttrRef	attrref;

  unsigned int asize, aoffs, aelem;
  pwr_tTid atid;
	
  sts = gdh_NameToAttrref( pwr_cNOid, name_str, &attrref);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharAttrref( &attrref, &atid, &asize, &aoffs, &aelem);
  if ( EVEN(sts)) return sts;

  switch ( atid ) {
  case pwr_eType_Objid: {
    pwr_tOid oid;
    
    sts = gdh_NameToObjid ( (char *)value_str, &oid);
    if (EVEN(sts)) 
      return sts; 
    
    memcpy( buffer, &oid, sizeof( oid));
    break;
  }
  case pwr_eType_AttrRef: {
    pwr_tAttrRef aref;
    
    sts = gdh_NameToAttrref( pwr_cNOid, value_str, &aref);
    if (EVEN(sts)) 
      return sts;
    
    memcpy( buffer, &aref, sizeof(aref));
    break;
  }
    
  default:
    sts = cdh_StringToAttrValue( atid, value_str, (void *)buffer); 
    break;
  }
  if (EVEN(sts)) return sts;
  
  sts = gdh_SetObjectInfo ( name_str, (void *)buffer, asize/aelem);
  if ( EVEN(sts)) return sts;
  
  return INI__SUCCESS;
}



/*************************************************************************
*
* Name:		ini_set_plcsim()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set value of a parameter in rtdb.
*
**************************************************************************/

static int ini_set_nodeattribute ( 
  char	*attribute_str,
  char	*value_str
)
{
	pwr_sNode	*nodeobjp;
	pwr_tObjid	nodeobjid;
	pwr_tObjid	objid;
	int		sts;
	pwr_tString256	nodename;
	pwr_tString256	attributename;
	pwr_tBoolean	value;

	/* Get pointer to node object */
	sts = gdh_GetNodeObject (0, &nodeobjid);
	if (EVEN (sts)) 
	{
	  errh_CErrLog (sts);
	  return sts;
	}
	sts = gdh_ObjidToName ( nodeobjid, nodename, sizeof(nodename), cdh_mNName);
	if ( EVEN(sts)) return sts;


	sts = gdh_ObjidToPointer (nodeobjid, (pwr_tAddress *)&nodeobjp);    
	if ( EVEN(sts)) return sts;

	if ( cdh_NoCaseStrcmp( attribute_str, "PLCSIM") == 0)
	{
	  /* Moved to IOHandler object */
	  sts = gdh_GetClassList ( pwr_cClass_IOHandler, &objid);
	  if ( EVEN(sts)) return sts;
	  sts = gdh_ObjidToName ( objid, nodename, sizeof(nodename), cdh_mNName);
	  if ( EVEN(sts)) return sts;

	  if ( cdh_NoCaseStrcmp( value_str, "YES") == 0)
	  {
	    strcpy( attributename, nodename);
	    strcat( attributename, ".IOReadWriteFlag");
	    value = 0;
	    sts = gdh_SetObjectInfo ( attributename, &value, sizeof( value));
	    if ( EVEN(sts)) return sts;
	    strcpy( attributename, nodename);
	    strcat( attributename, ".IOSimulFlag");
	    value = 1;
	    sts = gdh_SetObjectInfo ( attributename, &value, sizeof( value));
	    if ( EVEN(sts)) return sts;
	  }
	  else if ( cdh_NoCaseStrcmp( value_str, "NO") == 0)
	  {
	    strcpy( attributename, nodename);
	    strcat( attributename, ".IOReadWriteFlag");
	    value = 1;
	    sts = gdh_SetObjectInfo ( attributename, &value, sizeof( value));
	    if ( EVEN(sts)) return sts;
	    strcpy( attributename, nodename);
	    strcat( attributename, ".IOSimulFlag");
	    value = 0;
	    sts = gdh_SetObjectInfo ( attributename, &value, sizeof( value));
	    if ( EVEN(sts)) return sts;
	  }
	  else
	    return 0;
	}	
	else if ( cdh_NoCaseStrcmp( attribute_str, "ERRLOGFILE") == 0)
	{
	  strcpy( attributename, nodename);
	  strcat( attributename, ".ErrLogFile");
	  sts = gdh_SetObjectInfo ( attributename, value_str, 
			sizeof( nodeobjp->ErrLogFile));
	  if ( EVEN(sts)) return sts;
	}
	else if ( cdh_NoCaseStrcmp( attribute_str, "ERRLOGTERM") == 0)
	{
	  strcpy( attributename, nodename);
	  strcat( attributename, ".ErrLogTerm");
	  sts = gdh_SetObjectInfo ( attributename, value_str, 
			sizeof( nodeobjp->ErrLogTerm));
	  if ( EVEN(sts)) return sts;
	}
	else
	  return 0;

	return INI__SUCCESS;
}


/*************************************************************************
*
* Name:		ini_set_plcscan()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set value of a parameter in rtdb.
*
**************************************************************************/

static int ini_set_plcscan (char *value_str)
{
	pwr_tString40	classname[]	= { 
				"WindowPlc", 
				"WindowCond", 
				"WindowOrderact",
				"WindowSubstep", 
				""};
	pwr_tString40	*classname_p;
	pwr_tString256	objectname;
	pwr_tClassId	class;
	pwr_tObjid	objid;
	int		sts;
	pwr_tBoolean	value;

	if ( cdh_NoCaseStrcmp( value_str, "OFF") != 0)
	  return 0;

	/* Get all window classes */
	classname_p = (pwr_tString40 *)&classname;
	while ( strcmp( (char *)classname_p, "") != 0)
	{
	  
	  sts = gdh_ClassNameToNumber ( (char *)classname_p, &class);
	  if ( EVEN(sts)) return sts;

	  /* Get every object of each class */
	  sts = gdh_GetClassList ( class, &objid);
	  while ( ODD(sts))
	  {
	    
	    /* Set attribute ScanOff to true */
	    sts = gdh_ObjidToName ( objid, objectname, sizeof(objectname), cdh_mNName);
	    if ( EVEN(sts)) return sts;

	    strcat( objectname, ".ScanOff");

	    value = 1;
	    sts = gdh_SetObjectInfo ( objectname, &value, sizeof( value));
	    if ( EVEN(sts)) return sts;

	    sts = gdh_GetNextObject ( objid, &objid);
	  }
	  classname_p++;
	}
	
	return INI__SUCCESS;
}

