#ifndef cnv_tops_h
#define cnv_tops_h

/* cnv_tops.h -- Postscript output

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

using namespace std;

#include <iostream.h>
#include <vector.h>
#include <string>
#include "pwr.h"
#include "cnv_style.h"
#include "cnv_content.h"

#define ps_cMaxLevel 4
#define ps_cPageHeight 820
#define ps_cPageWidth 535
#define ps_cPageNumX (ps_cPageWidth - 10)
#define ps_cPageNumY (ps_cPageHeight - 13)
#define ps_cLeftMargin 100
#define ps_cTopMargin 100
#define ps_cBottomMargin 50
#define ps_cCellSize 110
#define ps_cTmpFile "/tmp/ptmp.ps"

typedef enum {
  ps_mPrintMode_Pos 	= 1 << 0,
  ps_mPrintMode_Start	= 1 << 1,
  ps_mPrintMode_Continue= 1 << 2,
  ps_mPrintMode_End	= 1 << 3,
  ps_mPrintMode_KeepY	= 1 << 4,
  ps_mPrintMode_FixX	= 1 << 5
} ps_mPrintMode;

typedef enum {
  ps_eFile_Info,
  ps_eFile_Body,
  ps_eFile__
} ps_eFile;

typedef enum {
  ps_eId_TitlePage,
  ps_eId_InfoPage,
  ps_eId_Content,
  ps_eId_Chapter,
  ps_eId_TopicL1,
  ps_eId_TopicL2,
  ps_eId_TopicL3,
  ps_eId_Function,
  ps_eId_Class,
  ps_eId__
} ps_eId;


class CnvToPs  {
 public:
  CnvToPs() :
    cf(0), ci(0), conf_pass(false)
    {
      for ( int i = 0; i < ps_cMaxLevel; i++)
	header_number[i] = 0;
      for ( int i = 0; i < ps_eFile__; i++)
	page_number[i] = 0;

      style[ps_eId_TitlePage].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 35, 0, 100, 20, cnv_eAlignment_Center, 0, 0, 0);
      style[ps_eId_TitlePage].h2 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 25, 0, 50, 20, cnv_eAlignment_Center, 0, 0, 0);
      style[ps_eId_TitlePage].h3 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 15, 0, 25, 10, cnv_eAlignment_Center, 0, 0, 0);
      style[ps_eId_TitlePage].text = 
	CnvStyle( "Helvetica-ISOLatin1", 8, 0, 9, 1, cnv_eAlignment_Center, 0, 0, 0);
      style[ps_eId_TitlePage].boldtext = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 8, 0, 9, 1, cnv_eAlignment_Center, 0, 0, 0);
      style[ps_eId_Chapter].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 24, 0, 24, 40, cnv_eAlignment_Left, 0, 1, 1);
      style[ps_eId_TopicL2].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 12, 0, 20, 10, cnv_eAlignment_Left, 0, 0, 1);
      style[ps_eId_TopicL2].h2 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 10, 0, 16, 8, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_TopicL3].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 10, 0, 16, 8, cnv_eAlignment_Left, 0, 0, 1);
      style[ps_eId_TopicL3].h2 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 8, 0, 16, 5, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_Function].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 24, 0, 24, 20, cnv_eAlignment_Left, 1, 0, 1);
      style[ps_eId_Class].h1 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 24, 0, 24, 20, cnv_eAlignment_Left, 1, 0, 1);
      style[ps_eId_Class].h2 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 20, 0, 24, 8, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_Class].h3 = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 16, -40, 24, 6, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_Content].boldtext = 
	CnvStyle( "Helvetica-Bold-ISOLatin1", 10, 0, 11, 1, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_Chapter].text = 
	CnvStyle( "TimesNewRoman-ISOLatin1", 10, 0, 11, 1, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_TopicL1].text = 
	CnvStyle( "TimesNewRoman-ISOLatin1", 10, 0, 11, 1, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_TopicL2].text = 
	CnvStyle( "TimesNewRoman-ISOLatin1", 10, 0, 11, 1, cnv_eAlignment_Left, 0, 0, 0);
      style[ps_eId_TopicL3].text = 
	CnvStyle( "TimesNewRoman-ISOLatin1", 10, 0, 11, 1, cnv_eAlignment_Left, 0, 0, 0);
    }
  ~CnvToPs();

  void close();
  void print_text( char *text, CnvStyle& style, int mode = ps_mPrintMode_Pos);
  void print_pagebreak( int print_num);
  void print_content();
  void print_h1( char *text, int hlevel, char *subject);
  void print_h2( char *text);
  void print_h3( char *text);
  void print_horizontal_line();
  int print_image( char *filename);
  void cnv_text( char *to, char *from);
  void set_confpass( bool conf) { 
    conf_pass = conf;
    if ( !conf) {
      // Reset
      for ( int i = 0; i < ps_cMaxLevel; i++)
	header_number[i] = 0;
      for ( int i = 0; i < ps_eFile__; i++)
	page_number[i] = 0;
      cf = 0;
      ci = 0;
    }
  }
  void set_ci( int val) { ci = val;}
  void set_cf( int val) { cf = val;}
  void set_filename( int idx, char *name) {
    strcpy( filename[idx], name);
  }
  void open();
  void incr_headerlevel();
  void decr_headerlevel();
  void reset_headernumbers( int level);
  void set_pageheader( char *text);

  CnvContent content;
  CnvIdStyle style[ps_eId__];
  ofstream fp[ps_eFile__];
  pwr_tFileName filename[ps_eFile__];
  int cf;
  int ci;
  double x;
  double y;
  int page_number[ps_eFile__];
  int header_number[ps_cMaxLevel];
  bool conf_pass;
  char current_chapter[160];
  char previous_chapter[160];
};

#endif









