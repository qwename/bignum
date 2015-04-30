OBJDIR := obj
SRCDIR := src
BINDIR := bin
DIRS = $(SRCDIR) $(OBJDIR) $(BINDIR)

_TYPES := float
TYPES := $(foreach e,$(_TYPES),big$(e))
_MODULES := init compare io add sub mul div mod
MODULES := $(foreach e,$(_TYPES),$(foreach f, $(_MODULES),$(e)_$(f)))
_SRCES := bignum $(TYPES) $(MODULES)
_HDRES := bignum $(TYPES)
_OPTBIN := bignum
SRCES = $(foreach obj,$(_SRCES:=.cc),$(SRCDIR)/$(obj))
HDRES = $(foreach obj,$(_HDRES:=.h),$(SRCDIR)/$(obj))
OBJS = $(foreach obj,$(_SRCES:=.o),$(OBJDIR)/$(obj))
OPTBIN = $(BINDIR)/$(_OPTBIN).a

CXX := g++
CXXFLAGS := -c -std=c++11 -Wall
LNKFLAGS := -Wall
COMPILE = $(CXX) $(CXXFLAGS) $< -o $@
STATICLIB = ar -rcs $@ $^

DEBUG ?= 0
SPEED ?= 0
ifeq ($(DEBUG),1)
	CXXFLAGS := $(CXXFLAGS) -g -DDEBUG
	OBJDIR := $(OBJDIR)/debug
	BINDIR := $(BINDIR)/debug
else
	ifeq ($(SPEED),1)
		CXXFLAGS := $(CXXFLAGS) -O2
	endif
endif



.PHONY : clean rebuild
ifeq ($(.DEFAULT_GOAL),)
	.DEFAULT_GOAL = all
endif

all : $(DIRS) $(OPTBIN)

$(OPTBIN) : $(OBJS)
	$(STATICLIB)

clean :
	-rm $(OPTBIN) $(OBJS)

rebuild : clean all

$(DIRS) : 
	-mkdir $@

$(OBJDIR)/float_%.o : $(SRCDIR)/float_%.cc $(SRCDIR)/bigfloat.h
	$(COMPILE)

$(OBJDIR)/big%.o : $(SRCDIR)/big%.cc $(SRCDIR)/big%.h
	$(COMPILE)
