/* rt_rtt_dir.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

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

