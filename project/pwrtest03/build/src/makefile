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
	@ $(pwre_croot)/project/pwrtest03/build_from_git.sh

clean_project:
	@ echo "Cleaing project"
	@ if [ -e $(pwre_croot)/project/pwrtest03/bld ]; then rm $(pwre_croot)/project/pwrtest03/bld; fi
	@ if [ -e $(pwre_croot)/project/pwrtest03/src/db/volpwrtest03a.db ]; then rm -r $(pwre_croot)/project/pwrtest03/src/db/volpwrtest03a.db; fi
	@ if [ -e $(pwre_croot)/project/pwrtest03/src/db/volpwrtest03a.db.lock ]; then rm -r $(pwre_croot)/project/pwrtest03/src/db/volpwrtest03a.db.lock; fi
	@ if [ -e $(release_root)/bld/project/pwrtest03/bld ]; then rm -r $(release_root)/bld/project/pwrtest03/bld; fi
