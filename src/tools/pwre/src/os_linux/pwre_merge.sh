#!/bin/bash
#
#  Merge build tree for a module to common build tree
#

merge_dir_func()
{
  local todir=$1
  local fromdir=$2

  files=`eval ls $fromdir 2>/dev/null`

  if [ ! -z "$files" ]; then
    for file in $files; do
      if [ ! -d $fromdir/$file ]; then
        if [ -e $todir/$file ]; then
          if [ $todir/$file -ot $fromdir/$file ]; then
            echo "Copy $fromdir/$file"
            cp $fromdir/$file $todir
	    if [ ${file##*.} = "dbs" ]; then
              # Change access on dbsfiles
	      chmod a+w $todir/$file
	    fi
          fi
        else
          echo "Copy $fromdir/$file"
          cp $fromdir/$file $todir
        fi
      fi
    done
  fi
}


fromroot=$1
toroot=$2

# Copy exe, load, obj and inc
merge_dir_func $toroot/exe $fromroot/exe
merge_dir_func $toroot/exe/sv_se $fromroot/exe/sv_se
merge_dir_func $toroot/exe/en_us $fromroot/exe/en_us
merge_dir_func $toroot/exe/de_de $fromroot/exe/de_de
merge_dir_func $toroot/inc $fromroot/inc
merge_dir_func $toroot/load $fromroot/load
merge_dir_func $toroot/obj $fromroot/obj

# Merge archives
cd ${toroot%/*}/tmp
libraries=`eval ls $fromroot/lib/* 2>/dev/null`
for lib in $libraries; do

  arname=${lib##/*/}
  if [ $arname = "libpwr_rt.a" ] || [ $arname = "libpwr_wb.a" ]; then
    echo "Merge $lib"
    modules=`eval ar -tf $lib`
    ar -xf $lib
    ar -rc $toroot/lib/$arname $modules
    rm $modules
  else
    echo "Copy $lib"
    cp $lib $toroot/lib/
  fi
done

#Merge io methods
echo "-- Merge io methods"
co_merge io_base $pwr_einc/rt_io_\*.meth $pwr_elib/libpwr_rt.a
echo "-- Merge wb methods"
co_merge wb_base $pwr_einc/wb_\*.meth $pwr_elib/libpwr_wb.a




