/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 * Users may use, copy or modify Sun RPC for the Windows NT Operating 
 * System according to the Sun copyright below.
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO 
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE 
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/

/* @(#)rpc_main.c	2.2 88/08/01 4.0 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
#if 0
static char sccsid[] = "@(#)rpc_main.c 1.7 87/06/24 (C) 1987 SMI";
#endif

/*
 * rpc_main.c, Top level of the RPC protocol compiler. 
 * Copyright (C) 1987, Sun Microsystems, Inc. 
 */

#include <stdio.h>
#ifdef WIN32
#include <stdlib.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include "pdr_util.h"
#include "pdr_pars.h"
#include "pdr_scan.h"
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include "pdr_util.h"
#include "pdr_parse.h"
#include "pdr_scan.h"
#endif


#define EXTEND	1		/* alias for TRUE */

struct commandline {
	int cflag;
	int hflag;
	int lflag;
	int sflag;
	int mflag;
	char *infile;
	char *outfile;
};

static char *cmdname;
#ifdef WIN32
static char CPP[] = "cl";
static char CPPFLAGS[] = "/C /EP /nologo";
#elif defined OS_VMS
static char CPP[] = "cc";
static char CPPFLAGS[] = "/prep=";
#else
static char CPP[] = "/lib/cpp";
static char CPPFLAGS[] = "-C";
#endif

#ifdef OS_VMS
static char H_DEFINE[] = "/define=\"PDR_HDR\"";
static char C_DEFINE[] = "/define=PDR_PDR";
static char CL_DEFINE[] = "/define=PDR_CLNT";
static char S_DEFINE[] = "/define=PDR_SVC";
#else
static char H_DEFINE[] = "-DPDR_HDR";
static char C_DEFINE[] = "-DPDR_PDR";
static char CL_DEFINE[] = "-DPDR_CLNT";
static char S_DEFINE[] = "-DPDR_SVC";
#endif


static char *allv[] = {
	"rpcgen", "-s", "udp", "-s", "tcp",
};


static int allc = sizeof(allv)/sizeof(allv[0]);


static void
c_output(char *infile,
         char *define,
         int extend,
         char *outfile);

static void
h_output(
         char *infile,
         char *define,
         int extend,
         char *outfile);

static void
s_output(int argc,
         char *argv[],
         char *infile,
         char *define,
         int extend,
         char *outfile,
         int nomain);



static void
l_output(char *infile,
         char *define,
         int extend,
         char *outfile);

static void
do_registers(int argc,
             char *argv[]);

static int
parseargs(int argc,
          char *argv[],
          struct commandline *cmd);





int main(int argc,
         char *argv[])
{
	struct commandline cmd;

	if (!parseargs(argc, argv, &cmd)) {
		f_print(stderr,
			"usage: %s infile\n", cmdname);
		f_print(stderr,
			"       %s [-c | -h] [-o outfile] [infile]\n",
			cmdname);
#if 0 /* Not supported by PDR */
		f_print(stderr,
			"       %s [-s udp|tcp]* [-o outfile] [infile]\n",
			cmdname);
#endif
		exit(1);
	}

	fin = NULL;

	if (cmd.cflag) {
		c_output(cmd.infile, C_DEFINE, !EXTEND, cmd.outfile);
	} else if (cmd.hflag) {
		h_output(cmd.infile, H_DEFINE, !EXTEND, cmd.outfile);
	} else if (cmd.lflag) {
		l_output(cmd.infile, CL_DEFINE, !EXTEND, cmd.outfile);
	} else if (cmd.sflag || cmd.mflag) {
		s_output(argc, argv, cmd.infile, S_DEFINE, !EXTEND,
			 cmd.outfile, cmd.mflag);
	} else {
		c_output(cmd.infile, C_DEFINE, EXTEND, "_pdr.c");
		reinitialize();
		h_output(cmd.infile, H_DEFINE, EXTEND, ".h");
		reinitialize();
		l_output(cmd.infile, CL_DEFINE, EXTEND, "_clnt.c");
		reinitialize();
		s_output(allc, allv, cmd.infile, S_DEFINE, EXTEND,
			 "_svc.c", cmd.mflag);
	}

#ifdef OS_VMS
	if (fin != NULL) {
		char name[256];
		fgetname(fin, name);
		remove(name);
	}
#endif
        
	exit(0);
}

/*
 * add extension to filename 
 */
static char *
extendfile(file, ext)
	char *file;
	char *ext;
{
#ifdef WIN32
	int i;
        char dirchar = '\\';
#else
        char dirchar = '/';
#endif
	char *res;
	char *p;

        p = strrchr(file, dirchar);
        if (p != NULL)
		file = p + 1;
        

	res = alloc(strlen(file) + strlen(ext) + 1);
	if (res == NULL) {
		abort();
	}
#ifdef WIN32
	p = NULL;
	for (i = 0; file[i] != '\0'; i++)
		if (file[i] == '.')
			p = &file[i];
#else
	p = rindex(file, '.');
#endif
	if (p == NULL) {
		p = file + strlen(file);
	}
	(void) strcpy(res, file);
	(void) strcpy(res + (p - file), ext);
	return (res);
}

/*
 * Open output file with given extension 
 */
static void
open_output(char *infile,
            char *outfile)
{
	if (outfile == NULL) {
		fout = stdout;
		return;
	}
	if (infile != NULL && streq(outfile, infile)) {
		f_print(stderr, "%s: output would overwrite %s\n", cmdname,
			infile);
		crash();
	}
	fout = fopen(outfile, "w");
	if (fout == NULL) {
		f_print(stderr, "%s: unable to open ", cmdname);
		perror(outfile);
		crash();
	}
	record_open(outfile);
}

/*
 * Open input file with given define for C-preprocessor 
 */
static void
open_input(char *infile,
           char *define,
	   char *outfile /* VMS. Use for temporary file */)
{
#ifdef WIN32
	int old;
	int pd[2];
	infilename = (infile == NULL) ? "<stdin>" : infile;
	_pipe(pd, 0xffff, O_TEXT);

		old = dup(1);
		(void) dup2(pd[1], 1);

		if (_spawnlp(_P_WAIT, CPP, CPP, CPPFLAGS, 
					define, infile, NULL) < 0) {
			f_print(stderr, "%s: unable to open ", cmdname);
			perror(CPP);
			crash();
		}

	(void) dup2(old, 1);

	(void) close(pd[1]);
	fin = fdopen(pd[0], "r");
	if (fin == NULL) {
		f_print(stderr, "%s: ", cmdname);
		perror(infilename);
		crash();
	}
#elif defined OS_VMS
        char cmd[1024];
	char tmpfile[256];
	char *cp, *fp;
	
	fp = outfile ? outfile : infile;
        fin = NULL;
	
	if ((cp = strrchr(fp, '.')) != NULL) {
		*cp = 0;
		sprintf(tmpfile, "%s.i", fp);
		*cp = '.';
		sprintf(cmd, "%s;*", tmpfile);
		remove(cmd);
		sprintf(cmd, "%s%s%s%s %s", CPP, CPPFLAGS, tmpfile, define, infile);
		system(cmd);
        	fin = fopen(tmpfile, "r");
	}
	if (fin == NULL) {
		f_print(stderr, "%s: ", cmdname);
		perror(infilename);
		crash();
	}
	
#else
	int pd[2];

	infilename = (infile == NULL) ? "<stdin>" : infile;
	(void) pipe(pd);
	switch (fork()) {
	case 0:
		(void) close(1);
		(void) dup2(pd[1], 1);
		(void) close(pd[0]);
		execl(CPP, CPP, CPPFLAGS, define, infile, NULL);
		perror("execl");
		exit(1);
	case -1:
		perror("fork");
		exit(1);
	}
	(void) close(pd[1]);
	fin = fdopen(pd[0], "r");
	if (fin == NULL) {
		f_print(stderr, "%s: ", cmdname);
		perror(infilename);
		crash();
	}
#endif
}

/*
 * Compile into an PDR routine output file
 */
static void
c_output(char *infile,
         char *define,
         int extend,
         char *outfile)
{
	definition *def;
	char *include;
	char *outfilename;
	long tell;

	open_input(infile, define, outfile);	
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include \"co_pdr.h\"\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	tell = ftell(fout);
	while ((def = get_definition())) {
		emit(def);
	}
	if (extend && tell == ftell(fout)) {
		(void) unlink(outfilename);
	}
}

/*
 * Compile into an PDR header file
 */
static void
h_output(
         char *infile,
         char *define,
         int extend,
         char *outfile)
{
	definition *def;
	char *outfilename;
	long tell;

	open_input(infile, define, outfile);
	outfilename =  extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	tell = ftell(fout);

	f_print(fout, "#include \"co_pdr.h\"\n");


	while ((def = get_definition())) {
		print_datadef(def);
	}
	if (extend && tell == ftell(fout)) {
		(void) unlink(outfilename);
	}
}

/*
 * Compile into an RPC service
 */
static void
s_output(int argc,
         char *argv[],
         char *infile,
         char *define,
         int extend,
         char *outfile,
         int nomain)
{
	char *include;
	definition *def;
	int foundprogram;
	char *outfilename;

	open_input(infile, define, outfile);
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include <stdio.h>\n");
	f_print(fout, "#include <rpc/rpc.h>\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	foundprogram = 0;
	while ((def = get_definition())) {
		foundprogram |= (def->def_kind == DEF_PROGRAM);
	}
	if (extend && !foundprogram) {
		(void) unlink(outfilename);
		return;
	}
	if (nomain) {
		write_programs((char *)NULL);
	} else {
		write_most();
		do_registers(argc, argv);
		write_rest();
		write_programs("static");
	}
}

static void
l_output(char *infile,
         char *define,
         int extend,
         char *outfile)
{
	char *include;
	definition *def;
	int foundprogram;
	char *outfilename;

	open_input(infile, define, outfile);
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include <rpc/rpc.h>\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	foundprogram = 0;
	while ((def = get_definition())) {
		foundprogram |= (def->def_kind == DEF_PROGRAM);
	}
	if (extend && !foundprogram) {
		(void) unlink(outfilename);
		return;
	}
	write_stubs();
}

/*
 * Perform registrations for service output 
 */
static void
do_registers(int argc,
             char *argv[])

{
	int i;

	for (i = 1; i < argc; i++) {
		if (streq(argv[i], "-s")) {
			write_register(argv[i + 1]);
			i++;
		}
	}
}

/*
 * Parse command line arguments 
 */
static int
parseargs(int argc,
          char *argv[],
          struct commandline *cmd)

{
	int i;
	int j;
	char c;
	char flag[(1 << 8 * sizeof(char))];
	int nflags;

	cmdname = argv[0];
	cmd->infile = cmd->outfile = NULL;
	if (argc < 2) {
		return (0);
	}
	flag['c'] = 0;
	flag['h'] = 0;
        flag['s'] = 0;
	flag['o'] = 0;
	flag['l'] = 0;
	flag['m'] = 0;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (cmd->infile) {
				return (0);
			}
			cmd->infile = argv[i];
		} else {
			for (j = 1; argv[i][j] != 0; j++) {
				c = argv[i][j];
				switch (c) {
				case 'c':
				case 'h':
/* not supported by PDR		case 'l':
 *				case 'm':
 */
					if (flag[(int)c]) {
						return (0);
					}
					flag[(int)c] = 1;
					break;

/* not supported by PDR         case 's': */
				case 'o':
					if (argv[i][j - 1] != '-' || 
					    argv[i][j + 1] != 0) {
						return (0);
					}
					flag[(int)c] = 1;
					if (++i == argc) {
						return (0);
					}
					if (c == 's') {
						if (!streq(argv[i], "udp") &&
						    !streq(argv[i], "tcp")) {
							return (0);
						}
					} else if (c == 'o') {
						if (cmd->outfile) {
							return (0);
						}
						cmd->outfile = argv[i];
					}
					goto nextarg;

				default:
					return (0);
				}
			}
	nextarg:
			;
		}
	}
	cmd->cflag = flag['c'];
	cmd->hflag = flag['h'];
	cmd->sflag = flag['s'];
	cmd->lflag = flag['l'];
	cmd->mflag = flag['m'];
	nflags = cmd->cflag + cmd->hflag + cmd->sflag + cmd->lflag + cmd->mflag;
	if (nflags == 0) {
		if (cmd->outfile != NULL || cmd->infile == NULL) {
			return (0);
		}
	} else if (nflags > 1) {
		return (0);
	}
	return (1);
}
