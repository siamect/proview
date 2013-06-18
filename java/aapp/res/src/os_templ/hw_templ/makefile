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
res_dirs_drawable := ../../../drawable
res_dirs_drawable_hdpi := ../../../drawable-hdpi
res_dirs_drawable_ldpi := ../../../drawable-ldpi
res_dirs_drawable_mdpi := ../../../drawable-mdpi
res_dirs_drawable_xhdpi := ../../../drawable-xhdpi
res_dirs_layout := ../../../layout
res_dirs_menu := ../../../menu
res_dirs_values := ../../../values
res_dirs_values_v11 := ../../../values-v11
res_dirs_values_v14 := ../../../values-v14

res_drawable_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_drawable), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

res_drawable_hdpi_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_drawable_hdpi), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

res_drawable_ldpi_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_drawable_ldpi), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

res_drawable_mdpi_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_drawable_mdpi), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

res_drawable_xhdpi_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_drawable_xhdpi), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

res_layout_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_layout), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

res_menu_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_menu), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

res_values_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_values), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

res_values_v11_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_values_v11), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

res_values_v14_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(res_dirs_values_v14), \
                 $(wildcard $(dir)/*.xml) \
               ), $(notdir $(file)) \
             ) \
           )

res_drawable_objects := $(addprefix $(app_res_dir)/drawable/, $(res_drawable_sources))
res_drawable_hdpi_objects := $(addprefix $(app_res_dir)/drawable-hdpi/, $(res_drawable_hdpi_sources))
res_drawable_ldpi_objects := $(addprefix $(app_res_dir)/drawable-ldpi/, $(res_drawable_ldpi_sources))
res_drawable_mdpi_objects := $(addprefix $(app_res_dir)/drawable-mdpi/, $(res_drawable_mdpi_sources))
res_drawable_xhdpi_objects := $(addprefix $(app_res_dir)/drawable-xhdpi/, $(res_drawable_xhdpi_sources))
res_layout_objects := $(addprefix $(app_res_dir)/layout/, $(res_layout_sources))
res_menu_objects := $(addprefix $(app_res_dir)/menu/, $(res_menu_sources))
res_values_objects := $(addprefix $(app_res_dir)/values/, $(res_values_sources))
res_values_v11_objects := $(addprefix $(app_res_dir)/values-v11/, $(res_values_v11_sources))
res_values_v14_objects := $(addprefix $(app_res_dir)/values-v14/, $(res_values_v14_sources))
local_objects := 

dirs : $(app_res_dir)$(dir_ext) \
	$(app_res_dir)/drawable$(dir_ext) \
	$(app_res_dir)/drawable-hdpi$(dir_ext) \
	$(app_res_dir)/drawable-ldpi$(dir_ext) \
	$(app_res_dir)/drawable-mdpi$(dir_ext) \
	$(app_res_dir)/drawable-xhdpi$(dir_ext) \
	$(app_res_dir)/layout$(dir_ext) \
	$(app_res_dir)/menu$(dir_ext) \
	$(app_res_dir)/values$(dir_ext) \
	$(app_res_dir)/values-v11$(dir_ext) \
	$(app_res_dir)/values-v14$(dir_ext)

copy: $(res_drawable_objects) $(res_drawable_hdpi_objects) $(res_drawable_ldpi_objects) \
	$(res_drawable_mdpi_objects) $(res_drawable_xhdpi_objects) $(res_layout_objects) \
	$(res_menu_objects) $(res_values_objects) $(res_values_v11_objects) \
	$(res_values_v14_objects)

$(app_res_dir)$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)$(dir_ext)

$(app_res_dir)/drawable$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/drawable$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/drawable$(dir_ext)

$(app_res_dir)/drawable-hdpi$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/drawable-hdpi$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/drawable-hdpi$(dir_ext)

$(app_res_dir)/drawable-ldpi$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/drawable-ldpi$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/drawable-ldpi$(dir_ext)

$(app_res_dir)/drawable-mdpi$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/drawable-mdpi$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/drawable-mdpi$(dir_ext)

$(app_res_dir)/drawable-xhdpi$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/drawable-xhdpi$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/drawable-xhdpi$(dir_ext)

$(app_res_dir)/layout$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/layout$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/layout$(dir_ext)

$(app_res_dir)/menu$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/menu$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/menu$(dir_ext)

$(app_res_dir)/values$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/values$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/values$(dir_ext)

$(app_res_dir)/values-v11$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/values-v11$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/values-v11$(dir_ext)

$(app_res_dir)/values-v14$(dir_ext) :
	@ echo "Create directory " $(app_res_dir)/values-v14$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_res_dir)/values-v14$(dir_ext)

$(app_res_dir)/drawable/%.png : ../../../../res/drawable/%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/drawable-hdpi/%.png : ../../../../res/drawable-hdpi/%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/drawable-ldpi/%.png : ../../../../res/drawable-ldpi/%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/drawable-mdpi/%.png : ../../../../res/drawable-mdpi/%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/drawable-xhdpi/%.png : ../../../../res/drawable-xhdpi/%.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/layout/%.xml : ../../../../res/layout/%.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/menu/%.xml : ../../../../res/menu/%.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/values/%.xml : ../../../../res/values/%.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/values-v11/%.xml : ../../../../res/values-v11/%.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(app_res_dir)/values-v14/%.xml : ../../../../res/values-v14/%.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

-include ../../special.mk
-include ../special.mk
-include special.mk


