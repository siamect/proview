ifeq ($(pwre_btype),rls)
  cxxflags := /rep=pwrb_root:[cxx_demang]/opt/deb=tra/flo=g_flo/noans/exte=str/assu=noali/pref=all/ree=mul
else
  cxxflags := /rep=pwrb_root:[cxx_demang]/noopt/deb/flo=g_flo/noans/exte=str/assu=noali/pref=all/ree=mul
endif
