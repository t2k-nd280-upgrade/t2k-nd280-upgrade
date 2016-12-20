# echo "Setting numuCCMultiPiAnalysis v0r0 in /home/evrazhka/ND280/nd280rep/v11r31/highlandUP"

if test "${CMTROOT}" = ""; then
  CMTROOT=/home/evrazhka/ND280/CMT/v1r18p20061003; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=numuCCMultiPiAnalysis -version=v0r0 -path=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

