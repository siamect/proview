# makefile  -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
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


.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : 

lib  : $(load_dir)/rt.dbs

exe  :

clean : $(clean_wbl)

realclean : clean

$(load_dir)/rt.dbs :
	@ echo "Generating snapshot for rt"
	@ wb_cmd create snapshot /file=\"$(pwre_sroot)/wbl/rt/src/rt.wb_load\"/out=\"$(target)\"
	@ chmod a+w $(target)

clean_wbl :
	@ if [ -e $(load_dir)/rt.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/rt.dbs; \
	  fi
