/* 
 * Proview   $Id: rt_io_methods.h,v 1.1 2005-12-30 15:45:31 claes Exp $
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

#ifndef rt_io_methods_h
#define rt_io_methods_h

/* rt_io_methods.h -- includefile for io methods. */

#ifndef pwr_h
#include "pwr.h"
#endif

typedef struct {
  pwr_tObjName MethodName;
  pwr_tStatus (*Method)();
} pwr_sMethodBinding;

typedef struct {
  pwr_tObjName ClassName;
  pwr_sMethodBinding (*Methods)[];
} pwr_sClassBinding;

/* Base methods */
#if defined (__DECC) || defined (OS_LYNX)  || defined (OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
# define pwr_BindIoMethods(Class) pwr_sMethodBinding pwr_g ## Class ## _IoMethods[]
# define pwr_BindIoClasses(Type) pwr_sClassBinding pwr_g ## Type ## _IoClassMethods[]
# define pwr_BindIoClass(Class) {#Class, (void *)pwr_g ## Class ## _IoMethods}
# define pwr_BindIoMethod(Method) {#Method, (pwr_tStatus (*)())Method}
#else
# define pwr_BindIoMethods(Class) pwr_sMethodBinding pwr_g/**/Class/**/_IoMethods[]
# define pwr_BindIoClasses(Type) pwr_sClassBinding pwr_g/**/Type/**/_IoClassMethods[]
# define pwr_BindIoClass(Class) {"Class", pwr_g/**/Class/**/_IoMethods}
# define pwr_BindIoMethod(Method) {"Method", (pwr_tStatus (*)())Method}
#endif

/* User methods */
#if defined (__DECC) || defined (OS_LYNX) || defined(OS_LINUX) || defined OS_MACOS || defined OS_FREEBSD
#define pwr_BindIoUserMethods(Class) pwr_sMethodBinding pwr_g ## Class ## _IoUserMethods[]
#define pwr_BindIoUserClasses(Type) pwr_sClassBinding pwr_g ## Type ## _IoUserClassMethods[]
#define pwr_BindIoUserClass(Class) {#Class, (void *)pwr_g ## Class ## _IoUserMethods}
#define pwr_BindIoUserMethod(Method) {#Method, (pwr_tStatus (*)())Method}
#else
#define pwr_BindIoUserMethods(Class) pwr_sMethodBinding pwr_g/**/Class/**/_IoUserMethods[]
#define pwr_BindIoUserClasses(Type) pwr_sClassBinding pwr_g/**/Type/**/_IoUserClassMethods[]
#define pwr_BindIoUserClass(Class) {"Class", pwr_g/**/Class/**/_IoUserMethods}
#define pwr_BindIoUserMethod(Method) {"Method", (pwr_tStatus (*)())Method}
#endif

#define pwr_NullMethod {"", NULL}

#define pwr_NullClass {"", NULL}

#endif

