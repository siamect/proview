/* 
 * Proview   $Id: rt_rtt_dir.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#define RTT_DIR_SEARCH_NEXT 0
#define RTT_DIR_SEARCH_INIT 1
#define RTT_DIR_SEARCH_END 2


int rtt_search_file( 		char 	*file_name , 
				char	*found_file,
				int	new);

int rtt_parse_filename( 	char	*filename,
				char	*dev,
				char	*dir,
				char	*file,
				char	*type,
				int	*version);

