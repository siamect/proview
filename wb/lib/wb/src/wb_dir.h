/* 
 * Proview   $Id: wb_dir.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

/* wb_dir.h -- Directory information
   This module returns information about directory and files.  */

pwr_tStatus dir_parse_filename( char	*filename,
				char	*dev,
				char	*dir,
				char	*file,
				char	*type,
				int	*version);

pwr_tStatus dir_get_fileinfo( 	char 	*file_name,
                         	pwr_tTime *time2_ptr,
			 	int 	*size_p,
				int 	*vmsvers_p,
				char	*found_file);

char *dir_TimeString( 
  pwr_tTime	*time,
  char		*timestr
);

pwr_tStatus dir_CopyFile( 
	char	*from,
	char	*to
);

pwr_tStatus dir_PurgeFile( 
	char	*filename,
	int	keep
);

pwr_tStatus dir_DeleteFile( 
	char	*filename
);

pwr_tStatus dir_DefineLogical( 
	char	*name,
	char	*value,
	char	*table
);

pwr_tStatus dir_DeassignLogical( 
	char	*name,
	char	*table
);
