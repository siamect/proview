#  makefile -- <short description>

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

code_sources := $(wildcard $(pwre_sroot)/lib/rt/src/rt_plc_macro_*.h) \
                $(wildcard $(pwre_sroot)/lib/rt/src/rt_plc_*.c) \
                $(wildcard $(pwre_sroot)/lib/rt/src/rt_c_*.c) \
		$(pwre_sroot)/lib/rt/src/rt_csup.c
code_sources := $(notdir $(code_sources))
export_code_en_us := $(patsubst %.c,$(doc_dir)/en_us/orm/%_c.html,$(code_sources))
export_code_en_us := $(patsubst %.h,$(doc_dir)/en_us/orm/%_h.html,$(export_code_en_us))
export_code_sv_se := $(patsubst %.c,$(doc_dir)/sv_se/orm/%_c.html,$(code_sources))
export_code_sv_se := $(patsubst %.h,$(doc_dir)/sv_se/orm/%_h.html,$(export_code_sv_se))

.SUFFIXES:

$(doc_dir)/en_us/orm/%_c.html : $(pwre_sroot)/lib/rt/src/%.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/en_us/orm/%_h.html : $(pwre_sroot)/lib/rt/src/%.h
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/%_c.html : $(pwre_sroot)/lib/rt/src/%.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/sv_se/orm/%_h.html : $(pwre_sroot)/lib/rt/src/%.h
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

.PHONY : all init copy lib exe dirs clean realclean\
         $(clean_wbl)

all : init copy lib exe | silent

init : dirs | silent

copy : $(inc_dir)/pwr_baseclasses.h \
       $(inc_dir)/pwr_baseclasses.hpp \
       | silent


lib  : $(load_dir)/pwrb.dbs | silent

exe : $(doc_dir)/en_us/orm/pwrb_allclasses.html \
      $(doc_dir)/sv_se/orm/pwrb_allclasses.html \
      $(doc_dir)/en_us/orm/pwr_baseclasses_h.html \
      $(doc_dir)/sv_se/orm/pwr_baseclasses_h.html \
      $(doc_dir)/en_us/orm/pwr_baseclasses_hpp.html \
      $(doc_dir)/sv_se/orm/pwr_baseclasses_hpp.html \
      $(doc_dir)/en_us/pwrb.ps \
      $(doc_dir)/sv_se/pwrb.ps \
      $(doc_dir)/en_us/pwrb.pdf \
      $(doc_dir)/sv_se/pwrb.pdf \
      $(exe_dir)/en_us/pwrb_xtthelp.dat \
      $(exe_dir)/sv_se/pwrb_xtthelp.dat \
      $(export_code_en_us) \
      $(export_code_sv_se) \
      | silent

dirs : $(bld_dir)$(dir_ext)

clean :

realclean : clean clean_pwrb_xtthelp clean_pwrb_html clean_baseclasses

silent :
	@ :

$(load_dir)/pwrb.dbs :
	@ $(log_load)
	@ wb_cmd -q -i create snapshot $(wblflags) /file=\"$(pwre_sroot)/wbl/pwrb/src/\"/out=\"$(target)\"/depend=\"$(bld_dir)/pwrb.d_wbl\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_baseclasses.h :
	@ $(log_struct)
	@ $(co_convert) -so -d $(inc_dir) -y $(bld_dir)/pwrb.d_h_wbl "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(inc_dir)/pwr_baseclasses.hpp :
	@ $(log_hpp)
	@ $(co_convert) -po -d $(inc_dir) -y $(bld_dir)/pwrb.d_hpp_wbl "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/en_us/orm/pwrb_allclasses.html :
	@ $(log_html)
	@ $(co_convert) -w -d $(doc_dir)/en_us/orm -y $(bld_dir)/pwrb.d_html_wbl -g $(pwre_sroot)/wbl/pwrb/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/en_us/orm/pwr_baseclasses_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_baseclasses.h

$(doc_dir)/en_us/orm/pwr_baseclasses_hpp.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_baseclasses.hpp

$(doc_dir)/sv_se/orm/pwrb_allclasses.html :
	@ $(log_html)
	@ $(co_convert) -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/pwrb/src/cnv_setup.dat "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/sv_se/orm/pwr_baseclasses_h.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_baseclasses.h

$(doc_dir)/sv_se/orm/pwr_baseclasses_hpp.html :
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_baseclasses.hpp

$(doc_dir)/en_us/pwrb.ps :
	@ $(log_ps)
	@ $(co_convert) -q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/sv_se/pwrb.ps :
	@ $(log_ps)
	@ $(co_convert) -q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/en_us/pwrb.pdf :
	@ $(log_pdf)
	@ $(co_convert) -Q -d $(doc_dir)/en_us "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(doc_dir)/sv_se/pwrb.pdf :
	@ $(log_pdf)
	@ $(co_convert) -Q -l sv_se -d $(doc_dir)/sv_se "$(pwre_sroot)/wbl/pwrb/src/pwrb_*.wb_load"

$(exe_dir)/en_us/pwrb_xtthelp.dat :
	@ $(log_xtthelp)
	@ $(co_convert) -x -d $(exe_dir)/en_us -y $(bld_dir)/pwrb.d_en_us_xtthelp_wbl "$(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load"

$(exe_dir)/sv_se/pwrb_xtthelp.dat :
	@ $(log_xtthelp)
	@ $(co_convert) -x -l sv_se -d $(exe_dir)/sv_se -y $(bld_dir)/pwrb.d_sv_se_xtthelp_wbl "$(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

clean_pwrb_xtthelp :
	@ if [ -e $(exe_dir)/en_us/pwrb_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/en_us/pwrb_xtthelp.dat; \
	  fi
	@ if [ -e $(exe_dir)/sv_se/pwrb_xtthelp.dat ]; then \
		 $(rm) $(rmflags) $(exe_dir)/sv_se/pwrb_xtthelp.dat; \
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

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)

-include $(bld_dir)/pwrb.d_h_wbl
-include $(bld_dir)/pwrb.d_hpp_wbl
-include $(bld_dir)/pwrb.d_html_wbl
-include $(bld_dir)/pwrb.d_en_us_xtthelp_wbl
-include $(bld_dir)/pwrb.d_sv_se_xtthelp_wbl
-include $(bld_dir)/pwrb.d_wbl
