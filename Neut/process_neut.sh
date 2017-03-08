#! /bin/sh -f

###
#set ND = 2
ND=3

###
FLAV=numu

###
VER=11

###
#set fluxdir = /home/bquilain/
#set fluxdir = /net/sukond1/export/data/akiram/flux10d/nd34
#set fluxdir = /home/bquilain/flux
fluxdir=/net/sukond1/export/data/bquilain/hbk
datadir=/net/sukond1/export/data/bquilain/neutfile



## Target
TARGET=ch
#set TARGET = fe

###
## card (target)
#set incard = /home/bquilain/neut/neut_5.1.3/src/t2kflux_zbs/Cards/neut_${FLAV}_nd${ND}_${TARGET}.card
#set incard = /usr/local/sklib_g77/neut_5.1.4.2/src/t2kflux_zbs/Cards/neut_${FLAV}_nd${ND}_${TARGET}.card
incard=/home/bquilain/Cards/neut_${FLAV}_nd${ND}_${TARGET}.card

###
num=`printf "%d" $1`

#set fluxname = 10d_nd34_$num.hbk
#set fluxname = 11_nd2_$num.hbk
fluxname=fluka_nd34_11b_nom_250ka_$num.hbk
#set fluxname = 10d_shoulder_v4_$num.hbk
#set fluxname = 10dfluka_nd10_real.$num.hbk

if [ ! -e $fluxdir/$fluxname  ]; then
 echo $fluxdir/$fluxname does not exist.
 exit
fi

###
#set datfile = ${VER}_nd${ND}_${FLAV}_${TARGET}_$num.dat
datfile=${VER}fluka_nd${ND}_${FLAV}_${TARGET}_$num.dat

### set env
#source /usr/local/etc/skel/off/cshrc
#setenv ROOTSYS /usr/local/root
#setenv LD_LIBRARY_PATH /usr/local/root/lib:/opt/intel/cce/10.0.023/lib:$LD_LIBRARY_PATH
#source /home/bquilain/neut/setup.csh
#source /usr/local/sklib_g77/cshenv_g77
#source /usr/local/sklib_g77/skofl_11c/env.csh
export RFLIST=rflist.$$.`hostname`
export RANFILE=/home/bquilain/neut/neut_5.1.3/src/t2kflux_zbs/nqs_dir/random/random.tb.$1

echo "Input Card=" $incard
echo "Flux dir=" $fluxdir
echo "Flux name=" $fluxname
echo "Output file=" $datadir/$datfile

###
cat <<EOF > $RFLIST
10{
{"$fluxdir/$fluxname",LOCAL,,RED,,,"recl=1024 status=old"}
}
20{{"$datadir/$datfile",LOCAL,,WRT,,,"recl=5670 status=new"}}
EOF

#./Linux_pc/t2kneut_ndall $incard
/home/bquilain/neut/neut_5.1.4.2/src/t2kflux_zbs/Linux_pc/t2kneut_ndall $incard

###
#rm $RFLIST
echo "End process"
