#  lib_generic.mk -- generic makefile for libraries
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a library.
#
ifndef lib_generic_mk
lib_generic_mk := 1


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

vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.x $(hw_source):$(os_source):$(co_source)
vpath %.pdr $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)


c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.c) \
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

xdr_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.x) \
               ), $(notdir $(file)) \
             ) \
           )

pdr_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.pdr) \
               ), $(notdir $(file)) \
             ) \
           )


xdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.x, %.h, $(xdr_sources)))
xdr_objects := $(patsubst %.x, %_xdr.obj, $(xdr_sources))

pdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.pdr, %.h, $(pdr_sources)))
pdr_objects := $(patsubst %.pdr, %_pdr.obj, $(pdr_sources))

export_includes := $(addprefix $(inc_dir)/, $(includes))
export_includes += $(xdr_includes) $(pdr_includes)
clean_includes := $(patsubst %.h, clean_%.h, $(includes))


objects := $(addsuffix $(obj_ext), $(basename $(c_sources)))
objects += $(xdr_objects) $(pdr_objects)

source_dependencies := $(notdir $(basename $(c_sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))


lib_name   = libpwr_$(comp_name)
export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe clean realclean \
	 dirs $(clean_includes) \
	 clean_bld clean_dirs clean_lib

all : init copy lib exe

init : dirs $(export_lib)

copy : $(export_includes)

lib : $(export_lib)($(objects))

exe :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

$(export_lib) :
	@ $(ar) $(arflags_cre) $(target)
 
clean : clean_bld clean_lib

realclean : clean $(clean_includes) clean_dirs

clean_bld :
	@ files := $(to-vms $(bld_dir)/)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*

clean_dirs :
	@ file := $(to-vms $(bld_dir)).dir
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'file';*

clean_lib :
	@ file := $(to-vms $(export_lib))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_includes) : clean_%.h : %.h
	@ file := $(to-vms $(inc_dir)/$*.h)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 


-include $(source_dependencies)

endif
