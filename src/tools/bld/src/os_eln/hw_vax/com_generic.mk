include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

vpath %.com $(hw_source):$(os_source):$(co_source)
vpath %.cda$$options $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

com_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.com) \
               ), $(notdir $(file)) \
             ) \
           )

export_com := $(patsubst %.com, $(exe_dir)/%.com, $(com_sources))
clean_com := $(patsubst %.com, clean_%.com, $(com_sources))

cda_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.cda$$options) \
               ), $(notdir $(file)) \
             ) \
           )

export_cda := $(patsubst %.cda$$options, $(exe_dir)/%.cda$$options, $(cda_sources))
clean_cda := $(patsubst %.cda$$options, clean_%.cda$$options, $(cda_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_com) $(clean_cda)

all : init copy

init :

copy : $(export_com) $(export_cda)

lib :

exe :

clean :

realclean : $(clean_com) $(clean_cda)

$(export_com) : $(exe_dir)/%.com : %.com
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_com) : clean_%.com : %.com
	@ file := $(to-vms $(exe_dir)/$*.com)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 

$(export_cda) : $(exe_dir)/%.cda$$options : %.cda$$options
	@ write sys$$output "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_cda) : clean_%.cda$$options : %.cda$$options
	@ file := $(to-vms $(exe_dir)/$*.cda$$options)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
