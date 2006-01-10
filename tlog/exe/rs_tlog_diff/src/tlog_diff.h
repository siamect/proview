/* 
 * Proview   $Id: tlog_diff.h,v 1.1 2006-01-10 14:38:36 claes Exp $
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

/************************************************************************
*
* Filename:		rs_tlog_diff.h
*			Date	Pgm.	Read.	Remark
* Modified		950914	CS		Initial creation
*
* Description:
*	Include file for Test Logging difference function.
*
**************************************************************************/


pwr_tStatus	tlog_diff( 	char *filename, 
				char *output, 
				char *timestr, 
				int parallell, 
				int attribute, 
				int text, 
				int ttext,
				int noorder, 
				int exact,
				char *since_str,
				char *before_str);

int	tlog_save( char *filename);

