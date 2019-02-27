#  makefile -- <short description>
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

vpath %.wb_load $(co_source)

source_dirs := $(co_source)

wb_databases := \
	$(cnf_dir)/wb_template.wb_vers

clean_db := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init : $(wb_databases)

copy :

lib :

exe :

clean :

realclean :

init_db: $(cnf_dir)/wb_template.wb_vers

$(cnf_dir)/wb_template.wb_vers : ../wb.sql
	@ echo "Building wb_template_$$pwre_dbversion"
	@ mysqladmin create wb_template_$$pwre_dbversion
	@ mysql -D wb_template_$$pwre_dbversion -e "source $$pwre_sroot/db/wb/src/os_linux/wb.sql"
	@ $(pwre_croot)/src/tools/com/src/pwr_show_version.sh $(source) $(target)
