#!/bin/bash
#
#  Merge build tree for a module to common build tree
#
merge_dir_func()
{
  local todir=$1
  local fromdir=$2
  local singlefile=$3

  if [ "$singlefile" != "" ]; then
    files=$singlefile
    todir=$todir/${singlefile%/*}
  else
    files=`eval ls $fromdir 2>/dev/null`
  fi

  if [ ! -z "$files" ]; then
    for file in $files; do
      if [ ! -d $fromdir/$file ]; then
        if [ -e $todir/$file ]; then
          if [ $todir/$file -ot $fromdir/$file ]; then
            if [ $ver -eq 1 ]; then
              echo "Copy $fromdir/$file"
            fi
            cp -p $fromdir/$file $todir
          fi
        else
	  if [ $ver -eq 1 ]; then
            echo "Copy $fromdir/$file"
          fi
          cp -p $fromdir/$file $todir
	  if [ ${file##*.} = "dbs" ]; then
            # Change access on dbsfiles
	    chmod a+w $todir/$file
	  fi
        fi
      fi
    done
  fi
}

fromroot=$2
toroot=$3
file=$4
if [ "$1" == "1" ]; then
  ver=1
else
  ver=0
fi

if [ -z "$pwre_host_exe" ]; then
  co_merge="co_merge"
else
  co_merge=$pwre_host_exe/co_merge
fi

if [ "$file" != "" ]; then
  # Merge only this file
  merge_dir_func $toroot $fromroot $file

else
  # Copy exe, load, obj and inc
  merge_dir_func $toroot/exe $fromroot/exe
  merge_dir_func $toroot/exe/sv_se $fromroot/exe/sv_se
  merge_dir_func $toroot/exe/en_us $fromroot/exe/en_us
  merge_dir_func $toroot/exe/de_de $fromroot/exe/de_de
  merge_dir_func $toroot/exe/fr_fr $fromroot/exe/fr_fr
  merge_dir_func $toroot/exe/zh_cn $fromroot/exe/zh_cn
  merge_dir_func $toroot/inc $fromroot/inc
  merge_dir_func $toroot/load $fromroot/load
  merge_dir_func $toroot/obj $fromroot/obj
  merge_dir_func $toroot/web $fromroot/web

  # Merge archives
  cd ${toroot%/*}/tmp
  libraries=`eval ls $fromroot/lib/* 2>/dev/null`
  for lib in $libraries; do

    arname=${lib##/*/}
    if [ $arname = "libpwr_rt.a" ] || [ $arname = "libpwr_cow.a" ] || [ $arname = "libpwr_cow_gtk.a" ] || [ $arname = "libpwr_cow_qt.a" ] || [ $arname = "libpwr_wb.a" ] || [ $arname = "libpwr_wb_gtk.a" ] || [ $arname = "libpwr_wb_qt.a" ] || [ $arname = "libpwr_xtt.a" ] || [ $arname = "libpwr_xtt_gtk.a" ] || [ $arname = "libpwr_xtt_qt.a" ]; then
      if [ $ver -eq 1 ]; then
	  echo "Merge $lib"
      fi
      modules=`eval ar -tf $lib`
      ar -xof $lib
      ar -roUc $toroot/lib/$arname $modules
      rm $modules
    else
      if [ $ver -eq 1 ]; then
        echo "Copy $lib"
      fi
      cp $lib $toroot/lib/
    fi
  done

  # Merge io methods
  methodfile=$pwr_inc/rt_io_$pwre_module.meth
  if [ -e $methodfile ]; then
    if [ $ver -eq 1 ]; then
      echo "-- Merge io methods"
    fi
    $co_merge io_base $pwr_einc/rt_io_\*.meth $pwr_elib/libpwr_rt.a
  fi
  # Merge wb methods
  methodfile=$pwr_inc/wb_$pwre_module.meth
  if [ -e $methodfile ]; then
    if [ $ver -eq 1 ]; then
      echo "-- Merge wb methods"
    fi
    $co_merge wb_base $pwr_einc/wb_\*.meth $pwr_elib/libpwr_wb.a
  fi
  # Merge xtt methods
  methodfile=$pwr_inc/xtt_$pwre_module.meth
  if [ -e $methodfile ]; then
    if [ $ver -eq 1 ]; then
      echo "-- Merge xtt methods"
    fi
    $co_merge xtt_base $pwr_einc/xtt_\*.meth $pwr_elib/libpwr_xtt.a
  fi
fi
