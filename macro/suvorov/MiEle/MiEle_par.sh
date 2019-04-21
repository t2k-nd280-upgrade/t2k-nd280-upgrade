back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/Michel/
out_name="deg20"
in_name_def="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/proto2_deg23_mu500-Exp"


first=0
last=30000
segm=30

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
  in_name+="$(((i*step)))"
  in_name+="-Nexpt"
  in_name+="$(((step)))"
  in_name+=".root"
  echo "root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ \"macro/suvorov/MIEle.C(0, $step, $i, \\\"${out_name}\\\", \\\"${in_name}\\\")\"" > $name
  #first=$((first+step))

  if [ "$i" -eq "0" ]
  then
    echo "source $name; echo \"job $i from $segm done\";" > $bars/tmp/$name_dir/command.sh
  else
    echo "source $name; echo \"job $i from $segm done\";" >> $bars/tmp/$name_dir/command.sh
  fi
done

echo "Running parallel"
nice -n15 parallel -j 50 -a $bars/tmp/$name_dir/command.sh
rm -r $bars/tmp/$name_dir

echo "done"

hadd -f "${prefix}/${out_name}.root" "${prefix}/${out_name}_"*
rm "$prefix/${out_name}_"*

cd $back

#echo "Plot for pile up ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru

