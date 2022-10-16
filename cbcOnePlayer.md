= Building a CBC RadioOne KW Player =

== Introduction ==

This is NOT:

* a how-to guide
* a set of instructions
* advice
* the best way to achieve this
* the cheapest way to make a radio player
* me showing off
* me looking for advice

I'm writing this to help me remember what I did in case I need to go back and revisit some part of this project. But now that its written, I might as well publish it.

== The Hardware and Cost ==

These are the things I bought:

* Orange Pi PC 1GB RAM **(60 CAD)**
* Orange Pi Power Adapter **(12 CAD)**
* Mini External USB Stereo Speaker **(18 CAD)**
* USB WiFi Dongle **(13 CAD)**
* 16GB Micro SD Card **(7 CAD)**

Amounts are rounded to nearest dollar and do not include tax and shipping.

The grand total is **110 CAD!!** Yes, I over-paid. Enough said.

This does not include other accessories and equipment such as:

* Micro SD USB flash Adapter
* Notebook PC
* WiFi Router
* Internet Connection
* HDMI Monitor
* Ethernet Cable

== Low Hanging Fruit ==

=== Armbian ===

I downloaded and flashed the latest Armbian image for OrangePi PC. This was easy but slow. Good time to enjoy a beer.

I connected the OrangePi PC to my router via ethernet and booted the OrangePi PC and went to my WiFi Modem's admin page to look for a new device. The device was running an SSH server already so, it was time to login (user: _root_ password: _1234_) and configure a new user.

It was then time to update and grab the kernel headers as they will be needed for building drivers for the USB WiFi dongle.

```
sudo apt update 
sudo apt upgrade
sudo apt install linux-headers-current-sunxi
```

Lesson learned the hard way: REBOOT after update or else the running kernel version will not align with the version of the headers you downloaded. After much fiddling, this caused me to start over again. 

=== CBC RadioOne Kitchener-Waterloo Stream ===

```
sudo apt install mplayer
mplayer https://cbcradiolive.akamaized.net/hls/live/2040995/ES_R1EKW/master.m3u8
```

I could hear the radio station by pluging in headphones into the jack on the OrangePi PC.

If you want to find your local regional CBC RadioOne station, please have a look at: https://gist.github.com/nevillepark/d8358256e05a23250ad845a70776776e

=== USB WiFi Dongle ===

```
git clone https://github.com/brektrou/rtl8821CU.git
cd rtl8821CU
sudo ./dkms-install.sh 
```

But wait!! No WiFi...Not even an LED!!

=== Speakers ===

I found this: https://learn.adafruit.com/usb-audio-cards-with-a-raspberry-pi/updating-alsa-config

I shutdown the OrangePi PC and plugged in the Mini USB speakers. It was time to boot!

In `/usr/share/alsa/alsa.conf` I found

```
defaults.ctl.card 0
defaults.pcm.card 0
```

I changed both 0s to 2s.

* 0 means headphones. 
* 1 means HDM audio.
* 2 means usb-snd speakers.

This was determined via the `alsamixer -c <cardnum>` command.

I then saved the file and ran the mplayer command again. Audio came through the speakers this time! It was loud; the following command was used to adjust the volume:

```
alsamixer -c 2
```

== Tougher Nuts to Crack ==

=== USB WiFi Dongle Revisted ===

The github page for the kernel driver gives some advice about what to do if wifi does not work. It turned out that the dongle was showing up as a cdrom when I ran the following command: 

```
lsusb
```

This indicated that I needed to switch the mode to WiFi mode. The following command did the trick.

```
sudo apt install usb_modeswitch
usb_modeswitch -KW -v 0bda -p 1a2b
```

After that, it was time to configure the OrangePi PC to connect to WiFi. 

```
sudo armbian-config
 --> Network
 --> WiFi
   Pick an SSID and enter password
 quit armbian-config

ifconfig
```

I now had an IP address for the WiFi connection. Time to get rid of the ethernet cable.

=== Boot Up ===

I wanted this to be a headless device. Just plug it in and have it play. I wanted it to feel like a consumer electronics device; not a single board computer. This was a bit tougher than I thought.

At first I thought the solution was to modify `/etc/rc.local` to get everything going. Unfortunately, this was nothing but frustration. I ended up setting up auto-login as root on boot and then creating a `.profile` that would do the work. To enable the auto-login as root, I found the `ExecStart` line in `/lib/systemd/system/getty@.service` and replace it with:

```
ExecStart=-/sbin/agetty -a root --keep-baud 115200,38400,9600 %I $TERM
```

Here are the contents of `/root/.profile`:

```
#!/bin/sh -e

echo "Waiting for a minute..." >&2
echo 1 > /sys/class/leds/orangepi\:red\:status/brightness
sleep 60

echo "...Activating WiFi..." >&2
usb_modeswitch -KW -v 0bda -p 1a2b

echo heartbeat > /sys/class/leds/orangepi\:red\:status/trigger
sleep 40

echo "...here comes the CBC..." >&2
mplayer -cache 4096 https://cbcradiolive.akamaized.net/hls/live/2040995/ES_R1EKW/master.m3u8 &

echo none > /sys/class/leds/orangepi\:red\:status/trigger
echo "Done!!" >&2
```

The USB WiFi dongle revertes to CDROM mode when the OrangePi PC powered down, so upon boot up, I needed to run the `usb_modeswitch` command to activate WiFi mode. But before that, I needed to wait a minute because if done too soon, the device is busy. After that, I wait some time to allow for the dongle to establish a WiFi connection.

Finally, I start mplayer.

Note that I change the LEDs to know what stage of the initialization it is at without an HDMI monitor.

== Pulling Weeds and Bugs: NOT Enjoying the Silence ==

Upon boot, I didn't hear anything. It was time to hook up to a monitor to see what was going on. 

=== BTRFS ===

The boot up scan for BTRFS got stuck on the USB WiFi dongle. Likely, its CDROM mode had something to do with it. I stopped the scan with the following command:

```
sudo apt remove btrfs-progs
```

=== HDMI Resolution ===

Normally, the OrangePI PC needs to setup a resolution for HDMI, so it scans for an HDMI monitor and lets the monitor inform it of supported resolutions. Without an HDMI monitor attached, it would keep on scanning, waiting for a monitor. This incrased the boot time; nothing but silence for 5 minutes.

My solution was to simply set the resoution. A Google search yielded the following addition to `/boot/armbianEnv.txt` :

```
extraargs=drm_kms_helper.edid_firmware=HDMI-A-1:edid/1920x1080.bin video=HDMI-A-1:1920x1080@60
```

=== mplayer ===

I noticed that sometimes the following message appeared and playback stalls: 

```
Cannot seek backward in linear streams!
```

Thus, I added `-cache 4096`.

=== Disabling HDMI-Audio ===

Sometimes, on reboot, audio would not play. I could see that mplayer was running just fine on my monitor. 

Using the following command:

```
alsamixer -c 1
alsamixer -c 2
```

I could see that HDMI-audio and usb-snd had swapped audio card numbers! This happened randomly. My solution was to blacklist the HDMI-audio kernel module. In `/etc/modprobe.d/blacklist.conf` I added

```
blacklist snd_soc_hdmi_codec
```

== Things to Improve ==

* Sleep time during startup is arbitrary and hacky. Might want to
** decrease wait times
** poll on or wait for something
* Replace with cheaper components:
** Raspberry Pi Zero 2 W when its available. 19 CAD vs 73 CAD for Orange Pi PC + USB WiFi dongle
** micro-controller that can parse streams
** DAC HAT with better speakers

== Next Steps ==

* Make a wooden case
* Add the LCD display and add a UI to the display:
** Change volume
** Select and play audio files
