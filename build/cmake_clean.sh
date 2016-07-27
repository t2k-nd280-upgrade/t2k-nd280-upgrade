
echo "Remove Makefile and cmake files"

rm Makefile CMakeCache.txt cmake_install.cmake
rm -rf app CMakeFiles

echo "Remove ROOT dictionary"

cd $T2KND280UP_EVT
rm dict.*

cd $T2KND280UP/build