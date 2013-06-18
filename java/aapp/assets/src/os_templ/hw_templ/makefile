include $(pwre_dir_symbols)

extern_java_sources := 

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif


#
# Copy Gdh from rt_client
#
assets_dirs_fonts := ../../../fonts

assets_fonts_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(assets_dirs_fonts), \
                 $(wildcard $(dir)/*.ttf) \
               ), $(notdir $(file)) \
             ) \
           )


assets_fonts_objects := $(addprefix $(app_assets_dir)/fonts/, $(assets_fonts_sources))
local_objects := 

dirs : $(app_assets_dir)$(dir_ext) \
	$(app_assets_dir)/fonts$(dir_ext)

copy: $(assets_fonts_objects)

$(app_assets_dir)$(dir_ext) :
	@ echo "Create directory " $(app_assets_dir)$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_assets_dir)$(dir_ext)

$(app_assets_dir)/fonts$(dir_ext) :
	@ echo "Create directory " $(app_assets_dir)/fonts$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_assets_dir)/fonts$(dir_ext)

$(app_assets_dir)/fonts/%.ttf : ../../../../assets/fonts/%.ttf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

-include ../../special.mk
-include ../special.mk
-include special.mk


