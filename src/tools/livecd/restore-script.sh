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

media=$(echo $medialist | zenity --list --text "Please select the mass storage to restore your Proview data from:"  --column=Media)

if [[ -z "$media" ]] ; then
	echo "You have to select a media to restore the data from" >&2
	zenity --error --text "You have to select a media to restore the data from."
	exit 1
else
	echo "Using \"$media\" for restore"
fi

target="${media}/proview-livecd-backup.tar.lzma"
if [[ ! -f "${target}" ]] ; then
	echo "Backup file does not exist: ${target}" >&2
	zenity --error --text "Backup file does not exist:\n\n${target}"
	exit 2
fi

# make backup of old files & directories to be overwritten by this restore script
cd /usr/local
sudo /bin/rm -rf _backup_pwr*
for old in pwr* ; do test -e "$old" && sudo /bin/mv "$old" "_backup_$old" ; done
sudo /bin/rm -rf /usr/pwrp/_backup_adm
test -e /usr/pwrp/adm && sudo /bin/mv /usr/pwrp/adm /usr/pwrp/_backup_adm

nice -n 19 unlzma <${target} | pv | nice sudo /bin/tar -xp -C /
if [ $? ] ; then
	ls -l ${target}
	zenity --info --text "Restoring the backup succeeded.\n\nStill, please check that there are no error messages shown in the terminal."

	# remove temporary backup
	cd /usr/local
	sudo /bin/rm -rf _backup_pwr*
	sudo /bin/rm -rf /usr/pwrp/_backup_adm
else
	ls -l ${target}
	zenity --error --text "Restoring the backup failed.\n\nPlease check the error messages as shown in the terminal."

	# restore went wrong, restore previous state
	cd /usr/local
	sudo /bin/rm -rf pwr*
	sudo /bin/rm -rf /usr/pwrp/adm
	for old in _backup_pwr* ; do test -e "$old" && sudo /bin/mv "$old" "${old/_backup_}" ; done
	test -e /usr/pwrp/_backup_adm && sudo /bin/mv /usr/pwrp/_backup_adm /usr/pwrp/adm
fi

sleep 5


