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

ssab_volumes := \
	$(cnf_dir)/ssab_v.wb_vers

ssab_hierarchies := \
	$(cnf_dir)/ssab_ch_ssabclasses.wb_vers

ssab_layouts := \
	$(cnf_dir)/ssab_l_base.wb_vers

ssab_classes := \
	$(cnf_dir)/ssab_c_buff1440.wb_vers \
	$(cnf_dir)/ssab_c_buff256.wb_vers \
	$(cnf_dir)/ssab_c_buff32k.wb_vers \
	$(cnf_dir)/ssab_c_buff4096.wb_vers \
	$(cnf_dir)/ssab_c_loggconfig.wb_vers \
	$(cnf_dir)/ssab_c_loctrans.wb_vers \
	$(cnf_dir)/ssab_c_multicast.wb_vers \
	$(cnf_dir)/ssab_c_remai.wb_vers \
	$(cnf_dir)/ssab_c_remao.wb_vers \
	$(cnf_dir)/ssab_c_remchan_ai.wb_vers \
	$(cnf_dir)/ssab_c_remchan_ao.wb_vers \
	$(cnf_dir)/ssab_c_remchan_co.wb_vers \
	$(cnf_dir)/ssab_c_remchan_di.wb_vers \
	$(cnf_dir)/ssab_c_remchan_do.wb_vers \
	$(cnf_dir)/ssab_c_remchan_dv.wb_vers \
	$(cnf_dir)/ssab_c_remco.wb_vers \
	$(cnf_dir)/ssab_c_remdi.wb_vers \
	$(cnf_dir)/ssab_c_remdo.wb_vers \
	$(cnf_dir)/ssab_c_remdv.wb_vers \
	$(cnf_dir)/ssab_c_remnode.wb_vers \
	$(cnf_dir)/ssab_c_remtrans.wb_vers

export_wbl := \
	$(ssab_volumes) \
	$(ssab_hierarchies) \
	$(ssab_classes) \
	$(ssab_layouts)

clean_wbl := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_ssabclasses.h

lib : $(export_wbl)

exe : $(doc_dir)/ssab_allclasses.html \
      $(exe_dir)/ssab_xtthelp.dat

clean : 

realclean : clean $(clean_wbl)

$(inc_dir)/pwr_ssabclasses.h :
	@ echo "Generating struct files for ssab classes..."
	@ co_convert -sv -d $(inc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

$(doc_dir)/ssab_allclasses.html :
	@ echo "Generating html files for ssab classes..."
	@ co_convert -w -d $(doc_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"
	@ echo "Generating html files for ssab structs..."
	@ co_convert -c -d $(doc_dir) $(inc_dir)/pwr_ssabclasses.h
	@ echo "Generating html files for ssab code..."
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_macro_ssab.h"
	@ co_convert -c -d $(doc_dir) "$(pwre_sroot)/lib/rs/src/rs_plc_ssab*.c"

$(exe_dir)/ssab_xtthelp.dat :
	@ echo "Generating xtt help files for ssab classes"
	@ co_convert -x -d $(exe_dir) "$(pwre_sroot)/wbl/ssab/src/ssab_c_*.wb_load"

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_wbl) : clean_%.wb_vers : %.wb_vers
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

