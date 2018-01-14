obj-m += hifimems-soundcard.o hifimems-codec.o
SRC := $(shell pwd)

all: devicetree
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules

install: devicetree
	cp hifimems-soundcard.dtbo /boot/overlays/
	cp hifimems-codec.ko /lib/modules/$(shell uname -r)
	cp hifimems-soundcard.ko /lib/modules/$(shell uname -r)
	depmod -a

clean: clean-local
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean

clean-local:
	rm hifimems-soundcard.dtbo || true

hifimems-soundcard.dtbo:
	dtc -@ -I dts -O dtb -o hifimems-soundcard.dtbo hifimems-soundcard-overlay.dts

devicetree: hifimems-soundcard.dtbo
