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
## Help                                                                       ##
##----------------------------------------------------------------------------##

.PHONY: help

help:
	@echo
	@echo "WELCOME TO ROBOT"
	@echo "----------------"
	@echo
	@echo "MAKE"
	@echo "  $$ make help    - Prints out these help instructions"
	@echo "  $$ make build   - Builds robot as a static library"
	@echo "  $$ make clean   - Cleans and removes generated files"
	@echo "  $$ make install - Installs robot onto your system"
	@echo "  $$ make remove  -  Removes robot from your system"
	@echo "  $$ make test    - Builds the robot testing framework"
	@echo
	@echo "MODES"
	@echo "  This project can be built using debug or release settings"
	@echo "  By default, this project is built using release settings"
	@echo "  $$ make mode=debug <cmd>  -or-  $$ make mode=release <cmd>"
	@echo
	@echo "DOCS"
	@echo "  Visit http://getrobot.net for product documentation"
	@echo



##----------------------------------------------------------------------------##
## Build                                                                      ##
##----------------------------------------------------------------------------##

.PHONY: all _init build test clean

all: build test

_init:
ifeq ($(mode), debug)
	@echo "Building with debug settings"
else
	@echo "Building with release settings"
endif

build: _init
	cd Source; make _build=robot --no-print-directory build

test: _init
	cd   Test; make _build=robot --no-print-directory build

clean:
	cd Source; make _build=robot --no-print-directory clean
	cd   Test; make _build=robot --no-print-directory clean



##----------------------------------------------------------------------------##
## Install                                                                    ##
##----------------------------------------------------------------------------##

.PHONY: install remove

install:
	cd Source; make _build=robot --no-print-directory install

remove:
	cd Source; make _build=robot --no-print-directory remove
