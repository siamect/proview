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

all : init copy exe | silent

init : dirs | silent

copy : $(inc_dir)/pwr_nmpsclasses.h \
       $(inc_dir)/pwr_nmpsclasses.hpp \
       | silent

lib  : $(load_dir)/nmps.dbs | silent

exe : $(doc_dir)/en_us/orm/nmps_allclasses.html \
      $(exe_dir)/en_us/nmps_xtthelp.dat \
      $(exe_dir)/sv_se/nmps_xtthelp.dat \
      | silent

dirs : $(bld_dir)$(dir_ext)

clean : 

realclean : clean $(clean_wbl)

silent :
	@ :

$(load_dir)/nmps.dbs :
	@ echo "Generating loadfile for nmps"
	@ export pwr_load=$(pwr_eload);\
	  wb_cmd -q -i create snapshot $(wblflags) /file=\"$(pwre_sroot)/wbl/nmps/src/\"/out=\"$(target)\"/depend=\"$(bld_dir)/nmps.d_wbl\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_nmpsclasses.h :
	@ echo "Generating struct files for nmps classes..."
	@ $(co_convert) -so -d $(inc_dir) -y $(bld_dir)/nmps.d_h_wbl "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"

$(inc_dir)/pwr_nmpsclasses.hpp :
	@ echo "Generating hpp files for nmps classes..."
	@ $(co_convert) -po -d $(inc_dir) -y $(bld_dir)/nmps.d_hpp_wbl "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"

$(doc_dir)/en_us/orm/nmps_allclasses.html :
	@ echo "Generating html files for nmps classes en_us..."
	@ $(co_convert) -w -d $(doc_dir)/en_us/orm -y $(bld_dir)/nmps.d_html_wbl -g $(pwre_sroot)/wbl/nmps/src/cnv_setup.dat "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"
	@ echo "Generating html files for nmps structs en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_nmpsclasses.h
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_nmpsclasses.hpp
	@ echo "Generating html files for nmps code en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/nmps/src/nmps_plc_macro.h"
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/nmps/src/nmps_plc*.c"
	@ echo "Generating html files for nmps classes sv_se..."
	@ $(co_convert) -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/nmps/src/cnv_setup.dat "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"
	@ echo "Generating html files for nmps structs sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_nmpsclasses.h
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_nmpsclasses.hpp
	@ echo "Generating html files for nmps code sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/nmps/src/nmps_plc_macro.h"
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/nmps/src/nmps_plc*.c"
	@ echo "Generating postscript file for nmps classes..."
	@ $(co_convert) -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"
	@ $(co_convert) -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/nmps/src/nmps_*.wb_load"

$(exe_dir)/en_us/nmps_xtthelp.dat :
	@ echo "Generating xtt help files for nmps classes en_us"
	@ $(co_convert) -x -d $(exe_dir)/en_us -y $(bld_dir)/nmps.d_en_us_xtthelp_wbl "$(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load"

$(exe_dir)/sv_se/nmps_xtthelp.dat :
	@ echo "Generating xtt help files for nmps classes sv_se"
	@ $(co_convert) -x -l sv_se -d $(exe_dir)/sv_se -y $(bld_dir)/nmps.d_sv_se_xtthelp_wbl "$(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load"

clean_wbl :
	@ if [ -e $(load_dir)/nmps.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/nmps.dbs; \
	  fi

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)

-include $(bld_dir)/nmps.d_h_wbl
-include $(bld_dir)/nmps.d_hpp_wbl
-include $(bld_dir)/nmps.d_html_wbl
-include $(bld_dir)/nmps.d_en_us_xtthelp_wbl
-include $(bld_dir)/nmps.d_sv_se_xtthelp_wbl
-include $(bld_dir)/remote.d_wbl




