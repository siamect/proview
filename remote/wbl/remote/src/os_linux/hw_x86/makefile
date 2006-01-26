#  makefile -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif



.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_remoteclasses.h \
       $(inc_dir)/pwr_remoteclasses.hpp

lib  : $(load_dir)/remote.dbs

exe : $(doc_dir)/en_us/orm/remote_allclasses.html \
      $(exe_dir)/remote_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(load_dir)/remote.dbs :
	@ echo "Generating loadfile for remote"
	@ export pwr_load=$(pwr_eload);\
	  wb_cmd -q -i create snapshot /file=\"$(pwre_sroot)/wbl/remote/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_remoteclasses.h :
	@ echo "Generating struct files for remote classes..."
	@ co_convert -s -d $(inc_dir) "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(inc_dir)/pwr_remoteclasses.hpp :
	@ echo "Generating hpp files for remote classes..."
	@ co_convert -po -d $(inc_dir) "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(doc_dir)/en_us/orm/remote_allclasses.html :
	@ echo "Generating html files for remote classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/remote/src/cnv_setup.dat "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ echo "Generating html files for remote structs en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_remoteclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_remoteclasses.hpp
	@ echo "Generating html files for remote code en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_remote.h"
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_*.c"
	@ echo "Generating html files for remote classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/remote/src/cnv_setup.dat "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ echo "Generating html files for remote structs sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_remoteclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_remoteclasses.hpp
	@ echo "Generating html files for remote code sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_remote.h"
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_*.c"
	@ echo "Generating postscript file for remote classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(exe_dir)/remote_xtthelp.dat :
	@ echo "Generating xtt help files for remote classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/remote/src/remote_c_*.wb_load"
	@ co_convert -x -l sv_se -d $(exe_dir) "$(pwre_sroot)/wbl/remote/src/remote_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/remote.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/remote.dbs; \
	  fi



