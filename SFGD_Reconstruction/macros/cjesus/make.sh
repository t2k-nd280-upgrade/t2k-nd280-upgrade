#!bin/sh

name="${1%.*}"
echo "******* Compiling ${name}.C to ${name}.exe *******"
make SRC="../../utils/mydict.C ${name}.C" EXE="${name}.exe"
