#ifndef cnv_help_h
#define cnv_help_h

/* cnv_help.h -- Documentation

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#if defined __cplusplus
extern "C" {
#endif

class XhelpToHtml {
  public:
    XhelpToHtml( char *x_name, char *x_directory);
    ofstream fp;
    char directory[80];
    char name[80];
    char filename[120];

    void subject_to_fname( char *fname, char *subject, int path);
    int generate();
};


#if defined __cplusplus
}
#endif
#endif





