if test "${CMTROOT}" = ""; then
  CMTROOT=/home/evrazhka/ND280/CMT/v1r18p20061003; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=numuCCMultiPiAnalysis -version=v0r0 -path=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

