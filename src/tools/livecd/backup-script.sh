#!/bin/bash

lsof 2>&1 | grep -cqE 'usr/(local/)?pwr' && zenity --info --text "There are files in /usr/pwr* in use. It is strongly recommended to close all Proview instances and files before continuing."

medialist=$(mount | awk '/on \/media\// {print $3}')
echo -n "Found media:"
if [[ -z "$medialist" ]] ; then
	echo " NONE"
	zenity --error --text "No backup media found.\n\nPlease mount an external storage device such as an USB flash drive."
	exit 2
else
	echo
	echo $medialist
fi

media=$(echo $medialist | zenity --list --text "Please select the mass storage to backup your Proview data to:"  --column=Media)

if [[ -z "$media" ]] ; then
	echo "You have to select a media to store the data on" >&2
	zenity --error --text "You have to select a media to store the data on."
	exit 1
else
	echo "Using \"$media\" for backup"
fi

echo "Compressing data, please wait ..."

target="${media}/proview-livecd-backup.tar.lzma"
cd /
nice sudo /bin/tar -cp --exclude '*.lock' usr/local/pwr* usr/pwrp/adm | pv | nice -n 19 lzma >${target} || zenity --error --text "Creating the backup failed.\n\nPlease check the error messages as shown in the terminal."
ls -l ${target}

test -s "${target}" && zenity --info --text 'The backup has been saved as\n\n'"${target}"

sleep 5

