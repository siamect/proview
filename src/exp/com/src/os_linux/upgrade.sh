#! /bin/bash
#
# Proview   $Id: upgrade.sh,v 1.11 2006-09-07 14:14:07 claes Exp $
# Copyright (C) 2005 SSAB Oxelösund AB.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation, either version 2 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with the program, if not, write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

let reload__success=0
let reload__userclasses=1
let reload__usertypes=2
let reload__settemplate=3
let reload__loaddb=4

let pass__continue=1
let pass__execute=2
v41_root="/data1/pwr/x4-1-3/rls_dbg"

reload_dumpdb()
{
  # Dump V4.1.3 databases

  reload_checkpass "dumpdb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dump database"

  dmpfiles=`eval ls $pwrp_db/*.wb_dmp`
  if [ ! -z "$dmpfiles" ]; then
    rm $pwrp_db/*.wb_dmp
  fi

  for cdb in $databases; do
     
    dump_file=$pwrp_db/$cdb.wb_dmp

    echo "Dumping volume $cdb in $dump_file"
    export pwr_load=$v41_root/os_linux/hw_x86/exp/load
    $v41_root/os_linux/hw_x86/exp/exe/wb_cmd -v $cdb wb dump/out=\"$dump_file\"
    export pwr_load=$pwrb_root/os_linux/hw_x86/exp/load
  done
}

reload_classvolumes()
{
  reload_checkpass "classvolumes" $start_pass

  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  list=`eval ls -1d $pwrp_db/*.wb_load`
  echo ""
  for file in $list; do
    volume=`eval grep pwr_eClass_ClassVolume $file | awk '{ print $2 }'`
    if [ "$volume" == "" ]; then
      volume=`eval grep ClassVolume $file | awk '{ print $2 }'`
      volumelow=`eval grep ClassVolume $file | awk '{ print tolower($2) }'`
    fi
    if [ "$volume" != "" ]; then
      echo $file
    fi
  done
  echo ""

  reload_continue "Pass create structfiles and loadfiles for classvolumes"

  list=`eval ls -1d $pwrp_db/*.wb_load`
  for file in $list; do
    volume=`eval grep pwr_eClass_ClassVolume $file | awk '{ print $2 }'`
    volumelow=`eval grep pwr_eClass_ClassVolume $file | awk '{ print tolower($2) }'`
    if [ "$volume" == "" ]; then
      volume=`eval grep ClassVolume $file | awk '{ print $2 }'`
      volumelow=`eval grep ClassVolume $file | awk '{ print tolower($2) }'`
    fi
    if [ "$volume" != "" ]; then
      echo "-- Creating structfile and loadfile for $volume"
      if co_convert -sv -d $pwrp_inc $file
      then
        reload_status=$reload__success
      else
        reload_status=$reload__userclasses
        return
      fi

      if wb_cmd create snapshot/file=\"$file\"/out=\"$pwrp_load/$volumelow.dbs\"
      then
        reload_status=$reload__success
      else
        reload_status=$reload__userclasses
        return
      fi
    fi
  done
}

reload_renamedb()
{
  reload_checkpass "renamedb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass rename old databases"

  for cdb in $databases; do
    reload_save_file $pwrp_db/$cdb.db
  done
}

reload_dirvolume()
{
  if [ -e "$pwrp_db/directory.db" ]; then
    return
  fi

  reload_checkpass "dirvolume" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dirvolume"

  wb_cmd create volume/directory
  wb_cmd wb load /load=\"$pwrp_db/directory.wb_dmp\"
}

reload_cnvclassvolume()
{
  reload_checkpass "cnvclassvolume" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass convert classvolume wb_load files"

  dmpfiles=`eval ls $pwrp_db/*.wb_load`
  echo $dmpfiles

  for dmpfile in $dmpfiles; do

    reload_save_file $dmpfile
    source $pwr_exe/upgrade_cnvdmp.sh $dmpfile.1 $pwrp_tmp/t.wb_dmp
    mv $pwrp_tmp/t.wb_dmp $dmpfile
  done
}

reload_cnvdump()
{
  reload_checkpass "cnvdump" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass convert dumpfiles"

  dmpfiles=`eval ls $pwrp_db/*.wb_dmp`
  echo $dmpfiles

  for dmpfile in $dmpfiles; do
    file=${dmpfile##/*/}
    db="${file%.*}.db"
    if [ $db = "wb.db" ]; then
      db=""
    else
      source $pwr_exe/upgrade_cnvdmp.sh $dmpfile $pwrp_tmp/t.wb_dmp
      mv $pwrp_tmp/t.wb_dmp $dmpfile
    fi
  done
}

reload_loaddb()
{
  reload_checkpass "loaddb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass load database"

  if [ -z "$1" ]; then
    tmp=`eval ls -1 $pwrp_db/*.wb_dmp`
    databases=""
    for db in $tmp; do
      db=${db##/*/}
      db="${db%.*}"
      if [ "$db" != "directory" ]; then
        databases="$databases $db"
      fi
    done
  else
    databases=$@
  fi

  for cdb in $databases; do
    if [ $cdb != "directory" ]; then
      echo "-- Loading volume $cdb"
     
      dump_file=$pwrp_db/$cdb.wb_dmp
      list_file=$pwrp_db/$cdb.lis
      if wb_cmd wb load/load=\"$dump_file\"/out=\"$list_file\"
      then
        reload_status=$reload__success
      else
        cat $list_file
        reload_status=$reload__loaddb
      fi
    fi
  done
}

reload_cnvobjects()
{
  reload_checkpass "cnvobjects" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass convert objects in loaded db"

  list=`eval ls -1d $pwrp_db/*.db`
  for file in $list; do
    file=${file##/*/}
    file=${file%%.*}

    if [ $file != "directory" ] && [ $file != "rt_eventlog" ]; then
      wb_cmd -v $file @$pwr_exe/upgrade_pb.pwr_com
    fi
  done

  reload_status=$reload__success
}

reload_compile()
{
  reload_checkpass "compile" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass compile plcprograms"

  list=`eval ls -1d $pwrp_db/*.db`
  for file in $list; do
    file=${file##/*/}
    file=${file%%.*}

    if [ $file != "directory" ] && [ $file != "rt_eventlog" ]; then
      wb_cmd -v $file compile /all
    fi
  done

#  wb_cmd compile /all
  reload_status=$reload__success
}

reload_removeload()
{
  reload_checkpass "removeload" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass remove old loadfiles"

  # Remove all old loadfiles
  echo "-- Removing old loadfiles"
  rm $pwrp_load/ld_vol*.dat
  reload_status=$reload__success
}

reload_createload()
{
  reload_checkpass "createload" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass create loadfiles"

  # Remove all old loadfiles
  echo "-- Removing old loadfiles"
  rm $pwrp_load/ld_vol*.dat

  list=`eval ls -1d $pwrp_db/*.db`
  for file in $list; do
    file=${file##/*/}
    file=${file%%.*}

    if [ $file != "directory" ] && [ $file != "rt_eventlog" ]; then
      wb_cmd -v $file create load/volume=$file
    fi
  done

  # wb_cmd create load/all
  reload_status=$reload__success
}

reload_createboot()
{
  reload_checkpass "createboot" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  echo ""
  echo "Before this pass you should compile the modules included by ra_plc_user."
  echo ""

  reload_continue "Pass create bootfiles"

  echo "-- Creating bootfiles for all nodes"
  wb_cmd create boot/all
  reload_status=$reload__success
}

reload_convertge()
{
  reload_checkpass "convertge" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  echo ""
  echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  echo "!                                                                   !"
  echo "! Note ! This pass should not be executed when upgrading from V3.9 !!"
  echo "!                                                                   !"
  echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

  reload_continue "Pass convert ge graphs"

  # Create a script that dumps each volume
  tmpfile=$pwrp_tmp/convertv41.ge_com
  cat > $tmpfile << EOF
function int process( string graph)
  printf( "Converting %s...\n", graph);
  open 'graph'
  convert v41
  save
endfunction

main()
EOF
  list=`eval ls -1 $pwrp_pop/*.pwg`
  for file in $list; do
    file=${file##/*/}
    file=${file%%.*}
    echo "process( \"$file\");" >> $tmpfile

  done

  echo "exit" >> $tmpfile
  echo "endmain" >> $tmpfile
  chmod a+x $tmpfile
  wb_ge @$tmpfile

  reload_status=$reload__success
}

reload_exit()
{
  source pwrp_env.sh setdb
  exit $reload_status
}

reload_continue()
{
  echo
  echo "----------------------------------------------------------------------------------------"
  echo " $1"
  echo "----------------------------------------------------------------------------------------"
  if [ $go -eq 1 ]; then
    return
  fi

  echo -n "Do you want to continue ? [y/n/go] "
  read repl
  case $repl in
    go ) go=1; return ;;
    y ) return ;;
    n ) reload_exit ;;
  esac

  reload_continue "$1"
}

reload_checkpass()
{
  checkpass=$1
  wantedpass=$2

  pass_status=$pass__continue
  for item in $passes; do
    if [ $item = $wantedpass ]; then
      pass_status=$pass__execute
    fi
    if [ $item = $checkpass ]; then
      return
    fi
  done

  echo "No such pass"
  reload_exit
}

reload_save_file()
{
  new_file=$1
  
  if [ -e $new_file ]; then
    let version=9

    while [ $version -ge 1 ]
    do
      old_file=$new_file.$version
      old_file_ren=$new_file.$((version+1))
      if [ -e $old_file ]; then
        mv $old_file $old_file_ren
      fi
      let version=$version-1
    done

    old_file=$new_file.1
    echo "-- Saving file $new_file -> $old_file"
    mv $new_file $old_file
  fi
}

usage()
{
  cat << EOF

  upgrade.sh  Upgrade from V3.4 or V3.9 to V4.0.

  Arguments   Project name.

  Pass

    dumpdb	   Dump database to textfile \$pwrp_db/'volume'.wb_dmp
    classvolumes   Create loadfiles for classvolumes.
    renamedb       Rename old databases.
    dirvolume      Load the directory volume.
    loaddb         Load dumpfiles.
    compile        Compile all plcprograms in the database
    createload     Create new loadfiles.
    createboot     Create bootfiles for all nodes in the project.

  Note!
EOF
}

if [ "$1" = "help" ] || [ "$1" = "-h" ]; then
  usage
  exit
fi


let reload_status=$reload__success
let check_status=0
let go=0

#if [ -z "$1" ]; then
#  usage
#  exit
#fi

project=${pwrp_root##/*/}

usage

echo ""
echo "-- Upgrade $project"
echo ""

tmp=`eval ls -1d $pwrp_db/*.db`
databases=""
for db in $tmp; do
  db=${db##/*/}
  db="${db%.*}"
  if [ "$db" != "rt_eventlog" ]; then
    databases="$databases $db"
  fi
done

passes="dumpdb classvolumes renamedb dirvolume loaddb compile createload createboot"
echo "Pass: $passes"
echo ""
echo -n "Enter start pass [dumpdb] > "
read start_pass

if [ -z $start_pass ]; then
  start_pass="dumpdb"
fi

for cpass in $passes; do
  reload_$cpass
  if [ $reload_status -ne $reload__success ]; then
    echo "Exiting due to error"
    reload_exit
  fi
done

echo ""
echo "-- The upgrade procedure is now accomplished."
echo "   Please remove the dumpfiles: \$pwrp_db/*.wb_dmp*"
echo "   when you are satisfied with the upgrade."
echo ""
echo "-- Remaining tasks:"
echo "   Build the applications"
echo "   Create packages from the distributor"
echo ""

reload_exit











