include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif

ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif

vpath %.gsd $(hw_source):$(os_source):$(co_source)
vpath %.xml $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

gsd_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gsd) \
               ), $(notdir $(file)) \
             ) \
           )

xml_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

export_gsd := $(patsubst %.gsd, $(exe_dir)/%.gsd, $(gsd_sources))
clean_gsd := $(patsubst %.gsd, clean_%.gsd, $(gsd_sources))
export_xml := $(patsubst %.xml, $(exe_dir)/%.xml, $(xml_sources))
clean_xml := $(patsubst %.xml, clean_%.xml, $(xml_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_gsd) $(clean_xml)

all : init copy

init :

copy : $(export_gsd) $(export_xml)

lib :

exe :

clean :

realclean : clean $(clean_gsd) $(clean_xml)

$(export_gsd) : $(exe_dir)/%.gsd : %.gsd
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_gsd) : clean_%.gsd : %.gsd
	@ rm $(exe_dir)/$*.gsd

$(export_xml) : $(exe_dir)/%.xml : %.xml
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_xml) : clean_%.xml : %.xml
	@ rm $(exe_dir)/$*.xml


