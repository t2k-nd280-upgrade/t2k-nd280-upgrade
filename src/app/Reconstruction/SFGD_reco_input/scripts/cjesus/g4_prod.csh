#!/bin/csh -f

set j=0
set f=1
set n=10
set s=1

set hostname="`hostname`"
echo "hostname is: $hostname"

if ($hostname == "ifae-pc") then
	set jobdir="/home/cjesus/Work/Data/SFGD_final/MC/PROD/"
else
	set jobdir="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/"
endif

while ($j < $s)
	echo "$T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_sfgd_final --softw-dir $T2KND280UP --job-dir ${jobdir} --resource interactive --nexpts $n --nruns $f"
	perl $T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_sfgd_final --softw-dir $T2KND280UP --job-dir ${jobdir} --resource interactive --nexpts $n --nruns $f
	@ j = $j + 1
end