/*  wb_note.h -- load and dump the notes database */

pwr_tStatus note_dump(
  ldh_tSesContext Session,
  char		  *filename
);

pwr_tStatus node_load(
	  ldh_tSesContext 	Session,
	  char			*loadfile,
	  char			*listfile,
	  int			oflag,
	  int			iflag
);
