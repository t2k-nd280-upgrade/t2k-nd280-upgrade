back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/PileUp/
out_name="plot_hits_mi_ele_v1"
in_name="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/proto2_mu500_deg_25000.root"

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
# neutron
# 4w - 744 736
# 4a - 2 216 248
# 7b - 1 176 630
# gamma
# 4w - 126 632
# 7b - 168 925
last=25000
segm=25

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "pile_up.C($first, $step, $i, \"${out_name}\", \"${in_name}\", 0)"
	first=$((first+step))
done

hadd -f "$prefix/${out_name}.root" "$prefix/${out_name)}_"*
#rm "$prefix/${out_name}_"*

cd $back

echo "Plot for pile up ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru