################################################################################
## -------------------------------------------------------------------------- ##
##                                                                            ##
##                       (C) 2010-2016 Robot Developers                       ##
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
	LIBRARY  = libRobotd.a
	OBJECT   = $(OUTPUT)Robotd/
	CXXFLAGS = -std=c++11 -Wall -pedantic-errors -fno-rtti -O0 -g -DDEBUG

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++ -x objective-c++
	endif

else
	LIBRARY  = libRobot.a
	OBJECT   = $(OUTPUT)Robot/
	CXXFLAGS = -std=c++11 -Wall -pedantic-errors -fno-rtti -O3 -DNDEBUG

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++ -x objective-c++
	else
		CXXFLAGS += -s
	endif

endif

SOURCES = $(shell find ./ -name "*.cc")
HEADERS = $(shell find ./ -name "*.h" )
OBJECTS = $(patsubst %.cc, $(OBJECT)%.o, $(SOURCES))

INSTALL_LIB = /usr/local/lib/
INSTALL_INC = /usr/local/include/Robot/



##----------------------------------------------------------------------------##
## Build                                                                      ##
##----------------------------------------------------------------------------##

.PHONY: all _init build clean

all:
	@echo "Please run make from the root directory"
	exit 1

_init:
ifneq ($(_build), robot)
	@echo "Please run make from the root directory"
	exit 1
endif
	mkdir -p $(OUTPUT) $(OBJECT)

$(OBJECT)%.o: $(SRC)%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

build: _init $(OBJECTS)
	ar rcs $(OUTPUT)$(LIBRARY) $(OBJECTS)

clean:
	rm -rf $(OUTPUT)



##----------------------------------------------------------------------------##
## Install                                                                    ##
##----------------------------------------------------------------------------##

.PHONY: install remove

install: build $(HEADERS) $(OUTPUT)$(LIBRARY)
	mkdir -p $(INSTALL_INC) $(INSTALL_LIB)
	install -p -m 0644 $(HEADERS) $(INSTALL_INC)
	install -p -m 0644 $(OUTPUT)$(LIBRARY) $(INSTALL_LIB)

remove:
	rm -rf $(INSTALL_INC)
	rm -f $(INSTALL_LIB)$(LIBRARY)
