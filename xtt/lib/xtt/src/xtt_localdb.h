#ifndef xtt_xnav_localdb_h
#define xtt_xnav_localdb_h

/* xtt_localdb.h -- Local parameter database

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


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
