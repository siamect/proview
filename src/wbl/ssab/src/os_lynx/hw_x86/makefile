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

copy : $(inc_dir)/pwr_ssabclasses.h

lib : 

exe : $(doc_dir)/ssab_allclasses.html \
      $(exe_dir)/ssab_xtthelp.dat

clean :

realclean : clean clean_ssab_xtthelp clean_ssab_html clean_ssabclasses

$(inc_dir)/pwr_ssabclasses.h :
	@ echo "Generating struct files for ssab classes..."
	@ co_convert -s -d $(inc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

$(doc_dir)/ssab_allclasses.html :
	@ echo "Generating html files for ssab classes..."
	@ co_convert -w -d $(doc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"
	@ echo "Generating html files for ssab structs..."
	@ co_convert -c -d $(doc_dir) $(inc_dir)/pwr_ssabclasses.h

$(exe_dir)/ssab_xtthelp.dat :
	@ echo "Generating xtt help files for ssab classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_ssab_xtthelp :
	@ if [ -e $(exe_dir)/ssab_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/ssab_xtthelp.dat; \
	  fi
clean_ssab_html :
	@ if [ -e $(doc_dir)/ssab_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/ssab_*.html; \
	  fi

clean_ssabclasses :
	@ if [ -e $(inc_dir)/pwr_ssabclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_ssabclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/ssab_c_*.h; \
	  fi

