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
	@ $(pwre_croot)/project/pwrdemo/build_from_git.sh

clean_project:
	@ echo "Cleaing project"
	@ if [ -e $(pwre_croot)/project/pwrdemo/bld ]; then rm $(pwre_croot)/project/pwrdemo/bld; fi
	@ if [ -e $(pwre_croot)/project/pwrdemo/src/db/volpwrdemo.db ]; then rm -r $(pwre_croot)/project/pwrdemo/src/db/volpwrdemo.db; fi
	@ if [ -e $(release_root)/bld/project/pwrdemo/bld ]; then rm -r $(release_root)/bld/project/pwrdemo/bld; fi
