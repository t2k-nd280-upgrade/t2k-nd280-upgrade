back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/gamma/
out_name="out_tm_8"
in_name="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/gamma_e800_v1-Exp8000-Nexpt1000.root"

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
last=1000
segm=5

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "gamma.C($first, $step, $i, \"${out_name}\", \"${in_name}\")"
	first=$((first+step))
done

hadd -f "${prefix}/out_800_8.root" "$prefix/out_tm_8"*
#rm "$prefix/out_"*

cd $back

#echo "Plot for MiEle ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru