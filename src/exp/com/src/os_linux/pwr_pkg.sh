#!/bin/bash

pkg_install_func ()
{
  if [ ! -e "$1" ]; then
    echo "No package $1"
    exit 1
  fi

  pkg=`eval locate $1`

  pkg_remove_func "force"
  echo "-- Installing package $1"
  cd /tmp
  if ! tar -xzf $pkg pkg_unpack.sh; then
    echo "Package $1 incomplete"
    exit 1
  fi

  chmod a+x pkg_unpack.sh
  ./pkg_unpack.sh
}

pkg_list_func ()
{
  if [ -z $1 ]; then
    # List installed package

    if [ ! -e $pwrp_load/pwr_pkg.dat ]; then
	echo "-- No package installed"
	exit 1
    fi  

    datfile=$pwrp_load/pwr_pkg.dat
  else
    # Extract datfile from package
    if [ ! -e $1 ]; then
      echo "-- No such package"
      exit 1
    fi

    pkg=`eval locate $1`

    echo "-- Opening file $pkg"

    cd /tmp
    if ! tar -xzf $pkg pwr_pkg.dat; then
      echo "Package $1 incomplete"
      exit 1
    fi
    datfile=/tmp/pwr_pkg.dat
  fi

  {
    let printout=0
    while read line; do
      if [ "$line" = "%Description:" ]; then
        printout=1
      else
        if [ "$line" = "%Files:" ]; then
          break
        fi
        if [ $printout -eq 1 ]; then
          echo $line
        fi
      fi
    done
  } < $datfile
}

pkg_listfiles_func ()
{
  if [ -z $1 ]; then
    # List installed package

    if [ ! -e $pwrp_load/pwr_pkg.dat ]; then
	echo "-- No package installed"
	exit 1
    fi  

    datfile=$pwrp_load/pwr_pkg.dat
  else
    # Extract datfile from package
    if [ ! -e $1 ]; then
      echo "-- No such package"
      exit 1
    fi

    pkg=`eval locate $1`

    echo "-- Opening file $pkg"

    cd /tmp
    if ! tar -xzf $pkg pwr_pkg.dat; then
      echo "Package $1 incomplete"
      exit 1
    fi
    datfile=/tmp/pwr_pkg.dat
  fi

  {
    let printout=0
    while read line date time; do
      if [ "$line" = "%Files:" ]; then
        printout=1
      else
        if [ "$line" = "%End:" ]; then
          break
        fi
        if [ $printout -eq 1 ]; then
          echo $date $time $line
        fi
      fi
    done
  } < $datfile
}

pkg_brief_func ()
{
  if [ -z $1 ]; then
    # List installed package

    if [ ! -e $pwrp_load/pwr_pkg.dat ]; then
	echo "-- No package installed"
	exit 1
    fi  

    datfile=$pwrp_load/pwr_pkg.dat
    {
      let printout=0
      while read line; do
        if [ "$line" = "%Brief:" ]; then
          printout=1
        else
          if [ "$line" = "%Description:" ]; then
            break
          fi
          if [ $printout -eq 1 ]; then
            echo $line
          fi
        fi
      done
    } < $datfile

  else
    # Extract datfile from package

    for file
    do
      if [ ! -e $file ]; then
        echo "-- No such package"
        exit 1
      fi

      pkg=`eval locate $file`
      dir=`eval pwd`

      cd /tmp
      if ! tar -xzf $pkg pwr_pkg.dat; then
        echo "Package $pkg incomplete"
      else
        datfile=/tmp/pwr_pkg.dat

        {
          let printout=0
          while read line; do
            if [ "$line" = "%Brief:" ]; then
              printout=1
            else
              if [ "$line" = "%Description:" ]; then
                break
              fi
              if [ $printout -eq 1 ]; then
                echo $line
              fi
            fi
          done
        } < $datfile
      fi
      cd $dir
    done
  fi
}

pkg_remove_func ()
{
  if [ ! -e $pwrp_load/pwr_pkg.dat ]; then
    echo "-- No package installed"
    return
  fi

  # Get the name of the current package
  {
    let found=0
    while read line; do
      if [ "$line" = "%Package:" ]; then
        found=1
      else
        if [ $found -eq 1 ]; then
	  pkg=$line
	  break
        fi
      fi
    done
  } < $pwrp_load/pwr_pkg.dat

  if [ ! $1 = "force" ]; then
    echo ""
    echo -n "Do you wan't to remove package $pkg (y/n) [n] "
    read remove_pkg
    if [ ! "$remove_pkg" = "y" ]; then
      return
    fi
  fi

  echo "-- Removing package $pkg"

  {
    let removefile=0
    while read line date time; do
      if [ "$line" = "%Files:" ]; then
        removefile=1
      else
        if [ "$line" = "%End:" ]; then
          break
        fi
        if [ $removefile -eq 1 ]; then
	  file=`eval echo $line`
	  #echo "rm $file"
          rm $file
        fi
      fi
    done
  } < $pwrp_load/pwr_pkg.dat

  rm $pwrp_load/pwr_pkg.dat
}

force="no"
while getopts ":i:l:b:fra:" opt; do

  case $opt in
    i ) pkg_install_func $OPTARG ;;

    l ) pkg_list_func $OPTARG ;;

    a ) pkg_listfiles_func $OPTARG;;

    b ) shift
        pkg_brief_func $@ ;;

    r ) pkg_remove_func $force ;;

    f ) force="force" ;;

    \? ) echo 'usage: pwr_pkg [-i pkg] [-l] [-b] [-r]'
         exit 1
  esac
done








