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

copy : $(inc_dir)/pwr_othermanufacturerclasses.h \
       $(inc_dir)/pwr_othermanufacturerclasses.hpp \
       $(inc_dir)/pwr_abbclasses.h \
       $(inc_dir)/pwr_abbclasses.hpp \
       $(inc_dir)/pwr_siemensclasses.h \
       $(inc_dir)/pwr_siemensclasses.hpp \
       $(inc_dir)/pwr_telemecaniqueclasses.h \
       $(inc_dir)/pwr_telemecaniqueclasses.hpp \
       $(inc_dir)/pwr_ssaboxclasses.h \
       $(inc_dir)/pwr_ssaboxclasses.hpp


lib  : $(load_dir)/othermanufacturer.dbs \
       $(load_dir)/abb.dbs \
       $(load_dir)/siemens.dbs \
       $(load_dir)/telemecanique.dbs \
       $(load_dir)/ssabox.dbs

exe : $(doc_dir)/en_us/orm/othermanufacturer_allclasses.html \
      $(doc_dir)/sv_se/orm/othermanufacturer_allclasses.html \
      $(exe_dir)/en_us/othermanufacturer_xtthelp.dat \
      $(exe_dir)/sv_se/othermanufacturer_xtthelp.dat \
      $(doc_dir)/en_us/orm/abb_allclasses.html \
      $(doc_dir)/sv_se/orm/abb_allclasses.html \
      $(exe_dir)/en_us/abb_xtthelp.dat \
      $(exe_dir)/sv_se/abb_xtthelp.dat \
      $(doc_dir)/en_us/orm/siemens_allclasses.html \
      $(doc_dir)/sv_se/orm/siemens_allclasses.html \
      $(exe_dir)/en_us/siemens_xtthelp.dat \
      $(exe_dir)/sv_se/siemens_xtthelp.dat \
      $(doc_dir)/en_us/orm/telemecanique_allclasses.html \
      $(doc_dir)/sv_se/orm/telemecanique_allclasses.html \
      $(exe_dir)/en_us/telemecanique_xtthelp.dat \
      $(exe_dir)/sv_se/telemecanique_xtthelp.dat \
      $(exe_dir)/en_us/ssabox_xtthelp.dat \
      $(exe_dir)/sv_se/ssabox_xtthelp.dat

clean : 

realclean : clean clean_othermanufacturer_xtthelp clean_othermanufacturer_html clean_othermanufacturerclasses

#
# OtherManufacturer
#
$(load_dir)/othermanufacturer.dbs : ../../othermanufacturer.wb_load
	@ echo "Generating snapshot for othermanufacturer"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_othermanufacturerclasses.h : ../../othermanufacturer.wb_load
	@ echo "Generating struct files for othermanufacturer classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_othermanufacturerclasses.hpp : ../../othermanufacturer.wb_load
	@ echo "Generating hpp files for othermanufacturer classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/othermanufacturer_allclasses.html : ../../othermanufacturer.wb_load
	@ echo "Generating html files for othermanufacturer classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_othermanufacturerclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_othermanufacturerclasses.hpp
	@ echo "Generating html files for othermanufacturer classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_othermanufacturerclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_othermanufacturerclasses.hpp
	@ echo "Generating postscript file for othermanufacturer classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(source)"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/othermanufacturer_xtthelp.dat : ../../othermanufacturer.wb_load
	@ echo "Generating xtt help files for othermanufacturer classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/othermanufacturer_xtthelp.dat : ../../othermanufacturer.wb_load
	@ echo "Generating xtt help files for othermanufacturer classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"

#
# ABB
#
$(load_dir)/abb.dbs : ../../abb.wb_load
	@ echo "Generating snapshot for abb"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_abbclasses.h : ../../abb.wb_load
	@ echo "Generating struct files for abb classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_abbclasses.hpp : ../../abb.wb_load
	@ echo "Generating hpp files for abb classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/abb_allclasses.html : ../../abb.wb_load
	@ echo "Generating html files for abb classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_abbclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_abbclasses.hpp
	@ echo "Generating html files for abb classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_abbclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_abbclasses.hpp
	@ echo "Generating postscript file for abb classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(source)"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/abb_xtthelp.dat : ../../abb.wb_load
	@ echo "Generating xtt help files for abb classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/abb_xtthelp.dat : ../../abb.wb_load
	@ echo "Generating xtt help files for abb classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"
#
# Siemens
#
$(load_dir)/siemens.dbs : ../../siemens.wb_load
	@ echo "Generating snapshot for siemens"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_siemensclasses.h : ../../siemens.wb_load
	@ echo "Generating struct files for siemens classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_siemensclasses.hpp : ../../siemens.wb_load
	@ echo "Generating hpp files for siemens classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/siemens_allclasses.html : ../../siemens.wb_load
	@ echo "Generating html files for siemens classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_siemensclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_siemensclasses.hpp
	@ echo "Generating html files for siemens classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_siemensclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_siemensclasses.hpp
	@ echo "Generating postscript file for siemens classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(source)"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/siemens_xtthelp.dat : ../../siemens.wb_load
	@ echo "Generating xtt help files for siemens classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/siemens_xtthelp.dat : ../../siemens.wb_load
	@ echo "Generating xtt help files for siemens classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"

#
# Telemecanique
#
$(load_dir)/telemecanique.dbs : ../../telemecanique.wb_load
	@ echo "Generating snapshot for telemecanique"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_telemecaniqueclasses.h : ../../telemecanique.wb_load
	@ echo "Generating struct files for telemecanique classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_telemecaniqueclasses.hpp : ../../telemecanique.wb_load
	@ echo "Generating hpp files for telemecanique classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/telemecanique_allclasses.html : ../../telemecanique.wb_load
	@ echo "Generating html files for telemecanique classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_telemecaniqueclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_telemecaniqueclasses.hpp
	@ echo "Generating html files for telemecanique classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_telemecaniqueclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_telemecaniqueclasses.hpp
	@ echo "Generating postscript file for telemecanique classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(source)"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/telemecanique_xtthelp.dat : ../../telemecanique.wb_load
	@ echo "Generating xtt help files for telemecanique classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/telemecanique_xtthelp.dat : ../../telemecanique.wb_load
	@ echo "Generating xtt help files for telemecanique classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"

#
# SsabOx
#
$(load_dir)/ssabox.dbs : ../../ssabox.wb_load
	@ echo "Generating snapshot for ssabox"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_ssaboxclasses.h : ../../ssabox.wb_load
	@ echo "Generating struct files for ssabox classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_ssaboxclasses.hpp : ../../ssabox.wb_load
	@ echo "Generating hpp files for ssabox classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/ssabox_allclasses.html : ../../ssabox.wb_load
	@ echo "Generating html files for ssabox classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_ssaboxclasses.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_ssaboxclasses.hpp
	@ echo "Generating html files for ssabox classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_ssaboxclasses.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_ssaboxclasses.hpp
	@ echo "Generating postscript file for ssabox classes..."
	@ co_convert -q -d $(doc_dir)/en_us "$(source)"
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/ssabox_xtthelp.dat : ../../ssabox.wb_load
	@ echo "Generating xtt help files for ssabox classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/ssabox_xtthelp.dat : ../../ssabox.wb_load
	@ echo "Generating xtt help files for ssabox classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"

#
# Clean
#

clean_othermanufacturer_xtthelp :
	@ if [ -e $(exe_dir)/en_us/othermanufacturer_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/en_us/othermanufacturer_xtthelp.dat; \
	  fi
clean_othermanufacturer_html :
	@ if [ -e $(doc_dir)/orm/en_us/othermanufacturer_allclasses.html ]; then \
	        $(rm) $(rmflags) $(doc_dir)/orm/othermanufacturer_*.html; \
	  fi

clean_othermanufacturerclasses :
	@ if [ -e $(inc_dir)/pwr_othermanufacturerclasses.h ]; then \
		$(rm) $(rmflags) $(inc_dir)/pwr_systemclasses.h; \
		$(rm) $(rmflags) $(inc_dir)/othermanufacturer_c_*.h; \
	  fi

clean_wbl :
	@ if [ -e $(load_dir)/othermanufacturer.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/othermanufacturer.dbs; \
	  fi





