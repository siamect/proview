#  exe_generic.mk -- generic makefile for executables
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build an executable.
#
ifndef exe_generic_mk
exe_generic_mk := 1

ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif

ifndef rules_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif

vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.h $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c) \
               ), $(notdir $(file)) \
             ) \
           )

includes := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.h) \
               ), $(notdir $(file)) \
             ) \
           )

exe_name   := $(comp_name)
exe_source := $(comp_name)
sources    := $(filter-out $(exe_source), $(sources))

export_exe := $(exe_dir)/$(exe_name)$(exe_ext)
export_obj := $(obj_dir)/$(exe_name)$(obj_ext)
export_includes := $(addprefix $(inc_dir)/,$(includes))

objects := $(filter-out $(exe_name).%,$(sources))
objects := $(addprefix $(bld_dir)/,$(basename $(objects)))
objects := $(strip $(addsuffix .obj/,$(objects)))

src_dep := $(notdir $(basename $(sources)))
src_dep := $(addprefix $(bld_dir)/, $(src_dep))
src_dep := $(addsuffix $(d_ext), $(src_dep))
exe_src_dep := $(obj_dir)/$(exe_name)$(d_ext)
source_dependencies := $(src_dep) $(exe_src_dep)
link_dependencies := $(bld_dir)/$(exe_name)$(d_ld_ext)

clean_includes := $(patsubst %.h,clean_%.h, $(includes))

.PHONY : all init copy lib exe clean realclean \
         dirs $(clean_includes) clean_bld clean_dirs\
	 clean_exe

all : init copy lib exe

init : dirs

copy : $(export_includes)

lib :

exe : $(export_obj) $(export_exe)

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

clean : clean_exe

realclean : clean $(clean_includes) clean_dirs

$(export_exe) : $(exe_name)$(opt_ext) $(export_obj) $(objects)

clean_exe :
	@ file := $(to-vms $(export_exe))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(export_obj))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

clean_bld :
	@ files := $(to-vms $(bld_dir)/)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*

clean_dirs :
	@ file := $(to-vms $(bld_dir)).dir
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_includes) : clean_%.h : %.h
	@ file := $(to-vms $(inc_dir)/$*.h)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 


-include $(source_dependencies)
-include $(link_dependencies)

endif
