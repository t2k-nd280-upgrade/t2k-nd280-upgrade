#!/bin/bash -f

hostname="`hostname`"
echo "hostname is: $hostname"

original_pwd="`pwd`"

if [ $hostname == "ifae-pc" ]; then
  source $T2KND280UP/build/compile_script.sh -n -d
  cd $original_pwd
else
  source $T2KND280UP/build/compile_script.sh -n -d
  cd $original_pwd
fi