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

copy : $(inc_dir)/libusbio.h $(inc_dir)/openSocket.h $(inc_dir)/setport.h $(inc_dir)/pwr_arduino_uno.ino | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_obj)

silent :
	@ :

.SUFFIXES:

#$(lib_dir)/libusbio.a : ../libusbio.a
#	@ echo "Copying libusbio.a"
#	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/libusbio.h : libusbio.h
	@ echo "Copying libusbio.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/openSocket.h : openSocket.h
	@ echo "Copying openSocket.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/setport.h : setport.h
	@ echo "Copying setport.h"
	@ $(cp) $(cpflags) $(source) $(target)

$(inc_dir)/pwr_arduino_uno.ino : pwr_arduino_uno.ino
	@ echo "Copying pwr_arduino_uno.ino"
	@ $(cp) $(cpflags) $(source) $(target)
