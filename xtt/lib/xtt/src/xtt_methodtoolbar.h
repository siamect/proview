/* 
 * Proview   $Id: xtt_methodtoolbar.h,v 1.1 2007-01-17 06:19:57 claes Exp $
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

#ifndef xtt_methodtoolbar_h
#define xtt_methodtoolbar_h

class XNav;

typedef struct {
  char name[80];
  char method[80];
  char filter[80];
  char image[80];
  char tooltip[80];
} xtt_sMethodButton;

class XttMethodToolbar {
 public:
  static const int m_size = 14;
  static xtt_sMethodButton m_data[m_size];
  XNav *m_xnav;

  XttMethodToolbar( XNav *xnav) : m_xnav(xnav) {}
  virtual ~XttMethodToolbar() {}
  virtual void set_sensitive() {}
};

#endif
