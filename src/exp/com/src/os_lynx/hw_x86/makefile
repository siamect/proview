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

vpath %.sh $(hw_source):$(os_source):$(co_source)
vpath %.pwr_com $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sh_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.sh) \
               ), $(notdir $(file)) \
             ) \
           )

pwr_com_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pwr_com) \
               ), $(notdir $(file)) \
             ) \
           )

export_sh := $(patsubst %.sh, $(exe_dir)/%.sh, $(sh_sources))
clean_sh := $(patsubst %.sh, clean_%.sh, $(sh_sources))
export_pwr_com := $(patsubst %.pwr_com, $(exe_dir)/%.pwr_com, $(pwr_com_sources))
clean_pwr_com := $(patsubst %.pwr_com, clean_%.pwr_com, $(pwr_com_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_sh)

all : init copy

init :

copy : $(export_sh) $(export_pwr_com)

lib :

exe :

clean :

realclean : clean $(clean_sh) $(clean_pwr_com)

$(export_sh) : $(exe_dir)/%.sh : %.sh
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)


$(export_pwr_com) : $(exe_dir)/%.pwr_com : %.pwr_com
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_sh) : clean_%.sh : %.sh
	@ rm $(exe_dir)/$*.sh

$(clean_pwr_com) : clean_%.pwr_com : %.pwr_com
	@ rm $(exe_dir)/$*.pwr_com


