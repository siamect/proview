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


vpath %.gif $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

gif_sources := $(filter %.gif,$(sources))

export_gif := $(patsubst %.gif,$(doc_dir)/%.gif,$(gif_sources))
clean_gif := $(patsubst %.gif,clean_%.gif,$(gif_sources))

.SUFFIXES:

$(doc_dir)/%.gif : %.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         $(clean_gif)

all : init copy lib exe

init :

copy : $(export_gif)

lib :

exe :


clean :

realclean : clean $(clean_gif) $(clean_c)

$(clean_gif) : clean_%.gif : %.gif
	@ $(rm) $(rmflags) $(doc_dir)/$*.gif


