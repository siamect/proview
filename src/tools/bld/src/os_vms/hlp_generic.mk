#  hlp_generic.mk -- generic makefile for help files
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a help file.
#
ifndef hlp_generic_mk
hlp_generic_mk := 1


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


vpath %.hlp $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.hlp) \
               ), $(notdir $(file)) \
             ) \
           )

export_libs := $(addprefix $(lib_dir)/, $(patsubst %.hlp, %.hlb, $(sources)))
clean_libs := $(patsubst %.hlp, clean_%.hlb, $(sources))

#lib_name   = libpwr_msg_$(comp_name)
#export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe clean realclean\
         $(clean_includes)

all : copy

init :

copy : $(export_libs)

lib :

exe :

clean :

realclean : $(clean_libs)

$(clean_libs) : clean_%.hlb : %.hlp
	@ file := $(to-vms $(lib_dir)/$*.hlb)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
endif
