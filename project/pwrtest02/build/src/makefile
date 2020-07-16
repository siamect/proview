include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif


all : init copy lib exe | silent

init :

copy :

lib :

exe : build_project | silent


clean : clean_project

realclean : clean

silent :
	@ :

build_project :
	@ $(pwre_croot)/project/pwrtest02/build_from_git.sh

clean_project:
	@ echo "Cleaing project"
	@ if [ -e $(pwre_croot)/project/pwrtest02/bld ]; then rm $(pwre_croot)/project/pwrtest02/bld; fi
	@ if [ -e $(pwre_croot)/project/pwrtest02/src/db/directory.wb_load ]; then rm $(pwre_croot)/project/pwrtest02/src/db/directory.wb_load; fi
	@ if [ -e $(pwre_croot)/project/pwrtest02/src/db/cvolpwrtest02.wb_load ]; then rm $(pwre_croot)/project/pwrtest02/src/db/cvolpwrtest02.wb_load; fi
	@ if [ -e $(pwre_croot)/project/pwrtest02/src/db/volpwrtest02.db ]; then rm -r $(pwre_croot)/project/pwrtest02/src/db/volpwrtest02.db; fi
	@ if [ "`eval ls $(pwre_croot)/project/pwrtest02/src/db/*.lock 2>/dev/null`" != "" ]; then rm $(pwre_croot)/project/pwrtest02/src/db/*.lock; fi
	@ if [ "`eval ls $(pwre_croot)/project/pwrtest02/src/db/* 2>/dev/null`" != "" ]; then rm -r $(pwre_croot)/project/pwrtest02/src/db/*; fi
	@ if [ -e $(release_root)/bld/project/pwrtest02/bld ]; then rm -r $(release_root)/bld/project/pwrtest02/bld; fi
