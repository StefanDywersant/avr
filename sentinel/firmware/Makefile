#
# Sentinel
# Multi-aquarium controller.
#
# Copyright: (c) 2010 by Karol Maciaszek
# E-mail   : <karol.maciaszek@gmail.com>
# WWW      : http://www.czystybeton.pl/
#
# $Id$
#

MAKE=make
REMOTE_HOST=twarzromana.czystybeton.pl
REMOTE_DIR=__avr

all: masterfw.hex slavefw.hex

masterfw.hex:
	cd master && $(MAKE) hex

slavefw.hex:
	cd slave && $(MAKE) hex

rmasterflash: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make clean masterflash"

rslaveflash: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make clean slaveflash"

rslavefuse: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make slavefuse"

rslavereset: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make slavereset"

rsync:
	ssh $(REMOTE_HOST) "mkdir -p $(REMOTE_DIR)" || exit 1
	rsync --rsh=ssh -r * $(REMOTE_HOST):$(REMOTE_DIR) || exit 1

masterflash:
	cd master && $(MAKE) flash

slaveflash:
	cd slave && $(MAKE) flash

slavefuse:
	cd slave && $(MAKE) fuse

slavereset:
	cd slave && $(MAKE) reset

clean: masterclean slaveclean

masterclean:
	cd master && $(MAKE) clean

slaveclean:
	cd slave && $(MAKE) clean

