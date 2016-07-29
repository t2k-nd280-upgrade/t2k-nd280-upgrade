
# Clean everything

cd  $T2KND280UP/build
echo "Moved to $PWD"
echo "Clean everything"
echo ""

source cmake_clean.sh

# Create the ROOT dictionary

cd $T2KND280UP_EVT
echo "Moved to $PWD"
echo "Remove the ROOT dictionary"
echo ""

rm dict.*

echo "Create a new ROOT dictionary"
echo ""
echo "rootcint dict.cc -c *.hh LinkDef.h"

rootcint -f dict.cc -c *.hh LinkDef.h

# Run the cmake

cd $T2KND280UP/build
echo $PWD

echo "Create MakeFile"
echo "cmake ../src"
echo ""

cmake ../src

# Compile

echo "Compile"
echo ""

make