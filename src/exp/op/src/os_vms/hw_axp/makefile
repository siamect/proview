include $(pwre_dir_symbols)
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
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.dat $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.h $(dir)/*.c $(dir)/*.dat) \
               ), $(notdir $(file)) \
             ) \
           )

h_sources := $(filter %.h,$(sources))
c_sources := $(filter %.c,$(sources))
dat_sources := $(filter %.dat,$(sources))

export_h := $(patsubst %.h,$(inc_dir)/%.h,$(h_sources))
export_c := $(patsubst %.c,$(src_dir)/%.c,$(c_sources))
export_dat := $(patsubst %.dat,$(src_dir)/%.dat,$(dat_sources))
clean_h := $(patsubst %.h,clean_%.h,$(h_sources))
clean_c := $(patsubst %.c,clean_%.c,$(c_sources))
clean_dat := $(patsubst %.dat,clean_%.dat,$(dat_sources))

.SUFFIXES:

$(inc_dir)/%.h : %.h
	@ $(cp) $(cpflags) $(source) $(target)
		
$(src_dir)/%.c : %.c
	@ $(cp) $(cpflags) $(source) $(target)
		
$(src_dir)/%.dat : %.dat
	@ $(cp) $(cpflags) $(source) $(target)
		
.PHONY : all init copy lib exe clean realclean \
         clean_obj $(clean_h) $(clean_c) $(clean_dat)

all : init copy lib exe

init :

copy : $(export_h) $(export_c) $(export_dat)

lib : $(obj_dir)/op_appltable$(obj_ext) $(obj_dir)/op_appltypetable$(obj_ext)

exe :


clean : clean_obj

realclean : clean $(clean_h) $(clean_c) $(clean_dat)

$(obj_dir)/op_appltable$(obj_ext) : op_appltable.c
$(obj_dir)/op_appltypetable$(obj_ext) : op_appltable.c

clean_obj :
	@ file := $(to-vms $(obj_dir)/op_appltable$(obj_ext))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(obj_dir)/op_appltypetable$(obj_ext))
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_h) : clean_%.h : %.h
	@ file := $(to-vms $(inc_dir)/$*.h)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

$(clean_c) : clean_%.c : %.c
	@ file := $(to-vms $(src_dir)/$*.c)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

$(clean_dat) : clean_%.dat : %.dat
	@ file := $(to-vms $(src_dir)/$*.dat)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
