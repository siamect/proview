#  special.mk -- special makefile for rt_client
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build library pwr_rt_client.
#  This is a parsitic library that steals everything from jpwr/rt, except
#  the Gdh class.
#  When compiling Gdh.java it is placed in jpwr/rt and everything in jpwr/rt
#  is then copied into pwr_rt_client.jar. The new Gdh.class is then deleted.
#
ifndef jpwr_generic_mk
jpwr_generic_mk := 1


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


vpath %.java $(hw_source):$(os_source):$(co_source)
vpath %.gif $(hw_source):$(os_source):$(co_source)
#VPATH = $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

java_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.java) \
               ), $(notdir $(file)) \
             ) \
           )

gif_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )


jpwr_name   := pwr_$(comp_name)
export_lib := $(lib_dir)/$(jpwr_name).jar

java_classes := $(addsuffix .class, $(basename $(local_java_sources)))
gif_objects := $(addprefix $(bld_dir)/, $(gif_sources))
java_objects := $(addprefix $(bld_dir)/, $(java_classes))
objects := $(gif_objects) $(java_objects)

source_dependencies := $(notdir $(basename $(sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

.PHONY : all init copy lib exe clean realclean \
	 dirs clean_includes \
	 clean_bld clean_dirs clean_lib

#	 dirs $(clean_h_includes) $(clean_hpp_includes)

all : init copy lib exe

init : dirs

copy : 

lib : $(export_lib)

exe :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)/
 
clean : clean_bld clean_lib

realclean : clean clean_includes clean_dirs

clean_bld :
	@ echo "Cleaning build directory"
	@ $(rm) $(rmflags) $(bld_dir)/*

clean_dirs :
	@ echo "Removing build directory"
	@ $(rm) -r $(rmflags) $(bld_dir)

clean_lib :
	@ echo "Removing archive $(notdir $(export_lib))"
	@ $(rm) $(rmflags) $(export_lib)

clean_includes : $(clean_h_includes) $(clean_hpp_includes)

$(clean_h_includes) : clean_%.h : %.h
	@ echo "Removing h-includes"
	@ $(rm) $(rmflags) $(inc_dir)/$*.h  

$(clean_hpp_includes) : clean_%.hpp : %.hpp
	@ echo "Removing hpp-includes"
	@ $(rm) $(rmflags) $(inc_dir)/$*.hpp  

$(export_lib) : $(objects)
	@ echo "Building archive $(notdir $(export_lib)) special"
	@ if [ -e $(export_lib) ]; then \
		$(rm) $(export_lib); \
	  fi
	@ cd $(pwre_broot)/$(pwre_target)/bld; \
	 jar cf $(export_lib) jpwr/rt/*.class; \
	 rm jpwr/rt/Gdh.class;

#-include $(source_dependencies)

endif





