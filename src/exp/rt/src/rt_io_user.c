/* rt_io_user.c -- dummy for user methods.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "pwr.h"
#include "rt_io_base.h"

/* Dummy for declaration of user methods, when no methods are defined 	*/

/* Example of declaration of user methods 				*/
/*									*/
/* 	pwr_dImport pwr_BindIoUserMethods($Node);			*/
/* 	pwr_dImport pwr_BindIoUserMethods(Co_PI24BO);			*/
/*									*/
/* 	pwr_BindIoUserClasses(User) = {					*/
/* 	  pwr_BindIoUserClass($Node),					*/
/* 	  pwr_BindIoUserClass(Co_PI24BO),				*/
/* 	  pwr_NullClass							*/
/* 	};								*/

pwr_dExport pwr_BindIoUserClasses(User) = {
  pwr_NullClass
};


