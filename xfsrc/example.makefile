#
#  EXAMPLE makefile
#
#  modify this file for your own AP/DLL
#

IS_WIN:=$(findstring win,$(MK_PORT))
IS_DEBUG:=$(findstring 1,$(MK_DEBUG))

# location of intermediate files (eg .obj). Set to "." (without quotes) for current dir.
OBJDIR:=$(MK_XFLIB)

# location of final executable. Set to "." (without quotes) for current dir.
EXEDIR:=$(OBJDIR)

# binary to create
TARGET:=		$(EXEDIR)/example$(MK_EXEEXT)

# sources from which it is built
SOURCES:=	example.c

# ignore include file dependencies - build them in if you need them

APLLIB:=$(if $(IS_WIN),$(if $(IS_DEBUG),apld,apl),apl)
APLLIB:=$(APLLIB)$(MK_LIBEXT)
APLLIB:=$(LIBDIR)/$(APLLIB)

RMCMD:=$(if $(IS_WIN),erase /q,rm -f)

OBJECTS:=$(OBJDIR)/$(SOURCES:.c=$(MK_OBJEXT))

all: $(TARGET)

$(OBJDIR)/%$(MK_OBJEXT): %.c                  # given a .c file: how to get a .o
	mk_cc $<

$(TARGET): $(OBJECTS)
	@mk_lnk_ap $@ $(OBJECTS) $(APLLIB)

clean:
	-@$(RMCMD) $(TARGET)
	-@$(RMCMD) $(OBJECTS)

