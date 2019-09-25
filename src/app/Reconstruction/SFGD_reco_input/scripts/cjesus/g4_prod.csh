#!/bin/csh -f

set j=0
set f=1
set n=1000
set s=1
set mode="pion"
set remove="yes"

set hostname="`hostname`"
echo "hostname is: $hostname"

if ($hostname == "ifae-pc" && $remove == "yes") then
	if ($mode == "pion") then
		rm /home/cjesus/Work/Data/SFGD_final/MC/PROD/PION_RECON/*SFGD_MC*
	else
		rm /home/cjesus/Work/Data/SFGD_final/MC/PROD/*SFGD_MC*
	endif
endif

if ($hostname == "ifae-pc") then
	if ($mode == "pion") then
		set jobdir="/home/cjesus/Work/Data/SFGD_final/MC/PROD/PION_RECON"
	else
		set jobdir="/home/cjesus/Work/Data/SFGD_final/MC/PROD"
	endif
else
	set jobdir="/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/"
endif

while ($j < $s)
	echo "$T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_sfgd_final --softw-dir $T2KND280UP --job-dir ${jobdir} --resource interactive --nexpts $n --nruns $f"
	perl $T2KND280UP/src/app/Reconstruction/SFGD_reco_input/submit/submit_nd280upgrade_sfgd_final --softw-dir $T2KND280UP --job-dir ${jobdir} --resource interactive --nexpts $n --nruns $f
	@ j = $j + 1
end

if ($hostname == "ifae-pc" && $remove == "yes") then
	if ($mode == "pion") then
		cd /home/cjesus/Work/Data/SFGD_final/MC/PROD/PION_RECON
		hadd SFGD_MC.root *SFGD_MC_FINAL_SIZE*
		cd -
	else
		cd /home/cjesus/Work/Data/SFGD_final/MC/PROD
		hadd SFGD_MC.root *SFGD_MC_FINAL_SIZE*
		cd -
	endif
endif
