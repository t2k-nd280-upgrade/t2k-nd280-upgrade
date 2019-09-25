#!/bin/csh -f

set j=0
set f=1
set n=100
set s=1
set r=interactive

set hostname="`hostname`"
echo "hostname is: $hostname"

set name="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/SFGD_MC_FINAL_SIZE-0-Exp0-Nexpt1000.root"
set jobdir="/nfs/neutrinos/cjesus/work/"

if ($hostname == "ifae-pc") then
	set name="/home/cjesus/Work/Data/SFGD_final/MC/PROD/PION_RECON/SFGD_MC.root"
	set jobdir="/home/cjesus/Work/Data/SFGD_final/ANALYSIS"
endif

while ($j < $s)
    set command = "$T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_pionreco --softw-dir $T2KND280UP --job-dir $jobdir  --resource $r --nexpts $n --nruns $f --inputfile ${name} --tag ${j} --debug 0"
    echo ${command}
    ${command}
    @ j = $j + 1
end