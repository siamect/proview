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

vpath %.gif $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

gif_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

html_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

pdf_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

sv_se_html_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/web/sv_se, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

export_gif := $(addprefix $(doc_dir)/,$(gif_sources))
export_html := $(addprefix $(doc_dir)/,$(html_sources))
export_html_sv_se := $(addprefix $(doc_dir)/sv_se/,$(sv_se_html_sources))
export_pdf := $(addprefix $(doc_dir)/,$(pdf_sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(sources))
clean_html := $(patsubst %.html,clean_%.html,$(sources))
export_doc := $(export_gif) $(export_html) $(export_pdf) $(export_html_sv_se)
clean_ge := $(clean_gif) $(clean_html)
.SUFFIXES:


.PHONY : all init copy lib exe clean realclean

$(doc_dir)/%.pdf : ../../%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)


all : init copy lib exe

init :

copy : $(export_doc)

lib :

exe :

clean : $(clean_dweb)

realclean : $(clean_html)


$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/dox/$*.html







