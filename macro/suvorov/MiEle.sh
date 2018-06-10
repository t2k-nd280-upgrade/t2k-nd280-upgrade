back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/Michel/

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
last=25000
segm=100

step=$(((last-first) / segm))

for i in `seq 0 $((segm-2))`;
do
	first=$((first+step))
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "MIEle.C($first, $step, $i)"
done

hadd -f "$prefix/out.root" "$prefix/out_"*
rm "$prefix/out_"*

cd $back