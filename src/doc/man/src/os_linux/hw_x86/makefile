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


vpath %.pdf $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

sv_se_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/sv_se, \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

pdf_sources := $(filter %.pdf,$(sources))
pdf_sv_se_sources := $(filter %.pdf,$(sv_se_sources)) 

export_pdf := $(patsubst %.pdf,$(doc_dir)/%.pdf,$(pdf_sources))
export_pdf_sv_se := $(patsubst %.pdf,$(doc_dir)/sv_se/%.pdf,$(pdf_sv_se_sources))
clean_pdf := $(patsubst %.pdf,clean_%.pdf,$(pdf_sources))

.SUFFIXES:

$(doc_dir)/%.pdf : ../../%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.pdf : $(pwre_sroot)/doc/man/sv_se/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         $(clean_pdf)

all : init copy lib exe

init :

copy : $(export_pdf) $(export_pdf_sv_se)

lib :

exe :


clean :

realclean : clean $(clean_pdf) $(clean_c)

$(clean_pdf) : clean_%.pdf : %.pdf
	@ $(rm) $(rmflags) $(doc_dir)/$*.pdf




