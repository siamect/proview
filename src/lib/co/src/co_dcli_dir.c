/* xtt_dcli_dir.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines for handling files and directories in dcli. */


/*_Include files_________________________________________________________*/



#if defined(OS_ELN)
# include stdio
# include stdarg
# include stdlib
# include signal
# include string
# include descrip
# include starlet
# include lib$routines
# include $vaxelnc
# include $kernelmsg
# include $elnmsg
# include $get_message_text
# include $file_utility
#elif defined(OS_VMS)
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <starlet.h>
# include <rms.h>
# include <descrip.h>
#elif defined(OS_LYNX) || defined(OS_LINUX)
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <string.h>
# include <dirent.h>
#endif

#include "pwr.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_dcli_msg.h"

/*_Globala variabler______________________________________________________*/

/*_Local proceduer________________________________________________________*/

/* Globala procedurer_____________________________________________________*/

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)


/*************************************************************************
*
* Name:		dcli_search_file
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*		Söker efter en fil. 
*		Filnamnet kan innehålla wildcard.
*		Vid sökning av flera filer mha wildcard anropas
*		rutinen första gången med new=1 och sedan med new=0.
*		Två sökningar med wildcard kan ej pågå parallellt efter
*		som sökvillkor lagras i interna variabler.
*
* Parametrar
*	file_name	*char	I  Namn på fil, kan innehålla wildcard.
*	found_file	*char	O  Hittad fil.
*	new		int	I  1 vid ej wildcard sökning och vid första
*				   wildcardsökningsanropet. 0 vid sökning
*				   efter fler filer med samma wildcard.
*
**************************************************************************/

int dcli_search_file( 		char 	*name , 
				char	*found_file,
				int	new)
{
#if defined (OS_VMS)
	static struct NAM 	nam1;
	static struct FAB 	fab1;
	static struct	XABDAT	dat1; 
	static char		result_name[NAM$C_MAXRSS];
	static char		expand_name[NAM$C_MAXRSS];
	int 			sts;


	if ( new == DCLI_DIR_SEARCH_END)
	  return 1;

	if ( new == DCLI_DIR_SEARCH_INIT)
	{
	  /* Initialize RMS user structures for the checking file. */
	  fab1  	   = cc$rms_fab;
	  fab1.fab$l_xab = (char *) &dat1;
	  fab1.fab$l_nam = &nam1;
	  fab1.fab$w_ifi = 0;
  
	  dat1  	   = cc$rms_xabdat;
	  dat1.xab$l_nxt = 0;

	  fab1.fab$l_fna = name;
	  fab1.fab$b_fns = strlen (name);

	  nam1  	   = cc$rms_nam;
	  nam1.nam$l_rsa = result_name;
	  nam1.nam$b_rss = sizeof(result_name);
	  nam1.nam$l_esa = expand_name;
	  nam1.nam$b_ess = sizeof(expand_name);

	  sts = sys$parse(&fab1);
	  if (EVEN(sts)) return sts;
	}

	if ( new == DCLI_DIR_SEARCH_INIT || new == DCLI_DIR_SEARCH_NEXT)
	{
	  sts= sys$search (&fab1);
	  if (EVEN(sts)) return sts;
	  result_name[nam1.nam$b_rsl] = 0;
	  strcpy( found_file, result_name);
	}
	return 1;
#elif defined (OS_ELN)

	static ELN$DIR_FILE *dir_file;
	VARYING_STRING(255) search_name, volume_name, directory_name,
			file_name, server_name;
	char	directory_str[255], file_str[255],
		volume_str[255], server_str[255];
	int	sts;
	static FILE$ATTRIBUTES_RECORD *fattr;

	if ( new == DCLI_DIR_SEARCH_INIT)
	{
	  dir_file = calloc( 1, sizeof(*dir_file));
	  CSTRING_TO_VARYING( name, search_name);
	  eln$directory_open( &dir_file, &search_name, &volume_name, 
		&directory_name, &sts, &server_name, &fattr);
	  if ( EVEN(sts)) return sts;
	}

	if ( new == DCLI_DIR_SEARCH_INIT || new == DCLI_DIR_SEARCH_NEXT)
	{
	  eln$directory_list( &dir_file, &directory_name, &file_name, &sts,
		&fattr);
	  if (EVEN(sts)) return sts;

	  VARYING_TO_CSTRING( directory_name, directory_str);
	  VARYING_TO_CSTRING( file_name, file_str);
	  VARYING_TO_CSTRING( volume_name, volume_str);
	  VARYING_TO_CSTRING( server_name, server_str);
	  strcpy( found_file, server_str);
	  strcat( found_file, volume_str);
	  strcat( found_file, directory_str);
	  strcat( found_file, file_str);
	}
	if ( new == DCLI_DIR_SEARCH_END)
	  eln$directory_close( &dir_file, &sts);

	return sts;
#elif defined(OS_LYNX) || defined(OS_LINUX)

	static DIR *directory;
	static char pattern[200];
	static char dir[200];
	char	dev[2], dir2[200], file[80], type[80];
	char	cwd[200];
	int	version;
	int	found;
	static int wildcard;
	struct dirent *dir_entry;

	if ( new == DCLI_DIR_SEARCH_INIT)
	{
	  dcli_parse_filename( name, dev, dir, file, type, &version);
	  dcli_replace_env( dir, dir2);
	  if ( dir2[0] != '/')
	  {
	    /* Add cwd to the path */
	    if ( getcwd( cwd, sizeof(cwd)) == NULL)
	      return DCLI__NOFILE;
	    strcpy( dir, cwd);
            if ( dir[strlen(dir)-1] != '/')
              strcat( dir, "/");
	    strcat( dir, dir2);
	  }
	  else
	    strcpy( dir, dir2);

	  directory = opendir( dir);
	  if ( directory == NULL)
	    return DCLI__NOFILE;

	  strcpy( pattern, file);
	  strcat( pattern, type);
	  if ( strchr( pattern, '*') != 0)
	    wildcard = 1;
	  else
	    wildcard = 0;
          if ( wildcard)
	    dcli_toupper( pattern, pattern);
	}

	if ( new == DCLI_DIR_SEARCH_INIT || new == DCLI_DIR_SEARCH_NEXT)
	{

	  found = 0;
	  while ( (dir_entry = readdir( directory)) != NULL)
	  {
	    if ( dir_entry->d_name[0] == '.')
	      continue;

	    if ( wildcard)
	    {
	      if ( dcli_wildcard( pattern, dir_entry->d_name) == 0)
	      {
		strcpy( found_file, dir);
	        strcat( found_file, dir_entry->d_name);
	        found = 1;
	      }
	    }
	    else
	    {
	      if ( strcmp( pattern, dir_entry->d_name) == 0)
	      {
		strcpy( found_file, dir);
	        strcat( found_file, dir_entry->d_name);
	        found = 1;
	      }
	    }
	    if ( found)
	      break;
	  }
	  if ( !found)
	    return DCLI__NOFILE;
	  else
	    return DCLI__SUCCESS;
	}

	if ( new == DCLI_DIR_SEARCH_END)
	  closedir( directory);

	return DCLI__SUCCESS;
#else
  	return 0;
#endif
	return DCLI__SUCCESS;
}  


/*************************************************************************
*
* Name:		dcli_parse_filename
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Delar upp ett filnamn i device, directory, namn, typ och version.
*
* Parameterar
*	filenam		*char	I  Filnamn som ska delas.
*	dev		*char	O  device.
*	dir		*char	O  directory.
*	file		*char	O  filnamn.
*	type		*char	O  typ.
*	version		*int	O  version.
*
**************************************************************************/

int	dcli_parse_filename( 	char	*filename,
				char	*dev,
				char	*dir,
				char	*file,
				char	*type,
				int	*version)
{
#if defined (OS_VMS)
	struct NAM 	nam1;
	struct FAB 	fab1;
	struct	XABDAT	dat1; 
	char		result_name[NAM$C_MAXRSS];
	char		expand_name[NAM$C_MAXRSS];
	int 		sts;
	int		nr;


	/* Initialize RMS user structures for the checking file. */
	fab1  	   = cc$rms_fab;
	fab1.fab$l_xab = (char *) &dat1;
	fab1.fab$l_nam = &nam1;
	fab1.fab$w_ifi = 0;
  
	dat1  	   = cc$rms_xabdat;
	dat1.xab$l_nxt = 0;

	fab1.fab$l_fna = filename;
	fab1.fab$b_fns = strlen (filename);

	nam1  	   = cc$rms_nam;
	nam1.nam$l_rsa = result_name;
	nam1.nam$b_rss = sizeof(result_name);
	nam1.nam$l_esa = expand_name;
	nam1.nam$b_ess = sizeof(expand_name);

	sts = sys$parse(&fab1);
	if (EVEN(sts)) return sts;

	strcpy( dev, nam1.nam$l_dev);
	dev[nam1.nam$b_dev] = 0;
	strcpy( dir, nam1.nam$l_dir);
	dir[nam1.nam$b_dir] = 0;
	strcpy( file, nam1.nam$l_name);
	file[nam1.nam$b_name] = 0;
	strcpy( type, nam1.nam$l_type);
	type[nam1.nam$b_type] = 0;
	nam1.nam$l_ver[nam1.nam$b_ver] = 0;
	nr = sscanf( nam1.nam$l_ver+1, "%d", version);
	if ( nr != 1)
	  *version = -1;

	return 1;
#elif defined (OS_ELN)
	char	*s, *t;
	char	ldev[200];
	char	ldir[200];
	char	lfile[200];
	char	ltype[80];
	char	lversion[80];
	int	sts;

	if ( s = strstr( filename, "::"))
	  s += 2;
	else
	  s = filename;
	strcpy( ldev, s);

	/* Device */
	if ( s = strchr( ldev, ':'))
	  s++;
	else
	  s = ldev;
	strcpy( ldir, s);
	*s = 0;

	/* Directory */
	if ( s = strchr( ldir, '>'))
	  s++;
	else
	{
	  if ( s = strchr( ldir, ']'))
	    s++;
	  else
	    s = ldir;
	}
	strcpy( lfile, s);
	*s = 0;

	/* File */
	if ( (s = strchr( lfile, '.')) == 0)
	  s = lfile;
	strcpy( ltype, s);
	*s = 0;

	/* Directory */
	if ( s = strchr( ltype, ';'))
	  t = s++;
	else
	{
	  if ( s = strchr( ltype + 1, '.'))
	    t = s++;
	  else
	  {
	    s = ltype;
	    t = s + strlen(s);
	  }
	}
	strcpy( lversion, t);
	*t = 0;

	if ( strcmp( lversion, "") == 0)
	{
	  *version = 0;
	}
	else
	{
	  sts = sscanf( lversion, "%d", version);
	  if ( sts != 1)
	    *version = 0;
	}

	strcpy( dev, ldev);
	strcpy( dir, ldir);
	strcpy( file, lfile);
	strcpy( type, ltype);
	return 1;

#elif defined(OS_LYNX) || defined(OS_LINUX)

	char	*s;
	char	ldev[200];
	char	ldir[200];
	char	lfile[200];
	char	ltype[80];

	if ( (s = strstr( filename, "::")))
	  s += 2;
	else
	  s = filename;
	strcpy( ldev, "");
	strcpy( ldir, s);

	/* Directory */
	if ( (s = strrchr( ldir, '/')))
	  s++;
	else
	  s = ldir;
	strcpy( lfile, s);
	*s = 0;

	/* File */
	if ( (s = strchr( lfile, '.')) == 0)
	  s = &lfile[strlen(lfile)];
	strcpy( ltype, s);
	*s = 0;

	/* Type */
	if ( (s = strchr( ltype+1, '.')) != 0)
	  *s = 0;
	if ( (s = strchr( ltype+1, ';')) != 0)
	  *s = 0;

	*version = 0;

	strcpy( dev, ldev);
	strcpy( dir, ldir);
	strcpy( file, lfile);
	strcpy( type, ltype);
	return 1;
#else
	return 0;
#endif
}  

