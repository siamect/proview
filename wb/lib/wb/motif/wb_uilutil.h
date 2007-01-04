/* 
 * Proview   $Id: wb_uilutil.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_nav_macros_h
#define wb_nav_macros_h
 
#ifndef _Xm_h
#include <Xm/Xm.h>
#endif

#ifndef MrmPublic
#include <Mrm/MrmPublic.h>
#endif

#undef Status

#if defined __cplusplus
extern "C" {
#endif

#define EOS '\0'
#define loop for(;;)
#define And &&
#define Or ||
#define Not !

/* Nice functions */
#ifndef ODD
#define ODD(a)	(((int)(a) & 1) != 0)
#endif

#ifndef EVEN
#define EVEN(a)	(((int)(a) & 1) == 0)
#endif

#ifndef co_max
#define co_max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#endif

#ifndef co_min
#define co_min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#endif

#define SwapInts(Dragon,Eagle)\
 {int ShakingCrane;ShakingCrane = Dragon;Dragon = Eagle;Eagle = ShakingCrane;}
#define ORD(x,y)  ( (int) &( ((struct x *) 0)->y ) )

/* Error checking */
#define	If_Error_Then_Return_Status  if ( (status & 1) != 1) return( status )
#define	If_Error_Then  if ( (status & 1) != 1)
#define	If_Not_Error_Then  if ( (status & 1) )
#define ShowErr( str ) if ( (status & 1) != 1) vms_perror( str, status )
#define	ShowErr_Return( str ) \
If_Error_Then { vms_perror( str, status ); return( status ); }

/* DECwindows macros */
#ifdef VAXC
#define REGARG( NAME ) { "NAME", (XtPointer) NAME }
#else
#define REGARG( NAME ) { #NAME, (XtPointer) NAME }
#endif

/* General procedure type definition */
typedef void (*voidp)();

/* Common part type definition */
typedef struct s_commonpart
{
  void		*parent_ctx;
  Widget	parent_wid;
  char		*name;
} t_commonpart, *commonpart;

/* UIL fetcher */
void uilutil_fetch(
		char **,
		int,
		MrmRegisterArg *,
		int ,
		Widget,
		char *,
		char *,
		ArgList,
		int,
		Widget *,
		MrmHierarchy *
		);


void vms_perror (
  char	*userstring,
  int	msgid
);


#if defined __cplusplus
}
#endif
#endif
