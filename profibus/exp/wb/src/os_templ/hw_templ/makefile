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

copy : $(exe_dir)/pb_module_classes.dat $(exe_dir)/pn_module_classes.dat

lib :

exe :

clean :

realclean : clean $(clean_obj) 

.SUFFIXES:

$(exe_dir)/pb_module_classes.dat : ../../pb_module_classes.dat
	@ echo "Copying $(source)"
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/pn_module_classes.dat : ../../pn_module_classes.dat
	@ echo "Copying $(source)"
	@ $(cp) $(cpflags) $(source) $(target)



