/* pwr_authorize.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <string.h>
#include <clidef.h>
#include <climsgdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <lib$routines.h>
#include <processes.h>
#include <smgdef.h>
#include <trmdef.h>

#include "pwr.h"
#include "pwr_class.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_foe.h"

int *pwr_authorize_cld();  /* module name in pwr_authorize_cld.cld */

extern pwr_tStatus smg$create_virtual_keyboard(); 
extern pwr_tStatus smg$read_composed_line(); 
extern pwr_tStatus cli$dcl_parse(); 
extern pwr_tStatus cli$dispatch(); 
main(int argc, char *argv[])
{

  pwr_tStatus	sts ;
  int		i;
  static char 	str[256] ;
  int 		key_id ;

  $DESCRIPTOR ( device_desc , "sys$input" );
  $DESCRIPTOR ( outdevice_desc , "sys$output" );
  $DESCRIPTOR ( str_desc , str );
  $DESCRIPTOR ( prompt_desc , "pwr_au> " );
  unsigned short res_len ;

  /* avanti */



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
      &str_desc ,  pwr_authorize_cld, 0,  0 , 0  ) ;
    
    /* send the command to the rtn associated */ 
    if ( sts == CLI$_NORMAL ) 
    {	 
      sts = cli$dispatch ( ) ;  
      info_msg( sts);
    }
  }
}
