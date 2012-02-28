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

copy : $(inc_dir)/libusbio.h $(inc_dir)/openSocket.h $(inc_dir)/setport.h $(inc_dir)/pwr_arduino_uno.pde

lib :

exe :

clean :

realclean : clean $(clean_obj) 

.SUFFIXES:

#$(lib_dir)/libusbio.a : ../libusbio.a
#	@ echo "Copying libusbio.a"
#	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/libusbio.h : ../../libusbio.h
	@ echo "Copying libusbio.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/openSocket.h : ../../openSocket.h
	@ echo "Copying openSocket.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/setport.h : ../../setport.h
	@ echo "Copying setport.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/pwr_arduino_uno.pde : ../../pwr_arduino_uno.pde
	@ echo "Copying pwr_arduino_uno.pde"
	@ $(cp) $(cpflags) $(source) $(target)






