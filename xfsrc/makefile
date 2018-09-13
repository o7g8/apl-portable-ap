#
#  AP makefile. Makes all the APS 
#
#  see example.makefile for a makefile to make a single AP.

OBJDIR:=$(MK_XFLIB)
EXEDIR:=$(OBJDIR)

export MK_AP_BUILD:=1

IS_DEBUG:=$(findstring 1,$(MK_DEBUG))
IS_UNICODE:=$(findstring 1,$(MK_UNICODE))
IS_WIN:=$(findstring win,$(MK_PORT))
IS_WIN_LIKE:=$(findstring win,$(MK_API))
IS_64BIT:=$(findstring 64,$(MK_BITS))

PLATFORM:=$(if $(IS_64BIT),x64,x86)

APLLIB:=$(if $(IS_WIN),$(if $(IS_DEBUG),apld,apl),apl)
APLLIB:=$(APLLIB)$(MK_LIBEXT)
APLLIB:=$(LIBDIR)/$(APLLIB)

CLRLIB:=$(if $(IS_DEBUG),aplclrd,aplclr)
CLRLIB:=$(CLRLIB)$(MK_LIBEXT)
CLRLIB:=$(LIBDIR)/$(CLRLIB)

ALLTARGETS:= 	$(EXEDIR)/xutils${MK_EXEEXT}		\
	 	$(EXEDIR)/strand${MK_EXEEXT}		\
	 	$(EXEDIR)/example${MK_EXEEXT}		\
		$(EXEDIR)/ademo$(MK_BITS)$(MK_DLLEXT)	\
		$(EXEDIR)/dyalog$(MK_BITS)$(MK_DLLEXT)	\

C32TARGETS:=

CLASSIC32TARGETS:=$(if $(IS_64BIT),,$(if $(IS_UNICODE),,$(C32TARGETS)))

CTARGETS:=

CLASSICTARGETS:=$(if $(IS_UNICODE),,$(CTARGETS))

WINUNICODETARGETS:= $(EXEDIR)/managed_example${MK_DLLEXT}	\
	 	 $(EXEDIR)/dotnet_example${MK_EXEEXT}		\
	 	 $(EXEDIR)/man${MK_DLLEXT}			\
	 	 $(EXEDIR)/useapl${MK_EXEEXT}			\

UNICODETARGETS:=$(if $(IS_WIN),$(ALLUNICODETARGETS) $(WINUNICODETARGETS),$(ALLUNICODETARGETS))

TARGETS:=	$(if $(IS_UNICODE), $(UNICODETARGETS),)	\
		$(CLASSIC32TARGETS)			\
		$(CLASSICTARGETS)			\
		$(ALLTARGETS)				\


RESOURCES:=$(if $(IS_WIN_LIKE),$(OBJDIR)/dyalog${MK_RESEXT},)

all: $(TARGETS)	$(MAKEFILE)


${OBJDIR}/%${MK_OBJEXT}: %.c                  # given a .c file: how to get a .o
	mk_cc $<

${OBJDIR}/%${MK_RESEXT}: %.rc                  # given a .rc file: how to get a .res
	mk_drc $<

${OBJDIR}/example${MK_EXEEXT}:
	$(MAKE) -j$(MK_NJOBS) --no-print-directory -f example.makefile

-include $(OBJDIR)/xutils$(MK_DEPEXT)
$(EXEDIR)/xutils${MK_EXEEXT}: $(OBJDIR)/xutils$(MK_OBJEXT)	$(APLLIB) $(RESOURCES)
	mk_lnk_ap $@ $(OBJDIR)/xutils$(MK_OBJEXT) $(APLLIB) $(RESOURCES)

-include $(OBJDIR)/strand$(MK_DEPEXT)
$(EXEDIR)/strand$(MK_EXEEXT): $(OBJDIR)/strand$(MK_OBJEXT) $(APLLIB) $(RESOURCES)			
	mk_lnk_ap $@ $(OBJDIR)/strand$(MK_OBJEXT) $(APLLIB) $(RESOURCES)

-include $(OBJDIR)/dotnet_example$(MK_DEPEXT)
$(EXEDIR)/dotnet_example$(MK_EXEEXT): $(OBJDIR)/dotnet_example$(MK_OBJEXT) $(APLLIB) $(RESOURCES)			
	mk_lnk_ap $@ $(OBJDIR)/dotnet_example$(MK_OBJEXT) $(APLLIB) $(RESOURCES)

-include $(OBJDIR)/managed_example$(MK_DEPEXT)
$(EXEDIR)/managed_example$(MK_DLLEXT): managed_example.cpp $(CLRLIB) $(OBJDIR)/managed_example${MK_RESEXT} 
	qcl -nologo -Od -Z7 -LD -clr -I. -I$(MK_SRCDIR) -I$(MK_PORTDIR)/src -DxxBIT=$(MK_BITS) /Fo"$(OBJDIR)/managed_example${MK_OBJEXT}" managed_example.cpp /link $(CLRLIB) /Machine:$(PLATFORM) /nologo /manifest:NO $(OBJDIR)/managed_example${MK_RESEXT} /out:"$@"

-include $(OBJDIR)/man$(MK_DEPEXT)
$(EXEDIR)/man$(MK_DLLEXT): man.cpp $(CLRLIB) $(OBJDIR)/managed_example${MK_RESEXT} $(LIBDIR)/$(MK_APBRIDGENAME)
	qcl -AI$(OBJDIR) -nologo -Od -Z7 -LD -clr -I. -I$(MK_SRCDIR) -I$(MK_PORTDIR)/src -DxxBIT=$(MK_BITS) -DHAS_UNICODE=$(MK_UNICODE) /Fo"$(OBJDIR)/man${MK_OBJEXT}" /FU "$(MK_APBRIDGENAME)" man.cpp /link $(CLRLIB) /Machine:$(PLATFORM) /nologo /manifest:NO $(OBJDIR)/managed_example${MK_RESEXT} /out:"$@"

$(EXEDIR)/useapl$(MK_EXEEXT): useapl.cs $(EXEDIR)/managed_example$(MK_DLLEXT)
	csc /Platform:$(PLATFORM) /nologo /debug /r:$(EXEDIR)/managed_example$(MK_DLLEXT) /out:$@ useapl.cs

$(EXEDIR)/useapl2$(MK_EXEEXT): useapl2.cs $(EXEDIR)/man$(MK_DLLEXT)
	csc /Platform:$(PLATFORM) /nologo /debug /r:$(EXEDIR)/man$(MK_DLLEXT) /out:$@ useapl2.cs

-include $(OBJDIR)/dyalog$(MK_DEPEXT)
$(EXEDIR)/dyalog$(MK_BITS)$(MK_DLLEXT): $(OBJDIR)/dyalog$(MK_OBJEXT) $(RESOURCES) 
	mk_lnk_ap $@ $(OBJDIR)/dyalog$(MK_OBJEXT) $(RESOURCES)

-include $(OBJDIR)/ademo$(MK_DEPEXT)
-include $(OBJDIR)/array$(MK_DEPEXT)
-include $(OBJDIR)/data$(MK_DEPEXT)
$(EXEDIR)/ademo$(MK_BITS)$(MK_DLLEXT): $(OBJDIR)/ademo$(MK_OBJEXT) $(OBJDIR)/array$(MK_OBJEXT) $(OBJDIR)/data$(MK_OBJEXT) $(RESOURCES) 
	mk_lnk_ap $@ $(OBJDIR)/ademo$(MK_OBJEXT) $(OBJDIR)/array$(MK_OBJEXT) $(OBJDIR)/data$(MK_OBJEXT) $(RESOURCES)

clean:
	rm -f $(EXEDIR)/*.*
	rm -f $(OBJDIR)/*.*

