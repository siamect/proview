include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

vpath %.h $(co_source)

source_dirs := $(co_source)

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

all : init copy lib exe | silent

init :

copy : $(export_includes) $(exe_dir)/pwrp_rules.mk | silent

lib :

exe :

clean :

realclean : $(clean_includes)

silent :
	@ :

$(clean_includes) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h

$(exe_dir)/pwrp_rules.mk : pwrp_rules.mk
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
