#  makefile -- <short description>
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



vpath %.wb_load $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

tlog_volumes := \
	$(cnf_dir)/tlog_v.wb_vers

tlog_hierarchies := \
	$(cnf_dir)/tlog_ch_tlogclasses.wb_vers

tlog_layouts := \
	$(cnf_dir)/tlog_l_base.wb_vers

tlog_classes := \
	$(cnf_dir)/tlog_c_atlog.wb_vers \
	$(cnf_dir)/tlog_c_catlog.wb_vers \
	$(cnf_dir)/tlog_c_cdtlog.wb_vers \
	$(cnf_dir)/tlog_c_chgatlog.wb_vers \
	$(cnf_dir)/tlog_c_chgotlog.wb_vers \
	$(cnf_dir)/tlog_c_cotlog.wb_vers \
	$(cnf_dir)/tlog_c_dtlog.wb_vers \
	$(cnf_dir)/tlog_c_execplcoff.wb_vers \
	$(cnf_dir)/tlog_c_execplcon.wb_vers \
	$(cnf_dir)/tlog_c_tlogconfigure.wb_vers \
	$(cnf_dir)/tlog_c_tlogexecalloff.wb_vers \
	$(cnf_dir)/tlog_c_tlogexecallon.wb_vers \
	$(cnf_dir)/tlog_c_tlogmodify.wb_vers \
	$(cnf_dir)/tlog_c_tlogopen.wb_vers

export_wbl := \
	$(tlog_volumes) \
	$(tlog_hierarchies) \
	$(tlog_classes) \
	$(tlog_layouts)

clean_wbl := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_tlogclasses.h

#lib : $(export_wbl)
lib  : $(load_dir)/tlog.dbs

exe : $(doc_dir)/tlog_allclasses.html \
      $(exe_dir)/tlog_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(load_dir)/tlog.dbs :
	@ echo "Generating snapshot for tlog"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/tlog/src/\"/out=\"$(target)\"

$(inc_dir)/pwr_tlogclasses.h :
	@ echo "Generating struct files for tlog classes..."
	@ co_convert -sv -d $(inc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"

$(doc_dir)/tlog_allclasses.html :
	@ echo "Generating html files for tlog classes..."
	@ co_convert -w -d $(doc_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"
	@ echo "Generating html files for tlog structs..."
	@ co_convert -c -d $(doc_dir) $(inc_dir)/pwr_tlogclasses.h
	@ echo "Generating html files for tlog code..."
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_macro_tlog.h"
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_tlog*.c"

$(exe_dir)/tlog_xtthelp.dat :
	@ echo "Generating xtt help files for tlog classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/tlog/src/tlog_c_*.wb_load"


#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_wbl) : clean_%.wb_vers : %.wb_vers
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

