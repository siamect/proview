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

vpath %.pwg $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

export_ge := $(addprefix $(exe_dir)/,$(sources))
clean_ge := $(patsubst %.pwg,clean_%.pwg,$(sources))

.SUFFIXES:

		
.PHONY : all init copy lib exe clean realclean \
         $(clean_ge)

all : init copy lib exe

init :

copy : $(export_ge)

lib :

exe :

clean :

realclean : $(clean_ge)

$(clean_ge) : clean_%.pwg : %.pwg
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwg

