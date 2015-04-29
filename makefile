OBJDIR := obj
SRCDIR := src
BINDIR := bin
DIRS = $(SRCDIR) $(OBJDIR) $(BINDIR)

_SRCES := bignum init compare io add test
_HDRES := bignum
_OPTBIN := test
SRCES = $(foreach obj,$(_SRCES:=.cc),$(SRCDIR)/$(obj))
HDRES = $(foreach obj,$(_HDRES:=.h),$(SRCDIR)/$(obj))
OBJS = $(foreach obj,$(_SRCES:=.o),$(OBJDIR)/$(obj))
OPTBIN = $(BINDIR)/$(_OPTBIN)

CC := g++
CCFLAGS := -c -std=c++11 -Wall
LKFLAGS := -Wall
COMPILE = $(CC) $(CCFLAGS) $^ -o $@
LINK = $(CC) $(LKFLAGS) $^ -o $@

DEBUG ?= 0
ifeq ($(DEBUG),1)
	CCFLAGS := $(CCFLAGS) -g -DDEBUG
	OBJDIR := $(OBJDIR)/debug
	BINDIR := $(BINDIR)/debug
endif


.PHONY : clean rebuild
ifeq ($(.DEFAULT_GOAL),)
	.DEFAULT_GOAL = all
endif

all : $(DIRS) $(OPTBIN)

$(OPTBIN) : $(OBJS)
	$(LINK)

clean :
	-rm $(OPTBIN) $(OBJS)

rebuild : clean all

$(DIRS) : 
	-mkdir $@

$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	$(COMPILE)
