include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

#vpath %.msg $(hw_source):$(os_source):$(co_source)

#source_dirs = $(hw_source) $(os_source) $(co_source)

#sources := $(sort \
#             $(foreach file, \
#               $(foreach dir, \
#                 $(source_dirs), \
#                 $(wildcard $(dir)/$(comp_name)*.msg) \
#               ), $(notdir $(file)) \
#             ) \
#           )

#export_includes := $(addprefix $(inc_dir)/, $(patsubst %.msg, %.h, $(sources)))
#clean_includes := $(patsubst %.msg, clean_%.h, $(sources))

#lib_name   = libpwr_msg_$(comp_name)
#export_lib = $(lib_dir)/$(lib_name)$(lib_ext)

.PHONY : all init copy lib exe clean realclean\
         $(clean_includes)

all : init copy

init : $(export_lib)

copy : $(export_includes)

lib :

exe :

clean :

realclean : $(clean_includes) clean_lib

#$(export_lib) :
#	@ $(ar) $(arflags_cre) $(target)
 
#clean_lib :
#	@ file := $(to-vms $(export_lib))
#	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

#$(clean_includes) : clean_%.h : %.msg
#	@ file := $(to-vms $(inc_dir)/$*.h)
#	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
