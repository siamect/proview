#! bin/bash
#
# Keep the old versions of bootfiles
#

new_file=$1
  
if [ -e $new_file ]; then
  let version=30

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
