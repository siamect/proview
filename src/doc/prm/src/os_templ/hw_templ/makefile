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

.SUFFIXES:


.PHONY : all init copy lib exe clean realclean

all : init copy lib exe | silent

init :

copy :

lib :

exe : $(doc_dir)/prm/index.html | silent

clean : $(clean_dox)

realclean : $(clean_html)

silent :
	@ :

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/prm/$*.html

$(doc_dir)/prm/index.html : ../../doxygen.c
	@ echo "Generating Programmer documentation: doxygen $(source) $(target)"
	@ doxygen
#	@ if [ "`eval which pdflatex`" != "" ]; then \
#	    ./repl.sh latex/*.tex; \
#	    cd latex; \
#	    make pdf; \
#	    cp refman.pdf $(pwr_doc)/en_us/man_prm.pdf; \
#	    cp classra__appl.pdf $(pwr_doc)/en_us/; \
#	    cp classrt__appl.pdf $(pwr_doc)/en_us/; \
#	    cd ..; \
#	    rm -f repl.sed; \
#	    rm -f repl.tmp; \
#	  fi; \
#	  rm -r -f latex;
