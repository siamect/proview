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


vpath %.pdf $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

p_en_us_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/dsh/en_us, \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

h_en_us_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/dsh/en_us, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

p_sv_se_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/dsh/sv_se, \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

h_sv_se_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/dsh/sv_se, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

pdf_en_us_sources := $(filter %.pdf,$(p_en_us_sources)) 
html_en_us_sources := $(filter %.html,$(h_en_us_sources)) 
pdf_sv_se_sources := $(filter %.pdf,$(p_sv_se_sources)) 
html_sv_se_sources := $(filter %.html,$(h_sv_se_sources)) 

export_pdf_en_us := $(patsubst %.pdf,$(doc_dir)/en_us/dsh/%.pdf,$(pdf_en_us_sources))
export_html_en_us := $(patsubst %.html,$(doc_dir)/en_us/dsh/%.html,$(html_en_us_sources))
export_pdf_sv_se := $(patsubst %.pdf,$(doc_dir)/sv_se/dsh/%.pdf,$(pdf_sv_se_sources))
export_html_sv_se := $(patsubst %.html,$(doc_dir)/sv_se/dsh/%.html,$(html_sv_se_sources))
clean_pdf := $(patsubst %.pdf,clean_%.pdf,$(pdf_sources))
clean_html := $(patsubst %.html,clean_%.html,$(html_sources))

.SUFFIXES:

$(doc_dir)/en_us/dsh/%.pdf : $(pwre_sroot)/doc/dsh/en_us/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/dsh/%.html : $(pwre_sroot)/doc/dsh/en_us/%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/dsh/%.pdf : $(pwre_sroot)/doc/dsh/sv_se/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/dsh/%.html : $(pwre_sroot)/doc/dsh/sv_se/%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         $(clean_pdf)

all : init copy lib exe

init :

copy : $(export_pdf_en_us) $(export_html_en_us) \
       $(export_pdf_sv_se) $(export_html_sv_se)

lib :

exe :


clean :

realclean : clean $(clean_pdf) $(clean_html)

$(clean_pdf) : clean_%.pdf : %.pdf
	@ $(rm) $(rmflags) $(doc_dir)/$*.pdf

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/$*.html











