#!/bin/csh -f

set j=0
set f=1
set n=1000
set s=2
set r=interactive

set folder_name    = "/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/ROOT/"
set base_inp_name  = "${folder_name}MC_output-"

if( $1 == job ) then
    set s=200
    set r=PIC
endif

while ($j < $s)
    set inp_name    = ${base_inp_name}${j}-Exp${j}000-Nexpt${n}.root
    set out_name    = ${folder_name}TXT/file_${j}.txt
    set command     = "/software/neutrinos/t2k-nd280-upgrade/SFGD_Reconstruction/macros/inputNN.exe -j -b -o ${out_name} -i ${inp_name}"
    set batch_file  = ${folder_name}TXT/job_${j}.batch
    set sub_command = "qsub -q short -l nodes=1 ${batch_file}"

    echo "........  file $j -----> ${inp_name} ........ "
    touch ${batch_file}
    echo \#\!/bin/bash > ${batch_file}
    echo "" >> ${batch_file}
    echo "$command" >> ${batch_file}
    
    if( $1 == job) ${sub_command}
    if( $1 != job) ${command}

    rm ${batch_file}

    @ j = $j + 1
end
