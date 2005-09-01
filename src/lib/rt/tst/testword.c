/* 
 * Proview   $Id: testword.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

main ()
{

  union {
    int i;
    struct {
      char  c0;
      char  c1;
      char  c2;
      char  c3;
    } c;
    struct {
      unsigned int b0 : 4;
      unsigned int b1 : 4;
      unsigned int b2 : 8;
      unsigned int b3 : 8;
      unsigned int b4 : 8;
    } b;
    char vc[4];
    float f;
  } a, b;

  a.c.c0 = 1;
  a.c.c1 = 2;
  a.c.c2 = 3;
  a.c.c3 = 4;
    
  b.i = 123456789;
}
