/* 
 * Proview   $Id: xtt_localdb.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef xtt_xnav_localdb_h
#define xtt_xnav_localdb_h

/* xtt_localdb.h -- Local parameter database */


typedef enum {
	xnav_eLdbType_Boolean,
	xnav_eLdbType_Int,
	xnav_eLdbType_Float,
	xnav_eLdbType_String
	} xnav_eLdbType;

typedef struct sLdbEntry {
	char		name[80];
	xnav_eLdbType	type;
	void		*value_p;
	sLdbEntry	*next;
} xnav_sLdbEntry;


class XNavLdb {
  public:
    XNavLdb() : root(NULL) {};
    ~XNavLdb()	{ remove_all();};		
    xnav_sLdbEntry	*root;	
    int			entry_cnt;

    int			add( char *name, xnav_eLdbType type, void *value_p);
    int			remove( char *name);
    void		remove_all();
    int			get( char *name, xnav_eLdbType *type, void **value_p);
};

#endif
