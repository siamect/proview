include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

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
	@ \@pwrd_exe:pwrd_library get_root pwrd_ pwrd_msg_ $(to-vms $(exp_root)/)
	@ define/user pwr_db pwrb_root:[db]
	@ define/user/trans=(term,conc) pwrb_root "''pwrd_'"
	@ mc sql$$ @$(source)
 
$(clean_db) : clean_%.sql : %.sql
	@ files := $(to-vms $(db_dir)/)$(sname)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*

