#ifndef cnv_xtthelpread_h
#define cnv_xtthelpread_h

/* cnv_xtthelpread.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

class CnvXtthelpTo;

class CnvReadXtthelp {
  public:
    CnvReadXtthelp( char *x_name, char *x_directory,
		    CnvXtthelpTo *to);
    ofstream fp;
    char directory[80];
    char name[80];
    char filename[120];
    CnvXtthelpTo *xtthelpto;

    int read_xtthelp();
};


#if defined __cplusplus
}
#endif
#endif





