#-- start of make_header -----------------

#====================================
#  Application RunNumuCCMultiPiAnalysis
#
#   Generated Tue Dec 20 15:58:57 2016  by evrazhka
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunNumuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNumuCCMultiPiAnalysis

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag)_RunNumuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag)_RunNumuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNumuCCMultiPiAnalysis)

ifdef cmt_RunNumuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNumuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysis_RunNumuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysis_RunNumuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = $(bin)RunNumuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNumuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = $(bin)RunNumuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiPiAnalysissetup.make
endif

RunNumuCCMultiPiAnalysis ::


ifdef READONLY
RunNumuCCMultiPiAnalysis ::
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
	@echo 'RunNumuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNumuCCMultiPiAnalysis/
RunNumuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNumuCCMultiPiAnalysis :: dirs  $(bin)RunNumuCCMultiPiAnalysis${application_suffix}
	@/bin/echo "------> RunNumuCCMultiPiAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNumuCCMultiPiAnalysis${application_suffix} :: $(bin)RunNumuCCMultiPiAnalysis.o $(use_stamps) $(RunNumuCCMultiPiAnalysisstamps) requirements $(use_requirements)
	$(link_echo) $@
	$(link_silent) cd $(bin); $(cpplink) -o RunNumuCCMultiPiAnalysis.exe.new $(bin)RunNumuCCMultiPiAnalysis.o $(use_linkopts) $(RunNumuCCMultiPiAnalysislinkopts); mv -f RunNumuCCMultiPiAnalysis.exe.new RunNumuCCMultiPiAnalysis${application_suffix}

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNumuCCMultiPiAnalysisinstallname = RunNumuCCMultiPiAnalysis${application_suffix}

RunNumuCCMultiPiAnalysis :: RunNumuCCMultiPiAnalysisinstall

install :: RunNumuCCMultiPiAnalysisinstall

RunNumuCCMultiPiAnalysisinstall :: $(install_dir)/$(RunNumuCCMultiPiAnalysisinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(RunNumuCCMultiPiAnalysisinstallname) :: $(bin)$(RunNumuCCMultiPiAnalysisinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

##RunNumuCCMultiPiAnalysisclean :: RunNumuCCMultiPiAnalysisuninstall

uninstall :: RunNumuCCMultiPiAnalysisuninstall

RunNumuCCMultiPiAnalysisuninstall ::
	@echo "------> (RunNumuCCMultiPiAnalysis.make) Removing installed files"
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

#-- end of application
#-- start of dependency ------------------

$(bin)RunNumuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNumuCCMultiPiAnalysis_dependencies.make :: ../app/RunNumuCCMultiPiAnalysis*.cxx  requirements $(use_requirements) $(cmt_final_setup_RunNumuCCMultiPiAnalysis)
	@echo "------> (RunNumuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) RunNumuCCMultiPiAnalysis -all_sources $${args}
endif

#$(RunNumuCCMultiPiAnalysis_dependencies)

-include $(bin)RunNumuCCMultiPiAnalysis_dependencies.make

#-- end of dependency -------------------
#-- start of cpp ------

$(bin)$(binobj)RunNumuCCMultiPiAnalysis.o : $(RunNumuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)RunNumuCCMultiPiAnalysis.o $(use_pp_cppflags) $(RunNumuCCMultiPiAnalysis_pp_cppflags) $(app_RunNumuCCMultiPiAnalysis_pp_cppflags) $(RunNumuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(RunNumuCCMultiPiAnalysis_cppflags) $(app_RunNumuCCMultiPiAnalysis_cppflags) $(RunNumuCCMultiPiAnalysis_cppflags) $(RunNumuCCMultiPiAnalysis_cxx_cppflags) -I../app ../app/RunNumuCCMultiPiAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNumuCCMultiPiAnalysisclean
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




RunNumuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	@echo "------> (RunNumuCCMultiPiAnalysis.make) Removing application files"
	-$(cleanup_echo) RunNumuCCMultiPiAnalysis${application_suffix}
	-@cd $(bin); /bin/rm -f RunNumuCCMultiPiAnalysis${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)RunNumuCCMultiPiAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNumuCCMultiPiAnalysis_deps RunNumuCCMultiPiAnalysis_dependencies.make
#-- end of cleanup_objects ------
