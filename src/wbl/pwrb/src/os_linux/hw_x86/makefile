#  makefile -- <short description>

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

copy : $(inc_dir)/pwr_baseclasses.h \
       $(inc_dir)/pwr_baseclasses.hpp


lib  : $(load_dir)/pwrb.dbs

exe : $(doc_dir)/en_us/orm/pwrb_allclasses.html \
      $(exe_dir)/en_us/pwrb_xtthelp.dat \
      $(exe_dir)/sv_se/pwrb_xtthelp.dat

clean : 

realclean : clean clean_pwrb_xtthelp clean_pwrb_html clean_baseclasses

$(load_dir)/pwrb.dbs :
	@ echo "Generating snapshot for pwrb"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/pwrb/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_baseclasses.h :
	@ echo "Generating struct files for pwrb classes..."
	@ co_convert -sv -d $(inc_dir) "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(inc_dir)/pwr_baseclasses.hpp :
	@ echo "Generating hpp files for pwrb classes..."
	@ co_convert -po -d $(inc_dir) "$(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load"

$(doc_dir)/en_us/orm/pwrb_allclasses.html :
	@ echo "Generating html files for pwrb classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/pwrb/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"
	@ echo "Generating html files for pwrb structs en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_baseclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_baseclasses.hpp
	@ echo "Generating html files for pwrb code en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rt/src/rt_plc_macro_*.h"
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rt/src/rt_plc_*.c"
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rt/src/rt_c_*.c"
	@ co_convert -c -d $(doc_dir)/en_us/orm "$(pwre_sroot)/lib/rt/src/rt_csup.c"
	@ echo "Generating html files for pwrb classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/pwrb/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"
	@ echo "Generating html files for pwrb structs sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_baseclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_baseclasses.hpp
	@ echo "Generating html files for pwrb code sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rt/src/rt_plc_macro_*.h"
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rt/src/rt_plc_*.c"
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rt/src/rt_c_*.c"
	@ co_convert -c -d $(doc_dir)/sv_se/orm "$(pwre_sroot)/lib/rt/src/rt_csup.c"
	@ echo "Generating postscript file for pwrb classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(exe_dir)/en_us/pwrb_xtthelp.dat :
	@ echo "Generating xtt help files for pwrb classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load"

$(exe_dir)/sv_se/pwrb_xtthelp.dat :
	@ echo "Generating xtt help files for pwrb classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_pwrb_xtthelp :
	@ if [ -e $(exe_dir)/en_us/pwrb_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/en_us/pwrb_xtthelp.dat; \
	  fi
clean_pwrb_html :
	@ if [ -e $(doc_dir)/orm/en_us/pwrb_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/orm/pwrb_*.html; \
	  fi

clean_baseclasses :
	@ if [ -e $(inc_dir)/pwr_baseclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_systemclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/pwrb_c_*.h; \
	  fi

clean_wbl :
	@ if [ -e $(load_dir)/pwrb.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/pwrb.dbs; \
	  fi

