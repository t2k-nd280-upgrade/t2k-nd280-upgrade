#!/bin/csh -f

set j=0
set f=1
set n=20
set s=1
set r=interactive
set mode="pion"

set hostname="`hostname`"
echo "hostname is: $hostname"

set name="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/PROD/PION_RECON/SFGD_MC.root"
set outname="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/RECO/PION_RECON/MC_output"

if ($hostname == "ifae-pc") then
	set name="/home/cjesus/Work/Data/SFGD_final/MC/PROD/PION_RECON/SFGD_MC.root"
	set outname="/home/cjesus/Work/Data/SFGD_final/MC/RECO/PION_RECON/MC_output"
endif

if($mode == "pion") then
	if ($hostname == "ifae-pc") then
        rm /home/cjesus/Work/Data/SFGD_final/MC/RECO/PION_RECON/*MC_output*
        rm /home/cjesus/Work/Data/SFGD_final/MC/RECO/PION_RECON/Reconstructed_SFGD_MC.root
	endif
endif

if( $1 == job ) then
    set r=PIC
endif

while ($j < $s)
    set command = "$T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_targetreco --softw-dir $T2KND280UP --job-dir $PWD  --resource $r --nexpts $n --nruns $f --inputfile ${name} --tag ${outname}"
    echo ${command}
    ${command}
    @ j = $j + 1
end

if ($hostname == "ifae-pc") then
    cd /home/cjesus/Work/Data/SFGD_final/MC/RECO/PION_RECON
    hadd Reconstructed_SFGD_MC.root *MC_output*
    cp Reconstructed_SFGD_MC.root /home/cjesus/Work/Dev/SFGD/sfgd_framework/analysis/data
    cd -
endif




