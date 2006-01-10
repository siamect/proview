/* 
 * Proview   $Id: rs_tlog_diff.c,v 1.1 2006-01-10 14:38:36 claes Exp $
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clidef.h>
#include <climsgdef.h>
#include <processes.h>
#include <smgdef.h>
#include <trmdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>

#include "pwr.h"
#include "pwr_class.h"

int cli$dcl_parse();
int cli$dispatch();




int *tlog_cld () ;    /* module name in tlog_cld.cld file */


/*************************************************************************
*
* Name:		info_msg()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	If the message is a error, warning or info message it is printed.
*
**************************************************************************/
void sutil_msg( unsigned long 	sts)
{
	static int msgsts;
	static int msglen;
	static char msg[256];
	struct dsc$descriptor_s	msgdesc = {sizeof(msg)-1,DSC$K_DTYPE_T,
					DSC$K_CLASS_S,};

	msgdesc.dsc$a_pointer = msg;

	if ( ( EVEN(sts)) || ((sts & 1) && (sts & 2)) )
	{
	  msgsts = sts;
	  lib$sys_getmsg(&msgsts, &msglen, &msgdesc, 0, 0);
	  msg[msglen]='\0';
	  printf("%s\n", msg);
	}
}

main(int argc, char *argv[])
{

  long		sts ;
  int		i;
  char 		str[256] ;
  int 		key_id ;

  unsigned short res_len ;
  $DESCRIPTOR ( device_desc , "sys$input" );
  $DESCRIPTOR ( outdevice_desc , "sys$output" );
  $DESCRIPTOR ( prompt_desc , "tlog> " );
  struct dsc$descriptor_s	str_desc = {sizeof(str)-1,DSC$K_DTYPE_T,
					DSC$K_CLASS_S,};

  str_desc.dsc$a_pointer = str;

  /* avanti */


  /* If arguments, treat them as a command and then exit */
  if ( argc >= 2 )
  {

    str[0] = 0;
    for ( i = 1; i < argc; i++)
    {
      if ( i != 1)
      strcat( str, " ");
      strcat( str, argv[i]);
    }
    sts = cli$dcl_parse ( &str_desc ,  tlog_cld, 0,  0 , 0  ) ;
    /* send the command to the rtn associated */ 
    if ( sts == CLI$_NORMAL ) 
      sts = cli$dispatch();  
    sutil_msg( sts);
    exit(sts);
  }


  /* init input */
        

  sts = smg$create_virtual_keyboard ( 
		&key_id , 
		&device_desc ,
 		0 ,0, 0 );


  while ( 1 )
  {


    /* get and parse the command */

    /* get input */

    sts = smg$read_composed_line ( 	
		&key_id , 0, &str_desc , &prompt_desc , &res_len);
    if ( res_len == 0)
      continue;

    if ( !(sts && 1 ) ) lib$signal (sts ) ;
    str[res_len] = 0 ;

    sts = cli$dcl_parse ( 
      &str_desc ,  tlog_cld, 0,  0 , 0  ) ;
    
    /* send the command to the rtn associated */ 
    if ( sts == CLI$_NORMAL ) 
    {	 
      sts = cli$dispatch ( ) ;  
      sutil_msg( sts);
    }
  }
}
