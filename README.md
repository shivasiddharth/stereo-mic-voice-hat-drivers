# AoSC Driver Simple I2S Hifiberry and ICS43432 MEMS

> Use Hifiberry-dac and ICS43432 simultaneously

Tested with:
 - [Adafruit I2S MEMS Microphone Breakout](https://www.adafruit.com/product/3421) and [Pimoroni Speaker PHAT for Raspberry Pi Zero](https://www.adafruit.com/product/3401)
 - [Adafruit I2S MEMS Microphone Breakout](https://www.adafruit.com/product/3421) and [Adafruit I2S 3W Class D Amplifier Breakout](https://www.adafruit.com/product/3006)

Based on googlevoicehat-{codec,soundcard}.c by Peter Malkin.

## Install

This was installed successfully on Linux 4.9.59+ on a Raspberry Pi Zero W

```sh
sudo apt-get update
sudo apt-get dist-upgrade
sudo apt-get install raspberrypi-kernel-headers

git clone https://github.com/Sarev0k/hifimems-kmod
cd hifimems-kmod

make KERNEL_SRC=/lib/modules/$(uname -r)/build all
sudo make KERNEL_SRC=/lib/modules/$(uname -r)/build install
```

## Boot Config Setup
```sh
sudo sed -i "/^#dtparam=i2s=on$/ s|#||" /boot/config.txt
sudo sed -i "/^dtparam=audio=on$/ s|^|#|" /boot/config.txt
echo "dtoverlay=hifimems-soundcard" | sudo tee -a /boot/config.txt
```

## Audio Config Setup
```sh
sudo cp asound.conf /etc/asound.conf


# Reduce speaker pops and crackles
sudo apt-get install pulseaudio

sudo sed -i "s/^.ifexists module-udev-detect.so\b/&.ignore/" /etc/pulse/default.pa
sudo sed -i "s/^load-module module-native-protocol-unix\b/& auth-anonymous=1 socket=\/tmp\/pulseaudio.socket/" /etc/pulse/default.pa
sudo sed -i "/^#load-module module-native-protocol-tcp$/ s|#||" /etc/pulse/default.pa
sudo sed -i "s/^load-module module-native-protocol-tcp\b/& auth-ip-acl=127.0.0.1/" /etc/pulse/default.pa
sudo sed -i "s/^load-module module-suspend-on-idle\b/& timeout=604800/" /etc/pulse/default.pa

echo "autospawn = no" | sudo tee -a /etc/pulse/client.conf
echo "default-server = unix:/tmp/pulseaudio.socket" | sudo tee -a /etc/pulse/client.conf

echo "exit-idle-time = -1" | sudo tee -a /etc/pulse/daemon.conf
echo "resample-method = ffmpeg" | sudo tee -a /etc/pulse/daemon.conf
echo "enable-remixing = yes" | sudo tee -a /etc/pulse/daemon.conf
echo "flat-volumes = no" | sudo tee -a /etc/pulse/daemon.conf
echo "default-sample-rate = 48000" | sudo tee -a /etc/pulse/daemon.conf

sudo cp pulseaudio.service /etc/systemd/system/pulseaudio.service
sudo systemctl daemon-reload
sudo systemctl enable pulseaudio
sudo reboot
```
