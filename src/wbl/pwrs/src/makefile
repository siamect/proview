# makefile  -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

.PHONY : all init copy lib exe dirs clean realclean\
         $(clean_wbl)

all : init copy lib exe | silent

init : dirs | silent

copy : $(inc_dir)/pwr_systemclasses.h \
       $(inc_dir)/pwr_systemclasses.hpp \
       | silent

lib  : $(load_dir)/pwrs.dbs | silent

exe : $(doc_dir)/en_us/orm/pwrs_allclasses.html \
      $(doc_dir)/sv_se/orm/pwrs_allclasses.html \
      $(doc_dir)/en_us/orm/pwr_systemclasses_h.html \
      $(doc_dir)/sv_se/orm/pwr_systemclasses_h.html \
      $(doc_dir)/en_us/orm/pwr_h.html \
      $(doc_dir)/sv_se/orm/pwr_h.html \
      $(doc_dir)/en_us/orm/pwr_class_h.html \
      $(doc_dir)/sv_se/orm/pwr_class_h.html \
      $(doc_dir)/en_us/pwrs.ps \
      $(doc_dir)/sv_se/pwrs.ps \
      $(doc_dir)/en_us/pwrs.pdf \
      $(doc_dir)/sv_se/pwrs.pdf \
      $(exe_dir)/en_us/pwrs_xtthelp.dat \
      $(exe_dir)/sv_se/pwrs_xtthelp.dat \
      | silent

dirs : $(bld_dir)$(dir_ext)

clean : $(clean_wbl)

realclean : clean clean_pwrs_xtthelp clean_pwrs_html clean_systemclasses

silent :
	@ :

$(load_dir)/pwrs.dbs :
	@ $(log_load)
	@ wb_cmd -q -i create snapshot $(wblflags) /file=\"$(pwre_sroot)/wbl/pwrs/src/\"/out=\"$(target)\"/depend=\"$(bld_dir)/pwrs.d_wbl\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_systemclasses.h :
	@ $(log_gen)
	@ $(co_convert) -so -d $(inc_dir) -y $(bld_dir)/pwrs.d_h_wbl  "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"

$(inc_dir)/pwr_systemclasses.hpp :
	@ $(log_gen)
	@ $(co_convert) -po -d $(inc_dir) -y $(bld_dir)/pwrs.d_hpp_wbl "$(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load"

$(doc_dir)/en_us/orm/pwrs_allclasses.html :
	@ $(log_html)
	@ $(co_convert) -w -d $(doc_dir)/en_us/orm -y $(bld_dir)/pwrs.d_html_wbl -g $(pwre_sroot)/wbl/pwrs/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(doc_dir)/en_us/orm/pwr_systemclasses_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_systemclasses.h

$(doc_dir)/en_us/orm/pwr_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr.h

$(doc_dir)/en_us/orm/pwr_class_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_class.h

$(doc_dir)/sv_se/orm/pwrs_allclasses.html :
	@ $(log_html)
	@ $(co_convert) -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/pwrs/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(doc_dir)/sv_se/orm/pwr_systemclasses_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_systemclasses.h

$(doc_dir)/sv_se/orm/pwr_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr.h

$(doc_dir)/sv_se/orm/pwr_class_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_class.h

$(doc_dir)/en_us/pwrs.ps :
	@ $(log_ps)
	@ $(co_convert) -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(doc_dir)/sv_se/pwrs.ps :
	@ $(log_ps)
	@ $(co_convert) -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(doc_dir)/en_us/pwrs.pdf :
	@ $(log_pdf)
	@ $(co_convert) -Q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(doc_dir)/sv_se/pwrs.pdf :
	@ $(log_pdf)
	@ $(co_convert) -Q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(exe_dir)/en_us/pwrs_xtthelp.dat :
	@ $(log_xtthelp)
	@ $(co_convert) -x -d $(exe_dir)/en_us -y $(bld_dir)/pwrs.d_en_us_xtthelp_wbl "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"

$(exe_dir)/sv_se/pwrs_xtthelp.dat :
	@ echo $(log_xtthelp)
	@ $(co_convert) -x -l sv_se -d $(exe_dir)/sv_se -y $(bld_dir)/pwrs.d_sv_se_xtthelp_wbl "$(pwre_sroot)/wbl/pwrs/src/pwrs_*.wb_load"


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

$(bld_dir)$(dir_ext) :
	@ $(log_mkdir)
	@ $(mkdir) $(mkdirflags) $(basename $@)

-include $(bld_dir)/pwrs.d_h_wbl
-include $(bld_dir)/pwrs.d_hpp_wbl
-include $(bld_dir)/pwrs.d_html_wbl
-include $(bld_dir)/pwrs.d_en_us_xtthelp_wbl
-include $(bld_dir)/pwrs.d_sv_se_xtthelp_wbl
-include $(bld_dir)/pwrs.d_wbl
