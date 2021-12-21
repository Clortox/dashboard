dashboard
=========

My dashboard, for my house. Display RSS feeds and other custom bits of info via
direct SDL2 calls. 

Features/TODO
=============

- Write straight to framebuffer (sdl2) (DONE)
- Display Weather rss feed (DONE)
- Display Wifi qrcode (DONE)
- Display camera feed
- Display who is home
- Display network speed
- Display plex currently playing
- Play audio files (background music/tracks)?

Depends on
==========

- sdl2
- stdlib

optional dependencies
---------------------

- Weather/RSS
  - rapidxml
  - libcurl

Building sdl2 on rpi
--------------------

Run in this order
```
sudo apt-get install libdrm-dev libgdm-dev
```

Next downlad the SDL source tarball

```
cd sdl-src
./configure --enable-video-kmsdrm
make -j4
sudo make install
```

compile the test.cpp file provided in ./tests . There should be output on the
screen as flashing. Feel free to change the desired graphics driver in the
raspberry pi config

Developing your own pannels
===========================

Refer to the
[development]
(DEVELOPMENT.md)
documentation
Usage
=====

To compile:

`make all`

To install:

`make install`

To uninstall:

`make uninstall`

Wifi
----

The makefile by default will call a script that will generate a wifi qr code
for you. You can add your SSID and password by editing the `WIFI_SSID` and
`WIFI_PASS` variables in the make file, or by calling the script yourself in
`./scripts`. You can cleanup the wifi image at any time by calling 
`make clean-wifi`. Next time the executable is built it will regenerate the
image.

The qr code generation relies on the program `qrencode`. It can be installed on
any debian system using `sudo apt install qrencode`. By default the image has a
transparent background. If you would like a different background, you will need
to adjust wifi.cpp to adjust where it is located to your preference.

