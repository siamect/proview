/* 
 * Proview   $Id: cnv_wblto.cpp,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#include <iostream>
#include <fstream>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "cnv_wblto.h"

using namespace std;

int CnvWblTo::init( char *) 
{ 
  return 1;
}
