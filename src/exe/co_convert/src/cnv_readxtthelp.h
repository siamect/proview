/* 
 * Proview   $Id: cnv_readxtthelp.h,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#ifndef cnv_xtthelpread_h
#define cnv_xtthelpread_h

#include <fstream>

using namespace std;

#if defined __cplusplus
extern "C" {
#endif

class CnvXtthelpTo;

class CnvReadXtthelp {
  public:
    CnvReadXtthelp( char *x_name, char *x_directory,
		    CnvXtthelpTo *to);
    ofstream fp;
    char directory[80];
    char name[80];
    char filename[120];
    CnvXtthelpTo *xtthelpto;

    int read_xtthelp();
};


#if defined __cplusplus
}
#endif
#endif





