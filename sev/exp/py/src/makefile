include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.py $(co_source)

source_dirs := $(co_source)

py_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.py) \
               ), $(notdir $(file)) \
             ) \
           )

png_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

export_py := $(patsubst %.py, $(exe_dir)/%.py, $(py_sources))
export_png := $(patsubst %.png, $(exe_dir)/%.png, $(png_sources))
clean_py := $(patsubst %.py, clean_%.py, $(py_sources))
clean_png := $(patsubst %.png, clean_%.png, $(png_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_py)

all : init copy | silent

init : silent

copy : $(export_py) $(export_png) | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_py) $(clean_png)

silent :
	@ :

$(export_py) : $(exe_dir)/%.py : %.py
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(export_png) : $(exe_dir)/%.png : %.png
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_py) : clean_%.py : %.py
	@ rm $(exe_dir)/$*.py

$(clean_png) : clean_%.png : %.png
	@ rm $(exe_dir)/$*.png
