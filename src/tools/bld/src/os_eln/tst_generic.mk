#  tst_generic.mk -- generic makefile for test executables
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build an test executable.
#
ifndef exe_generic_mk
exe_generic_mk := 1
ifndef tst_generic_mk
tst_generic_mk := 1
endif

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




vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.sc $(hw_source):$(os_source):$(co_source)
vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.hpp $(hw_source):$(os_source):$(co_source)
vpath %.cld $(hw_source):$(os_source):$(co_source)
vpath %.sqlmod $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c $(dir)/*.cpp $(dir)/*.sc) \
               ), $(notdir $(file)) \
             ) \
           )

o_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.cld $(dir)/*.sqlmod) \
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


objects := $(addprefix $(bld_dir)/,$(basename $(c_sources)))
objects += $(addprefix $(bld_dir)/,$(basename $(o_sources)))
objects := $(strip $(addsuffix $(obj_ext),$(objects)))

c_src_dep := $(notdir $(basename $(c_sources)))
c_src_dep := $(addprefix $(bld_dir)/, $(c_src_dep))
c_src_dep := $(addsuffix $(d_ext), $(c_src_dep))
exe_src_dep := $(obj_dir)/$(exe_name)$(d_ext)
source_dependencies := $(c_src_dep) $(exe_src_dep)

clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))

.PHONY : all init copy lib exe clean realclean \
         dirs $(clean_h_includes) $(clean_hpp_includes) clean_bld clean_dirs\
	 clean_exe


all : init copy lib exe

init : dirs

copy : $(export_includes)


lib :


exe : $(objects) $(export_obj) $(export_exe)

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

clean : clean_exe

realclean : clean $(clean_h_includes) $(clean_hpp_includes) clean_dirs

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

$(clean_h_includes) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h  

$(clean_hpp_includes) : clean_%.hpp : %.hpp
	@ $(rm) $(rmflags) $(inc_dir)/$*.hpp  

ifeq ($(nodep),)
-include $(source_dependencies)
endif

endif
