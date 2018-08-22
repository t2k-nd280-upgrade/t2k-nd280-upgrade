back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/PileUp/
out_name="plot_sand_muon"
in_name_def="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/sand_muon_all_hits-Exp"

#cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
# neutron
# 4w - 744 736
# 4a - 2 216 248
# 7b - 1 176 630
# gamma
# 4w - 126 632
# 7b - 168 925
# sand
# 3  - 689 053
last=690000
segm=370

step=$(((last-first) / segm))

name_dir="$RANDOM"
mkdir "$bars/tmp/$name_dir"

first_i=$(((first/step)))

for i in `seq $(((first_i))) $((segm-1+first_i))`;
do
  name="$bars/tmp/$name_dir/"
  name+="$(((RANDOM*1000000+RANDOM)))"
  name+=".sh"

  in_name="$in_name_def"
  in_name+="$(((i*1000)))"
  in_name+="-Nexpt1000.root"
  echo "root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ \"macro/suvorov/pile_up.C(0, $step, $i, \\\"${out_name}\\\", \\\"${in_name}\\\", 0)\"" > $name
  #first=$((first+step))

  if [ "$i" -eq "0" ]
  then
    echo "source $name; echo \"job $i from $segm done\";" > $bars/tmp/$name_dir/command.sh
  else
    echo "source $name; echo \"job $i from $segm done\";" >> $bars/tmp/$name_dir/command.sh
  fi
done

echo "Running parallel"
nice -n19 parallel -j 6 -a $bars/tmp/$name_dir/command.sh
rm -r $bars/tmp/$name_dir

echo "done"

#hadd -f "$prefix/${out_name}.root" "$prefix/${out_name)}_*"
#rm "$prefix/${out_name}_"*

cd $back

echo "Pile up script task complete" | mailx -s "Pile up task complete for ${out_name}" suvorov@inr.ru

