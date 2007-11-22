include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_croot)/src/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_croot)/src/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/$(os_name)/rules.mk
endif


.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib

init :

copy : $(lib_dir)/libusbio.a $(inc_dir)/libusbio.h

lib :

exe :

clean :

realclean : clean $(clean_obj) 

.SUFFIXES:

$(lib_dir)/libusbio.a : ../libusbio.a
	@ echo "Copying libusbio.a"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/libusbio.h : ../libusbio.h
	@ echo "Copying libusbio.h"
	@ $(cp) $(cpflags) $(source) $(target)






