# put your own CVS user name, if you don't have one, put "anoncvs@repo.nd280.org"
setenv CVSROOT :ext:mlam@repo.nd280.org:/home/trt2kmgr/ND280Repository

setenv CVS_RSH ssh
unset CVS_SERVER

# user folder
setenv USER /sps/t2k/mlamoure

# standard nd280 software location
setenv CMTSTABLE ${USER}/repository/v12r1

# standard nd280 highland location
setenv CMTHSTABLE ${USER}/dev/Highland2

# adding to cmtpath the path to upgrade software
setenv CMTPATH ${USER}/dev/Upgrade/t2k-nd280-upgrade:${CMTHSTABLE}:${CMTSTABLE}


setenv WORKDIR ${USER}/dev/Upgrade/t2k-nd280-upgrade/highlandUP/numuCC4piAnalysis/v0r0/output/

source ${USER}/CMT/setup.csh
source ${USER}/dev/Highland2/nd280Highland2/v2r9/cmt/setup.csh
source ${USER}/dev/Upgrade/t2k-nd280-upgrade/highlandUP/numuCC4piAnalysis/v0r0/cmt/setup.csh

