include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

init: pwrxtt_build_init

copy: pwrxtt_build_copy

exe: pwrxtt_build_exe

pwrxtt_build_init :
	@ echo "Build PwrXtt init"
	@ $(pwre_sroot)/aapp/bld/src/pwrxtt_build.sh init

pwrxtt_build_copy :
	@ echo "Build PwrXtt copy"
	@ $(pwre_sroot)/aapp/bld/src/pwrxtt_build.sh copy

pwrxtt_build_exe :
	@ echo "Build PwrXtt exe"
	@ $(pwre_sroot)/aapp/bld/src/pwrxtt_build.sh exe

-include special.mk
