include $(pwre_dir_symbols)
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk

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

$(inc_dir)/%.h : %.h
	@ $(cp) $(cpflags) $(source) $(target)
		
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
