#  wbl_generic.mk -- generic makefile for wb_load-files
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To load a wb_load-file.
#
ifndef wbl_generic_mk
wbl_generic_mk := 1


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
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

vpath %.wb_load $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

wbl_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.wb_load) \
               ), $(notdir $(file)) \
             ) \
           )

export_wbl := $(patsubst %.wb_load,$(cnf_dir)/%.wb_vers,$(wbl_sources))
clean_wbl := $(patsubst %.wb_load,clean_%.wb_load,$(wbl_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy

init :

copy : $(export_wbl)

lib :

exe :

clean : $(clean_wbl)

realclean : clean

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#	@ write sys$$output "Exporting $< ..."
#	@ $(copy) $(copyflags) $(source) $(target)
#	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_wbl) : clean_%.wb_load : %.wb_load
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

endif
