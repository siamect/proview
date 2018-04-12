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

/* @(#)rpc_svcout.c	2.1 88/08/01 4.0 RPCSRC */
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
static char sccsid[] = "@(#)rpc_svcout.c 1.6 87/06/24 (C) 1987 SMI";
#endif

/*
 * rpc_svcout.c, Server-skeleton outputter for the RPC protocol compiler
 * Copyright (C) 1987, Sun Microsytsems, Inc. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pdr_parse.h"
#include "pdr_util.h"

static char RQSTP[] = "rqstp";
static char TRANSP[] = "transp";
static char ARG[] = "argument";
static char RESULT[] = "result";
static char ROUTINE[] = "local";


static void write_program(definition *def, 
                          char *storage);
static void printerr(char *err,
                     char *transp);

static void printif(char *proc,
                    char *transp,
                    char *prefix,
                    char *arg);


static int nullproc(proc_list *proc);





/*
 * write most of the service, that is, everything but the registrations. 
 */
void
write_most()
{
	list *l;
	definition *def;
	version_list *vp;

	for (l = defined; l != NULL; l = l->next) {
		def = (definition *) l->val;
		if (def->def_kind == DEF_PROGRAM) {
			for (vp = def->def.pr.versions; vp != NULL; vp = vp->next) {
				f_print(fout, "\nstatic void ");
				pvname(def->def_name, vp->vers_num);
				f_print(fout, "();");
			}
		}
	}
	f_print(fout, "\n\n");
	f_print(fout, "main()\n");
	f_print(fout, "{\n");
	f_print(fout, "\tSVCXPRT *%s;\n", TRANSP);
	f_print(fout, "\n");
#ifdef WIN32
	f_print(fout, "#ifdef WIN32\n\trpc_nt_init();\n#endif\n\n");
#endif
	for (l = defined; l != NULL; l = l->next) {
		def = (definition *) l->val;
		if (def->def_kind != DEF_PROGRAM) {
			continue;
		}
		for (vp = def->def.pr.versions; vp != NULL; vp = vp->next) {
 			f_print(fout, "\t(void)pmap_unset(%s, %s);\n", def->def_name, vp->vers_name);
		}
	}
}


/*
 * write a registration for the given transport 
 */
void
write_register(char *transp)
{
	list *l;
	definition *def;
	version_list *vp;

	f_print(fout, "\n");
	f_print(fout, "\t%s = svc%s_create(RPC_ANYSOCK", TRANSP, transp);
	if (streq(transp, "tcp")) {
		f_print(fout, ", 0, 0");
	}
	f_print(fout, ");\n");
	f_print(fout, "\tif (%s == NULL) {\n", TRANSP);
 	f_print(fout, "\t\t(void)fprintf(stderr, \"cannot create %s service.\\n\");\n", transp);
#ifdef WIN32
	f_print(fout, "#ifdef WIN32\n\t\trpc_nt_exit();\n#endif\n");
#endif
	f_print(fout, "\t\texit(1);\n");
	f_print(fout, "\t}\n");

	for (l = defined; l != NULL; l = l->next) {
		def = (definition *) l->val;
		if (def->def_kind != DEF_PROGRAM) {
			continue;
		}
		for (vp = def->def.pr.versions; vp != NULL; vp = vp->next) {
			f_print(fout,
				"\tif (!svc_register(%s, %s, %s, ",
				TRANSP, def->def_name, vp->vers_name);
			pvname(def->def_name, vp->vers_num);
			f_print(fout, ", IPPROTO_%s)) {\n",
				streq(transp, "udp") ? "UDP" : "TCP");
			f_print(fout,
 				"\t\t(void)fprintf(stderr, \"unable to register (%s, %s, %s).\\n\");\n",
				def->def_name, vp->vers_name, transp);
#ifdef WIN32
			f_print(fout, "#ifdef WIN32\n\t\trpc_nt_exit();\n#endif\n");
#endif
			f_print(fout, "\t\texit(1);\n");
			f_print(fout, "\t}\n");
		}
	}
}


/*
 * write the rest of the service 
 */
void
write_rest()
{
	f_print(fout, "\tsvc_run();\n");
 	f_print(fout, "\t(void)fprintf(stderr, \"svc_run returned\\n\");\n");
#ifdef WIN32
	f_print(fout, "#ifdef WIN32\n\trpc_nt_exit();\n#endif\n");
#endif
	f_print(fout, "\texit(1);\n");
	f_print(fout, "}\n");
}

void
write_programs(char *storage)
{
	list *l;
	definition *def;

	for (l = defined; l != NULL; l = l->next) {
		def = (definition *) l->val;
		if (def->def_kind == DEF_PROGRAM) {
			write_program(def, storage);
		}
	}
}


static void
write_program(definition *def, 
              char *storage)
{
	version_list *vp;
	proc_list *proc;
	int filled;

#ifdef WIN32
	f_print(fout, "\n#ifdef MULTITHREAD\n");
	f_print(fout, "\
struct call_params {\n\
	struct svc_req *rqstp;\n\
	SVCXPRT *transp;\n\
	void *arg;\n\
	char *(*local)();\n\
	bool_t (*xdr_argument)(), (*xdr_result)();\n\
};\n\
");

	for (vp = def->def.pr.versions; vp != NULL; vp = vp->next) {
		if (storage != NULL) {
			f_print(fout, "%s ", storage);
		}
		f_print(fout, "void ");
		pvname(def->def_name, vp->vers_num);
		f_print(fout, "_a ();\n");
	}
	f_print(fout, "#endif\n");
		
#endif

	for (vp = def->def.pr.versions; vp != NULL; vp = vp->next) {
		f_print(fout, "\n");

		if (storage != NULL) {
			f_print(fout, "%s ", storage);
		}
		f_print(fout, "void\n");
		pvname(def->def_name, vp->vers_num);
		f_print(fout, "(%s, %s)\n", RQSTP, TRANSP);
		f_print(fout, "	struct svc_req *%s;\n", RQSTP);
		f_print(fout, "	SVCXPRT *%s;\n", TRANSP);
		f_print(fout, "{\n");

		filled = 0;
		f_print(fout, "\tunion {\n");
		for (proc = vp->procs; proc != NULL; proc = proc->next) {
			if (streq(proc->arg_type, "void")) {
				continue;
			}
			filled = 1;
			f_print(fout, "\t\t");
			ptype(proc->arg_prefix, proc->arg_type, 0);
			pvname(proc->proc_name, vp->vers_num);
			f_print(fout, "_arg;\n");
		}
		if (!filled) {
			f_print(fout, "\t\tint fill;\n");
		}
		f_print(fout, "\t} %s;\n", ARG);
		f_print(fout, "\tchar *%s;\n", RESULT);
		f_print(fout, "\tbool_t (*xdr_%s)(), (*xdr_%s)();\n", ARG, RESULT);
		f_print(fout, "\tchar *(*%s)();\n", ROUTINE);
		f_print(fout, "\n");

#ifdef WIN32
		f_print(fout, "\n#ifdef MULTITHREAD\n");
		f_print(fout, "\
	DWORD TID = 0;\n\
	HANDLE threadHandle = NULL;\n\
	struct call_params	*params;\n");
		f_print(fout, "#endif\n");
		
#endif

		f_print(fout, "\tswitch (%s->rq_proc) {\n", RQSTP);

		if (!nullproc(vp->procs)) {
			f_print(fout, "\tcase NULLPROC:\n");
 			f_print(fout, "\t\t(void)svc_sendreply(%s, xdr_void, (char *)NULL);\n", TRANSP);
			f_print(fout, "\t\treturn;\n\n");
		}
		for (proc = vp->procs; proc != NULL; proc = proc->next) {
			f_print(fout, "\tcase %s:\n", proc->proc_name);
			f_print(fout, "\t\txdr_%s = xdr_%s;\n", ARG, 
				stringfix(proc->arg_type));
			f_print(fout, "\t\txdr_%s = xdr_%s;\n", RESULT, 
				stringfix(proc->res_type));
			f_print(fout, "\t\t%s = (char *(*)()) ", ROUTINE);
			pvname(proc->proc_name, vp->vers_num);
			f_print(fout, ";\n");
			f_print(fout, "\t\tbreak;\n\n");
		}
		f_print(fout, "\tdefault:\n");
		printerr("noproc", TRANSP);
		f_print(fout, "\t\treturn;\n");
		f_print(fout, "\t}\n");

 		f_print(fout, "\tbzero((char *)&%s, sizeof(%s));\n", ARG, ARG);
		printif("getargs", TRANSP, "&", ARG);
		printerr("decode", TRANSP);
		f_print(fout, "\t\treturn;\n");
		f_print(fout, "\t}\n");

#ifdef WIN32
		f_print(fout, "\n#ifdef MULTITHREAD\n");
		f_print(fout, "\
	params = (struct call_params*) malloc (sizeof(struct call_params));\n\
\n\
	params->%s = (struct svc_req*) malloc (sizeof(struct svc_req));\n\
	bcopy(%s, params->rqstp, sizeof(struct svc_req));\n\
\n\
	params->transp = %s;\n\
\n\
	params->arg = malloc (sizeof(%s));\n\
	bcopy(&%s, params->arg, sizeof(%s));\n\
\n\
	params->local = %s;\n\
\n\
	params->xdr_argument = xdr_%s;\n\
	params->xdr_result = xdr_%s;\n\
\n", RQSTP, RQSTP, TRANSP, ARG, ARG, ARG, ROUTINE, ARG, RESULT);


		f_print(fout, "\
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)", RQSTP, TRANSP);

		pvname(def->def_name, vp->vers_num);
	
		f_print(fout, "_a, params, 0, &TID);\n\
	if (!threadHandle)\n\t\t");

		pvname(def->def_name, vp->vers_num);

		f_print(fout, "_a(&params);\n\
}\n\
\n\
static void\n");

		pvname(def->def_name, vp->vers_num);

		f_print(fout, "_a(params)\n\
struct call_params *params;\n\
{\n\
	struct svc_req *%s;\n\
	SVCXPRT *%s;\n\
	void *%s;\n", RQSTP, TRANSP, ARG);

		f_print(fout, "\tvoid (*destroy_proc)();\n\n");
		f_print(fout, "\tchar *%s;\n", RESULT);
		f_print(fout, "\tbool_t (*xdr_%s)(), (*xdr_%s)();\n", ARG, RESULT);
		f_print(fout, "\tchar *(*%s)();\n", ROUTINE);
		f_print(fout, "\n\n");

		f_print(fout, "\t%s = params->arg;\n", ARG);
		f_print(fout, "\t%s = params->rqstp;\n", RQSTP);
		f_print(fout, "\t%s = params->transp;\n", TRANSP);
		f_print(fout, "\txdr_%s = params->xdr_argument;\n", ARG);
		f_print(fout, "\txdr_%s = params->xdr_result;\n", RESULT);
		f_print(fout, "\t%s = params->local;\n", ROUTINE);

		f_print(fout, "\tdestroy_proc = %s->xp_ops->xp_destroy;\n", TRANSP);
		f_print(fout, "\t%s->xp_ops->xp_destroy = xprt_unregister;\n", TRANSP);

		f_print(fout, "#endif\n\n");
#endif
		f_print(fout, "\t%s = (*%s)(&%s, %s);\n", RESULT, ROUTINE, ARG,
			RQSTP);
		f_print(fout, 
			"\tif (%s != NULL && !svc_sendreply(%s, xdr_%s, %s)) {\n",
			RESULT, TRANSP, RESULT, RESULT);
		printerr("systemerr", TRANSP);
		f_print(fout, "\t}\n");

#ifdef WIN32
		f_print(fout, "#ifdef MULTITHREAD\n");
		f_print(fout, "\tfree(params->rqstp);\n");
		f_print(fout, "\tfree(params);\n");
		f_print(fout, "#endif\n");
#endif

		printif("freeargs", TRANSP, "&", ARG);
 		f_print(fout, "\t\t(void)fprintf(stderr, \"unable to free arguments\\n\");\n");
#ifdef WIN32
		f_print(fout, "#ifdef WIN32\n\t\trpc_nt_exit();\n#endif\n");
#endif
		f_print(fout, "\t\texit(1);\n");
		f_print(fout, "\t}\n");

#ifdef WIN32
		f_print(fout, "#ifdef MULTITHREAD\n");
		f_print(fout, "\tfree(%s);\n", ARG);
		f_print(fout, "\ttransp->xp_ops->xp_destroy = destroy_proc;\n", TRANSP);
		f_print(fout, "#endif\n");
#endif

		f_print(fout, "}\n\n");
	}
}

static void
printerr(char *err,
	char *transp)
{
	f_print(fout, "\t\tsvcerr_%s(%s);\n", err, transp);
}

static void
printif(char *proc,
  char *transp,
  char *prefix,
  char *arg)
{
	f_print(fout, "\tif (!svc_%s(%s, xdr_%s, %s%s)) {\n",
		proc, transp, arg, prefix, arg);
}


static int
nullproc(proc_list *proc)
{
	for (; proc != NULL; proc = proc->next) {
		if (streq(proc->proc_num, "0")) {
			return (1);
		}
	}
	return (0);
}
