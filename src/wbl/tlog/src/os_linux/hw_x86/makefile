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

copy : $(inc_dir)/pwr_tlogclasses.h

lib  : $(load_dir)/tlog.dbs

exe : $(doc_dir)/tlog_allclasses.html \
      $(exe_dir)/tlog_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(load_dir)/tlog.dbs :
	@ echo "Generating snapshot for tlog"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/tlog/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_tlogclasses.h :
	@ echo "Generating struct files for tlog classes..."
	@ co_convert -sv -d $(inc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"

$(doc_dir)/tlog_allclasses.html :
	@ echo "Generating html files for tlog classes..."
	@ co_convert -w -d $(doc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"
	@ echo "Generating html files for tlog structs..."
	@ co_convert -c -d $(doc_dir) $(inc_dir)/pwr_tlogclasses.h
	@ echo "Generating html files for tlog code..."
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_macro_tlog.h"
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_tlog*.c"

$(exe_dir)/tlog_xtthelp.dat :
	@ echo "Generating xtt help files for tlog classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/tlog.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/tlog.dbs; \
	  fi
