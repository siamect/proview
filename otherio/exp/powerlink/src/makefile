include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib | silent

init : silent

copy : $(inc_dir)/objdict.h | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_obj)

silent :
	@ :

.SUFFIXES:

$(inc_dir)/objdict.h : objdict.h
	@ echo "Copying objdict.h"
	@ $(cp) $(cpflags) $(source) $(target)
