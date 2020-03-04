
#include <stdio.h>
#include <string.h>
#include "pwr.h"
#include "co_dcli.h"
#include "co_string.h"

static pwr_tFileName inc_path[10];
static unsigned int inc_path_cnt = 0;

void usage()
{
  printf("\nco_jsconcat\n\n> co_jsconcat [-I include-directory] -o outfile infile\n\n");
}

int read_file(FILE *ofp, char *incfile)
{
  pwr_tFileName fname;
  FILE *ifp;
  int i;
  char line[400];
  
  for (i = 0; i < inc_path_cnt; i++) {
    strcpy(fname, inc_path[i]);
    strcat(fname, "/");
    strcat(fname, incfile);

    dcli_translate_filename(fname, fname);
    ifp = fopen(fname, "r");
    if (ifp)
      break;
    else if (i == inc_path_cnt - 1) {
      printf("** Unable to open file %s\n", incfile);
      exit(0);
    }
  }

  while (dcli_read_line(line, sizeof(line), ifp)) {
    if (strncmp(line, "#jsc_include", 12) == 0) {
      str_trim(incfile, &line[13]);
      read_file(ofp, (char *)incfile);
    }
    else {
      fputs(line, ofp);
      fputc('\n', ofp);
      //      fprintf(ofp, "%s\n", line);
    }
  }
  return 1;
}

int main(int argc, char* argv[]) 
{
  pwr_tFileName outfile = "";
  pwr_tFileName infile = "";
  pwr_tFileName incfile;
  pwr_tFileName fname;
  int i;
  FILE *ifp;
  FILE *ofp;
  char line[400];

  for (i = 1; i < argc; i++) {
    if ((argv)[i][0] == '-') {
      int i_incr = 0;
      for (int j = 1; (argv)[i][j] != 0 && (argv)[i][j] != ' '
	     && (argv)[i][j] != '	';
           j++) {
        switch ((argv)[i][j]) {
        case 'I':
          if (i + 1 >= argc
              || !((argv)[i][j + 1] == ' ' || (argv)[i][j + 1] != '	')) {
            usage();
            exit(0);
          }
	  if (inc_path_cnt >= sizeof(inc_path)/sizeof(inc_path[0])) {
	    printf("** Max number of include paths exceeded\n");
	    exit(0);
	  }
          strncpy(inc_path[inc_path_cnt], (argv)[i + 1], sizeof(inc_path[0]));
	  inc_path_cnt++;
          i++;
          i_incr = 1;
          break;
        case 'o':
          if (i + 1 >= argc
              || !((argv)[i][j + 1] == ' ' || (argv)[i][j + 1] != '	')) {
            usage();
            exit(0);
          }
          strncpy(outfile, (argv)[i + 1], sizeof(outfile));
	  inc_path_cnt++;
	  i++;
	  i_incr = 1;
          break;
        default:
          usage();
          exit(0);
        }
        if (i_incr)
          break;
      }
    } else {
      // Input file
      strcpy(infile, (argv)[i]);
    }
  }

  printf("-I %s -l %s %s\n", inc_path[0], outfile, infile);

  dcli_translate_filename(fname, infile);
  ifp = fopen(fname, "r");
  if (!ifp) {
    printf("** Unable to open file %s\n", fname);
    exit(0);
  }

  dcli_translate_filename(fname, outfile);
  ofp = fopen(fname, "w");
  if (!ofp) {
    printf("** Unable to open file %s\n", fname);
    exit(0);
  }

  while (dcli_read_line(line, sizeof(line), ifp)) {
    if (strncmp(line, "#jsc_include", 12) == 0) {
      str_trim(incfile, &line[13]);
      read_file(ofp, incfile);
    }
    else {
      fputs(line, ofp);
      fputc('\n', ofp);
      //      fprintf(ofp, "%s\n", line);
    }
  }
}
