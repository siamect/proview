#  lib_generic.mk -- generic makefile for libraries
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a library.
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
vpath %.sc $(hw_source):$(os_source):$(co_source)
vpath %.cld $(hw_source):$(os_source):$(co_source)
vpath %.graph $(hw_source):$(os_source):$(co_source)
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

dtt_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.dtt) \
               ), $(notdir $(file)) \
             ) \
           )

xdr_includes := $(addprefix $(inc_dir)/,$(patsubst %.x, %.h, $(xdr_sources)))
xdr_objects := $(patsubst %.x, %_xdr.obj, $(xdr_sources))

export_includes := $(addprefix $(inc_dir)/, $(includes))
export_includes += $(xdr_includes)
export_dtt_m := $(bld_dir)/dtt_appl_rttsys_m.c
export_obj := $(addprefix $(obj_dir)/, $(c_sources))
export_obj := $(patsubst %.c, %.obj, $(export_obj))

h_includes := $(filter %.h, $(includes))
hpp_includes := $(filter %.hpp, $(includes))
clean_h_includes := $(patsubst %.h,clean_%.h, $(h_includes))
clean_hpp_includes := $(patsubst %.hpp,clean_%.hpp, $(hpp_includes))


export_graph := $(addprefix $(db_dir)/,$(graph_sources))
clean_graph := $(patsubst %.graph, clean_%.graph, $(graph_sources))

objects := $(addsuffix $(obj_ext), $(basename $(dtt_sources)))
#objects += $(xdr_objects)


source_dependencies := $(notdir $(basename $(c_sources)))
source_dependencies := $(addprefix $(bld_dir)/, $(source_dependencies))
source_dependencies := $(addsuffix $(d_ext), $(source_dependencies))

lib_name   = libpwr_$(comp_name)
export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe $(clean realclean \
	 dirs $(clean_h_includes) $(clean_hpp_includes) %(clean_graph) \
	 clean_bld clean_dirs clean_lib export_stuff

all : init copy lib exe

init : dirs $(export_lib)

copy : $(export_includes) $(export_graph)

lib : $(export_dtt_m)\
	$(inc_dir)/dtt_appl_rttsys_m.rdecl\
	$(inc_dir)/dtt_appl_rttsys_m.rhlp\
	$(inc_dir)/dtt_appl_rttsys_m.rh\
	$(export_lib)($(objects))\
	$(export_obj)

exe :

dirs : $(bld_dir)$(dir_ext)

$(inc_dir)/dtt_appl_rttsys_m.rdecl : $(bld_dir)/dtt_appl_rttsys_m.rdecl
	@ copy/log $(source) $(target)

$(inc_dir)/dtt_appl_rttsys_m.rhlp : ../../dtt_appl_rttsys_m.rhlp
	@ copy/log $(source) $(target)

$(inc_dir)/dtt_appl_rttsys_m.rh : $(bld_dir)/dtt_appl_rttsys_m.rh
	@ copy/log $(source) $(target)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

$(export_lib) :
	@ $(ar) $(arflags_cre) $(target)

clean : clean_bld clean_lib

realclean : clean $(clean_h_includes) $(clean_hpp_includes) $(clean_graph) \
            clean_dirs

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


#$(bld_dir)/%. : %.dtt
#	@ write sys$$output "$(source)"
#	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(obj_dir)/%.obj : %.c
	@ $(log_c_obj)
	@ define/user pwrp_rtt $(sdir)
	@ define/user pwrp_rttbld $(to-vms $(bld_dir)/)
	@ $(cc) $(cflags) $(depmms) $(cbldinc) $(csetos) $(cobj) $(clis) $(source) $(clib) 
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

(%.obj) : %.dtt
	@ write sys$$output "$(source)"
	@ mc pwr_exe:wb_rtt rttsys generate/id=$(sname)

$(bld_dir)/%.c : %.dtt_m
	@ write sys$$output "$(source)"
	@ mc pwr_exe:wb_rtt rttsys generate/menu

#	@ $(cc) $(cflags) $(cinc) $(clibobj) $(clis) $(source) $(clib) 
#	@ dep_mms2make $(tdepmms) $(bld_dir)/$(sname).d $(depaddlib) $(export_lib)  
#	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname).d)

#ifeq ($(nodep),)
#-include $(source_dependencies)
#endif

endif
