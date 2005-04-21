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

copy : $(inc_dir)/pwr_ssabclasses.h \
       $(inc_dir)/pwr_ssabclasses.hpp

lib  : $(load_dir)/ssab.dbs

exe : $(doc_dir)/en_us/orm/ssab_allclasses.html \
      $(exe_dir)/ssab_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(load_dir)/ssab.dbs :
	@ echo "Generating snapshot for ssab"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/ssab/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_ssabclasses.h :
	@ echo "Generating struct files for ssab classes..."
	@ co_convert -s -d $(inc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

$(inc_dir)/pwr_ssabclasses.hpp :
	@ echo "Generating hpp files for ssab classes..."
	@ co_convert -po -d $(inc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

$(doc_dir)/en_us/orm/ssab_allclasses.html :
	@ echo "Generating html files for ssab classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"
	@ echo "Generating html files for ssab structs en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_ssabclasses.h
	@ echo "Generating html files for ssab code en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_ssab.h"
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_ssab*.c"
	@ echo "Generating html files for ssab classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"
	@ echo "Generating html files for ssab structs sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_ssabclasses.h
	@ echo "Generating html files for ssab code sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_ssab.h"
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_ssab*.c"
	@ echo "Generating postscript file for ssab classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/ssab/src/ssab_*.wb_load"

$(exe_dir)/ssab_xtthelp.dat :
	@ echo "Generating xtt help files for ssab classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/ssab.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/ssab.dbs; \
	  fi
