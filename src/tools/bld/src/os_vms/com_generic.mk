#  com_generic.mk -- generic makefile for command procedure files
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To copy a command file.
#
ifndef com_generic_mk
com_generic_mk := 1


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


vpath %.com $(hw_source):$(os_source):$(co_source)
vpath %.cda_options $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

com_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.com) \
               ), $(notdir $(file)) \
             ) \
           )

export_com := $(patsubst %.com, $(exe_dir)/%.com, $(com_sources))
clean_com := $(patsubst %.com, clean_%.com, $(com_sources))

cda_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.cda_options) \
               ), $(notdir $(file)) \
             ) \
           )

export_cda := $(patsubst %.cda_options, $(exe_dir)/%.cda_options, $(cda_sources))
clean_cda := $(patsubst %.cda_options, clean_%.cda_options, $(cda_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_com) $(clean_cda)

all : init copy

init :

copy : $(export_com) $(export_cda)

lib :

exe :

clean :

realclean : $(clean_com) $(clean_cda)

$(export_com) : $(exe_dir)/%.com : %.com
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_com) : clean_%.com : %.com
	@ file := $(to-vms $(exe_dir)/$*.com)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

$(export_cda) : $(exe_dir)/%.cda_options : %.cda_options
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_cda) : clean_%.cda_options : %.cda_options
	@ file := $(to-vms $(exe_dir)/$*.cda_options)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

endif
