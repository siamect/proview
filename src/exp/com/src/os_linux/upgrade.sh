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

reload_createproject()
{
  reload_checkpass "createproject" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  cat <<  EOF
    
    You should now create the project in wb_adm.
    Login in wb_adm and activate 'Create project' from
    the menu.

EOF

  reload_continue "Pass create project"
  wb_adm
  source pwrp_env.sh set project $project
}

reload_copyfile()
{

  reload_checkpass "copyfile" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  echo "*********************************************************************"
  echo ""
  echo "Before this pass upgrade.com on VMS should be executed !"
  echo ""
  echo "**********************************************************************"

  reload_continue "Pass copy files"

  if [ -e $pwrp_tmp/upgrade_files.sh ]; then
    source $pwrp_tmp/upgrade_files.sh
  else
    echo ""
    echo "Files copied from VMS host is missing on \$pwrp_tmp"
    echo ""
  fi

}

reload_createdbs()
{
  reload_checkpass "createdbs" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  cat > $pwrp_tmp/rename_dbid.pwr_com <<  EOF
! Script to change dbid in dbconfig objects to lowercase
!
main()
  string name;
  string value;
  string attr;
  string class;
  int sts;
  string volume;

  verify(0);

  volume = GetVolumeList();
  while ( volume != "")
    class = GetVolumeClass( volume);
    if ( class == "\$DirectoryVolume")
      break;
    endif
    volume = GetNextVolume( volume);
  endwhile

  if ( volume == "")
    exit(0);
  endif

  set volume/volume='volume'

  name = GetRootList();
  while ( name != "")
    class  = GetObjectClass( name);
    if ( class == "DbConfig")
      attr = name + ".Id";
      value = GetAttribute( attr, sts);
      if ( sts)
        value = tolower( value);
        set attr/name='name'/attr=Id/value="'value'"/noconfirm
      endif
    endif
    if ( class == "NodeConfig")
      attr = name + ".NodeName";
      value = GetAttribute( attr, sts);
      if ( sts)
        value = tolower( value);
        set attr/name='name'/attr=NodeName/value="'value'"/noconfirm
      endif
      attr = name + ".BootNode";
      value = GetAttribute( attr, sts);
      if ( sts)
        value = tolower( value);
        set attr/name='name'/attr=BootNode/value="'value'"/noconfirm
      endif
    endif
    if ( class == "\$System")
      attr = name + ".SystemName";
      value = GetAttribute( attr, sts);
      if ( sts)
        value = tolower( value);
        set attr/name='name'/attr=SystemName/value="'value'"/noconfirm
      endif
    endif
    name = GetNextSibling(name);
  endwhile
  save

endmain
EOF

  wb_cmd @$pwrp_tmp/rename_dbid.pwr_com

  cat <<  EOF
    
    You should now enter the project volume, and check that the
    volume is loaded correctly.

    You should then set edit mode, and save.
    This operation will create the databases that are configured in
    the project volume.

EOF

  reload_continue "Pass create databases"

  source pwrp_env.sh setdb
  wb sysansv sysansv
}

reload_dumpdb()
{
  reload_checkpass "dumpdb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass dump database"

  for cdb in $databases; do
    echo "Dumping database $cdb"
     
    source pwrp_env.sh setdb $cdb

    if [ $cdb != "dbdirectory" ]; then
      #Store versions of classvolumes
      wb_cmd @$pwr_exe/reload_vol_versions $cdb
    fi

    dump_file=$pwrp_db/$cdb.wb_dmp
    reload_save_file $dump_file
    wb_cmd wb dump/out=\"$dump_file\"
  done
}

reload_templatedb()
{
  reload_checkpass "templatedb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass copy template database"

  for cdb in $databases; do
    echo "-- Copy template database to $cdb"
     
    source pwrp_env.sh copy template $cdb noconfirm
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

  databases=`eval source pwrp_env.sh show db -a`

  for cdb in $databases; do
    if [ $cdb != "dbdirectory" ]; then
      source pwrp_env.sh setdb $cdb

      # Load usertypes
      fname="$pwrp_db/$cdb/usertypes.wb_load"
      list_file="$pwrp_db/"$cdb"_usertypes.lis"
      if [ -e $fname ]; then
        echo "-- Loading usertypes in $cdb $fname"
        if wb_cmd wb load/load=\"$fname\"/out=\"$list_file\"
        then
          reload_status=$reload__success
        else
          reload_status=$reload__usertypes
          cat $list_file
          return
        fi
      else
        fname="$pwrp_db/usertypes.wb_load"
        list_file="$pwrp_db/"$cdb"_usertypes.lis"
        if [ -e $fname ]; then
          echo "-- Loading usertypes in $cdb $fname"
          if wb_cmd wb load/load=\"$fname\"/out=\"$list_file\"
          then
            reload_status=$reload__success
          else
            reload_status=$reload__usertypes
            cat $list_file
            return
          fi
        fi
      fi

      # Load userclasses
      fname="$pwrp_db/$cdb/userclasses.wb_load"
      list_file="$pwrp_db/"$cdb"_userclasses.lis"
      if [ -e $fname ]; then
        echo "-- Loading userclasses in $cdb $fname"
        if wb_cmd wb load/load=\"$fname\"/out=\"$list_file\"
        then
          reload_status=$reload__success
        else
          reload_status=$reload__userclasses
          cat $list_file
          return
        fi
      else
        fname="$pwrp_db/userclasses.wb_load"
        list_file="$pwrp_db/"$cdb"_userclasses.lis"
        if [ -e $fname ]; then
          echo "-- Loading userclasses in $cdb $fname"
          if wb_cmd wb load/load=\"$fname\"/out=\"$list_file\"
          then
            reload_status=$reload__success
          else
            reload_status=$reload__userclasses
            cat $list_file
            return
          fi
        fi
      fi
    fi
         
  done
}

reload_settemplate()
{
  reload_checkpass "settemplate" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  cat << EOF
   
    The set_template file should be converted from a VMS-command file
    to a pwrc script.

    Remove all VMS rows, beginning with $, and save the file.

    nedit will be started so you can do this.

    Then the file will be executed.
EOF

  reload_continue "Pass set template values"

  nedit $pwrp_db/set_template.pwr_com

  databases=`eval source pwrp_env.sh show db -a`

  fname="$pwrp_db/set_template.pwr_com"
  if [ -e $fname ]; then
    for cdb in $databases; do
      if [ $cdb != "dbdirectory" ]; then
        source pwrp_env.sh setdb $cdb

        echo "-- Setting template values in $cdb $fname"
        if wb_cmd @$fname
        then
          reload_status=$reload__success
        else
          reload_status=$reload__settemplate
          return
        fi
      fi
    done
  else
    echo "-- No set_template file found"
    reload_status=$reload__success
  fi
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

reload_convertdb()
{
  reload_checkpass "convertdb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass convert database"

  databases=`eval source pwrp_env.sh show db -a`

  for cdb in $databases; do
    echo "-- Loading database $cdb"
     
    source pwrp_env.sh setdb $cdb

    wb_drive_convert
 done
}

reload_loaddirectorydb()
{
  reload_checkpass "loaddirectorydb" $start_pass
  if [ $pass_status -ne $pass__execute ]; then
    reload_status=$reload__success
    return
  fi

  reload_continue "Pass load database"

  databases="dbdirectory"

  for cdb in $databases; do
    echo "-- Loading database $cdb"
     
    source pwrp_env.sh setdb $cdb
    wb_cmd delete volume/name=ProjectVolume/noconf

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
      reload_status=$reload__success
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

  databases=`eval source pwrp_env.sh show db -a`

  for cdb in $databases; do
    if [ $cdb != "dbdirectory" ]; then
      source pwrp_env.sh setdb $cdb

      echo "-- Compiling database $cdb"
      wb_cmd compile/allvolumes
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

  databases=`eval source pwrp_env.sh show db -a`

  for cdb in $databases; do
    if [ $cdb != "dbdirectory" ]; then
      source pwrp_env.sh setdb $cdb

      echo "-- Creating loadfiles for database $cdb"
      wb_cmd create load/class/all
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

  upgrade.sh  Upgrade from V3.3 to V3.4.

  The upgrade procedure will be done in three steps:

  - Create project on linux (Pass 1)
  - Execute upgrade.com on V3.3
  - Contiue with pass copyfile

  Arguments   Project name.

  Pass

    createproject  Create project on linux
    copyfile     Distribute files copied from V3.3
    loaddirectorydb Load the project volume into the directory db.
    createdbs    Create configured databases.
    userclasses  Load userclasses.wb_load (and usertypes.wb_load)
                 reload will first the load_files in  \$pwrp_db/'dbid'/
                 and then in \$pwrp_db/
    settemplate  Set template values. \$pwrp_db/set_template.pwr_com is
                 executed
    loaddb       Load the dump into the new database
    convertdb    Convert some classes (Document classes)
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
  usage
  exit
fi

project=$1

usage

echo ""
echo "-- Upgrade $project"
echo ""

passes="createproject copyfile loaddirectorydb createdbs userclasses settemplate loaddb convertdb compile createload createboot"
echo "Pass: $passes"
echo ""
echo -n "Enter start pass [createproject] > "
read start_pass

if [ -z $start_pass ]; then
  start_pass="createproject"
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











