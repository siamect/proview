#  msg_generic.mk -- generic makefile for messages
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a message file.
#
ifndef msg_generic_mk
msg_generic_mk := 1

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


vpath %.msg $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.msg) \
               ), $(notdir $(file)) \
             ) \
           )

export_includes := $(addprefix $(inc_dir)/, $(patsubst %.msg, %.h, $(sources)))
clean_includes := $(patsubst %.msg, clean_%.h, $(sources))

cmsgs := $(addprefix $(bld_dir)/,$(basename $(sources)))
cmsgs := $(strip $(addsuffix .cmsg,$(cmsgs)))
export_cmsg = $(obj_dir)/pwr_msg_$(comp_name).cmsg
export_obj = $(obj_dir)/pwr_msg_$(comp_name)$(obj_ext)

.PHONY : all init copy lib exe clean realclean\
         dirs $(clean_includes) clean_lib clean_obj clean_dirs

all : init copy lib

init : dirs 

copy : $(export_includes) $(export_cmsg)

lib : $(export_obj)

exe :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	$(mkdir) $(mkdirflags) $(basename $@)

clean :

realclean : $(clean_includes) clean_obj clean_dirs


$(export_cmsg) : $(cmsgs)
	@ echo Creating: obj_dir/$(notdir $@)
	@ cat $(cmsgs) > $(export_cmsg)


$(export_obj) : $(export_cmsg)


clean_lib :
	@ $(rm) $(rmflags) $(export_lib)

clean_obj :
	@ $(rm) $(rmflags) $(export_obj)

$(clean_includes) : clean_%.h : %.msg
	@ $(rm) $(rmflags) $(inc_dir)/$*.h 

clean_dirs :
	@ $(rmdir) $(rmdirflags) $(bld_dir)
endif
