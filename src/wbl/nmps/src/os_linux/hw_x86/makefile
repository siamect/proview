#  makefile -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/rules.mk
endif



.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_nmpsclasses.h

lib  : $(load_dir)/nmps.dbs

exe : $(doc_dir)/orm/nmps_allclasses.html \
      $(exe_dir)/nmps_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(load_dir)/nmps.dbs :
	@ echo "Generating snapshot for nmps"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/nmps/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_nmpsclasses.h :
	@ echo "Generating struct files for nmps classes..."
	@ co_convert -s -d $(inc_dir) "$(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load"

$(doc_dir)/orm/nmps_allclasses.html :
	@ echo "Generating html files for nmps classes..."
	@ co_convert -w -d $(doc_dir)/orm -g $(pwre_sroot)/wbl/nmps/src/cnv_setup.dat "$(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load"
	@ echo "Generating html files for nmps structs..."
	@ co_convert -c -d $(doc_dir)/orm $(inc_dir)/pwr_nmpsclasses.h
	@ echo "Generating html files for nmps code..."
	@ co_convert -c -d $(doc_dir)/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_nmps.h"
	@ co_convert -c -d $(doc_dir)/orm "$(pwre_sroot)/lib/rs/src/rs_plc_*.c"

$(exe_dir)/nmps_xtthelp.dat :
	@ echo "Generating xtt help files for nmps classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/nmps.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/nmps.dbs; \
	  fi

