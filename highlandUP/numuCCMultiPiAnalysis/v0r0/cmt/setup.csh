# echo "Setting numuCCMultiPiAnalysis v0r0 in /home/evrazhka/ND280/nd280rep/v11r31/highlandUP"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /home/evrazhka/ND280/CMT/v1r18p20061003
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=numuCCMultiPiAnalysis -version=v0r0 -path=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

