#ifndef rt_ini_alias_h
#define rt_ini_alias_h

/* rt_ini_alias.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

int	ini_GetAlias( char *filename, char *nodename, char *aliasname);
int	ini_SetAttributeAfterPlc( char *filename, char *nodename, int output);
int	ini_SetAttribute( char *filename, char *nodename, int output);

#endif
