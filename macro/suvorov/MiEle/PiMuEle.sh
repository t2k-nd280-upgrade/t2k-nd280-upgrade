back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/Michel/

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
last=1000
segm=1

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "PiMuEle.C($first, $step, $i)"
	first=$((first+step))
done

#hadd  -f "$prefix/out.root" "$prefix/out_"*
#rm "$prefix/out_"*

cd $back

#echo "Plot for PiMuEle ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru
