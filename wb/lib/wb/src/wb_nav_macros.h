#ifndef wb_nav_macros_h
#define wb_nav_macros_h
 
/* wb_nav_macros.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef _Xm_h
#include <Xm/Xm.h>
#endif

#ifndef MrmPublic
#include <Mrm/MrmPublic.h>
#endif

#undef Status


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

#ifndef max
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#endif

#ifndef min
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
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


#endif
