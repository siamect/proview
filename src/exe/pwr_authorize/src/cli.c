/* cli.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clidef.h>
#include <climsgdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <lib$routines.h>
#include "wb_foe_macros.h"

#include <Xm/Xm.h>

#include <processes.h>
#include <smgdef.h>
#include <trmdef.h>

#include "pwr.h"
#include "pwr_class.h"
#include "wb_login_msg.h"
#include "wb_foe_show.h"
#include "wb_login.h"

static login_t_userptr	userlist = NULL; 

pwr_tStatus login_cli_help ( )
{
  char command[120] ;

  strcpy ( command , "help/libra=pwr_lib:wb_foe pwr_au" ) ;
  system ( command );   
  return LOGIN__SUCCESS;
}

void login_cli_quit () 
{
  /* Quit */
  login_db_close_userlist( userlist);
  exit( LOGIN__SUCCESS);
}
void login_cli_exit () 
{ 
  pwr_tStatus	sts;

  /* Save the session and quit */
  sts = login_db_save_userlist( userlist);
  login_db_close_userlist( userlist);
  exit( sts);
}

pwr_tStatus login_cli_save ()
{ 
  pwr_tStatus sts;

  sts = login_db_save_userlist( userlist);
  if (EVEN(sts))
    printf( "Unable to open file");

  return LOGIN__SUCCESS;
}


pwr_tStatus login_cli_add ()
{
  char		usernamestr[80] ;
  char		systemgroupstr[80] ;
  char		passwordstr[80] ;
  char		privstr[80] ;
  char		*usernamestr_p;
  char		*systemgroupstr_p;
  char		*passwordstr_p;
  char		*privstr_p;
  unsigned long	priv;
  pwr_tStatus	sts ;
  int		nr;
  unsigned long	priv_add;
  unsigned long priv_remove;
  pwr_tStatus	condval;

   priv_add = 0;
   priv_remove = 0;

   if ( foe_get_qual ( "allpwr" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mAccess_AllPwr;
     else
       priv_add |= pwr_mAccess_AllPwr;
   }
   if ( foe_get_qual ( "rtread" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_RtRead;
     else
       priv_add |= pwr_mPrv_RtRead;
   }
   if ( foe_get_qual ( "rtwrite" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_RtWrite;
     else
       priv_add |= pwr_mPrv_RtWrite;
   }
   if ( foe_get_qual ( "devread" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevRead;
     else
       priv_add |= pwr_mPrv_DevRead;
   }
   if ( foe_get_qual ( "devplc" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevPlc;
     else
       priv_add |= pwr_mPrv_DevPlc;
   }
   if ( foe_get_qual ( "devconfig" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevConfig;
     else
       priv_add |= pwr_mPrv_DevConfig;
   }
   if ( foe_get_qual ( "devclass" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevClass;
     else
       priv_add |= pwr_mPrv_DevClass;
   }

  if ( foe_get_qualval ( "username" , usernamestr, 80))
    usernamestr_p = usernamestr;
  else
    usernamestr_p = NULL;
  if ( foe_get_qualval ( "systemgroup" , systemgroupstr, 80))
    systemgroupstr_p = systemgroupstr;
  else
    systemgroupstr_p = NULL;
  if ( foe_get_qualval ( "password" , passwordstr, 80))
    passwordstr_p = passwordstr;
  else
    passwordstr_p = NULL;
  if ( foe_get_qualval ( "maskpriv" , privstr, 80))
  {
    /* Convert to int */
    nr = sscanf( privstr, "%d", &priv);
    if ( nr != 1)
    {
      printf("Priv sytax error\n");
      return LOGIN__SUCCESS;	
    }
  }
  else
    priv = 0;

  sts = login_db_open_userlist( &userlist);

  sts = login_db_user_add( &userlist, usernamestr_p, systemgroupstr_p, 
      passwordstr_p, priv, priv_add, priv_remove);
  if ( EVEN(sts))
    printf("User already exists\n");

  return LOGIN__SUCCESS;
}




pwr_tStatus login_cli_modify ()
{
  char		usernamestr[80] ;
  char		systemgroupstr[80] ;
  char		passwordstr[80] ;
  char		privstr[80] ;
  char		*usernamestr_p;
  char		*systemgroupstr_p;
  char		*passwordstr_p;
  unsigned long	*priv_p;
  unsigned long	priv;
  pwr_tStatus	sts ;
  int		nr;
  unsigned long	priv_add;
  unsigned long priv_remove;
  pwr_tStatus	condval;

   priv_add = 0;
   priv_remove = 0;

   if ( foe_get_qual ( "allpwr" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mAccess_AllPwr;
     else
       priv_add |= pwr_mAccess_AllPwr;
   }
   if ( foe_get_qual ( "rtread" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_RtRead;
     else
       priv_add |= pwr_mPrv_RtRead;
   }
   if ( foe_get_qual ( "rtwrite" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_RtWrite;
     else
       priv_add |= pwr_mPrv_RtWrite;
   }
   if ( foe_get_qual ( "devread" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevRead;
     else
       priv_add |= pwr_mPrv_DevRead;
   }
   if ( foe_get_qual ( "devplc" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevPlc;
     else
       priv_add |= pwr_mPrv_DevPlc;
   }
   if ( foe_get_qual ( "devconfig" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevConfig;
     else
       priv_add |= pwr_mPrv_DevConfig;
   }
   if ( foe_get_qual ( "devclass" , &condval ))
   {
     if (condval == CLI$_NEGATED)
       priv_remove |= pwr_mPrv_DevClass;
     else
       priv_add |= pwr_mPrv_DevClass;
   }

  if ( foe_get_qualval ( "username" , usernamestr, 80))
    usernamestr_p = usernamestr;
  else
    usernamestr_p = NULL;
  if ( foe_get_qualval ( "systemgroup" , systemgroupstr, 80))
    systemgroupstr_p = systemgroupstr;
  else
    systemgroupstr_p = NULL;
  if ( foe_get_qualval ( "password" , passwordstr, 80))
    passwordstr_p = passwordstr;
  else
    passwordstr_p = NULL;
  if ( foe_get_qualval ( "maskpriv" , privstr, 80))
  {
    /* Convert to int */
    nr = sscanf( privstr, "%d", &priv);
    if ( nr != 1)
    {
      printf("Priv sytax error\n");
      return LOGIN__SUCCESS;	
    }
    priv_p = &priv;
  }
  else
    priv_p = NULL;

  sts = login_db_open_userlist( &userlist);

  sts = login_db_user_mod( userlist, usernamestr_p, systemgroupstr_p, 
      passwordstr_p, priv_p, priv_add, priv_remove);
  if ( EVEN(sts))
    printf("User not found\n");

  return LOGIN__SUCCESS;	
}


pwr_tStatus login_cli_remove()
{
  char		usernamestr[80] ;
  char		systemgroupstr[80] ;
  char		*usernamestr_p;
  char		*systemgroupstr_p;
  pwr_tStatus	sts ;

  if ( foe_get_qualval ( "username" , usernamestr, 80))
    usernamestr_p = usernamestr;
  else
    usernamestr_p = NULL;
  if ( foe_get_qualval ( "systemgroup" , systemgroupstr, 80))
    systemgroupstr_p = systemgroupstr;
  else
    systemgroupstr_p = NULL;

  sts = login_db_open_userlist( &userlist);

  sts = login_user_db_remove( userlist, usernamestr_p, systemgroupstr_p);
  if ( EVEN(sts))
    printf("User not found\n");

  return LOGIN__SUCCESS;	
}



pwr_tStatus login_cli_show ()
{
  char		usernamestr[80] ;
  char		systemgroupstr[80] ;
  char		passwordstr[80] ;
  char		privstr[80] ;
  char		*usernamestr_p;
  char		*systemgroupstr_p;
  char		*passwordstr_p;
  char		*privstr_p;
  unsigned long	priv;
  pwr_tStatus	sts ;
  int		nr;
  pwr_tStatus	condval;
  int		showpassword;

  showpassword = foe_get_qual ( "__password" , &condval );

  if ( foe_get_qual ( "all" , &condval ))
  {    
    sts = login_db_open_userlist( &userlist);
    sts = login_db_show_userlist( userlist, showpassword);
    return sts;
  }
  else
  {
    if ( foe_get_qualval ( "username" , usernamestr, 80))
      usernamestr_p = usernamestr;
    else
      usernamestr_p = NULL;
    if ( foe_get_qualval ( "systemgroup" , systemgroupstr, 80))
      systemgroupstr_p = systemgroupstr;
    else
      systemgroupstr_p = NULL;

    sts = login_db_open_userlist( &userlist);

    sts = login_db_show_user( userlist, usernamestr_p, systemgroupstr_p, 
		showpassword);
    return sts;
  }
}
