# makefile  -- <short description>
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

all : init copy lib exe

init :

copy : $(inc_dir)/pwr_systemclasses.h \
       $(inc_dir)/pwr_systemclasses.hpp

lib  : $(load_dir)/pwrs.dbs

exe : $(doc_dir)/en_us/orm/pwrs_allclasses.html \
      $(exe_dir)/en_us/pwrs_xtthelp.dat \
      $(exe_dir)/sv_se/pwrs_xtthelp.dat

clean : $(clean_wbl)

realclean : clean clean_pwrs_xtthelp clean_pwrs_html clean_systemclasses

$(load_dir)/pwrs.dbs :
	@ echo "Generating loadfile for pwrs"
	@ wb_cmd -q -i create snapshot $(wblflags) /file=\"$(pwre_sroot)/wbl/pwrs/src/\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_systemclasses.h :
	@ echo "Generating struct files for pwrs classes..."
	@ co_convert -so -d $(inc_dir) "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"

$(inc_dir)/pwr_systemclasses.hpp :
	@ echo "Generating hpp files for pwrs classes..."
	@ co_convert -po -d $(inc_dir) "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"

$(doc_dir)/en_us/orm/pwrs_allclasses.html :
	@ echo "Generating html files for pwrs classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/pwrs/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"
	@ echo "Generating html files for pwrs structs en_us..."
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_systemclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_class.h
	@ echo "Generating html files for pwrs classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/pwrs/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"
	@ echo "Generating html files for pwrs structs sv_se..."
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_systemclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_class.h
	@ echo "Generating postscript file for pwrs classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(exe_dir)/en_us/pwrs_xtthelp.dat :
	@ echo "Generating xtt help files for pwrs classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"

$(exe_dir)/sv_se/pwrs_xtthelp.dat :
	@ echo "Generating xtt help files for pwrs classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"


#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_wbl :
	@ if [ -e $(load_dir)/pwrs.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/pwrs.dbs; \
	  fi

clean_pwrs_xtthelp :
	@ if [ -e $(exe_dir)/en_us/pwrs_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/en_us/pwrs_xtthelp.dat; \
	  fi

clean_pwrs_html :
	@ if [ -e $(doc_dir)/en_us/orm/pwrs_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/en_us/orm/pwrs_*.html; \
	  fi

clean_systemclasses :
	@ if [ -e $(inc_dir)/pwr_systemclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_systemclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/pwrs_c_*.h; \
	  fi

