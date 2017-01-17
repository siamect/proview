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



.PHONY : all init copy lib exe dirs clean realclean\
         $(clean_wbl)

all : init copy lib exe | silent

init : dirs | silent

copy : $(inc_dir)/pwr_remoteclasses.h \
       $(inc_dir)/pwr_remoteclasses.hpp \
       | silent

lib  : $(load_dir)/remote.dbs | silent

exe : $(doc_dir)/en_us/orm/remote_allclasses.html \
      $(exe_dir)/en_us/remote_xtthelp.dat \
      $(exe_dir)/sv_se/remote_xtthelp.dat \
      | silent

dirs : $(bld_dir)$(dir_ext)

clean : 

realclean : clean $(clean_wbl)

silent :
	@ :

$(load_dir)/remote.dbs :
	@ echo "Generating loadfile for remote"
	export pwr_load=$(pwr_eload);\
	  wb_cmd -q -i create snapshot $(wblflags) /file=\"$(pwre_sroot)/wbl/remote/src/\"/out=\"$(target)\"/depend=\"$(bld_dir)/remote.d_wbl\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_remoteclasses.h :
	@ echo "Generating struct files for remote classes..."
	@ $(co_convert) -s -d $(inc_dir) -y $(bld_dir)/remote.d_h_wbl "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(inc_dir)/pwr_remoteclasses.hpp :
	@ echo "Generating hpp files for remote classes..."
	@ $(co_convert) -po -d $(inc_dir) -y $(bld_dir)/remote.d_hpp_wbl "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(doc_dir)/en_us/orm/remote_allclasses.html :
	@ echo "Generating html files for remote classes en_us..."
	@ $(co_convert) -w -d $(doc_dir)/en_us/orm -y $(bld_dir)/remote.d_html_wbl -g $(pwre_sroot)/wbl/remote/src/cnv_setup.dat "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ echo "Generating html files for remote structs en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_remoteclasses.h
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_remoteclasses.hpp
	@ echo "Generating html files for remote code en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_remote.h"
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rs/src/rs_plc_*.c"
	@ echo "Generating html files for remote classes sv_se..."
	@ $(co_convert) -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/remote/src/cnv_setup.dat "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ echo "Generating html files for remote structs sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_remoteclasses.h
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_remoteclasses.hpp
	@ echo "Generating html files for remote code sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_macro_remote.h"
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rs/src/rs_plc_*.c"
	@ echo "Generating postscript file for remote classes..."
	@ $(co_convert) -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"
	@ $(co_convert) -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/remote/src/remote_*.wb_load"

$(exe_dir)/en_us/remote_xtthelp.dat :
	@ echo "Generating xtt help files for remote classes"
	@ $(co_convert) -x -d $(exe_dir)/en_us -y $(bld_dir)/remote.d_en_us_xtthelp_wbl "$(pwre_sroot)/wbl/remote/src/remote_c_*.wb_load"

$(exe_dir)/sv_se/remote_xtthelp.dat :
	@ echo "Generating xtt help files for remote classes"
	@ $(co_convert) -x -l sv_se -d $(exe_dir)/sv_se -y $(bld_dir)/remote.d_sv_se_xtthelp_wbl "$(pwre_sroot)/wbl/remote/src/remote_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/remote.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/remote.dbs; \
	  fi

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)

-include $(bld_dir)/remote.d_h_wbl
-include $(bld_dir)/remote.d_hpp_wbl
-include $(bld_dir)/remote.d_html_wbl
-include $(bld_dir)/remote.d_en_us_xtthelp_wbl
-include $(bld_dir)/remote.d_sv_se_xtthelp_wbl
-include $(bld_dir)/remote.d_wbl

