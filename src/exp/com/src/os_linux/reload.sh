#! /bin/bash
#
#
#
#

let reload__success=0
let reload__userclasses=1
let reload__usertypes=2
let reload__settemplate=3
let reload__loaddb=4

let pass__continue=1
let pass__execute=2

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

reload_dumpdb()
{
  reload_checkpass "dumpdb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dump database"

  # Rename old dumpfiles
  dmpfiles=`eval ls -1 $pwrp_db/*.wb_dmp`
  for dmpfile in $dmpfiles; do
    reload_save_file $dmpfile
  done

  for cdb in $databases; do
     
    dump_file=$pwrp_db/$cdb.wb_dmp

    echo "Dumping volume $cdb in $dump_file"
    wb_cmd -v $cdb wb dump/out=\"$dump_file\"
  done
}

reload_renamedb()
{
  reload_checkpass "renamedb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass copy rename old database"

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

reload_compile()
{
  reload_checkpass "compile" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass compile plcprograms"

  for cdb in $databases; do
    if [ $cdb != "directory" ]; then
      wb_cmd -v $cdb compile /all
    fi
  done

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

  for cdb in $databases; do
    if [ $cdb != "directory" ]; then
      echo "-- Creating loadfiles for database $cdb"
      wb_cmd -v $cdb create load
    fi
  done
  reload_status=$reload__success
}

reload_createboot()
{
  reload_checkpass "createboot" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass create bootfiles"

  echo "-- Creating bootfiles for all nodes"
  wb_cmd create boot/all
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

  echo -n "Do you wan't to continue ? [y/n/go] "
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

  reload.sh   Dump and reload of database.

  Arguments   Database or databases to reload.
              I no arguments is supplied, all databases will be reloaded.

  Pass

    dumpdb       Dump database to textfile \$pwrp_db/'volume'.wb_dmp
    classvolumes Create structfiles and loadfiles for classvolumes
    renamedb     Rename the old database database
    dirvolume    Load directory volume
    loaddb       Load the dump into the new database
    compile      Compile all plcprograms in the database
    createload   Create new loadfiles.
    createboot   Create bootfiles for all nodes in the project.

EOF
}

if [ "$1" = "help" ] || [ "$1" = "-h" ]; then
  usage
  exit
fi


let reload_status=$reload__success
let check_status=0
let go=0

if [ -z "$1" ]; then
  tmp=`eval ls -1d $pwrp_db/*.db`
  databases=""
  for db in $tmp; do
    db=${db##/*/}
    db="${db%.*}"
    if [ "$db" != "rt_eventlog" ]; then
      databases="$databases $db"
    fi
  done
else
  databases=$@
fi

usage

echo ""
echo "-- Reloading volume $databases"
echo ""

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
echo "-- The reload is now accomplished."
echo "   Please remove the dumpfiles: \$pwrp_db/*.wb_dmp*"
echo "   when you are satisfied with the reload."
echo ""

reload_exit











