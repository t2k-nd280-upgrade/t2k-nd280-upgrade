back=$PWD
prefix="/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/"
out_name="n_4w_hits"
in_name="/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/g4_neutron_4w.root"

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
last=1177000
segm=745

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "HitsSaver.C($first, $step, $i, \"${out_name}\", \"${in_name}\")"
	first=$((first+step))
done

hadd -f "${prefix}/${out_name}.root" "${prefix}/${out_name}_"*
#rm "$prefix/out_"*

cd $back

#echo "Plot for MiEle ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru