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


source_dirs = $(hw_source) $(os_source) $(co_source)


.PHONY : all init copy lib exe clean realclean

all : init copy exe

init :

copy : $(inc_dir)/pwr_tlogclasses.h

lib : 

exe : $(doc_dir)/tlog_allclasses.html \
      $(exe_dir)/tlog_xtthelp.dat

clean :

realclean : clean clean_tlog_xtthelp clean_tlog_html clean_tlogclasses

$(inc_dir)/pwr_tlogclasses.h :
	@ echo "Generating struct files for tlog classes..."
	@ co_convert -s -d $(inc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"

$(doc_dir)/tlog_allclasses.html :
	@ echo "Generating html files for tlog classes..."
	@ co_convert -w -d $(doc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"
	@ echo "Generating html files for tlog structs..."
	@ co_convert -c -d $(doc_dir) $(inc_dir)/pwr_tlogclasses.h

$(exe_dir)/tlog_xtthelp.dat :
	@ echo "Generating xtt help files for tlog classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_tlog_xtthelp :
	@ if [ -e $(exe_dir)/tlog_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/tlog_xtthelp.dat; \
	  fi
clean_tlog_html :
	@ if [ -e $(doc_dir)/tlog_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/tlog_*.html; \
	  fi

clean_tlogclasses :
	@ if [ -e $(inc_dir)/pwr_tlogclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_tlogclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/tlog_c_*.h; \
	  fi

