#-- start of make_header -----------------

#====================================
#  Library numuCCMultiPiAnalysis
#
#   Generated Tue Dec 20 15:58:35 2016  by evrazhka
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_numuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_numuCCMultiPiAnalysis

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag)_numuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
cmt_local_tagfile_numuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag)_numuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_numuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_numuCCMultiPiAnalysis)

ifdef cmt_numuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_numuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysis_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = /tmp/CMT_numuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysis_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = $(bin)numuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_numuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = /tmp/CMT_numuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = $(bin)numuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiPiAnalysissetup.make
endif

numuCCMultiPiAnalysis ::


ifdef READONLY
numuCCMultiPiAnalysis ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	@echo 'numuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = numuCCMultiPiAnalysis/
numuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

numuCCMultiPiAnalysislibname   = $(bin)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix)
numuCCMultiPiAnalysislib       = $(numuCCMultiPiAnalysislibname).a
numuCCMultiPiAnalysisstamp     = $(bin)numuCCMultiPiAnalysis.stamp
numuCCMultiPiAnalysisshstamp   = $(bin)numuCCMultiPiAnalysis.shstamp

numuCCMultiPiAnalysis :: dirs  numuCCMultiPiAnalysisLIB
	@/bin/echo "------> numuCCMultiPiAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

numuCCMultiPiAnalysisLIB :: $(numuCCMultiPiAnalysislib) $(numuCCMultiPiAnalysisshstamp)
	@/bin/echo "------> numuCCMultiPiAnalysis : library ok"

$(numuCCMultiPiAnalysislib) :: $(bin)numuCCMultiPiAnalysis.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(numuCCMultiPiAnalysislib) $?
	$(lib_silent) $(ranlib) $(numuCCMultiPiAnalysislib)
	$(lib_silent) cat /dev/null >$(numuCCMultiPiAnalysisstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(numuCCMultiPiAnalysislibname).$(shlibsuffix) :: $(numuCCMultiPiAnalysislib) requirements $(use_requirements) $(numuCCMultiPiAnalysisstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" numuCCMultiPiAnalysis $(numuCCMultiPiAnalysis_shlibflags)

$(numuCCMultiPiAnalysisshstamp) :: $(numuCCMultiPiAnalysislibname).$(shlibsuffix)
	@if test -f $(numuCCMultiPiAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(numuCCMultiPiAnalysisshstamp) ; fi

numuCCMultiPiAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)numuCCMultiPiAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf numuCCMultiPiAnalysis_deps numuCCMultiPiAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
numuCCMultiPiAnalysisinstallname = $(library_prefix)numuCCMultiPiAnalysis$(library_suffix).$(shlibsuffix)

numuCCMultiPiAnalysis :: numuCCMultiPiAnalysisinstall

install :: numuCCMultiPiAnalysisinstall

numuCCMultiPiAnalysisinstall :: $(install_dir)/$(numuCCMultiPiAnalysisinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(numuCCMultiPiAnalysisinstallname) :: $(bin)$(numuCCMultiPiAnalysisinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##numuCCMultiPiAnalysisclean :: numuCCMultiPiAnalysisuninstall

uninstall :: numuCCMultiPiAnalysisuninstall

numuCCMultiPiAnalysisuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)numuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)numuCCMultiPiAnalysis_dependencies.make :: $(src)*.cxx  requirements $(use_requirements) $(cmt_final_setup_numuCCMultiPiAnalysis)
	@echo "------> (numuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) numuCCMultiPiAnalysis -all_sources $${args}
endif

#$(numuCCMultiPiAnalysis_dependencies)

-include $(bin)numuCCMultiPiAnalysis_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)numuCCMultiPiAnalysis.o : $(numuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)numuCCMultiPiAnalysis.o $(use_pp_cppflags) $(numuCCMultiPiAnalysis_pp_cppflags) $(lib_numuCCMultiPiAnalysis_pp_cppflags) $(numuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(numuCCMultiPiAnalysis_cppflags) $(lib_numuCCMultiPiAnalysis_cppflags) $(numuCCMultiPiAnalysis_cppflags) $(numuCCMultiPiAnalysis_cxx_cppflags)  $(src)numuCCMultiPiAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: numuCCMultiPiAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	@echo "You should provide a target for this file  $@"
#	@exit 0
endif

#ifdef PEDANTIC
#.DEFAULT::
#	@echo "You should provide a target for this file  $@"
#	@exit 1
#endif




numuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix).a $(binobj)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix).s? $(binobj)numuCCMultiPiAnalysis.stamp $(binobj)numuCCMultiPiAnalysis.shstamp
#-- end of cleanup_library ---------------

