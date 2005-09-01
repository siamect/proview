/* 
 * Proview   $Id: pwr_lst.h,v 1.2 2005-09-01 14:57:49 claes Exp $
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

#ifndef pwr_lst_h
#define pwr_lst_h 

/* pwr_lst.h -- list macros
*/

#ifndef VAXC
#define LstType(a) \
    typedef struct s_LstLink_##a sLstLink_##a;\
    struct s_LstLink_##a { \
	sLstLink_##a *nex;\
	sLstLink_##a *pre;\
	a *obj;}
#else
#define LstType(a) \
    typedef struct s_LstLink_/**/a sLstLink_/**/a;\
    struct s_LstLink_/**/a { \
	sLstLink_/**/a *nex;\
	sLstLink_/**/a *pre;\
	a *obj;}
#endif


#ifndef VAXC
#define LstLink(a)\
	  sLstLink_##a
#else
#define LstLink(a)\
	  sLstLink_/**/a
#endif


#ifndef VAXC
#define LstHead(a)\
	  sLstLink_##a
#else
#define LstHead(a)\
	  sLstLink_/**/a
#endif

#define LstNex(p) ((p)->nex)
#define LstPre(p) ((p)->pre)
#define LstLas(h) ((h)->pre)
#define LstFir(h) ((h)->nex)
#define LstEnd(h) (h)
#define LstIns(p, o, e) (\
    (o)->e.obj = (void *)o,\
    (o)->e.pre = (void *)((p)->pre),\
    (o)->e.nex = (void *)(p),\
    (p)->pre->nex = (void *)(&(o)->e),\
    (p)->pre = (void *)(&(o)->e),\
    p)
#define LstRem(p) ((p)->nex->pre = (p)->pre,(p)->pre->nex = (p)->nex)
#define LstNul(p) ((p)->nex = (p)->pre = NULL)
#define LstInl(p) ((p)->nex != NULL && (p)->pre != NULL)
#define LstIni(h) ((h)->nex = (h)->pre = (h))
#define LstObj(p) ((p)->obj)
#define LstEmp(h) ((h) == (h)->nex)

#endif
