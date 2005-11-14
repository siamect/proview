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


vpath %.wb_load $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

wblsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.wb_load) \
               ), $(notdir $(file)) \
             ) \
           )

pwgsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_c_*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

pwsgsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pwsg) \
               ), $(notdir $(file)) \
             ) \
           )

flwsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.flw) \
               ), $(notdir $(file)) \
             ) \
           )

wbl_sources := $(filter %.wb_load,$(wblsources))
pwg_sources := $(filter %.pwg,$(pwgsources))
pwsg_sources := $(filter %.pwsg,$(pwsgsources))
flw_sources := $(filter %.flw,$(flwsources))
export_wbl_dbs := $(patsubst %.wb_load,$(load_dir)/%.dbs,$(wbl_sources))
export_wbl_h := $(patsubst %.wb_load,$(inc_dir)/pwr_%classes.h,$(wbl_sources))
export_wbl_hpp := $(patsubst %.wb_load,$(inc_dir)/pwr_%classes.hpp,$(wbl_sources))
export_wbl_xtthelp_en_us := $(patsubst %.wb_load,$(exe_dir)/en_us/%_xtthelp.dat,$(wbl_sources))
export_wbl_xtthelp_sv_se := $(patsubst %.wb_load,$(exe_dir)/sv_se/%_xtthelp.dat,$(wbl_sources))
export_wbl_html_en_us := $(patsubst %.wb_load,$(doc_dir)/en_us/orm/%_allclasses.html,$(wbl_sources))
export_wbl_html_sv_se := $(patsubst %.wb_load,$(doc_dir)/sv_se/orm/%_allclasses.html,$(wbl_sources))
export_wbl_ps_en_us := $(patsubst %.wb_load,$(doc_dir)/en_us/%.ps,$(wbl_sources))
export_wbl_ps_sv_se := $(patsubst %.wb_load,$(doc_dir)/sv_se/%.ps,$(wbl_sources))
export_pwg := $(patsubst %.pwg,$(exe_dir)/%.pwg,$(pwg_sources))
export_pwsg := $(patsubst %.pwsg,$(exe_dir)/%.pwsg,$(pwsg_sources))
export_flw := $(patsubst %.flw,$(load_dir)/%.flw,$(flw_sources))

clean_dbs := $(patsubst %.wb_load,clean_%.dbs,$(wbl_sources))
clean_h := $(patsubst %.wb_load,clean_%.h,$(wbl_sources))
clean_hpp := $(patsubst %.wb_load,clean_%.hpp,$(wbl_sources))
clean_xtthelp_sv_se := $(patsubst %.wb_load,clean_%_sv_se.xtthelp,$(wbl_sources))
clean_xtthelp_en_us := $(patsubst %.wb_load,clean_%_en_us.xtthelp,$(wbl_sources))
clean_html_sv_se := $(patsubst %.wb_load,clean_%_sv_se.html,$(wbl_sources))
clean_html_en_us := $(patsubst %.wb_load,clean_%_en_us.html,$(wbl_sources))
clean_ps_sv_se := $(patsubst %.wb_load,clean_%_sv_se.ps,$(wbl_sources))
clean_ps_en_us := $(patsubst %.wb_load,clean_%_en_us.ps,$(wbl_sources))

.SUFFIXES:

$(load_dir)/%.dbs : ../../%.wb_load
	@ echo "Generating snapshot for  $(source)"
	@ wb_cmd create snapshot /file=\"$(source)\"/out=\"$(target)\"
	@ chmod a+w $(target)

$(inc_dir)/pwr_%classes.h : ../../%.wb_load
	@ echo "Generating struct files for $(source) classes..."
	@ co_convert -sv -d $(inc_dir) "$(source)"

$(inc_dir)/pwr_%classes.hpp : ../../%.wb_load
	@ echo "Generating hpp files for $(source) classes..."
	@ co_convert -po -d $(inc_dir) "$(source)"

$(doc_dir)/en_us/orm/%_allclasses.html : ../../%.wb_load
	@ echo "Generating html files for $(source) classes en_us..."
	@ co_convert -w -d $(doc_dir)/en_us/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_$(source)classes.h
	@ co_convert -c -d $(doc_dir)/en_us/orm $(inc_dir)/pwr_$(source)classes.hpp
	@ co_convert -k -d $(doc_dir)/en_us/orm

$(doc_dir)/sv_se/orm/%_allclasses.html : ../../%.wb_load
	@ echo "Generating html files for $(source) classes sv_se..."
	@ co_convert -w -l sv_se -d $(doc_dir)/sv_se/orm -g $(pwre_sroot)/wbl/mcomp/src/cnv_setup.dat "$(source)"
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_$(source)classes.h
	@ co_convert -c -d $(doc_dir)/sv_se/orm $(inc_dir)/pwr_$(source)classes.hpp
	@ co_convert -k -d $(doc_dir)/sv_se/orm

$(doc_dir)/en_us/%.ps : ../../%.wb_load
	@ echo "Generating postscript file for $(source) classes en_us..."
	@ co_convert -q -l en_us -d $(doc_dir)/en_us "$(source)"

$(doc_dir)/sv_se/%.ps : ../../%.wb_load
	@ echo "Generating postscript file for $(source) classes sv_se..."
	@ co_convert -q -l sv_se -d $(doc_dir)/sv_se "$(source)"

$(exe_dir)/en_us/%_xtthelp.dat : ../../%.wb_load
	@ echo "Generating xtt help files for $(source) classes en_us"
	@ co_convert -x -d $(exe_dir)/en_us "$(source)"

$(exe_dir)/sv_se/%_xtthelp.dat : ../../%.wb_load
	@ echo "Generating xtt help files for $(source) classes sv_se"
	@ co_convert -x -l sv_se -d $(exe_dir)/sv_se "$(source)"

$(exe_dir)/%.pwg : ../../%.pwg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.pwsg : ../../%.pwsg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(load_dir)/%.flw : ../../%.flw
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

lib  : $(export_wbl_dbs)

copy : $(export_wbl_h) \
       $(export_wbl_hpp) \
       $(export_pwg) \
       $(export_pwsg) \
       $(export_flw)

exe:   $(export_wbl_xtthelp_en_us) \
       $(export_wbl_xtthelp_sv_se) \
       $(export_wbl_html_en_us) \
       $(export_wbl_html_sv_se) \
       $(export_wbl_ps_en_us) \
       $(export_wbl_ps_sv_se)

clean : \
       $(clean_xtthelp_sv_se) $(clean_xtthelp_en_us) \
       $(clean_html_sv_se) $(clean_html_en_us) \
       $(clean_ps_sv_se) $(clean_ps_en_us)

realclean : clean \
       $(clean_dbs) \
       $(clean_h) \
       $(clean_hpp) \

$(clean_dbs) : clean_%.dbs : $(load_dir)/%.dbs
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_h) : clean_%.h : $(inc_dir)/pwr_%classes.h
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_hpp) : clean_%.hpp : $(inc_dir)/pwr_%classes.hpp
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_ps_sv_se) : clean_%_sv_se.ps : $(doc_dir)/sv_se/%.ps
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_ps_en_us) : clean_%_en_us.ps : $(doc_dir)/en_us/%.ps
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_html_sv_se) : clean_%_sv_se.html : $(doc_dir)/sv_se/orm/%_allclasses.html
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_html_en_us) : clean_%_en_us.html : $(doc_dir)/en_us/orm/%_allclasses.html
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_xtthelp_sv_se) : clean_%_sv_se.xtthelp : $(exe_dir)/sv_se/%_xtthelp.dat
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi

$(clean_xtthelp_en_us) : clean_%_en_us.xtthelp : $(exe_dir)/en_us/%_xtthelp.dat
	@ echo "rm $(source)"
	@ if [ -e $(source) ]; then \
		 $(rm) $(rmflags) $(source); \
	  fi






