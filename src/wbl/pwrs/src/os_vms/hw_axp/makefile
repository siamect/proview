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


vpath %.wb_load $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

pwrs_volume := \
	$(cnf_dir)/pwrs_v.wb_vers

pwrs_hierarchies := \
	$(cnf_dir)/pwrs_ch_systemclasses.wb_vers \
	$(cnf_dir)/pwrs_ch_systemtypes.wb_vers

pwrs_types := \
	$(cnf_dir)/pwrs_t_attrref.wb_vers \
	$(cnf_dir)/pwrs_t_boolean.wb_vers \
	$(cnf_dir)/pwrs_t_classid.wb_vers \
	$(cnf_dir)/pwrs_t_char.wb_vers \
	$(cnf_dir)/pwrs_t_deltatime.wb_vers \
	$(cnf_dir)/pwrs_t_float32.wb_vers \
	$(cnf_dir)/pwrs_t_float64.wb_vers \
	$(cnf_dir)/pwrs_t_int16.wb_vers \
	$(cnf_dir)/pwrs_t_int32.wb_vers \
	$(cnf_dir)/pwrs_t_int8.wb_vers \
	$(cnf_dir)/pwrs_t_objid.wb_vers \
	$(cnf_dir)/pwrs_t_pgmname.wb_vers \
	$(cnf_dir)/pwrs_t_string.wb_vers \
	$(cnf_dir)/pwrs_t_struct.wb_vers \
	$(cnf_dir)/pwrs_t_text.wb_vers \
	$(cnf_dir)/pwrs_t_time.wb_vers \
	$(cnf_dir)/pwrs_t_typeid.wb_vers \
	$(cnf_dir)/pwrs_t_volumeid.wb_vers \
	$(cnf_dir)/pwrs_t_uint16.wb_vers \
	$(cnf_dir)/pwrs_t_uint32.wb_vers \
	$(cnf_dir)/pwrs_t_uint8.wb_vers \
	$(cnf_dir)/pwrs_t_objectix.wb_vers \
	$(cnf_dir)/pwrs_t_refid.wb_vers \
	$(cnf_dir)/pwrs_t_enum.wb_vers \
	$(cnf_dir)/pwrs_t_mask.wb_vers

pwrs_virgin_classes := \
	$(cnf_dir)/pwrs_c_attribute.wb_vers \
	$(cnf_dir)/pwrs_c_classdef.wb_vers \
	$(cnf_dir)/pwrs_c_objbodydef.wb_vers \
	$(cnf_dir)/pwrs_c_type.wb_vers \
	$(cnf_dir)/pwrs_c_typedef.wb_vers

pwrs_typedefs := \
	$(cnf_dir)/pwrs_td_graphname.wb_vers \
	$(cnf_dir)/pwrs_td_objname.wb_vers \
	$(cnf_dir)/pwrs_td_string132.wb_vers \
	$(cnf_dir)/pwrs_td_string16.wb_vers \
	$(cnf_dir)/pwrs_td_string256.wb_vers \
	$(cnf_dir)/pwrs_td_string32.wb_vers \
	$(cnf_dir)/pwrs_td_string40.wb_vers \
	$(cnf_dir)/pwrs_td_string8.wb_vers \
	$(cnf_dir)/pwrs_td_string80.wb_vers \
	$(cnf_dir)/pwrs_td_text1024.wb_vers \
	$(cnf_dir)/pwrs_td_xref.wb_vers

pwrs_classes := \
	$(cnf_dir)/pwrs_c_classvolume.wb_vers \
	$(cnf_dir)/pwrs_c_directoryvolume.wb_vers \
	$(cnf_dir)/pwrs_c_dynamicvolume.wb_vers \
	$(cnf_dir)/pwrs_c_rootvolume.wb_vers \
	$(cnf_dir)/pwrs_c_sharedvolume.wb_vers \
	$(cnf_dir)/pwrs_c_subvolume.wb_vers \
	$(cnf_dir)/pwrs_c_systemvolume.wb_vers \
	$(cnf_dir)/pwrs_c_workbenchvolume.wb_vers \
	$(cnf_dir)/pwrs_c_mountvolume.wb_vers \
	$(cnf_dir)/pwrs_c_mountobject.wb_vers \
	$(cnf_dir)/pwrs_c_createvolume.wb_vers \
	$(cnf_dir)/pwrs_c_alias.wb_vers \
	$(cnf_dir)/pwrs_c_appl.wb_vers \
	$(cnf_dir)/pwrs_c_attrxref.wb_vers \
	$(cnf_dir)/pwrs_c_bootversion.wb_vers \
	$(cnf_dir)/pwrs_c_buffer.wb_vers \
	$(cnf_dir)/pwrs_c_classhier.wb_vers \
	$(cnf_dir)/pwrs_c_dbcallback.wb_vers \
	$(cnf_dir)/pwrs_c_dochier.wb_vers \
	$(cnf_dir)/pwrs_c_graphplcconnection.wb_vers \
	$(cnf_dir)/pwrs_c_graphplcnode.wb_vers \
	$(cnf_dir)/pwrs_c_graphplcprogram.wb_vers \
	$(cnf_dir)/pwrs_c_graphplcwindow.wb_vers \
	$(cnf_dir)/pwrs_c_group.wb_vers \
	$(cnf_dir)/pwrs_c_groupref.wb_vers \
	$(cnf_dir)/pwrs_c_input.wb_vers \
	$(cnf_dir)/pwrs_c_intern.wb_vers \
	$(cnf_dir)/pwrs_c_layout.wb_vers \
	$(cnf_dir)/pwrs_c_libhier.wb_vers \
	$(cnf_dir)/pwrs_c_menu.wb_vers \
	$(cnf_dir)/pwrs_c_menubutton.wb_vers \
	$(cnf_dir)/pwrs_c_menucascade.wb_vers \
	$(cnf_dir)/pwrs_c_menuseparator.wb_vers \
	$(cnf_dir)/pwrs_c_node.wb_vers \
	$(cnf_dir)/pwrs_c_nodehier.wb_vers \
	$(cnf_dir)/pwrs_c_nodeversion.wb_vers \
	$(cnf_dir)/pwrs_c_object.wb_vers \
	$(cnf_dir)/pwrs_c_objxref.wb_vers \
	$(cnf_dir)/pwrs_c_output.wb_vers \
	$(cnf_dir)/pwrs_c_planthier.wb_vers \
	$(cnf_dir)/pwrs_c_point.wb_vers \
	$(cnf_dir)/pwrs_c_plcconnection.wb_vers \
	$(cnf_dir)/pwrs_c_plcnode.wb_vers \
	$(cnf_dir)/pwrs_c_plcprogram.wb_vers \
	$(cnf_dir)/pwrs_c_plcwindow.wb_vers \
	$(cnf_dir)/pwrs_c_system.wb_vers \
	$(cnf_dir)/pwrs_c_systemversion.wb_vers \
	$(cnf_dir)/pwrs_c_typehier.wb_vers

export_wbl := \
	$(pwrs_volume) \
	$(pwrs_hierarchies) \
	$(pwrs_types) \
	$(pwrs_virgin_classes) \
	$(pwrs_typedefs) \
	$(pwrs_classes)

clean_wbl := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_systemclasses.h

lib : $(export_wbl)

exe : $(exe_dir)/pwrs_xtthelp.dat

clean :

realclean : clean $(clean_wbl)

$(inc_dir)/pwr_systemclasses.h :
	@ write sys$$output "Generating struct files for pwrs classes..."
	@ mc pwr_exe:co_convert -s -d pwr_inc: $(to-vms $(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load)

$(doc_dir)/pwrs_allclasses.html :
	@ write sys$$output "Generating html files for pwrs classes..."
	@ mc pwr_exe:co_convert -w -d pwr_doc: $(to_vms $(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load)
	@ write sys$$output "Generating html files for pwrs structs..."
	@ mc pwr_exe:co_convert -c -d pwr_doc: $(inc_dir)/pwr_systemclasses.h

$(exe_dir)/pwrs_xtthelp.dat :
	@ write sys$$output "Generating xtt help files for pwrs classes"
	@ mc pwr_exe:co_convert -x -d pwr_exe: $(to-vms $(pwre_sroot)/wbl/pwrs/src/pwrs_c_*.wb_load)

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_wbl) : clean_%.wb_vers : %.wb_vers
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(cnf_dir)/pwrs_c_alias.wb_vers : pwrs_c_alias.wb_load \
	$(cnf_dir)/pwrs_c_dbcallback.wb_vers
