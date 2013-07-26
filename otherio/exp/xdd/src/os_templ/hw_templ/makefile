include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif

ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif

vpath %.xdd $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

xdd_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.xdd) \
               ), $(notdir $(file)) \
             ) \
           )

export_xdd := $(patsubst %.xdd, $(exe_dir)/%.xdd, $(xdd_sources))
clean_xdd := $(patsubst %.xdd, clean_%.xdd, $(xdd_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_xdd)

all : init copy

init :

copy : $(export_xdd)

lib :

exe :

clean :

realclean : clean $(clean_xdd)

$(export_xdd) : $(exe_dir)/%.xdd : %.xdd
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_xdd) : clean_%.xdd : %.xdd
	@ rm $(exe_dir)/$*.xdd


