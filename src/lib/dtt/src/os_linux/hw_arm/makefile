#  makefile -- makefile for dtt library
#
#  PROVIEW/R
#  Copyright (C) 2000 Mandator AB.
#
#  To build a dtt library.
#
include $(pwre_dir_symbols)

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
vpath %.hpp $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.x $(hw_source):$(os_source):$(co_source)
vpath %.dtt $(hw_source):$(os_source):$(co_source)
vpath %.dtt_m $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.c $(dir)/$(comp_name)*.cpp $(dir)/$(comp_name)*.sc) \
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

xdr_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.x) \
               ), $(notdir $(file)) \
             ) \
           )

dtt_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.dtt) \
               ), $(notdir $(file)) \
             ) \
           )

xdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.x, %.h, $(xdr_sources)))
xdr_objects := $(patsubst %.x, %_xdr.o, $(xdr_sources))

export_includes := $(addprefix $(inc_dir)/, $(includes))
export_includes += $(xdr_includes)
export_dtt_m := $(bld_dir)/dtt_appl_rttsys_m.c
export_obj := $(addprefix $(obj_dir)/, $(c_sources))
export_obj := $(patsubst %.c, %.o, $(export_obj))

h_includes := $(filter %.h, $(includes))
hpp_includes := $(filter %.hpp, $(includes))
clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))


objects := $(addsuffix $(obj_ext), $(basename $(dtt_sources)))
#objects += $(xdr_objects)


source_dependencies := $(notdir $(basename $(c_sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

lib_name   = libpwr_$(comp_name)
export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe $(clean realclean \
	 dirs $(clean_h_includes) $(clean_hpp_includes) \
	 clean_bld clean_dirs clean_lib export_stuff

all : init copy lib exe

init : dirs

copy : $(export_includes)

lib :   $(export_dtt_m)\
	$(inc_dir)/dtt_appl_rttsys_m.rdecl\
	$(inc_dir)/dtt_appl_rttsys_m.rhlp\
	$(inc_dir)/dtt_appl_rttsys_m.rh\
	$(export_lib)($(objects))\
	$(export_obj)
 
exe :

dirs : $(bld_dir)$(dir_ext)

$(inc_dir)/dtt_appl_rttsys_m.rdecl : $(bld_dir)/dtt_appl_rttsys_m.rdecl
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)

$(inc_dir)/dtt_appl_rttsys_m.rhlp : ../../dtt_appl_rttsys_m.rhlp
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)

$(inc_dir)/dtt_appl_rttsys_m.rh : $(bld_dir)/dtt_appl_rttsys_m.rh
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)

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
	@ echo "Building archive $(notdir $(export_lib))"
	@ $(rm) $(export_lib)
	@ $(ar) -cr $(export_lib) $(objects)

$(obj_dir)/%.o : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cc) -MM  -I$(bld_dir) $(cinc) $(csetos) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(obj_dir)/&|g'\'' > $(obj_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(csetos) -I$(bld_dir) $(cinc) $(cobj) $(source)

#$(obj_dir)/%.o : %.c
#	@ $(log_c_obj)
#	 $(cc) $(cflags) $(depmms) -I$(bld_dir) $(cinc) $(csetos) $(cobj) $(clis) $(source) $(clib) 

$(export_lib)(%.o) : %.dtt
	@ echo "Generate $(sname)"
	@ $(wb_rtt) rttsys generate/id=$(sname) arm_linux

$(bld_dir)/%.c : %.dtt_m
	@ echo "Generate menu"
	@ $(wb_rtt) rttsys generate/menu arm_linux

-include $(source_dependencies)

endif
