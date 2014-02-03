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

.SUFFIXES:


.PHONY : all init copy lib exe clean realclean \
         $(clean_ge)

all : init copy lib exe

init : dirs

copy :

lib :

exe : $(bld_dir)/control_pwrrt $(bld_dir)/control_pwr $(bld_dir)/control_pwrdemo $(bld_dir)/control_pwrsev
rpi : $(bld_dir)/control_pwrrpi

clean :

realclean :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) -p $(mkdirflags) $(basename $@)

$(bld_dir)/control_pwrrt : ../../../pwrrt/control
	@ echo "build $(target)"
	@ ../../../pwrrt/build.sh
	@ cp $(source) $(target)

$(bld_dir)/control_pwr : ../../../pwr/control
	@ echo "build $(target)"
	@ ../../../pwr/build.sh
	@ cp $(source) $(target)

$(bld_dir)/control_pwrdemo : ../../../pwrdemo/control
	@ echo "build $(target)"
	@ ../../../pwrdemo/build.sh
	@ cp $(source) $(target)

$(bld_dir)/control_pwrsev : ../../../pwrsev/control
	@ echo "build $(target)"
	@ ../../../pwrsev/build.sh
	@ cp $(source) $(target)

$(bld_dir)/control_pwrrpi : ../../../pwrrpi/control
	@ echo "build $(target)"
	@ ../../../pwrrpi/build.sh
	@ cp $(source) $(target)



