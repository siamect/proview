# Replace all ö in Oxelösund
#set -o xtrace

files=$@
from="ö"
to="o"

for file in $files; do
  cp $file $file.1
  echo "s/$from/$to/" > repl.sed
  sed -f repl.sed $file > repl.tmp
  cp repl.tmp $file
done
