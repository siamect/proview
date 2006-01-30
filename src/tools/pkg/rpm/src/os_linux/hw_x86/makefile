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

exe : $(bld_dir)/pwr.spec $(bld_dir)/pwrrt.spec $(bld_dir)/pwrdemo.spec

clean :

realclean :

dirs : $(bld_dir)$(dir_ext)

$(bld_dir)$(dir_ext) :
	@ $(mkdir) -p $(mkdirflags) $(basename $@)

$(bld_dir)/pwrrt.spec : ../../../pwrrt/pwrrt.spec
	@ echo "build $(target)"
	@ ../../../pwrrt/build.sh
	@ cp $(source) $(target)

$(bld_dir)/pwr.spec : ../../../pwr/pwr.spec
	@ echo "build $(target)"
	@ ../../../pwr/build.sh
	@ cp $(source) $(target)

$(bld_dir)/pwrdemo.spec : ../../../pwrdemo/pwrdemo.spec
	@ echo "build $(target)"
	@ ../../../pwrdemo/build.sh
	@ cp $(source) $(target)



