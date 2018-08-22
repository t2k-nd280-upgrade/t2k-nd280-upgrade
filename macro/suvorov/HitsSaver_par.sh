back=$PWD
prefix=/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/
out_name="sand_muon_hits"
in_name_def="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/sand_muon_all_hits-Exp"


first=0
last=690000
segm=690

det=10
iter=-1

step=$(((last-first) / segm))

name_dir="$RANDOM"
mkdir "$bars/tmp/$name_dir"

first_i=$(((first/step)))

for i in `seq $(((first_i))) $((segm-1+first_i))`;
do
  name="$bars/tmp/$name_dir/"
  name+="$(((RANDOM*1000000+RANDOM)))"
  name+=".sh"

  # if [ "$(((i%det)))" -eq "0" ]
  # then
  #   iter=$(((iter+1)))
  #   in_name_def="/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/anumu_1e21_${iter}-Exp"
  # fi

  in_name="$in_name_def"
  in_name+="$(((i*step)))"
  in_name+="-Nexpt"
  in_name+="$(((step)))"
  in_name+=".root"
  echo "root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ \"macro/suvorov/HitsSaver.C(0, $step, $i, \\\"${out_name}\\\", \\\"${in_name}\\\")\"" > $name
  #first=$((first+step))

  if [ "$i" -eq "0" ]
  then
    echo "source $name; echo \"job $i from $segm done\";" > $bars/tmp/$name_dir/command.sh
  else
    echo "source $name; echo \"job $i from $segm done\";" >> $bars/tmp/$name_dir/command.sh
  fi
done

echo "Running parallel"
parallel -j 6 -a $bars/tmp/$name_dir/command.sh
rm -r $bars/tmp/$name_dir

echo "done"

#hadd -f "${prefix}/${out_name}.root" "${prefix}/${out_name}_*"
#rm "$prefix/${out_name}_"*

cd $back

echo "Hits storage for ${out_name}" | mailx -s "Hits saver task complete. Result is stored if $prefix/$out_name" suvorov@inr.ru

