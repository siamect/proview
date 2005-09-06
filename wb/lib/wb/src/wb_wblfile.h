/* 
 * Proview   $Id: wb_wblfile.h,v 1.4 2005-09-06 10:43:32 claes Exp $
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
 **/

#ifndef wb_wblfile_h
#define wb_wblfile_h

#include <string>

// #include "antlr/AST.hpp"
// #include "wb_wbllexer.hpp"
// #include "wb_wblparser.hpp"

class wb_wblparser;
class wb_wbllexer;
class wb_wblnode;

class wb_wblfile {
public:
  wb_wbllexer *lexer;
  wb_wblparser *parser;
  wb_wblnode *rootAST;
  char file_name[200];
  pwr_tTime time;
};

#endif
