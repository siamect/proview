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

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.h) \
               ), $(notdir $(file)) \
             ) \
           )

export_includes := $(addprefix $(inc_dir)/,$(sources))
clean_includes := $(patsubst %.h,clean_%.h,$(sources))

.SUFFIXES:

		
.PHONY : all init copy lib exe clean realclean \
         $(clean_includes)

all : init copy lib exe

init :

copy : $(export_includes)

lib :

exe :

clean :

realclean : $(clean_includes)

$(clean_includes) : clean_%.h : %.h
	@ file := $(to-vms $(inc_dir)/$*.h)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';* 
