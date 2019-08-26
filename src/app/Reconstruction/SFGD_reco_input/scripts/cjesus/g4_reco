#!/bin/csh -f

set j=0
set f=1
set n=1000
set s=1
set r=interactive

set name="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/SFGD_MC_FINAL_SIZE-0-Exp0-Nexpt1000.root"
set outname="/nfs/neutrinos/cjesus/work/MC_output.root"

if( $1 == job ) then
    set s=200
    set r=PIC
endif

while ($j < $s)
    if( $1 == job) set name="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/SFGD_MC_FINAL_SIZE-$j-Exp${j}000-Nexpt1000.root"
    if ($j == 0 && $1 == job) set name="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/SFGD_MC_FINAL_SIZE-$j-Exp0-Nexpt1000.root"
    if( $1 == job) set outname="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/ROOT/MC_output-$j-Exp${j}000-Nexpt1000.root"
    set command = "$T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_targetreco --softw-dir $T2KND280UP --job-dir $PWD  --resource $r --nexpts $n --nruns $f --inputfile ${name} --tag ${outname}"
    echo ${command}
    ${command}
    @ j = $j + 1
end
