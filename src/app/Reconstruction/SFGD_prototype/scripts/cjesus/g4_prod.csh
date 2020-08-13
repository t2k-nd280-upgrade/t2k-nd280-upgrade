#!/bin/csh -f

set j=0
set f=1
set n=1
set s=1

set hostname="`hostname`"
echo "hostname is: $hostname"

      while ($j < $s)
      if ($hostname == "ifae-pc") then
      	echo "$T2KND280UP/src/app/Reconstruction/SFGD_prototype/submit/submit_nd280upgrade_prototype --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts $n --nruns $f --job-dir /home/cjesus/Work/Data/SFGD_prototype/MC/PROD"

      	perl $T2KND280UP/src/app/Reconstruction/SFGD_prototype/submit/submit_nd280upgrade_prototype --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts $n --nruns $f --job-dir /home/cjesus/Work/Data/SFGD_prototype/MC/PROD

      else
        echo "$T2KND280UP/src/app/Reconstruction/SFGD_prototype/submit/submit_nd280upgrade_prototype --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts $n --nruns $f "

      	$T2KND280UP/src/app/Reconstruction/SFGD_prototype/submit/submit_nd280upgrade_prototype --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts $n --nruns $f
      
      endif

      @ j = $j + 1
      end
