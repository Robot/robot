################################################################################
## -------------------------------------------------------------------------- ##
##                                                                            ##
##                       (C) 2010-2014 Robot Developers                       ##
##                       See LICENSE for licensing info                       ##
##                                                                            ##
## -------------------------------------------------------------------------- ##
################################################################################

makepp_no_builtin = 1

##----------------------------------------------------------------------------##
## Variables                                                                  ##
##----------------------------------------------------------------------------##

UNAME = $(shell uname -s)

ifeq ($(UNAME), Darwin)
	CXX    = clang++
	OUTPUT = ../Binaries/Mac/
else
	CXX    = g++
	OUTPUT = ../Binaries/Linux/
endif

ifeq ($(mode), debug)
	BINARY   = $(OUTPUT)Testd.bin
	OBJECT   = $(OUTPUT)Testd/
	CXXFLAGS = -std=c++0x -fno-rtti -O0 -g
	 LDFLAGS = -L$(OUTPUT) -lRobotd

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++
		 LDFLAGS += -framework ApplicationServices
	else
		 LDFLAGS += -lX11 -lXtst
	endif

else
	BINARY   = $(OUTPUT)Test.bin
	OBJECT   = $(OUTPUT)Test/
	CXXFLAGS = -std=c++0x -fno-rtti -O3
	 LDFLAGS = -L$(OUTPUT) -lRobot

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++
		 LDFLAGS += -framework ApplicationServices
	else
		CXXFLAGS += -s
		 LDFLAGS += -lX11 -lXtst
	endif

endif

SOURCES = $(shell find ./ -name "*.cc")
HEADERS = $(shell find ./ -name "*.h" )
OBJECTS = $(patsubst %.cc, $(OBJECT)%.o, $(SOURCES))



##----------------------------------------------------------------------------##
## Build                                                                      ##
##----------------------------------------------------------------------------##

.PHONY: all _init build clean

all:
	@echo "Please run make from the root directory"
	exit 1

_init:
ifneq ($(_build), Robot)
	@echo "Please run make from the root directory"
	exit 1
endif
	mkdir -p $(OUTPUT) $(OBJECT)

$(OBJECT)%.o: $(SRC)%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

build: _init $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(BINARY) $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(OUTPUT)
