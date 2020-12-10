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
	@ if [ "$(PWRE_CONF_PYDEV)" = "1" ]; then\
	    $(log_pyext);\
	    python3 setup_pwrrt.py -q build --build-base $(bld_dir);\
	    if [ "$(pwre_hw)" = "hw_x86_64" ]; then \
	      mv $(bld_dir)/lib.linux-x86_64-3.*/pwrrt.cpython-3*-linux-gnu.so $(pwr_exe)/pwrrt.so; \
	    elif [ "$(pwre_hw)" = "hw_x86" ]; then \
	      mv $(bld_dir)/lib.linux-i686-3.*/pwrrt.cpython-3*-linux-gnu.so $(pwr_exe)/pwrrt.so; \
	    elif [ "$(pwre_hw)" = "hw_arm" ]; then \
	      mv $(bld_dir)/lib.linux-armv*-3.*/pwrrt.cpython-3*-linux-gnu.so $(pwr_exe)/pwrrt.so; \
	    fi;\
	    python3 setup_pwrrt.py -q install_egg_info --install-dir $(pwr_exe);\
	  else\
	    $(log_no);\
	  fi

clean_pwrrt.so : pwrrtmodule.c
	rm $(pwr_exe)/pwrrt.so
