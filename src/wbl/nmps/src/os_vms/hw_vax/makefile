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

nmps_volumes := \
	$(cnf_dir)/nmps_v.wb_vers

nmps_hierarchies := \
	$(cnf_dir)/nmps_ch_nmpsclasses.wb_vers

nmps_layouts := \
	$(cnf_dir)/nmps_l_base.wb_vers

nmps_classes := \
	$(cnf_dir)/nmps_c_celldisp.wb_vers \
	$(cnf_dir)/nmps_c_cellupdate.wb_vers \
	$(cnf_dir)/nmps_c_cloop.wb_vers \
	$(cnf_dir)/nmps_c_convdef.wb_vers \
	$(cnf_dir)/nmps_c_currentdata.wb_vers \
	$(cnf_dir)/nmps_c_currentindex.wb_vers \
	$(cnf_dir)/nmps_c_datacopy.wb_vers \
	$(cnf_dir)/nmps_c_datacnv.wb_vers \
	$(cnf_dir)/nmps_c_datarcv.wb_vers \
	$(cnf_dir)/nmps_c_datarequest.wb_vers \
	$(cnf_dir)/nmps_c_datareset.wb_vers \
	$(cnf_dir)/nmps_c_dataselect.wb_vers \
	$(cnf_dir)/nmps_c_datasend.wb_vers \
	$(cnf_dir)/nmps_c_displink.wb_vers \
	$(cnf_dir)/nmps_c_func.wb_vers \
	$(cnf_dir)/nmps_c_funcextend.wb_vers \
	$(cnf_dir)/nmps_c_funcinput.wb_vers \
	$(cnf_dir)/nmps_c_funcoutput.wb_vers \
	$(cnf_dir)/nmps_c_getdap.wb_vers \
	$(cnf_dir)/nmps_c_getobjid.wb_vers \
	$(cnf_dir)/nmps_c_getobjidp.wb_vers \
	$(cnf_dir)/nmps_c_mirrorconfig.wb_vers \
	$(cnf_dir)/nmps_c_nmpsbackupconfig.wb_vers \
	$(cnf_dir)/nmps_c_nmpscell.wb_vers \
	$(cnf_dir)/nmps_c_nmpsconvconfig.wb_vers \
	$(cnf_dir)/nmps_c_nmpsmirrorcell.wb_vers \
	$(cnf_dir)/nmps_c_nmpsoutcell.wb_vers \
	$(cnf_dir)/nmps_c_nmpsstorecell.wb_vers \
	$(cnf_dir)/nmps_c_nmpstransconfig.wb_vers \
	$(cnf_dir)/nmps_c_nmpstrp.wb_vers \
	$(cnf_dir)/nmps_c_nmpstrpff.wb_vers \
	$(cnf_dir)/nmps_c_nmpstrprr.wb_vers \
	$(cnf_dir)/nmps_c_remtransrcv.wb_vers \
	$(cnf_dir)/nmps_c_remtranssend.wb_vers \
	$(cnf_dir)/nmps_c_runningtime.wb_vers

export_wbl := \
	$(nmps_volumes) \
	$(nmps_hierarchies) \
	$(nmps_classes) \
	$(nmps_layouts)

clean_wbl := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_nmpsclasses.h

lib : $(export_wbl)

exe : $(exe_dir)/nmps_xtthelp.dat

clean :

realclean : clean $(clean_wbl)

$(inc_dir)/pwr_nmpsclasses.h :
	@ write sys$$output "Generating struct files for nmps classes..."
	@ mc pwr_exe:co_convert -s -d pwr_inc: $(to-vms $(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load)

$(doc_dir)/nmps_allclasses.html :
	@ write sys$$output "Generating html files for nmps classes..."
	@ mc pwr_exe:co_convert -w -d pwr_doc: $(to_vms $(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load)
	@ write sys$$output "Generating html files for nmps structs..."
	@ mc pwr_exe:co_convert -c -d pwr_doc: $(inc_dir)/pwr_nmpsclasses.h

$(exe_dir)/nmps_xtthelp.dat :
	@ write sys$$output "Generating xtt help files for nmps classes"
	@ mc pwr_exe:co_convert -x -d pwr_exe: $(to-vms $(pwre_sroot)/wbl/nmps/src/nmps_c_*.wb_load)

$(clean_wbl) : clean_%.wb_vers : %.wb_vers
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

