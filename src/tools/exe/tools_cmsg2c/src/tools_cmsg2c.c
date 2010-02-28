/* 
 * Proview   $Id: tools_cmsg2c.c,v 1.2 2005-09-01 14:58:00 claes Exp $
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

/*
* Filename:
*   tools_cmsg2c.c
*
* Revision history:
*   Rev     Edit    Date        Who  Comment
*   ------- ----    ----------  ---- -------
*   X0.1       1    1996-10-01  ML   Created
*
* Description:	Creates a .c-file from a file which contains all c_msg-files
*		for which messages should be generated.
*		The c_msg-file should contain all 
*
*		The -b branch flag must be set if you wan't to generate messages
*		for PROVIEW.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pwr_lst.h"
#include "pwr_msg.h"

#include "cmsg2c.h"


#define MSG_NEW_STRING(str) strcpy(malloc(strlen(str) + 1), str)

typedef struct s_FacilityCB sFacilityCB;

LstType(sFacilityCB);



struct s_FacilityCB {
  LstLink(sFacilityCB)	FacL;
  char *facnam;
  int	facnum;
};




extern int lineno;


static int SyntaxError;

static LstHead(sFacilityCB)   lFacH;
static sFacilityCB	    *CurrFac = NULL;


static void CopyFile(FILE *ifp, FILE *ofp);
static void WriteFacility(FILE *cfp, char *branch);
static void WriteIncludes(FILE *cfp);




#if 0
static void dump()
{
  static int cnt = 1;
  LstLink(sFacilityCB) *fl;
  
  printf("dump: %d\n", cnt++);
  for (fl = LstFir(&lFacH); fl != LstEnd(&lFacH); fl = LstNex(fl))
    printf("%s\n", LstObj(fl)->facnam);

  
  printf("dump finished\n\n");

}
#endif


static void usage()
{
  printf("Usage: tools_cmsg2c [-b branch] input_file output_file\n");
  exit(1);
}




int main(int argc, char **argv) 
{
  extern FILE *yyin;
  extern int yylex();

  FILE *cfp = NULL;
  int bmsg = 0;
  char *branch = NULL;
  int inFile;
  int outFile;
  
  if (argc < 3) 
      usage();
    
  if (argv[1][0] == '-') {
    if (argv[1][1] == 'b') {
      branch = argv[2];
      bmsg = 1;
    }
    else 
      usage();
    if (argc != 5)
      usage(); 
  } else if (argc != 3)
    usage();

  inFile = bmsg ? 3 : 1; 
  outFile = inFile + 1;

  if (!(yyin = fopen(argv[inFile], "r"))) {
      printf("Can't open input file: %s", argv[inFile]);
      exit(1);
  }

  LstIni(&lFacH);
  SyntaxError = 0;
  lineno = 1;    
  
  yylex();

  if (!SyntaxError) {
    if (!(cfp = fopen(argv[outFile], "w+"))) {
	printf("Can't open output file: %s", argv[outFile]);
	exit(1);
    }  

    rewind(yyin);
    WriteIncludes(cfp);
    CopyFile(yyin, cfp);
    WriteFacility(cfp, branch);
  }

  fclose(yyin);
  fclose(cfp);

#if defined OS_VMS || defined OS_ELN
  exit(1);
#elif defined OS_LYNX || defined OS_LINUX || defined OS_MACOS
  exit(EXIT_SUCCESS);
#endif
}


/*
 * Routines called by lex
 */
void lex_FacName(char *facnam)
{
  sFacilityCB *facp = (sFacilityCB *)calloc(1, sizeof(sFacilityCB));
 
  facp->facnam = MSG_NEW_STRING(facnam);
  CurrFac = facp;

}


void lex_FacNum(int facnum)
{
  LstLink(sFacilityCB) *fl;
 
  /*
   * To do: Check that the facility number is within the valid range 
   *
   */
  
  /* Insert in ascending order */
  for (fl = LstFir(&lFacH); fl != LstEnd(&lFacH); fl = LstNex(fl)) {
    if (LstObj(fl)->facnum > facnum) {
      break;
    }
  }


  CurrFac->facnum = facnum;
  LstIns(fl, CurrFac, FacL);
  CurrFac = NULL;

}


/*
 * Local routines
 */

static void CopyFile(FILE *ifp, FILE *ofp)
{
  int c;

  while ((c = getc(ifp)) != EOF)
    putc(c, ofp);
}

static void WriteFacility(FILE *cfp, char *branch)
{
  LstLink(sFacilityCB) *fl;
  int i = 0;

  fprintf(cfp, "static msg_sFacility *Facilities[] = {\n"); 

  for (fl = LstFir(&lFacH); fl != LstEnd(&lFacH); fl = LstNex(fl)) {
    if (i++)
      fprintf(cfp, ",\n");

    fprintf(cfp, "\t%-20s /* %4d */", LstObj(fl)->facnam, LstObj(fl)->facnum); 

  }

  fprintf(cfp, "};\n\n");
  if (LstFir(&lFacH) == LstEnd(&lFacH))
    fprintf(cfp, "msg_sHead %sMsgHead = {0, 0};\n",
      branch ? branch : "pwrp");
  else
    fprintf(cfp, "msg_sHead %sMsgHead = {MSG_NOF(Facilities), Facilities};\n",
      branch ? branch : "pwrp");
   

}


static void WriteIncludes(FILE *cfp)
{
  fprintf(cfp, "#include \"pwr_msg.h\"\n\n");
}
