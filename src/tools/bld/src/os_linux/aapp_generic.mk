#  lib_generic.mk -- generic makefile for libraries
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a library.
#
ifndef aapp_generic_mk
aapp_generic_mk := 1


ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif

appname := PwrXtt
packagename := jpwr
app_src_dir := $(release_root)/bld/$(type_name)/$(appname)/src
app_res_dir := $(release_root)/bld/$(type_name)/$(appname)/res
app_assets_dir := $(release_root)/bld/$(type_name)/$(appname)/assets
app_package_dir = $(app_src_dir)/$(packagename)

ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif


vpath %.java $(hw_source):$(os_source):$(co_source):$(pwre_sroot)/jpwr/$(comp_name)/src
vpath %.gif $(hw_source):$(os_source):$(co_source)
vpath %.png $(hw_source):$(os_source):$(co_source)
#VPATH = $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

png_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
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


jpwr_name   := pwr_$(comp_name)
export_lib := $(lib_dir)/$(jpwr_name).jar

java_objects := $(addprefix $(app_package_dir)/$(comp_name)/, $(notdir $(addsuffix .java, $(basename $(extern_java_sources))))) $(addprefix $(app_package_dir)/$(comp_name)/, $(notdir $(addsuffix .java, $(basename $(java_sources))))) 
png_objects := $(addprefix $(bld_dir)/, $(png_sources))
objects := $(png_objects) $(java_objects)

source_dependencies := $(notdir $(basename $(sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

.PHONY : all init copy lib exe clean realclean \
	 dirs clean_includes \
	 clean_bld clean_dirs clean_lib

#	 dirs $(clean_h_includes) $(clean_hpp_includes)

all : init copy lib exe

init : dirs

copy : $(objects) $(local_objects)

lib :

exe :

dirs : 

#$(app_package_dir)/$(comp_name)$(dir_ext) $(app_res_dir)$(dir_ext)

 
 
clean : clean_bld

realclean : clean clean_dirs

clean_bld :
	@ echo "Cleaning build directory"
	@ $(rm) $(rmflags) $(bld_dir)/*

clean_dirs :
	@ echo "Removing build directory"
	@ $(rm) -r $(rmflags) $(bld_dir)

$(export_lib) : $(objects)
	@ echo "Copy $(notdir $(bld_lib))"

#-include $(source_dependencies)

endif











