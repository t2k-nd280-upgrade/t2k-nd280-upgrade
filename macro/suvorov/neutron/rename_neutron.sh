#!bin/sh

ls /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron//neutron_v7-1* > temp.ls
array=($(cat temp.ls))
echo "Modify ${#array[@]} files"
for ((i=0; i<${#array[@]}; i++));
do
	origin=${array[$i]}
	exp=${origin##*Exp}
	exp=${exp%-*}
	exp=$((200000 + ${exp}))

	modyfied="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron//neutron_v7-Exp${exp}-Nexpt1000.root"

	#echo "Origin ${origin}"
	#echo "Mody   ${modyfied}"
	#echo ${i}; ls ${modyfied}
	mv "${origin}" "${modyfied}"
done
