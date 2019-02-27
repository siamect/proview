include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib | silent

init : silent

copy : $(exe_dir)/pb_module_classes.dat $(exe_dir)/pn_module_classes.dat | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_obj)

silent :
	@ :

.SUFFIXES:

$(exe_dir)/pb_module_classes.dat : pb_module_classes.dat
	@ echo "Copying $(source)"
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/pn_module_classes.dat : pn_module_classes.dat
	@ echo "Copying $(source)"
	@ $(cp) $(cpflags) $(source) $(target)
