/* rt_pcm.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Program Control Monitor
   This program handles the protocol involved when reloading a
   PLC program in a runtime node.

      To initiate program reload, this program is started with
      the command:

         $ pcm [-k] [-d] (-g|-h|-w|-c|-n) volume version PLC-version
         
         where
              -k     Keep intermediate .diff-files
              -d     Load new PLC with debug
              -g     Glowing hot restart requested
              -h     Hot restart requested
              -w     Warm restart requested
              -c     Cold start requested (reboot)
              -n     Node to read difffile from
          volume     Name of the root volume representing the
                     node to be restarted.
         version     volume version number to change to
     PLC-version     PLC program version to change to
				
   To handle loading, this program communicates via a DECnet
   link on the ELN-machine. The named DECnet object "PCMSRC"
   is used when establish this link and the object must be
   declared by the ELN-machine.
		  
   Only one user can reload a program at one given time so
   all others will have to wait until the previous reload
   is finished.  */		  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <descrip.h>
#include <syidef.h>
#include <starlet.h>
#include <unixio.h>
#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "wb_ldhld.h"

#include "rt_pcm.h"

/* Local datastructures  */
struct item_list {
    short  item_length;			/* Item buffer length */
    short  item_code;			/* Item code */
    pwr_tAddress item_address;		/* Item buffer address */
    short *item_reslen;			/* Item resultant length */
    };

struct sparse
{
  int kflag;	/* Keep diff file */
  int dflag;	/* Load new PLC with debug */
  int nflag;	/* Node name to ldplcdiff */
  char nodename[20];
  char nodeobject[FILENAMESIZE];
  int version;
};

/******************************************************************************
*
*     NAME: usage
*
* DESCRIPT: Give errormessage for illegal command
*
* ARGUMENT:		IOGF 
*  char *mess		I	Additional errormessage
*	   
*   RETURN: ----
*	   
* CREATION DATE: 910620
*
* MODIFICATION HISTORY:
*
******************************************************************************/

void usage(char *mes)
{
  printf("pwr_pcm %s\n", mes);
  printf("Usage: pwr_pcm [-k] [-d] [-n node] volume-name volume-version PLC-version.\n");
  printf("  where:\n");
  printf("     volume-name     is the name or volume identity of the root volume to change.\n");
  printf("     volume-version  is the version of the volume you want to change to\n");
  printf("     PLC-version     is the version of the PLC-program you want to change to\n");
  exit(1);
}

/******************************************************************************
*
*     NAME: decode_params
*
* DESCRIPT: Decode input parameters and check for errors
*
* ARGUMENT:		IOGF 
*  int argc		I	Main program argument count
*  char *argv[]		I	Main program arguments
*  struct sparse *parse IO	Stucture to get parsed result
*	   
*   RETURN: ----
*	   
* CREATION DATE: 910620
*
* MODIFICATION HISTORY:
*
******************************************************************************/

decode_params( argc, argv, parse )
int argc;
char *argv[];
struct sparse *parse;
{
  char c;


  memset( parse, 0, sizeof( *parse ) );

  while (--argc > 0 && (*++argv)[0] == '-')
  {
    while (c = *++argv[0])
    {
      switch (c)
      {
	case 'k':
	  ++parse->kflag;
	  break;
	case 'd':
	  ++parse->dflag;
	  break;
	case 'n':
	  ++parse->nflag;
	  strcpy(parse->nodename, *++argv);
	  {
	  char *c;
	  for(c = parse->nodename; *c != '\0'; c++)
	    *c = _toupper(*c);
	  }	  
	  *(argv[0] + 1) = '\0';
	  break;
        default:
	  usage("Unknown flag");
      }
    }
  }

  if (argc < 1)
    usage("No volume name given");

  if (argc < 2)
    usage("No volume version given");

  if (argc < 3)
    usage("No PLC program version given");

  strcpy( &parse->nodeobject[0], *argv++ );

  if ( sscanf( *argv, "%d", &parse->version ) != 1)
    usage("Error in version number");

}

/******************************************************************************
*
*     NAME: main
*
* DESCRIPT: PCM main program.
*	    The program flow is as follows:
*
*	    Decode parameters, exit if error
*	    Produce the diff-file.
*	    Get the name of the node, the diff-file and the plcprogram.
*	    Attach to the PCM-server via object PCMSRC
*	    Send message to reload PLC
*	    Receive loop for return messages
*	    Close link
*	    Conditionally delete the diff-file
*
*
* ARGUMENT:		IOGF 
*  int argc		I	argument count
*  char *argv[]		I	arguments
*	   
*   RETURN: ----
*	   
* CREATION DATE: 910620
*
* MODIFICATION HISTORY:
*
******************************************************************************/

main(argc, argv)
int argc;
char *argv[];
{
  FILE *lf, *of;
  struct sparse parse;
  int	cnt;
  char	nodename[16];
  char	nodespec[256];
  char	pcmobject[FILENAMESIZE];
  char	diffile[FILENAMESIZE];
  char	plcfile[FILENAMESIZE];
  char	sendbuf[256];
  char	recbuf[256];
  int	type;
  int	version;
  int	status;
  char	text[256];
  pwr_tUInt32 iosb [2];
static short voidlen;
static unsigned int Area;
static unsigned int Number;
  struct dsc$descriptor_s d_node = {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0};
  struct item_list itmlst [] = {
			{sizeof Area,		/* Node area */
			 SYI$_NODE_AREA,
			 &Area,
			 &voidlen},
			{sizeof Number,		/* Node number */
			 SYI$_NODE_NUMBER,
			 &Number,
			 &voidlen},
			{0, 0, NULL, NULL}};	/* Terminator */

  decode_params( argc, argv, &parse );

  /*
   * Create the diff-file and
   * Get the name of the node, the diff-file and the plcprogram.
   */
  status = ldhld_Diff(&parse.nodeobject[0], parse.version, &diffile[0], &nodename[0]);
  if (EVEN(status)) {
    printf("Error creating diff-file\n");
    exit( status );
  }

  /* Process name of node where ldplcdiff file resides. */

  d_node.dsc$w_length = strlen(parse.nodename);
  d_node.dsc$a_pointer = parse.nodename;
  
  if (!parse.nflag) {
    strcpy(nodespec, "*");
  } else {
    status = sys$getsyiw (0, NULL, &d_node, itmlst, iosb, NULL, 0);
    if (ODD(status))
      if (EVEN(iosb[0])) status = iosb [0];
    if (ODD(status))
      sprintf(nodespec, "%d.%d", Area, Number);
    else
      strcpy(nodespec, parse.nodename);
    }

  /* Attach to the PCM-server via object PCMSRV */
  sprintf(pcmobject, "%s::\"TASK=PCMSRV\"", nodename);
  of = fopen(pcmobject, "r+");
  if (of == NULL) {
    sprintf(sendbuf, "Error attaching object, %s\n", pcmobject);
    perror(sendbuf);
    exit(1);
  }  

  /*
   * Send load message
   * Receive loop for return messages
   * Clean up and exit
   */


  /* Send message to reload PLC */
  if (!parse.dflag) {
    sprintf(sendbuf, "%d %d %s %s\n",
      C_PCM_LOAD, C_PCM_VERSION, diffile, nodespec );
  } else {
    sprintf(sendbuf, "%d %d %s %s\n",
      C_PCM_LOADDBG, C_PCM_VERSION, diffile, nodespec );

  }

  if (fputs(sendbuf, of) == EOF) {
    perror("Error sending load message");
    exit(1);
  }  

  while (1) {	/* Receive loop for return messages */

    if (fgets(recbuf, sizeof(recbuf), of) == NULL) {
      if (feof(of)) {
	break;
      } else {
	perror("Error reading message");
	continue;
      }
    }

    /* Parse message */
    cnt = sscanf(recbuf, "%d %d %d %[^\n]", &type, &version, &status, &text);
    if (cnt < 2 ) {
      type = C_PCM_NOP;
      perror("Error in message:");
      printf(">> \"%s\"\n", recbuf);
      continue;
    }

    if (version != C_PCM_VERSION) {
      printf("!! Wrong version of PCM-protocol: %d != %d\n", version, C_PCM_VERSION);
      continue;
    }

    if (type == C_PCM_DONE)
      break;

    if (type == C_PCM_MSG )
      printf("-- %s\n", text );
  }

  /* close the link to PCMSRV */
  fclose(of);

  if (type == C_PCM_DONE) {
    printf("PLC reload done\n");
  } else {
    printf("PLC reload failed, %d\n", status);
  }

  /* Conditionally delete the diff-file option -k(eep) or LOAD_FAIL */
  if (!parse.kflag)
    delete(diffile);
}

/******************************************************************************
*
*     NAME: findnode
*
* DESCRIPT: Routine not used
*
*
* ARGUMENT:		IOGF 
*  int *nodename	I	Name of node to search
*	   
*   RETURN: ----
*	   
* CREATION DATE: 910620
*
* MODIFICATION HISTORY:
*
******************************************************************************/

findnode( nodename )
char *nodename;
{
  pwr_tInt32		status;
  pwr_sNode	*node;
  char		objname[512];
  char		foundname[512];
  pwr_tObjid		NodeId;
  int		i;

/*
 * Try every possible node index for the wanted node
 */
  for ( i = 1; i < 256; ++i)
  {
    status = gdh_GetNodeObject( i, &NodeId );
    if (EVEN(status)) continue;

    status = gdh_ObjidToName( NodeId, objname, sizeof(objname) , cdh_mNName);
    if (EVEN(status)) continue;

    strcat( objname, ".NodeName" );
    status = gdh_GetObjectInfo(objname, foundname, sizeof(foundname) );
    if (EVEN(status)) continue;

    if (strcmp( foundname, nodename) == 0)
      return( TRUE );
  }

  return( FALSE );
}
