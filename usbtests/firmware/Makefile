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

rsync:
	ssh $(REMOTE_HOST) "mkdir -p $(REMOTE_DIR)" || exit 1
	rsync --rsh=ssh -r * $(REMOTE_HOST):$(REMOTE_DIR) || exit 1

masterflash:
	cd master && $(MAKE) flash

slaveflash:
	cd slave && $(MAKE) flash

clean: masterclean slaveclean

masterclean:
	cd master && $(MAKE) clean

slaveclean:
	cd slave && $(MAKE) clean

