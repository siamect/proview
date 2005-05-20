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

copy : $(inc_dir)/pwr_basecomponentclasses.h \
       $(inc_dir)/pwr_basecomponentclasses.hpp


lib  : $(load_dir)/basecomponent.dbs

exe : $(doc_dir)/en_us/orm/basecomponent_allclasses.html \
      $(exe_dir)/en_us/basecomponent_xtthelp.dat \
      $(exe_dir)/sv_se/basecomponent_xtthelp.dat

clean : 

realclean : clean clean_basecomponent_xtthelp clean_basecomponent_html clean_basecomponentclasses

$(load_dir)/basecomponent.dbs :
	@ echo "Generating snapshot for basecomponent"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_basecomponentclasses.h :
	@ echo "Generating struct files for basecomponent classes..."
	@ co_convert -sv -d $(inc_dir) "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"

$(inc_dir)/pwr_basecomponentclasses.hpp :
	@ echo "Generating hpp files for basecomponent classes..."
	@ co_convert -po -d $(inc_dir) "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"

$(doc_dir)/en_us/orm/basecomponent_allclasses.html :
	@ echo "Generating html files for basecomponent classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/bcomp/src/cnv_setup.dat "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"
	@ echo "Generating html files for basecomponent structs en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_basecomponentclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_basecomponentclasses.hpp
	@ echo "Generating html files for basecomponent code en_us..."
	@ echo "Generating html files for basecomponent classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/bcomp/src/cnv_setup.dat "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"
	@ echo "Generating html files for basecomponent structs sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_basecomponentclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_basecomponentclasses.hpp
	@ echo "Generating html files for basecomponent code sv_se..."
	@ echo "Generating postscript file for basecomponent classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"

$(exe_dir)/en_us/basecomponent_xtthelp.dat :
	@ echo "Generating xtt help files for basecomponent classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"

$(exe_dir)/sv_se/basecomponent_xtthelp.dat :
	@ echo "Generating xtt help files for basecomponent classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(pwre_sroot)/wbl/bcomp/src/basecomponent.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_basecomponent_xtthelp :
	@ if [ -e $(exe_dir)/en_us/basecomponent_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/en_us/basecomponent_xtthelp.dat; \
	  fi
clean_basecomponent_html :
	@ if [ -e $(doc_dir)/orm/en_us/basecomponent_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/orm/basecomponent_*.html; \
	  fi

clean_basecomponentclasses :
	@ if [ -e $(inc_dir)/pwr_basecomponentclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_systemclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/basecomponent_c_*.h; \
	  fi

clean_wbl :
	@ if [ -e $(load_dir)/basecomponent.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/basecomponent.dbs; \
	  fi





