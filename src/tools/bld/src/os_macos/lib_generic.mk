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
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif

source_dirs = $(hw_source) $(os_source) $(co_source)

-include ../../lib_local.mk
-include ../lib_local.mk
-include lib_local.mk


vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.hpp $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.x $(hw_source):$(os_source):$(co_source)
vpath %.pdr $(hw_source):$(os_source):$(co_source)
vpath %.java $(hw_source):$(os_source):$(co_source)
vpath %.pwsg $(hw_source):$(os_source):$(co_source)
vpath %.meth $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.c $(dir)/$(comp_name)*.cpp) \
               ), $(notdir $(file)) \
             ) \
           )

h_includes := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.h) \
               ), $(notdir $(file)) \
             ) \
           )

hpp_includes := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.hpp) \
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

java_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.java) \
               ), $(notdir $(file)) \
             ) \
           )

pwsg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pwsg) \
               ), $(notdir $(file)) \
             ) \
           )

meth_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.meth) \
               ), $(notdir $(file)) \
             ) \
           )

xdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.x, %.h, $(xdr_sources)))
xdr_objects := $(patsubst %.x, %_xdr.o, $(xdr_sources))

pdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.pdr, %.h, $(pdr_sources)))
pdr_objects := $(patsubst %.pdr, %_pdr.o, $(pdr_sources))


export_includes := $(addprefix $(inc_dir)/,$(h_includes) $(hpp_includes))
export_includes += $(xdr_includes) $(pdr_includes)

export_pwsg := $(addprefix $(exe_dir)/,$(pwsg_sources))
export_meth := $(addprefix $(inc_dir)/,$(meth_sources))

clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))


lib_name   := libpwr_$(comp_name)
ifneq ($(src_name),src)
  lib_name := libpwr_$(comp_name)_$(src_name)
endif
export_lib := $(lib_dir)/$(lib_name)$(lib_ext)

java_classes := $(addsuffix .class, $(basename $(java_sources)))
objects := $(addsuffix $(obj_ext), $(basename $(sources)))
objects += $(xdr_objects) $(pdr_objects)
objects += $(java_classes)
objects	:= $(addprefix $(bld_dir)/, $(objects))

source_dependencies := $(notdir $(basename $(sources) $(pdr_objects) $(xdr_objects)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

.PHONY : all init copy lib exe $(l_copy) clean realclean \
	 dirs clean_includes \
	 clean_bld clean_dirs clean_lib $(l_clean_copy)

#	 dirs $(clean_h_includes) $(clean_hpp_includes)

all : init copy lib exe

init : dirs

copy : $(export_includes) $(l_copy) $(export_pwsg) $(export_meth)

lib : $(export_lib)

exe :

echo: 
	echo $(source_dependencies)

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)/

clean : clean_bld clean_lib

clean_copy: $(l_clean_copy)

realclean : clean clean_includes clean_pwsg clean_dirs

clean_bld :
	@ echo "Cleaning build directory"
	@ $(rm) $(rmflags) $(bld_dir)/*

clean_dirs :
	@ echo "Removing build directory"
	@ $(rm) -r $(rmflags) $(bld_dir)

clean_lib :
	@if [ -e $(export_lib) ]; then \
		echo "Removing archive $(notdir $(export_lib))" \
		$(rm) $(export_lib); \
	fi


clean_includes : $(clean_h_includes) $(clean_hpp_includes)

$(clean_h_includes) : clean_%.h : %.h
	@ echo "Removing h-include: $(inc_dir)/$*.h"
	@ $(rm) $(rmflags) $(inc_dir)/$*.h  

$(clean_hpp_includes) : clean_%.hpp : %.hpp
	@ echo "Removing hpp-include: $(inc_dir)/$*.hpp"
	@ $(rm) $(rmflags) $(inc_dir)/$*.hpp  

$(clean_pwsg) : clean_%.pwsg : %.pwsg
	@ echo "Removing pwsg: $(exe_dir)/$*.pwsg"
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwsg  

$(export_lib) : $(objects)
	@ echo "Building archive $(notdir $(export_lib))"
	@ if [ "$(objects)" != "" ]; then \
	    $(ar) -cr $(export_lib) $(objects); \
	fi

#	@ if [ -e $(export_lib) ]; then \
#		$(rm) $(export_lib); \
#	fi

-include $(source_dependencies)

endif
