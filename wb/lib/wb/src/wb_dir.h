/* wb_dir.h -- Directory information

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

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
