#ifndef pwr_lst_h
#define pwr_lst_h 

/* pwr_lst.h -- list macros

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
