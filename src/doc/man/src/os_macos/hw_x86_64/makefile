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

en_us_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/en_us, \
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

htmlsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

htmlen_us_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/en_us, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

htmlsv_se_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/sv_se, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

gsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

pngsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

pdf_sources := $(filter %.pdf,$(sources))
pdf_sv_se_sources := $(filter %.pdf,$(sv_se_sources)) 
pdf_en_us_sources := $(filter %.pdf,$(en_us_sources)) 
html_sources := $(filter %.html,$(htmlsources))
html_sv_se_sources := $(filter %.html,$(htmlsv_se_sources)) 
html_en_us_sources := $(filter %.html,$(htmlen_us_sources)) 
gif_sources := $(filter %.gif,$(gsources))
png_sources := $(filter %.png,$(pngsources))

export_pdf := $(patsubst %.pdf,$(doc_dir)/%.pdf,$(pdf_sources))
export_pdf_sv_se := $(patsubst %.pdf,$(doc_dir)/sv_se/%.pdf,$(pdf_sv_se_sources))
export_pdf_en_us := $(patsubst %.pdf,$(doc_dir)/en_us/%.pdf,$(pdf_en_us_sources))
export_html := $(patsubst %.html,$(doc_dir)/%.html,$(html_sources))
export_html_sv_se := $(patsubst %.html,$(doc_dir)/sv_se/%.html,$(html_sv_se_sources))
export_html_en_us := $(patsubst %.html,$(doc_dir)/en_us/%.html,$(html_en_us_sources))
export_gif_sv_se := $(patsubst %.gif,$(doc_dir)/sv_se/%.gif,$(gif_sources))
export_gif_en_us := $(patsubst %.gif,$(doc_dir)/en_us/%.gif,$(gif_sources))
export_gif_exe := $(patsubst %.gif,$(exe_dir)/%.gif,$(gif_sources))
export_png_sv_se := $(patsubst %.png,$(doc_dir)/sv_se/%.png,$(png_sources))
export_png_en_us := $(patsubst %.png,$(doc_dir)/en_us/%.png,$(png_sources))
export_png_exe := $(patsubst %.png,$(exe_dir)/%.png,$(png_sources))
clean_pdf := $(patsubst %.pdf,clean_%.pdf,$(pdf_sources))


#docbook-related, added by jonas_h 2006-04-nn

#targetsuffixes := _single.html _index.html .pdf
targetsuffixes := _single.html
nopdf_targetsuffixes := _single.html

sv_se_xmlsources := dbpwr.xml upgrade42.xml
en_us_xmlsources := pwrgsg.xml upgrade42.xml
en_us_xml_nopdf_sources := changelog.xml

#add on the following line master files that include their parts / chapters by means of xinclude
#also add an explicit rule for each master file below
xi_dependents := $(pwre_sroot)/doc/man/sv_se/dbpwr.xml

sv_se_xmltargets := $(addprefix $(pwre_sroot)/doc/man/sv_se/, \
	$(foreach targetsuffix, $(targetsuffixes), \
	$(foreach basefile, $(basename $(sv_se_xmlsources)), \
	$(addsuffix $(targetsuffix), $(basefile)))))

en_us_xmltargets := $(addprefix $(pwre_sroot)/doc/man/en_us/, \
	$(foreach targetsuffix, $(targetsuffixes), \
	$(foreach basefile, $(basename $(en_us_xmlsources)), \
	$(addsuffix $(targetsuffix), $(basefile)))))

en_us_xml_nopdf_targets := $(addprefix $(pwre_sroot)/doc/man/en_us/, \
	$(foreach targetsuffix, $(nopdf_targetsuffixes), \
	$(foreach basefile, $(basename $(en_us_xml_nopdf_sources)), \
	$(addsuffix $(targetsuffix), $(basefile)))))


csstargets := $(addprefix $(doc_dir)/sv_se/, $(notdir $(wildcard $(pwre_sroot)/doc/man/src/*.css))) \
	$(addprefix $(doc_dir)/en_us/, $(notdir $(wildcard $(pwre_sroot)/doc/man/src/*.css)))

medianonsources = $(notdir $(wildcard $(pwre_sroot)/doc/man/src/dg_fig*)))
mediasources := $(filter-out $(medianonsources), $(notdir \
	$(wildcard $(pwre_sroot)/doc/man/src/*.png) \
	$(wildcard $(pwre_sroot)/doc/man/src/*.tif) \
	$(wildcard $(pwre_sroot)/doc/man/src/*.svg) \
	$(wildcard $(pwre_sroot)/doc/man/src/*.jpg) \
	$(wildcard $(pwre_sroot)/doc/man/src/*.gif)))
mediatargets := $(addprefix $(doc_dir)/, $(mediasources))

#end of


.SUFFIXES:

$(doc_dir)/%.pdf : ../../%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.pdf : $(pwre_sroot)/doc/man/sv_se/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.pdf : $(pwre_sroot)/doc/man/en_us/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/%.html : ../../%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.html : $(pwre_sroot)/doc/man/sv_se/%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.html : $(pwre_sroot)/doc/man/en_us/%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.png : ../../%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.png : ../../%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.png : ../../%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         $(clean_pdf)

all : init copy lib exe

init :

copy : $(export_pdf) $(export_pdf_en_us) $(export_pdf_sv_se) \
       $(export_gif_exe) $(export_gif_en_us) $(export_gif_sv_se) \
       $(export_png_exe) $(export_png_en_us) $(export_png_sv_se) \
       $(exe_dir)/en_us/wtt_help.dat $(exe_dir)/sv_se/wtt_help.dat \
       $(exe_dir)/en_us/man_dg.dat $(exe_dir)/sv_se/man_dg.dat \
       $(doc_dir)/en_us/man_dg_index.html $(doc_dir)/sv_se/man_dg_index.html \
       $(doc_dir)/en_us/man_dg.pdf $(doc_dir)/sv_se/man_dg.pdf \
       $(doc_dir)/en_us/man_dg.ps $(doc_dir)/sv_se/man_dg.ps \
       $(exe_dir)/en_us/man_geref.dat $(exe_dir)/sv_se/man_geref.dat \
       $(doc_dir)/en_us/man_geref_index.html $(doc_dir)/sv_se/man_geref_index.html \
       $(doc_dir)/en_us/man_geref.pdf $(doc_dir)/sv_se/man_geref.pdf \
       $(doc_dir)/en_us/man_geref.ps $(doc_dir)/sv_se/man_geref.ps \
       $(exe_dir)/man_subgraph.dat \
       $(doc_dir)/en_us/man_subgraph_index.html \
       $(doc_dir)/en_us/man_subgraph.pdf \
       $(exe_dir)/en_us/man_orm.dat $(exe_dir)/sv_se/man_orm.dat \
       $(doc_dir)/en_us/man_orm_index.html $(doc_dir)/sv_se/man_orm_index.html \
       $(exe_dir)/en_us/man_exlib.dat \
       $(doc_dir)/en_us/man_exlib_index.html \
       $(doc_dir)/en_us/man_exlib.pdf \
       $(doc_dir)/en_us/man_exlib.ps \
       $(mediatargets) $(csstargets) \
       $(export_html) $(export_html_en_us) $(export_html_sv_se)

lib :

exe :

docbook : $(xi_dependents) $(sv_se_xmltargets) $(en_us_xmltargets) $(en_us_xml_nopdf_targets) 

clean :

realclean : clean $(clean_pdf) $(clean_c)

# added by jonas_h 2006-04-nn


 
# end of

$(clean_pdf) : clean_%.pdf : %.pdf
	@ $(rm) $(rmflags) $(doc_dir)/$*.pdf


$(exe_dir)/en_us/wtt_help.dat : $(pwre_sroot)/doc/man/en_us/wtt_help.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)


$(exe_dir)/sv_se/wtt_help.dat : $(pwre_sroot)/doc/man/sv_se/wtt_help.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/en_us/man_dg.dat : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/man_dg.dat : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/man_subgraph.dat : $(pwre_sroot)/doc/man/en_us/man_subgraph.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/man_dg_index.html : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating html for en_us/man_dg"
	@ $(co_convert) -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_dg_index.html : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating html for sv_se/man_dg"
	@ $(co_convert) -t -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_dg.pdf : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating pdf for en_us/man_dg"
	@ $(co_convert) -f -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_dg.pdf : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating pdf for sv_se/man_dg"
	@ $(co_convert) -f -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_dg.ps : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating postscript for en_us/man_dg"
	@ $(co_convert) -n -d $(doc_dir)/en_us $(source)

$(exe_dir)/en_us/man_geref.dat : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/man_geref.dat : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/man_dg.ps : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating postscript for sv_se/man_dg"
	@ $(co_convert) -n -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref_index.html : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating html for en_us/man_geref"
	@ $(co_convert) -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref_index.html : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating html for sv_se/man_geref"
	@ $(co_convert) -t -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref.pdf : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating pdf for en_us/man_geref"
	@ $(co_convert) -f -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref.pdf : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating pdf for sv_se/man_geref"
	@ $(co_convert) -f -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref.ps : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating postscript for en_us/man_geref"
	@ $(co_convert) -n -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref.ps : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating postscript for sv_se/man_geref"
	@ $(co_convert) -n -d $(doc_dir)/sv_se $(source)

$(exe_dir)/en_us/man_orm.dat : $(pwre_sroot)/doc/man/en_us/man_orm.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/man_orm.dat : $(pwre_sroot)/doc/man/sv_se/man_orm.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/man_orm_index.html : $(pwre_sroot)/doc/man/en_us/man_orm.dat
	@ echo "Generating html for en_us/man_orm"
	@ $(co_convert) -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_orm_index.html : $(pwre_sroot)/doc/man/sv_se/man_orm.dat
	@ echo "Generating html for sv_se/man_orm"
	@ $(co_convert) -t -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_subgraph_index.html : $(pwre_sroot)/doc/man/en_us/man_subgraph.dat
	@ echo "Generating html for en_us/man_subgraph"
	@ $(co_convert) -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/en_us/man_subgraph.pdf : $(pwre_sroot)/doc/man/en_us/man_subgraph.dat
	@ echo "Generating pdf for en_us/man_subgraph"
	@ $(co_convert) -f -d $(doc_dir)/en_us $(source)

$(exe_dir)/en_us/man_exlib.dat : $(pwre_sroot)/doc/man/en_us/man_exlib.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/man_exlib.ps : $(pwre_sroot)/doc/man/en_us/man_exlib.dat
	@ echo "Generating postscript for en_us/man_exlib"
	@ $(co_convert) -n -d $(doc_dir)/en_us $(source)

$(doc_dir)/en_us/man_exlib_index.html : $(pwre_sroot)/doc/man/en_us/man_exlib.dat
	@ echo "Generating html for en_us/man_exlib"
	@ $(co_convert) -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/en_us/man_exlib.pdf : $(pwre_sroot)/doc/man/en_us/man_exlib.dat
	@ echo "Generating pdf for en_us/man_exlib"
	@ $(co_convert) -f -d $(doc_dir)/en_us $(source)


# implicit rules, docbook-related, added by jonas_h 2006-04-nn

%.fo :: %.xml
	$(xsltproc) $(xsltproc_args) $(fo_xsl) $(source) > $(target)

$(pwre_sroot)/doc/man/%.pdf : $(pwre_sroot)/doc/man/%.fo $(pwre_sroot)/doc/man/src/pwrtitlepages.xsl
	$(fop) $(source) -pdf $(target)
	rm -f $(source)

$(pwre_sroot)/doc/man/%_single.html : $(pwre_sroot)/doc/man/%.xml 
	@ if [ "$(PWRE_ARGS)" = "docbook" ]; then \
	    echo "xsltproc $(source)"; \
	    $(xsltproc) $(xsltproc_args) -o $(target) $(html_xsl) $(source); \
	  fi

$(doc_dir)/en_us/%_index.html :: $(pwre_sroot)/doc/man/en_us/%.xml
	$(xsltproc) $(xsltproc_args) $(chunk_args_en_us) $(chunk_xsl) $(source)

$(doc_dir)/sv_se/%_index.html :: $(pwre_sroot)/doc/man/sv_se/%.xml
	$(xsltproc) $(xsltproc_args) $(chunk_args_sv_se) $(chunk_xsl) $(source)

$(doc_dir)/sv_se/%.css $(doc_dir)/en_us/%.css :: $(pwre_sroot)/doc/man/src/%.css
	@ echo "copy $(source)"
	@ cp $(source) $(target)

# all-purpose rule for graphics/media
$(doc_dir)/% :: $(pwre_sroot)/doc/man/src/%
	@ echo "copy $(source)"	
	@ cp $(source) $(target)

# Rule for creating titlepage xsl templates
$(pwre_sroot)/doc/man/src/pwrtitlepages.xsl :: $(pwre_sroot)/doc/man/src/pwrtitlepage.spec.xml
	$(xsltproc) -o $(target) $(docbook_stylesheetdir)/template/titlepage.xsl $(source)

# explicit rules updating dependent master files if their includes have been changed.
%/dbpwr.xml :: %/dbpwr_intro.xml %/dbpwr_editering.xml %/dbpwr_exempel.xml %/dbpwr_utdata.xml %/dbpwr_omdocbook.xml 
	touch $(target)

#end of




