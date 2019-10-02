#!/bin/csh -f

set j=0
set f=1
set n=1000
set s=2
set r=interactive

set inp_name = "/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/RECO/PION_RECON/Reconstructed_SFGD_MC.root"
set folder_name = "/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/NN"
set prefix = "/nfs/neutrinos/cjesus/work/SFGD_MC_FINAL_SIZE/NN/InputNN"

set evtFirst = 0
set step = 1000
set evtLast = 0

@ evtLast = ${step} - 1

while (${evtFirst} < 59000)

    echo "evtFirst: ${evtFirst}, evtLast: ${evtLast}."
    
    set command     = "/software/neutrinos/t2k-nd280-upgrade/SFGD_Reconstruction/macros/cjesus/inputNN.exe -o ${prefix} -j -a ${evtFirst} -b ${evtLast} -i ${inp_name}"
    set batch_file  = "${folder_name}/job_${j}.batch"
    set sub_command = "qsub -q short -l nodes=1 ${batch_file}"

    touch ${batch_file}
    echo \#\!/bin/bash > ${batch_file}
    echo "" >> ${batch_file}
    echo "$command" >> ${batch_file}
    
    @ evtFirst = ${evtFirst} + ${step}
    @ evtLast = ${evtLast} + ${step}

    if( $1 == job) ${sub_command}
    if( $1 != job) ${command}

    rm ${batch_file}

    @ j = $j + 1
end
