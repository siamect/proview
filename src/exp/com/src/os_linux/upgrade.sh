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
v34_root="/data1/pwr/x3-4b/rls_dbg"

reload_dumpdb()
{
  # Dump V3.4b databases, one wbload file per volume

  reload_checkpass "dumpdb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dump database"

  rm $pwrp_db/*.wb_dmp

  for cdb in $databases; do
    echo "Dumping database $cdb"
     
    export pwr_inc=$v34_root/os_linux/hw_x86/exp/inc
    source $v34_root/os_linux/hw_x86/exp/exe/pwrp_env.sh setdb $cdb
    export pwr_inc=$pwrb_root/os_linux/hw_x86/exp/inc

    # Create a script that dumps each volume
    tmpfile=$pwrp_tmp/dump.sh
    cat > $tmpfile <<-EOF
	main()
	  string v;
	  string class;
	  string outfile;

	  v = GetVolumeList();
	  while( v != "")
	    class = GetVolumeClass( v);
	    if ( class != "\$ClassVolume")
	      outfile = "$pwrp_db/" + v + ".wb_dmp";
	      outfile = tolower( outfile);
	      printf( "-- Dump volume %s to %s\n", v, outfile);
	      wb dump/v40/volume='v'/out="'outfile'"
	    endif
	    v = GetNextVolume( v);
	  endwhile
  	endmain
EOF
    chmod a+x $tmpfile
    $v34_root/os_linux/hw_x86/exp/exe/wb_cmd @$tmpfile
  done
}

reload_userclasses()
{
  reload_checkpass "userclasses" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass load userclasses"

 # Load userclasses
  fname="$pwrp_db/userclasses.wb_load"
  if [ -e $fname ]; then
    echo "-- Loading userclasses $fname"

    volume=`eval grep pwr_eClass_ClassVolume $fname | awk '{ print tolower($2) }'`
    if wb_cmd create snapshot/file=\"$fname\"/out=\"$pwrp_load/$volume.dbs\"
    then
      reload_status=$reload__success
    else
      reload_status=$reload__userclasses
      return
    fi
  fi
}

reload_dirvolume()
{
  reload_checkpass "dirvolume" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dirvolume"

  dmpfiles=`eval ls $pwrp_db/*.wb_dmp`
  echo $dmpfiles

  for dmpfile in $dmpfiles; do
    volume=`eval grep DirectoryVolume $dmpfile | awk '{ print $2 }'`
    if [ ! -z "$volume" ]; then
      echo "Volume: $volume"
      #mv $dmpfile $pwrp_db/directory.wb_dmp
      l1="Volume $volume \$DirectoryVolume"
      l2="Volume Directory \$DirectoryVolume"
      sed 's/ '$volume' / Directory /' $dmpfile > $pwrp_db/directory.wb_dmp
      wb_cmd wb load /load=\"$pwrp_db/directory.wb_dmp\"
      mv $dmpfile $dmpfile"_old"
      mv $pwrp_db/directory.wb_dmp $pwrp_db/directory.wb_dmp_old
      break
    else
      db=""
    fi
  done
}

reload_cnvdirvolume()
{
  reload_checkpass "cnvdirvolume" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass cnvdirvolume"

  # Create a script that dumps each volume
  tmpfile=$pwrp_tmp/cnvdirvolume.sh
  cat > $tmpfile << EOF
main
  string object;
  string class;
  string child;
  string sibling;

  set volume/volume=Directory

!  verify(1);
  object = GetRootList();
  while ( object != "")
    class = GetObjectClass( object);
    if ( class == "DbConfig")
      child = GetChild( object);
      while ( child != "")
        sibling = GetNextSibling( child);
        move object/source='child'/dest='object'/after
	child = sibling;
      endwhile
      sibling = GetNextSibling( object);
      delete object/noconf/nolog/name='object'
      object = sibling;
    else
      object = GetNextSibling( object);
    endif
  endwhile
  save
endmain
EOF
  chmod a+x $tmpfile
  wb_cmd @$tmpfile
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
      sed 's/ GetIp / GetIpToA /; s/ StoIp / StoAtoIp /; s/ CStoIp / CStoAtoIp /; s/Class-GetIp/Class-GetIpToA/; s/Class-StoIp/Class-StoAtoIp/; s/Class-CStoIp/Class-CStoAtoIp/' $dmpfile > $pwrp_tmp/t.wb_dmp
      mv $pwrp_tmp/t.wb_dmp $dmpfile
    fi
  done
}


reload_createvolumes()
{
  reload_checkpass "createvolumes" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass create volumes"

  dmpfiles=`eval ls $pwrp_db/*.wb_dmp`
  echo $dmpfiles

  for dmpfile in $dmpfiles; do
    file=${dmpfile##/*/}
    db="${file%.*}.db"
    if [ $db = "wb.db" ]; then
      db=""
    else
      wb_cmd wb load /load=\"$dmpfile\"
    fi
  done
}


reload_localwb()
{
  reload_checkpass "localwb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass create localWb volume"

  cat << EOF >> $pwrp_db/wb.wb_load
!
! localWB volume
! This volume contains template objects and local listdescriptors.
!
Volume localWb pwr_eClass_WorkBenchVolume 254.254.254.252
EndVolume

EOF

  sed 's/SObject wb:/SObject localWb:/' $pwrp_db/wb.wb_dmp >> $pwrp_db/wb.wb_load
}

reload_loaddb()
{
  reload_checkpass "loaddb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass load database"

  databases=`eval source pwrp_env.sh show db -a`

  for cdb in $databases; do
    echo "-- Loading database $cdb"
     
    source pwrp_env.sh setdb $cdb

    if [ $cdb != "dbdirectory" ]; then
      #Restore versions of classvolumes
      if [ -e $pwrp_db/reload_vol_versions_$cdb.pwr_com ]; then
        wb_cmd @$pwrp_db/reload_vol_versions_$cdb
      fi
    fi

    dump_file=$pwrp_db/$cdb.wb_dmp
    list_file=$pwrp_db/$cdb.lis
    if wb_cmd wb load/load=\"$dump_file\"/out=\"$list_file\"/ignore
    then
      reload_status=$reload__success
    else
      cat $list_file
      reload_status=$reload__loaddb
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

  wb_cmd compile /all
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

  wb_cmd create load/all
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

reload_convertge()
{
  reload_checkpass "convertge" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass convert ge graphs"

  # Create a script that dumps each volume
  tmpfile=$pwrp_tmp/convertv40.ge_com
  cat > $tmpfile << EOF
function int process( string graph)
  open 'graph'
  printf( "Converting %s...\n", graph);
  convert v40
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

  upgrade.sh  Upgrade from V3.3 to V3.4.

  The upgrade procedure will be done in three steps:

  - Create project on linux (Pass 1)
  - Execute upgrade.com on V3.3
  - Continue with pass copyfile

  Arguments   Project name.

  Pass

    dumpdb	 Dump database to textfile \$pwrp_db/'volume'.wb_dmp
    userclasses  Load userclasses.wb_load
    dirvolume    Create directory volume.
    cnvdirvolume Convert the directory volume.
    cnvdump      Convert dumpfiles.
    createvolumes Create configured databases.
    localwb      Create LocalWb volume for lists and template objects.
    compile      Compile all plcprograms in the database
    createload   Create new loadfiles.
    createboot   Create bootfiles for all nodes in the project.
    convertge    Convert ge graphs.
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
  usage
  exit
fi

project=$1

usage

echo ""
echo "-- Upgrade $project"
echo ""

export pwr_inc=$v34_root/os_linux/hw_x86/exp/inc
echo $pwr_inc
databases=`eval source $v34_root/os_linux/hw_x86/exp/exe/pwrp_env.sh show db -a`
databases=$databases" dbdirectory"
export pwr_inc=$pwrb_root/os_linux/hw_x86/exp/inc

passes="dumpdb userclasses dirvolume cnvdirvolume cnvdump createvolumes localwb compile createload createboot convertge"
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











