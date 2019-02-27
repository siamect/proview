include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

export_obj := $(pwr_exe)/pwrrt.so
clean_obj := clean_pwrrt.so

.PHONY : all init copy lib exe clean

all : init copy lib exe | silent

init :
	@ if [ ! -e $(bld_dir) ]; then \
	    mkdir -p $(bld_dir); \
	  fi

copy :

lib :

exe : $(export_obj) | silent


clean : $(clean_obj)

silent :
	@ :

.SUFFIXES:

$(exe_dir)/pwrrt.so : pwrrtmodule.c
	@ if [ $(PWRE_CONF_PYDEV) -eq 1 ]; then\
	    echo "Bulding Python c extension pwrrt";\
	    cd ../..;\
	    python setup_pwrrt.py -q build --build-base $(bld_dir);\
	    if [ "$(pwre_hw)" == "hw_x86_64" ]; then \
	      mv $(bld_dir)/lib.linux-x86_64-2.7/pwrrt.so $(pwr_exe); \
	    elif [ "$(pwre_hw)" == "hw_x86" ]; then \
	      mv $(bld_dir)/lib.linux-i686-2.7/pwrrt.so $(pwr_exe); \
	    fi;\
	    python setup_pwrrt.py -q install_egg_info --install-dir $(pwr_exe);\
	  else\
	    echo "Not building Python c extension pwrrt";\
	  fi

clean_pwrrt.so : pwrrtmodule.c
	rm $(pwr_exe)/pwrrt.so
