OBJDIR := obj
SRCDIR := src
BINDIR := bin
DIRS = $(SRCDIR) $(OBJDIR) $(BINDIR)

_SRCES := bignum init compare io add sub mul div mod
_HDRES := bignum
_OPTBIN := bignum
SRCES = $(foreach obj,$(_SRCES:=.cc),$(SRCDIR)/$(obj))
HDRES = $(foreach obj,$(_HDRES:=.h),$(SRCDIR)/$(obj))
OBJS = $(foreach obj,$(_SRCES:=.o),$(OBJDIR)/$(obj))
OPTBIN = $(BINDIR)/$(_OPTBIN).a

CXX := g++
CXXFLAGS := -c -std=c++11 -Wall
LNKFLAGS := -Wall
COMPILE = $(CXX) $(CXXFLAGS) $^ -o $@
STATICLIB = ar -rcs $@ $^

DEBUG ?= 0
ifeq ($(DEBUG),1)
	CXXFLAGS := $(CXXFLAGS) -g -DDEBUG
	OBJDIR := $(OBJDIR)/debug
	BINDIR := $(BINDIR)/debug
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

$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	$(COMPILE)
