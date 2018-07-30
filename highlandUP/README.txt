------------------------------
Pre-requirements
------------------------------

It is needed to install nd280 software before anything, because the highlandUP is using RECPACK from nd280 software. To do it, please follow the instructions here :
http://nd280.lancs.ac.uk/stable/invariant/highlandND280Doc/install_nd280AnalysisTools.html#installnd280

It will also give the instructions to install cmt, that is needed for everything.

The version v12r7 should be ok. It will also install ROOT v5r34 that is needed too.


------------------------------
Installation
------------------------------

Source nd280 software following the instruction from previous link.
change the CMTPATH so that it links like that :

- export CMTPATH=/path/to/t2k-nd280-upgrade:/path/to/nd280software
(or using setenv is tcsh/csh environment)

Then :

- cd highlandUP/numuCC4piAnalysis/v0r0/cmt
- cmt broadcast cmt config
- source setup.(c)sh
- cmt broadcast make


------------------------------
Use
------------------------------

The software can be used as normal highland softare, including the use of DrawingTools, see:

http://nd280.lancs.ac.uk/stable/invariant/highlandDoc/microtree.html
http://nd280.lancs.ac.uk/stable/invariant/highlandDoc/DrawingTools.html
