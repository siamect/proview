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

source_dirs = $(hw_source) $(os_source) $(co_source)

-include ../../lib_local.mk
-include ../lib_local.mk
-include lib_local.mk

vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.hpp $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.sc $(hw_source):$(os_source):$(co_source)
vpath %.cld $(hw_source):$(os_source):$(co_source)
vpath %.graph $(hw_source):$(os_source):$(co_source)
vpath %.x $(hw_source):$(os_source):$(co_source)
vpath %.pdr $(hw_source):$(os_source):$(co_source)

c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.c $(dir)/$(comp_name)*.cpp $(dir)/$(comp_name)*.sc) \
               ), $(notdir $(file)) \
             ) \
           )

o_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.cld) \
               ), $(notdir $(file)) \
             ) \
           )

includes := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.h $(dir)/$(comp_name)*.hpp) \
               ), $(notdir $(file)) \
             ) \
           )

graph_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.graph) \
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
pdr_objects := $(patsubst %.pdr, %_pdr.o, $(pdr_sources))

export_includes := $(addprefix $(inc_dir)/, $(includes))
export_includes += $(xdr_includes) $(pdr_includes)

h_includes := $(filter %.h, $(includes))
hpp_includes := $(filter %.hpp, $(includes))
clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))


export_graph := $(addprefix $(db_dir)/,$(graph_sources))
clean_graph := $(patsubst %.graph, clean_%.graph, $(graph_sources))

objects := $(addsuffix $(obj_ext), $(basename $(c_sources) $(o_sources)))
objects += $(xdr_objects) $(pdr_objects)


source_dependencies := $(notdir $(basename $(c_sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

lib_name   = libpwr_$(comp_name)
export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe $(l_copy) $(clean realclean \
	 dirs $(clean_h_includes) $(clean_hpp_includes) %(clean_graph) \
	 clean_bld clean_dirs clean_lib clean_copy $(l_clean_copy)

all : init copy lib exe

init : dirs $(export_lib)

copy : $(export_includes) $(export_graph) $(l_copy)

lib : $(export_lib)($(objects))

exe :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

$(export_lib) :
	@ $(ar) $(arflags_cre) $(target)
 
clean : clean_bld clean_lib

clean_copy: $(l_clean_copy)

realclean : clean $(clean_h_includes) $(clean_hpp_includes) $(clean_graph) $(clean_pwsg)\
            clean_copy clean_dirs

clean_bld :
	@ files := $(to-vms $(bld_dir)/)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*

clean_dirs :
	@ file := $(to-vms $(bld_dir)).dir
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'file';*

clean_lib :
	@ file := $(to-vms $(export_lib))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_h_includes) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h  

$(clean_hpp_includes) : clean_%.hpp : %.hpp
	@ $(rm) $(rmflags) $(inc_dir)/$*.hpp  

$(clean_graph) : clean_%.graph : %.graph
	@ file := $(to-vms $(db_dir)/$*.graph)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

ifeq ($(nodep),)
-include $(source_dependencies)
endif

endif
