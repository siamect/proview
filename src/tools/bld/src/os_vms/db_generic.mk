#  db_generic.mk -- generic makefile for databases
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a database.
#
ifndef db_generic_mk
db_generic_mk := 1


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



vpath %.sql $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.sql) \
               ), $(notdir $(file)) \
             ) \
           )

db_name   = $(comp_name)
export_db = $(patsubst %.sql, $(db_dir)/%.rdb, $(sources))
clean_db  = $(patsubst %.sql, clean_%.sql, $(sources))

.PHONY : all init copy lib exe clean realclean \
	 $(clean_db)

all : init copy lib exe

init : $(export_db)

copy :

lib :

exe :

clean :

realclean : clean $(clean_db)

$(db_dir)/%.rdb : %.sql
	@ write sys$$output "Creating $@ ..."
	@ mc sql$$ @$(source)
 
$(clean_db) : clean_%.sql : %.sql
	@ files := $(to-vms $(db_dir)/)$(sname)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*

endif
