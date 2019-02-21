include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

vpath %.sh $(co_source)
vpath %.pwr_com $(co_source)

source_dirs := $(co_source)

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

all : init copy | silent

init : silent

copy : $(export_sh) $(export_pwr_com) | silent

lib : silent

exe :

clean :

realclean : clean $(clean_sh) $(clean_pwr_com)

silent :
	@ :

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
