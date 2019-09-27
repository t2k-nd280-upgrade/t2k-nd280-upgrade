#!/bin/csh -f

set j=0
set f=1
set n=1000
set s=1
set r=interactive

set hostname="`hostname`"
echo "hostname is: $hostname"

if ($hostname == "ifae-pc") then
        rm /home/cjesus/Work/Data/SFGD_prototype/MC/RECO/*MC_output*
endif


set name="/nfs/neutrinos/cjesus/work/SFGD_MC_Prototype/SFGD_MC_-0-Exp0-Nexpt1000.root"
set outname="/nfs/neutrinos/cjesus/work/MC_output"

if ($hostname == "ifae-pc") then
	set name="/home/cjesus/Work/Data/SFGD_prototype/MC/PROD/SFGD_MC_-0-Exp0-Nexpt10000.root"
	set outname="/home/cjesus/Work/Data/SFGD_prototype/MC/RECO/MC_output"
endif

if( $1 == job ) then
    set s=420
    set r=PIC
endif

while ($j < $s)
    if( $1 == job) set name="/nfs/neutrinos/cjesus/work/SFGD_MC_Prototype/SFGD_MC_Prototype-$j-Exp${j}000-Nexpt1000.root"
    if ($j == 0 && $1 == job) set name="/nfs/neutrinos/cjesus/work/SFGD_MC_Prototype/SFGD_MC_Prototype-$j-Exp0-Nexpt1000.root"
    if( $1 == job) set outname="/nfs/neutrinos/cjesus/work/SFGD_MC_Prototype/ROOT/MC_output-$j-Exp${j}000-Nexpt1000.root"
    set command = "$T2KND280UP/src/app/Reconstruction/SFGD_prototype/submit/submit_nd280upgrade_targetreco --softw-dir $T2KND280UP --job-dir $PWD  --resource $r --nexpts $n --nruns $f --inputfile ${name} --tag ${outname}"
    echo ${command}
    perl ${command}
    @ j = $j + 1
end

if ($hostname == "ifae-pc") then
    cd /home/cjesus/Work/Data/SFGD_prototype/MC/RECO/
    hadd MC_output.root *MC_output*
    cd -
endif