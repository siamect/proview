include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.xdd $(co_source)

source_dirs := $(co_source)

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

all : init copy | silent

init : silent

copy : $(export_xdd) | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_xdd)

silent :
	@ :

$(export_xdd) : $(exe_dir)/%.xdd : %.xdd
	@ $(log_exp)
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_xdd) : clean_%.xdd : %.xdd
	@ rm $(exe_dir)/$*.xdd
