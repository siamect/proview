include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

export_obj := $(pwr_exe)/pwrtest.so
clean_obj := clean_pwrtest.so

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

$(exe_dir)/pwrtest.so : pwrtestmodule.cpp
	@ if [ "$(PWRE_CONF_PYDEV)" = "1" ]; then\
	    echo "Bulding Python c extension pwrtest";\
	    python3 setup_pwrtest.py -q build --build-base $(bld_dir);\
	    if [ "$(pwre_hw)" = "hw_x86_64" ]; then \
	      mv $(bld_dir)/lib.linux-x86_64-3.7/pwrtest.cpython-37m-x86_64-linux-gnu.so $(pwr_exe); \
	    elif [ "$(pwre_hw)" = "hw_x86" ]; then \
	      mv $(bld_dir)/lib.linux-i686-3.7/pwrtest.so $(pwr_exe); \
	    elif [ "$(pwre_hw)" = "hw_arm" ]; then \
	      mv $(bld_dir)/lib.linux-armv7l-3.7/pwrtest.so $(pwr_exe); \
	    fi;\
	    python3 setup_pwrtest.py -q install_egg_info --install-dir $(pwr_exe);\
	  else\
	    echo "Not building Python c extension pwrtest";\
	  fi

clean_pwrtest.so : pwrtestmodule.cpp
	rm $(pwr_exe)/pwrtest.so
