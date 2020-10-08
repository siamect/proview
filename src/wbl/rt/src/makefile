# makefile  -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

.PHONY : all init copy lib exe dirs clean realclean\
         $(clean_wbl)

all : init copy exe | silent

init : dirs | silent

copy : silent

lib  : $(load_dir)/rt.dbs | silent

exe  : silent

dirs : $(bld_dir)$(dir_ext)

clean : $(clean_wbl)

realclean : clean

silent :
	@ :

$(load_dir)/rt.dbs :
	@ $(log_load)
	@ wb_cmd -i -q create snapshot /file=\"$(pwre_sroot)/wbl/rt/src/rt.wb_load\"/out=\"$(target)\"/depend=\"$(bld_dir)/rt.d_wbl\"
	@ if [ -e $(target) ]; then \
	    chmod a+w $(target); \
	  fi

clean_wbl :
	@ if [ -e $(load_dir)/rt.dbs ]; then \
		$(rm) $(rmflags) $(load_dir)/rt.dbs; \
	  fi

$(bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(basename $@)

-include $(bld_dir)/rt.d_wbl
