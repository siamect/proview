/* 
 * Proview   $Id: msg2cmsg.h,v 1.2 2005-09-01 14:58:00 claes Exp $
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

#ifndef co_msg2cmsg_h
#define co_msg2cmsg_h

void lex_FacName(const char *Facnam);
void lex_FacNum(int Facnum);
void lex_FacPrefix(const char *Prefix);
void lex_MsgName(const char *Name);
void lex_MsgText(const char *Text);
void lex_MsgSeverity(msg_eSeverity Severity);
void lex_LexError(int Lineno, char *Str);

#endif
