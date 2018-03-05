# Execute group level cshrc profile.
if ($?THRONG_DIR) then
   if ( -r $THRONG_DIR/group_cshrc ) then
      source $THRONG_DIR/group_cshrc
   endif
endif
setenv CVSROOT :ext:mlam@repo.nd280.org:/home/trt2kmgr/ND280Repository
setenv CVS_RSH ssh
unset CVS_SERVER
setenv CMTSTABLE /sps/t2k/mlamoure/repository/v12r1
setenv CMTHSTABLE /sps/t2k/mlamoure/dev/Highland2
setenv CMTPATH /sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade:${CMTHSTABLE}:${CMTSTABLE}

setenv WORKDIR /sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/highlandUP/numuCC4piAnalysis/v0r0/output/

source /sps/t2k/mlamoure/CMT/setup.csh
source /sps/t2k/mlamoure/dev/Highland2/nd280Highland2/v2r9/cmt/setup.csh
source /sps/t2k/mlamoure/dev/Upgrade/t2k-nd280-upgrade/highlandUP/numuCC4piAnalysis/v0r0/cmt/setup.csh

