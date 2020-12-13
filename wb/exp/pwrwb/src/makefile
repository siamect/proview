include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

export_obj := $(pwr_exe)/pwrwb.so
clean_obj := clean_pwrwb.so

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

$(exe_dir)/pwrwb.so : pwrwbmodule.cpp
	@ if [ "$(PWRE_CONF_PYDEV)" = "1" ]; then\
	    $(log_pyext);\
	    python3 setup_pwrwb.py -q build --build-base $(bld_dir);\
	    if [ "$(pwre_hw)" = "hw_x86_64" ]; then \
	      mv $(bld_dir)/lib.linux-x86_64-3.*/pwrwb.cpython-3*-linux-gnu.so $(pwr_exe)/pwrwb.so; \
	    elif [ "$(pwre_hw)" = "hw_x86" ]; then \
	      mv $(bld_dir)/lib.linux-i686-3.*/pwrwb.cpython-3*-linux-gnu.so $(pwr_exe)/pwrwb.so; \
	    elif [ "$(pwre_hw)" = "hw_arm" ]; then \
	      mv $(bld_dir)/lib.linux-armv*-3.*/pwrwb.cpython-3*-linux-gnueabihf.so $(pwr_exe)/pwrwb.so; \
	    fi;\
	    python3 setup_pwrwb.py -q install_egg_info --install-dir $(pwr_exe);\
	  else\
	    $(log_no);\
	  fi

clean_pwrwb.so : pwrwbmodule.cpp
	rm $(pwr_exe)/pwrwb.so
