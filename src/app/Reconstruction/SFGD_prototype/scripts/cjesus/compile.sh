#!/bin/sh -f

hostname_var="`hostname`"

if [ $hostname_var == "ifae-pc" ]; then
	echo "working @ $hostname_var"
	source $T2KND280UP/build/compile_script.sh -n -d
else
	source /software/neutrinos/t2k-nd280-upgrade/mypath.sh /software/neutrinos/t2k-nd280-upgrade/ /software/neutrinos/t2k-nd280-upgrade/build 
	source /software/neutrinos/t2k-nd280-upgrade/build/compile_script.sh
fi

cd $T2KND280UP/src/app/Reconstruction/SFGD_prototype/scripts/cjesus
