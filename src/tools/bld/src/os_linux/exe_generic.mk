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


ifndef link_rule_mk
  link_rule := link_rule.mk
  -include $(link_rule)
endif
ifndef link_rule_mk
  link_rule := ../link_rule.mk  
  -include $(link_rule)
endif
ifndef link_rule_mk
  link_rule := $(pwre_sroot)/tools/bld/src/$(os_name)/link_rule_generic.mk
  include $(link_rule)
endif



vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.l $(hw_source):$(os_source):$(co_source)
vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.hpp $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c $(dir)/*.cpp) \
               ), $(notdir $(file)) \
             ) \
           )

l_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.l) \
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

exe_name   := $(comp_name)
exe_source := $(filter $(exe_name).%, $(c_sources))

c_sources  := $(filter-out $(exe_source), $(c_sources))

export_exe := $(exe_dir)/$(exe_name)$(exe_ext)
export_obj := $(obj_dir)/$(exe_name)$(obj_ext)
export_includes := $(addprefix $(inc_dir)/,$(h_includes) $(hpp_includes))

l_targets := $(addprefix $(bld_dir)/,$(basename $(l_sources)))
l_targets := $(addsuffix $(c_ext),$(l_targets))

objects := $(addprefix $(bld_dir)/,$(basename $(c_sources)))
objects += $(addprefix $(bld_dir)/,$(basename $(l_sources)))
objects := $(strip $(addsuffix $(obj_ext),$(objects)))

lc_src_dep := $(objects:$(obj_ext)=$(d_ext)) 
exe_src_dep := $(obj_dir)/$(exe_name)$(d_ext)
source_dependencies := $(lc_src_dep) $(exe_src_dep)
link_dependencies := $(bld_dir)/$(exe_name)$(d_ld_ext)

clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))




.PHONY : all init copy lib exe clean realclean \
         dirs $(clean_includes) clean_bld clean_dirs\
	 clean_exe

all : init copy lib exe

init : dirs

copy : $(export_includes)

lib :

exe : $(l_targets) $(objects) $(export_obj) $(export_exe)

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)

clean : clean_exe clean_bld

realclean : clean $(clean_h_includes) $(clean_hpp_includes) clean_dirs

$(export_exe) : $(link_rule) $(export_obj) $(objects)
	@ $(log_link_exe)
	@ $(link)


# This is for Lynx 2.5 map files, it doesn't work for Linux. ML 
# ifeq ($(nodep),)
#	@ echo "$(export_exe) : \\" > $(link_dependencies)
#	@ $ sed -n "s|\(.*\)\($(lib_dir)/.*\.a\)\(\]\)\(.*\.o\)\(.*\)|\2(\4) \\\\|p" $(map) \
#	  | sort | uniq >> $(link_dependencies) 
# endif

clean_exe :
	@ if [ -e $(export_exe) ]; then \
		$(rm) $(export_exe); \
	fi
	@ if [ -e $(export_obj) ]; then \
		$(rm) $(export_obj); \
	fi

clean_bld :
	@ $(rm) $(rmflags) $(bld_dir)/*

clean_dirs :
	@ $(rm) $(rmflags) $(bld_dir)

$(clean_h_includes) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h  

$(clean_hpp_includes) : clean_%.hpp : %.hpp
	@ $(rm) $(rmflags) $(inc_dir)/$*.hpp  


-include $(source_dependencies)
-include $(link_dependencies)


endif
