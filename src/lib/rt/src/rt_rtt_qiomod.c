/* 
 * Proview   $Id: rt_rtt_qiomod.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_rtt_qiomod.c 
   This module contains routines for terminal read and write */

/*_Include files_________________________________________________________*/

#if defined(OS_ELN)
# include stdio
# include chfdef
# include string
# include starlet
# include iodef   
# include descrip
# include $vaxelnc
# include $elnmsg
# include $kernelmsg
# include $lat_utility
# include $dda_utility

# define STDIN_IDX  1
# define STDOUT_IDX 2
# define STDERR_IDX 3
#elif defined(OS_VMS)
# include <stdio.h>
# include <chfdef.h>
# include <string.h>
# include <starlet.h>
# include <iodef.h>   
# include <descrip.h>
# include <latdef.h>   
# include <ssdef.h>
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
# include <stdio.h>
# include <string.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/file.h>
# include <unistd.h>
# include <fcntl.h>
#endif

#define RTT_QIO_RETRY 10


#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_rtt.h"
#include "rt_rtt_global.h"
#include "rt_rtt_msg.h"
#include "rt_rtt_functions.h"

/***********************  D E F I N E ' S *******************************/
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
/***********************  T Y P E D E F ' S *****************************/
/***********************  G L O B A L   V A R I A B L E S ***************/
/***********************  L O C A L   V A R I A B L E S *****************/

/*__Local function prototypes_________________________________________*/

#if defined(OS_VMS) || defined(OS_ELN)
static int qio_ltconnect( int *chn);
static int qio_ltdisconnect( int *chn);
#endif

/************************************************************************
*
* Name:	qio_assign(char *s, int *chn)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* char		*s		     I	    String
* int		*chn		 O	    Kanal
*
* Description:	Gör en assign av s till kanalnummer chn
*************************************************************************/
int qio_assign( char *s, int *chn)
#ifdef OS_VMS
{
	/* Deklarationer som behövs i VMS */
	static $DESCRIPTOR(device,"");
	static char *stin = "SYS$OUTPUT:";
	char   *devn;

	/* Om input ska vara stdin, sätt stdinput */
	if ( strcmp(s,"stdin") == 0) 
		devn = stin; 
	else
		devn = s;

	device.dsc$a_pointer = devn;
	device.dsc$w_length  = strlen(devn);

	return sys$assign(&device, chn,0,0);
}
#elif  OS_ELN
{
	/* Deklarationer som behävs vid ELN */
    static	 $DESCRIPTOR(device,"");
    static	 char *stdin = "CONSOLE$";
    static	 char *stdinsufx = "ACCESS";
    int	    	nbbyte,bytebuff;
    unsigned char	typaheadbuf[200];

    DDA$_EXTENDED_STATUS_INFO	ext_sts;

    VARYING_STRING(255)	varyingstdin;
	static   char devn[40];
	static   unsigned int sts;
	static   PORT tmp;
	static   char *stp;

	/* Om stdin , ta eln-argumentet för detta */
	if ( strcmp(s,"stdin") == 0)

	   /* Kolla om programmet startades med parametrar enl. ELN */
		if (eln$program_argument_count() != 0) {
			eln$program_argument( &varyingstdin, STDIN_IDX);
			VARYING_TO_CSTRING(varyingstdin, stdin);
		}
			else return 0;
	else
		/* Inte stdin, ta s */
		stdin = s;

	/* Gör om "stdin:" till "stdin$ACCESS" */
	strcpy(devn,stdin);
	strcat(devn,stdinsufx);
	if ( (stp = strchr(devn,':')) != NULL)
		 *stp = '$';
	else
		if ((stp = strchr(devn,'$')) == NULL)
			return 0;

    device.dsc$a_pointer = devn;
    device.dsc$w_length  = strlen(devn);

    ker$create_port( &sts, (PORT *) chn, NULL);
	if ( sts ) 
	   ker$translate_name( &sts, &tmp, &device, NAME$LOCAL);
	   if ( sts ) 
		  ker$connect_circuit( &sts, (PORT *) chn, NULL, &device, NULL, NULL, NULL);
    /* 
    sylvie's modifications the 12.12.91 :
    When the terminal to which you want to redirect input/output is connected
    to the rt vax via a terminal driver(DH driver) you may get a lot of unwanted
    characters at the first read request because the DH driver keeps the
    characters in a type ahead buffer .
    So before requesting the user for some characters empty the typeahead
    driver:
    */

    bytebuff = sizeof(typaheadbuf);
    do
    {
     
      eln$tty_read(&sts,	/* Status		     */
      		  (PORT *) chn,	/* DDA port 		     */
		  bytebuff,	/* Antal tecken att läsa     */
		  &typaheadbuf,	/* Meddelande buffer	     */
		  &nbbyte,	/* Antal lästa tecken	     */
		  1,		/* Läs option		     */
		  NULL,		/* Termineringsmask	     */
		  NULL,		/* Timeout 		     */
		  0,		/* Minsta antal läsbyte	     */
		  &ext_sts,	/* Extended status	     */
		  NULL,		/* Message objekt	     */
	          NULL);	/* Message pointer	     */

      if(!sts)return(sts);
    }while( nbbyte >= bytebuff );		
	return sts;
}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	int chan = -1;
  	int sts;

	if ( strcmp( s,"stdin") == 0)
	  chan = STDIN_FILENO; 
	else
	{
	  chan = open( s, O_RDWR | O_NOCTTY);
  	  if ( chan == -1)
	  {
            printf( "No such device\n");
            return 0;
          }
	}
        sts = qio_set_attr( &chan);
	*chn = chan;
	return 1;
}
#endif




/************************************************************************
*
* Name:	qio_set_attr(int *chn)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* char		*s		     I	    String
* int		*chn		 O	    Kanal
*
* Description:	Set rtt attributes to a tty
*************************************************************************/
int qio_set_attr( int *chn)
#if defined(OS_VMS) || defined(OS_ELN)
{
	return RTT__SUCCESS;
}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	int chan;
  	int sts;
  	struct termios t;

	chan = *chn; 

  	sts = tcgetattr( chan, &t);
	if ( sts != 0) return 0;

	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = 10;
	t.c_lflag &= ~ICANON;
	t.c_lflag &= ~ECHO;
	/* t.c_iflag ...*/
 	sts = tcflush( chan, TCIFLUSH);
	sts = tcsetattr( chan, TCSAFLUSH, &t);  

	return 1;
}
#endif

/************************************************************************
*
* Name:	qio_reset(char *s, int *chn)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* char		*s		     I	    String
* int		*chn		 O	    Kanal
*
* Description:	Reset the channel before exit
*************************************************************************/
int qio_reset( int *chn)
#if defined(OS_VMS) || defined(OS_ELN)
{
	return RTT__SUCCESS;
}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	int chan;
  	int sts;
  	struct termios t;

	chan = *chn; 

  	sts = tcgetattr( chan, &t);
	if ( sts != 0) return 0;

	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = 0;
	t.c_lflag |= ICANON;
	t.c_lflag |= ECHO;
	/* t.c_iflag ...*/
 	sts = tcflush( chan, TCIFLUSH);
	sts = tcsetattr( chan, TCSAFLUSH, &t);  

	return 1;
}
#endif

/************************************************************************
*
* Name:	qio_readw(int chn, char *buf, int len)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
* char		*buf         O      Läst buffer
* int		len         I       Antal tecken som får läsas
*
* Description:	Läser med qiow från chn till buf
*************************************************************************/
int qio_readw( int *chn, char *buf, int len)
#ifdef OS_VMS
{
	struct statusblk {
		short condval;
		short transcount;
		int   devstat;
		} stsblk;
	static unsigned int code;
	int	i;
	int 	sts;

	code  = IO$_READLBLK | IO$M_NOECHO | IO$M_NOFILTR;
	for ( i = 0; i < RTT_QIO_RETRY; i++)
	{
	  sts = sys$qiow( 1, *chn, code, &stsblk,0,0,buf,len,0,0,0,0);
	  if ( ODD(sts)) return sts;
	}
	rtt_exit_now(1, sts);
	return 1;
}
#elif  OS_ELN
{
	static int                       nob=1;
	static int                       opt=1;
	static unsigned int 			 sts;
	static DDA$BREAK_MASK            code={0,0,0,0,0,0,0,0};
	static DDA$_EXTENDED_STATUS_INFO stsblk;
	int	i;
	

	for ( i = 0; i < RTT_QIO_RETRY; i++)
	{
	  eln$tty_read( &sts, (PORT *) chn, 1, buf, &nob, opt, code, NULL,
	 	1, stsblk,NULL, NULL);
	  if (ODD(sts)) return sts;
	}

	/* even return status ten times */
/*	 if (sts == KER$_DISCONNECT) rtt_exit_now(1);
*/
	rtt_exit_now(1, sts);

}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	int n = 0;

	while( n == 0)
    	  n = read( *chn, buf, len);
	return 1;
}
#endif

/************************************************************************
*
* Name:	qio_read(int chn, int tmo, char *buf, int len)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
* int		tmo         I       Timout-tid
* char		*buf         O      Läst buffer
* int		len         I       Antal tecken som får läsas
*
* Description:	Läser med qio från chn till buf med timout-tid tmo (ms)
*************************************************************************/
int qio_read( int *chn, int tmo, char *buf, int len)
#ifdef OS_VMS
{
	struct statusblk {
		short condval;
		short transcount;
		int   devstat;
		} stsblk;
	static unsigned int code;
	static unsigned int sts;
	int	i;

	if ( tmo < 1000)
	  tmo = 1000;

	code  = IO$_READLBLK | IO$M_NOECHO | IO$M_NOFILTR | IO$M_TIMED;
	for ( i = 0; i < RTT_QIO_RETRY; i++)
	{
	  sts   = sys$qiow( 1, *chn, code, &stsblk,0,0,buf,len,tmo/1000,0,0,0);
          if ( stsblk.condval == SS$_TIMEOUT) return 0;
	  if (ODD(sts)) return 1;
	}
	rtt_exit_now(1, sts);
	return 1;

}
#elif  OS_ELN
{
    static LARGE_INTEGER	timout;
	static int                       nob=1;
	static int                       opt=2;
	static unsigned int 			 sts;
    static DDA$BREAK_MASK            code={0,0,0,0,0,0,0,0};
    static DDA$_EXTENDED_STATUS_INFO stsblk;
	int			i;

	
    if ( tmo < 1000)
      tmo = 1000;

    timout.low  = - (tmo * 10000);
    timout.high = -1;

	for ( i = 0; i < RTT_QIO_RETRY; i++)
	{
	  eln$tty_read( &sts, (PORT *) chn, 1, buf, &nob, opt, NULL, &timout,
 		1, stsblk, NULL, NULL);
	  if ( sts == ELN$_TIMEOUT ) return 0;
	  if (ODD(sts)) return 1;
	}


	rtt_exit_now( 1, sts);
	return 1;
}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	int n;

    	n = read( *chn, buf, len);
	if ( n == 0)
	  /* Timeout */
	  return 0;
	return 1;
}
#endif

/************************************************************************
*
* Name:	qio_ltconnect(int chn)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
*
* Description:	Väntar med qio på en LAT-service
*************************************************************************/
#ifdef OS_VMS
static int qio_ltconnect( int *chn)
{
struct statusblk {
	short condval;
	short transcount;
	int   devstat;
	} stsblk;
static unsigned int code;
static unsigned int sts;

	code = IO$_TTY_PORT | IO$M_LT_CONNECT; /* Vänta på LAT */
	sts   = sys$qiow( 0, *chn, code, &stsblk,0,0,0,0,0,0,0,0);
	return sts;
}
#elif OS_ELN
static int qio_ltconnect( int *chn)
{
static unsigned int sts;
	eln$lat_wait_for_connection( (PORT *) chn,&sts);
	return sts;
}
#endif

/************************************************************************
*
* Name:	qio_ltdisconnect(int chn)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
*
* Description:	Kopplar ned en LAT LAT-service
*************************************************************************/
#ifdef OS_VMS
static int qio_ltdisconnect( int *chn)
{
struct statusblk {
	short condval;
	short transcount;
	int   devstat;
	} stsblk;
static unsigned int code;
static unsigned int sts;

	code = IO$_TTY_PORT | IO$M_LT_DISCON; /* Koppla ned LAT */
	sts   = sys$qiow( 0, *chn, code, &stsblk,0,0,0,0,0,0,0,0);
	return sts;
}
#elif OS_ELN
static int qio_ltdisconnect( int *chn)
{
	return RTT__SUCCESS;
}
#endif

/************************************************************************
*
* Name:	qio_writew(int chn, char *buf, int len)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
* char		*buf        I       Buffer
* int		len         I       Antal tecken som ska skrivas
*
* Description:	Skriver med qiow från buf till chn
*************************************************************************/
int qio_writew( int *chn, char *buf, int len)
#ifdef OS_VMS
{
	struct statusblk {
		short condval;
		short transcount;
		int   devstat;
		} stsblk;
    static unsigned int code;

	code  = IO$_WRITELBLK | IO$M_NOECHO | IO$M_NOFILTR;
	return sys$qiow( 0, *chn, code, &stsblk,0,0,buf,len,0,0,0,0);

}
#elif  OS_ELN
{
	static int                       nob;
	static unsigned int 		 sts;

	while (len > 0)
	{
	  eln$tty_write(&sts,
			(PORT *) chn,
			len,
			buf,
			&nob,
			NULL,
			NULL);
	  if (sts == KER$_DISCONNECT || sts == KER$_NO_SUCH_PORT) rtt_exit_now(1, sts);
	  len -= nob;
	  buf += nob;
	}

	return sts;

}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	if ( *chn == STDIN_FILENO)
	  write( STDOUT_FILENO, buf, len);
	else
	  write( *chn, buf, len);
	return 1;
}
#endif

/************************************************************************
*
* Name:	qio_write(int chn, int tmo, char *buf, int len)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* int		chn         I       Kanal
* int		tmo         I       Timout-tid
* char		*buf         O      Läst buffer
* int		len         I       Antal tecken som får läsas
*
* Description:	Skriver med qio från buf till chn med timout-tid tmo (ms)
*************************************************************************/
int qio_write( int *chn, int tmo, char *buf, int len)
#ifdef OS_VMS
{
	struct statusblk {
		short condval;
		short transcount;
		int   devstat;
		} stsblk;
    static unsigned int code;
    static unsigned int sts;

	if ( tmo < 1000)
	  tmo = 1000;

	code  = IO$_WRITELBLK | IO$M_NOECHO | IO$M_NOFILTR | IO$M_TIMED;
	sts   = sys$qiow( 0, chn, code, &stsblk,0,0,buf,len,tmo/1000,0,0,0);
        return stsblk.condval == SS$_TIMEOUT ? 0 : 1;        

}
#elif  OS_ELN
{
    static LARGE_INTEGER	timout;
	static int                       nob;
	static int                       opt=2;
	static unsigned int 			 sts;
    static DDA$BREAK_MASK            code={0,0,0,0,0,0,0,0};
    static DDA$_EXTENDED_STATUS_INFO stsblk;

    if ( tmo < 1000)
      tmo = 1000;

    nob = len;	
    timout.low  = - (tmo * 10000);
    timout.high = -1;

	eln$tty_write(&sts, 
                  (PORT *) chn,
		  1,
                  buf,
                 &nob,
                  opt,
                 NULL,
              &timout,
                    1,
               stsblk,
                 NULL,
                 NULL);

	if (sts == KER$_DISCONNECT || sts == KER$_NO_SUCH_PORT) rtt_exit_now(1, sts);
	return sts == ELN$_TIMEOUT ? 0 : 1;

}
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
{
	if ( *chn == STDIN_FILENO)
	  write( STDOUT_FILENO, buf, len);
	else
	  write( *chn, buf, len);
	return 1;
}
#endif

