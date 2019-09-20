#!/bin/bash -f

hostname="`hostname`"
echo "hostname is: $hostname"

original_pwd="`pwd`"

if [ $hostname == "ifae-pc" ]; then
  source $T2KND280UP/build/compile_script.sh -n -d
  cd $original_pwd
else
  source /software/neutrinos/t2k-nd280-upgrade/mypath.sh /software/neutrinos/t2k-nd280-upgrade/ /software/neutrinos/t2k-nd280-upgrade/build 
  source /software/neutrinos/t2k-nd280-upgrade/build/compile_script.sh
  cd $original_pwd
fi