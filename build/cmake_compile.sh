reco_only=0
no_cleaning=0
no_dict=0

while test $# -gt 0; do
	case "$1" in
		-h|--help)
			echo "t2k-nd280-upgrade compilation script"
			echo "   -h|--help          help"
			echo "   -r|--reco-only     compile only reco"
			echo "   -n|--no-cleaning   do not clean up before compilation"
			echo "   -d|--no-dict       do not recompile dictionary"
			exit 1
			;;
		-r|--reco-only)
			reco_only=1
			shift
			;;
		-n|--no-cleaning)
			no_cleaning=1
			shift
			;;
		-d|--no-dict)
			no_dict=1
			shift
			;;

	esac
done

# Clean everything
if ((($no_cleaning==0)))
then
	cd  $T2KND280UP/build
	echo "Moved to $PWD"
	echo "Clean everything"
	echo ""
	
	echo "Remove Makefile and cmake files"
	
	rm Makefile CMakeCache.txt cmake_install.cmake
	rm -rf app CMakeFiles
	
	cd $T2KND280UP/build
else
	echo "WARNING! No cleaning performed, proceed with caution."
fi

# Create the ROOT dictionary
if ((($no_dict==0) && ($reco_only==0)))
then
	cd $T2KND280UP_EVT
	echo "Moved to $PWD"
	echo "Remove the ROOT dictionary"
	echo ""

	cd $T2KND280UP_EVT
	rm dict.*
	rm *_cc*

	echo "Create a new ROOT dictionary"
	echo ""
	echo "rootcint dict.cc -c *.hh LinkDef.h"

	rootcint -f dict.cc -c *.hh LinkDef.h
fi

# Run the cmake

cd $T2KND280UP/build
echo $PWD

echo "Create MakeFile"
echo "cmake ../src"
echo ""

if ((($reco_only==0)))
then
	cmake ../src
else
	echo "Only reconstruction software will be constructed."
	cmake -DRECO_ONLY=ON ../src
fi

# Compile

echo "Compile"
echo ""

make -j6