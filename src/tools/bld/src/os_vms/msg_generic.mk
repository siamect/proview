#  msg_generic.mk -- generic makefile for message files
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a message file.
#
ifndef msg_generic_mk
msg_generic_mk := 1


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

vpath %.msg $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.msg) \
               ), $(notdir $(file)) \
             ) \
           )

export_includes := $(addprefix $(inc_dir)/, $(patsubst %.msg, %.h, $(sources)))
clean_includes := $(patsubst %.msg, clean_%.h, $(sources))

lib_name   = libpwr_msg_$(comp_name)
export_lib = $(lib_dir)/$(lib_name)$(lib_ext)
export_obj = $(obj_dir)/pwr_msg_$(comp_name)$(obj_ext)

.PHONY : all init copy lib exe clean realclean\
         dirs $(clean_includes) clean_lib clean_obj clean_dirs

all : init copy

init : dirs $(export_lib)

copy : $(export_includes) $(export_obj)

lib :

exe :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

clean :

realclean : $(clean_includes) clean_lib clean_obj clean_dirs

$(export_lib) :
	@ $(ar) $(arflags_cre) $(target)
 
$(export_obj) : $(export_includes)
	@ write sys$$output "Extracting $(notdir $(export_obj))"
	@ libr/extract=*/out=$(to-vms $(export_obj)) $(to-vms $(export_lib))
	@ $(pur) $(purflags) $(to-vms $(export_obj))	

clean_lib :
	@ file := $(to-vms $(export_lib))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

clean_obj :
	@ file := $(to-vms $(export_obj))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_includes) : clean_%.h : %.msg
	@ file := $(to-vms $(inc_dir)/$*.h)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

clean_dirs :
	@ file := $(to-vms $(bld_dir)).dir
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'file';*

endif
