include $(pwre_dir_symbols)
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

vpath %.opt $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c $(dir)/*.opt) \
               ), $(notdir $(file)) \
             ) \
           )

c_sources := $(filter %.c,$(sources))
opt_sources := $(filter %.opt,$(sources))

export_obj := $(patsubst %.c,$(obj_dir)/%.obj,$(c_sources))
export_opt := $(patsubst %.opt, $(exe_dir)/%.opt, $(opt_sources))
clean_obj := $(patsubst %.c,clean_%.obj,$(c_sources))
clean_opt := $(patsubst %.opt, clean_%.opt, $(opt_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_c) $(clean_opt)

all : init copy lib

init :

copy : $(export_opt)

lib : $(export_obj)

exe :

clean :

realclean : clean $(clean_obj) $(clean_opt) 


$(export_opt) : $(exe_dir)/%.opt : %.opt
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_obj) : clean_%.obj : %.c
	@ file := $(to-vms $(obj_dir)/$*.obj)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

$(clean_opt) : clean_%.opt : %.c
	@ file := $(to-vms $(exe_dir)/$*.opt)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
