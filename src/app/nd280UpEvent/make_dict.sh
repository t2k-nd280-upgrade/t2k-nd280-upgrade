
rm dict.cc dict.h
rm *.d *.so
rootcint -f dict.cc -c *.hh LinkDef.h