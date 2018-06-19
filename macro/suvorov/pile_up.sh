back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/PileUp/
out_name="out_4w"

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
# 4w - 744736
# 4a - 2216248
last=3
segm=3

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "pile_up.C($first, $step, $i)"
	first=$((first+step))
done

hadd -f "$prefix/${out_name}.root" "$prefix/${out_name)}_"*
rm "$prefix/${out_name}_"*

cd $back

echo "Plot for pile up ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru