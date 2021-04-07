obj-m += stereohat-soundcard.o stereohat-codec.o
SRC := $(shell pwd)

all: devicetree
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules

install: devicetree
	cp stereohat-soundcard.dtbo /boot/overlays/
	cp stereohat-codec.ko /lib/modules/$(shell uname -r)
	cp stereohat-soundcard.ko /lib/modules/$(shell uname -r)
	depmod -a

clean: clean-local
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean

clean-local:
	rm stereohat-soundcard.dtbo || true

stereohat-soundcard.dtbo:
	dtc -@ -I dts -O dtb -o stereohat-soundcard.dtbo stereohat-soundcard-overlay.dts

devicetree: stereohat-soundcard.dtbo
