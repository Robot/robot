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
	OUTPUT = Binaries/Mac/
else
	CXX    = g++
	OUTPUT = Binaries/Linux/
endif

ifeq ($(mode), debug)
	LIBRARY  = libRobotd.a
	OBJECT   = $(OUTPUT)Robotd/
	CXXFLAGS = -std=c++0x -Wall -pedantic-errors -fno-rtti -O0 -g

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++
	endif

else
	LIBRARY  = libRobot.a
	OBJECT   = $(OUTPUT)Robot/
	CXXFLAGS = -std=c++0x -Wall -pedantic-errors -fno-rtti -O3

	ifeq ($(UNAME), Darwin)
		CXXFLAGS += -stdlib=libc++
	else
		CXXFLAGS += -s
	endif

endif

SRC = Source/

SOURCES = $(shell find $(SRC) -name "*.cc")
HEADERS = $(shell find $(SRC) -name "*.h")
OBJECTS = $(patsubst $(SRC)%.cc, $(OBJECT)%.o, $(SOURCES))

INSTALL_INC = /usr/local/include/Robot/
INSTALL_LIB = /usr/local/lib/



##----------------------------------------------------------------------------##
## Help                                                                       ##
##----------------------------------------------------------------------------##

.PHONY: help

help:
	@echo
	@echo "WELCOME TO ROBOT"
	@echo "================"
	@echo
	@echo "MAKE"
	@echo "  $$ make help    - Prints out these help instructions"
	@echo "  $$ make build   - Builds this project as a static library"
	@echo "  $$ make clean   - Cleans and removes all generated files"
	@echo "  $$ make install - Installs this library onto your system"
	@echo "  $$ make remove  - Removes this library from your system"
	@echo
	@echo "MODES"
	@echo "  This project can be built using debug or release settings"
	@echo "  By default, this project is built using release settings"
	@echo "  $$ make mode=debug <cmd>  -or-  $$ make mode=release <cmd>"
	@echo
	@echo "DOCS"
	@echo "  Go to https://robot.github.io/ for product documentation"
	@echo



##----------------------------------------------------------------------------##
## Build                                                                      ##
##----------------------------------------------------------------------------##

.PHONY: all _init build clean

$(OBJECT)%.o: $(SRC)%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: build

_init:
ifeq ($(mode), debug)
	@echo "Building library using debug settings"
else
	@echo "Building library using release settings"
endif
	mkdir -p $(OUTPUT) $(OBJECT)

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
