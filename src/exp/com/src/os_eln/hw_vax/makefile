include $(pwre_dir_symbols)
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

vpath %.com $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

com_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.com) \
               ), $(notdir $(file)) \
             ) \
           )

export_com := $(patsubst %.com, $(exe_dir)/%.com, $(com_sources))
clean_com := $(patsubst %.com, clean_%.com, $(com_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_com)

all : init copy

init :

copy : $(export_com)

lib :

exe :

clean :

realclean : clean $(clean_com)

$(export_com) : $(exe_dir)/%.com : %.com
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_com) : clean_%.com : %.com
	@ file := $(to-vms $(exe_dir)/$*.com)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

