# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

RC     := root-config
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
endif
ifneq ($(MKARCH),)
include $(MKARCH)
else
include $(ROOTSYS)/Makefile.arch
endif
-include ../MyConfig.mk

#------------------------------------------------------------------------------


##CXX           =
ObjSuf        = o
SrcSuf        = C
##ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

##turning on debuging 6/11/13
CXXFLAGS += -g 

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<  $(OutPutOpt) $@

EXE =  exe

OBJS= $(SRC:.$(SrcSuf)=.$(ObjSuf)) 
RLXX=$(LFLAGS) $(ROOTLIBS) -lMinuit -lTreePlayer -lMathMore

##turning on debuging 9/26/2016
RLXX +=  -g

all: $(EXE)
$(EXE) :  $(OBJS)
##	  $(LD) $(RLXX) $^ $(OutPutOpt) $@   For some reason in ubuntu 12.04 the order matters.................
	  $(LD) $^ $(OutPutOpt) $@ $(RLXX)
	rm $(OBJS)

clean:
		rm -f $(OBJS) core *.so *.o *.exe


##arroabas to mask the command

